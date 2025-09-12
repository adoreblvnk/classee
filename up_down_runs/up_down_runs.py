import pandas as pd
import numpy as np


def analyze_up_down_runs(df, start_date=None, end_date=None):
    # ensure date col is datetime & sort
    df["Date"] = pd.to_datetime(df["Date"])
    df = df.sort_values("Date")
    df.set_index("Date", inplace=True)

    # Date range
    if start_date and end_date:
        df = df[start_date:end_date]

    # calc daily changes (close - prev close)
    df["Change"] = df["Close"].diff()

    # label days: Up (+1), Down (-1), No Change (0)
    df["Direction"] = df["Change"].apply(lambda x: 1 if x > 0 else (-1 if x < 0 else 0))

    # count runs (consecutive +-1 streaks)
    runs = []
    current_dir = 0
    current_length = 0

    for d in df["Direction"]:
        if d == current_dir and d != 0:  # continuing a run
            current_length += 1
        elif d != 0:  # new run starts
            current_dir = d
            current_length = 1
        else:  # no change day
            current_dir = 0
            current_length = 0
        runs.append(current_length)

    df["Run_Length"] = runs

    # find longest up/down runs
    longest_up = df[df["Direction"] == 1]["Run_Length"].max()
    longest_down = df[df["Direction"] == -1]["Run_Length"].max()

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

    return int(longest_up), int(longest_down)
