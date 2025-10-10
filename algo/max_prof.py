import pandas as pd
from typing import Dict, Any, Optional
from .validate import validate_dataset


class MaxProf:
    """class to calc max prof from stock prices

    provides funcs to calc max prof from buying & selling stocks multiple times
    """

    def maxProfit(
        self, df: pd.DataFrame, window_size: Optional[int] = None
    ) -> Dict[str, Any]:
        """calcs max prof from buying & selling stocks in a rolling window

        uses sliding window technique to find most profitable window

        args:
            df: df with 'Date' and 'Adj Close' cols
            window_size: num days in window, default to entire df

        returns:
            dict with max_profit, window dates, and chart data

        NOTE: uses sliding window algorithm with O(n) time complexity and O(n) space
        where n is number of days. space is O(n) due to storing daily profits.
        """
        # validate dataset to ensure required cols and sane size
        validate_dataset(df, required_cols=["Date", "Adj Close"], min_rows=2)
        # default to full df if no window size
        if window_size is None:
            window_size = len(df)
            print(f"window size: {window_size}")

        # convert to lists for faster access
        prices = df["Adj Close"].tolist()
        dates = df["Date"].tolist()

        # handle edge cases: window too small or df too small
        if window_size <= 1 or len(df) < window_size:
            return {
                "max_profit": 0.0,
                "best_window_start_date": None,
                "best_window_end_date": None,
                "all_dates": dates,
                "all_prices": prices,
                "buy_points": [],
                "sell_points": [],
            }

        # greedy algo: pre-compute daily profits (0 if price doesn't increase)
        # O(n) time, O(n) space
        daily_profits = []
        for i in range(1, len(df)):
            profit_today = prices[i] - prices[i - 1]
            daily_profits.append(profit_today if profit_today > 0 else 0)

        # window of N days has N-1 possible trades
        profit_window_size = window_size - 1

        # calculate profit for first window
        # O(window_size) time, O(1) space
        current_profit = sum(daily_profits[0:profit_window_size])
        max_profit = current_profit
        best_window_start_idx = 0

        # slide window through all possible positions
        # O(n) time, O(1) space
        # each iteration is O(1) since we just adjust window sum
        for i in range(1, len(df) - window_size + 1):
            # remove profit from day leaving window
            profit_leaving = daily_profits[i - 1]
            # add profit from new day entering window
            profit_entering = daily_profits[i + profit_window_size - 1]
            current_profit = current_profit - profit_leaving + profit_entering

            # update max profit if current window is better
            if current_profit > max_profit:
                max_profit = current_profit
                best_window_start_idx = i

        # get dates for best window
        best_start_date = dates[best_window_start_idx]
        best_end_date = dates[best_window_start_idx + window_size - 1]

        # reconstruct trade points for visualization in the best window
        # O(window_size) time, O(k) space where k is number of trades
        buy_points = []
        sell_points = []
        for i in range(best_window_start_idx + 1, best_window_start_idx + window_size):
            if prices[i] > prices[i - 1]:
                buy_points.append({"x": dates[i - 1], "y": prices[i - 1]})
                sell_points.append({"x": dates[i], "y": prices[i]})

        return {
            "max_profit": max_profit,
            "best_window_start_date": best_start_date,
            "best_window_end_date": best_end_date,
            "buy_points": buy_points,
            "sell_points": sell_points,
        }
