import pandas as pd
import numpy as np
from .validate import validate_dataset


def analyze_up_down_runs(df, start_date=None, end_date=None):

    """
    Analyze upward and downward runs in stock price data.

    Parameters:
        df (pd.DataFrame): DataFrame with 'Date' and 'Close' columns.
        start_date (str, optional): Filter start date (YYYY-MM-DD).
        end_date (str, optional): Filter end date (YYYY-MM-DD).

    Returns:
        dict: Summary with longest runs (with dates), medians, and totals.
    """

    # validate dataset before any computation
    validate_dataset(df, required_cols=["Date", "Close"], min_rows=2)

    # ensure date col is datetime & sort
    df["Date"] = pd.to_datetime(df["Date"])
    df = df.sort_values("Date").reset_index(drop=True)
    # df.set_index("Date", inplace=True)

    # Date range
    if start_date and end_date:
        df = df[(df["Date"] >= start_date) & (df["Date"] <= end_date)]
       # df = df[start_date:end_date]

    # calc daily changes (close - prev close)
    df["Change"] = df["Close"].diff()

    # label days: Up (+1), Down (-1), No Change (0)
    df["Direction"] = df["Change"].apply(lambda x: 1 if x > 0 else (-1 if x < 0 else 0))

    # count runs (consecutive +-1 streaks)
    runs = []
    current_dir, current_length, start_idx = 0, 0, None
    
    for i, d in enumerate(df["Direction"]):
        if d == current_dir and d != 0:  # continuing a run
            current_length += 1
        elif d != 0:  # new run starts
            if current_dir != 0:
                runs.append((current_dir, current_length, start_idx, i - 1))
            current_dir = d
            current_length = 1
            start_idx = i - 1
        else:  # no change day
            if current_dir != 0: # close prev run
                runs.append((current_dir, current_length, start_idx, i - 1))
            current_dir, current_length, start_idx = 0, 0, None

    # save last run if still active
    if current_dir != 0:
        runs.append((current_dir, current_length, start_idx, len(df) - 1))

    # seperate up and down runs
    up_runs = [r for r in runs if r[0] == 1]
    down_runs = [r for r in runs if r[0] == -1]

    # find longest up/down runs
    longest_up = max(up_runs, key=lambda x: x[1], default=None)
    longest_down = max(down_runs, key=lambda x: x[1], default=None)
    # longest_up = df[df["Direction"] == 1]["Run_Length"].max()
    # longest_down = df[df["Direction"] == -1]["Run_Length"].max() 

    # median lengths
    median_up = float(pd.Series([r[1] for r in up_runs]).median()) if up_runs else 0
    median_down = float(pd.Series([r[1] for r in down_runs]).median()) if down_runs else 0

    return {
        "longest_up": {
            "length": longest_up[1] if longest_up else 0,
            "start_date": df["Date"].iloc[longest_up[2]].strftime("%Y-%m-%d") if longest_up and len(longest_up) >= 4 else None,
            "end_date": df["Date"].iloc[longest_up[3]].strftime("%Y-%m-%d") if longest_up and len(longest_up) >= 4 else None,
        },
        "longest_down": {
            "length": longest_down[1] if longest_down else 0,
            "start_date": df["Date"].iloc[longest_down[2]].strftime("%Y-%m-%d") if longest_down and len(longest_down) >= 4 else None,
            "end_date": df["Date"].iloc[longest_down[3]].strftime("%Y-%m-%d") if longest_down and len(longest_down) >= 4 else None,
        },
        "median_up_run_length": median_up,
        "median_down_run_length": median_down,
        "total_upward_runs": len(up_runs),
        "total_downward_runs": len(down_runs),
    }


    # Optional visualisation
    if __name__ == "__main__":
        import mplfinance as mpf
        # prepare markers for mplfinance

        # mark upward runs with green
        up_marker = df["Close"].copy()
        up_marker[df["Direction"] != 1] = np.nan

        # mark downward runs with red
        down_marker = df["Close"].copy()
        down_marker[df["Direction"] != -1] = np.nan

        # create addplot objs
        ap_up = mpf.make_addplot(
            up_marker, type="scatter", markersize=50, marker="^", color="green"
        )
        ap_down = mpf.make_addplot(
            down_marker, type="scatter", markersize=50, marker="v", color="red"
        )

        # plot candlestick
        mpf.plot(
            df,
            type="candle",
            style="yahoo",
            volume=True,
            addplot=[ap_up, ap_down],
            title="TSLA stock Upward and Downward Runs",
            ylabel="Price ($)",
            ylabel_lower="Volume",
            mav=(12, 26),
        )
