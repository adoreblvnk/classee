import pandas as pd
import numpy as np
from validate import validate_dataset


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
    },df # return processed dataframe

if __name__ == "__main__":
    import pandas as pd
    import os

    print("MAIN FUNCTIONALITY TEST")
    # Load CSV test
    csv_path = r"C:\Users\User\Documents\SIT\INF1002\Proj 1 Repo\stock-market-analysis\data\TSLA.csv"
    df = pd.read_csv(csv_path)

    result, processed_df = analyze_up_down_runs(df)
    print("Up/Down Runs Analysis Result:", result)

    # EDGE CASE UNIT TESTS

    print("\nEDGE CASE TESTS")

    # Test 1: Empty DataFrame
    print("\n[Edge Case 1] Empty DataFrame:")
    try:
        analyze_up_down_runs(pd.DataFrame())
    except ValueError as e:
        print("Caught error:", e)

    # Test 2: Missing required columns
    print("\n[Edge Case 2] Missing required columns:")
    try:
        bad_df = pd.DataFrame({"Close": [100, 101, 102]})  # no Date column
        analyze_up_down_runs(bad_df)
    except ValueError as e:
        print("Caught error:", e)

    # Test 3: Too few rows
    print("\n[Edge Case 3] Too few rows:")
    try:
        small_df = pd.DataFrame({"Date": ["2024-01-01"], "Close": [100]})
        analyze_up_down_runs(small_df)
    except ValueError as e:
        print("Caught error:", e)

    # Test 4: Constant price (no runs)
    print("\n[Edge Case 4] Constant price data:")
    const_df = pd.DataFrame({
        "Date": pd.date_range("2024-01-01", periods=5),
        "Close": [100, 100, 100, 100, 100]
    })
    const_result, _ = analyze_up_down_runs(const_df)
    print("Result:", const_result)

    #  ASSERT TEST (optional but good for report)

    print("\nASSERT TEST")
    # Expected result: longest run lengths should never exceed total days
    assert result["longest_up"]["length"] <= len(df), " Longest up run is invalid!"
    assert result["longest_down"]["length"] <= len(df), " Longest down run is invalid!"
    print(" Assert checks passed. Longest runs are within valid range.")



# Optional visualisation
if __name__ == "__main__":
    import mplfinance as mpf
    import pandas as pd

    # ensure datetime index
    processed_df["Date"] = pd.to_datetime(processed_df["Date"])
    processed_df = processed_df.set_index("Date")

    # prepare markers for mplfinance
    up_marker = processed_df["Close"].copy()
    up_marker[processed_df["Direction"] != 1] = np.nan

    down_marker = processed_df["Close"].copy()
    down_marker[processed_df["Direction"] != -1] = np.nan

    # create addplot objects
    ap_up = mpf.make_addplot(
        up_marker, type="scatter", markersize=50, marker="^", color="green"
    )
    ap_down = mpf.make_addplot(
        down_marker, type="scatter", markersize=50, marker="v", color="red"
    )

    # plot candlestick chart
    mpf.plot(
        processed_df,
        type="candle",
        style="yahoo",
        volume=True,
        addplot=[ap_up, ap_down],
        title="TSLA Upward and Downward Runs",
        ylabel="Price ($)",
        ylabel_lower="Volume",
        mav=(12, 26),
    )

