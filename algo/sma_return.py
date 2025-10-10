import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from .validate import validate_dataset

plt.switch_backend("agg")


class SMA_Return:
    # def sma_return(df, window_size=5):
    def __init__(self, df):
        self.df: pd.DataFrame = df
        self.data_filtered: pd.DataFrame = None

    def make_chart_dir(self):
        import os

        os.makedirs("chart", exist_ok=True)

    def date_formatter(self, date):
        date_format = "%d-%m-%Y"
        return pd.to_datetime(date.lstrip(), format=date_format)

    def calculate_sma(self):
        data = self.data_filtered
        window = self.window_size
        dates = data.index
        date_len = len(dates)

        if date_len <= 4:
            return "Please enter a larger range of dates"

        if window > date_len:
            return f"Window size should be < {date_len}"

        if window < 2:
            return "Window size should be > 2"

        prices = data["Adj Close"].to_numpy()

        # set all to NaN
        data["SMA"] = np.nan

        # get first_run price
        run_data = 0

        # sum of first run of previous n window
        for i in prices[:window]:
            run_data += i

        # lookup the first date according to window, average it and set the SMA accordingingly
        data.loc[dates[window - 1], "SMA"] = round(float(run_data / window), 2)

        # start from second window since we had already calculated the first window
        # formula = (subsequent_runs + current window - previous window) / window
        for i in range(window, date_len):
            run_data += prices[i] - prices[i - window]
            data.loc[dates[i], "SMA"] = round(float(run_data / window), 2)
        return data

    def get_date_data(self, start_date, end_date):
        # ensure all three date object is to be the same type
        self.df["Date"] = pd.to_datetime(self.df["Date"])
        start_date = self.date_formatter(start_date)
        end_date = self.date_formatter(end_date)

        date_range_filtering = (self.df["Date"] >= start_date) & (
            self.df["Date"] <= end_date
        )
        self.data_filtered = self.df.loc[date_range_filtering].copy()

        return self.data_filtered

    # def format_data(self, sma: pd.DataFrame):
    #     formatted = sma[["Date", "Adj Close", "SMA"]]
    #     res = formatted.to_dict(orient="records")

    #     return res

    def plot_chart(self, sma_data: pd.DataFrame):
        self.make_chart_dir()
        chart_name = "chart.png"
        df = sma_data

        plt.figure(figsize=(10, 5))
        plt.plot(df["Date"], df["Adj Close"], label="Adj Close", marker="o")
        plt.plot(
            df["Date"],
            df["SMA"],
            label=f"SMA (window={self.window_size})",
            linestyle="-",
            color="orange",
        )

        plt.title(f"Stock Prices with {self.window_size}-Day SMA")
        plt.xlabel("Date")
        plt.ylabel("Price")
        plt.legend()
        plt.grid(True)
        plt.xticks(rotation=45)
        plt.tight_layout()
        plt.savefig("chart/chart.png", dpi=150)
        plt.close()

        return chart_name

    def sma_return(self, start_date=None, end_date=None, window_size=None):
        """
        Smoothens out the price history according to window size.

        Parameters:
            start_date (str, optional): Filter start date (dd-mm-yyyy).
            end_date (str, optional): Filter end date (dd-mm-yyyy).
            window_size (int, optional): Sets the window size of past periods

        Returns:
            dict: A success return of the directory of the chart filename
            dict: An error returns the appropriate error message
        """
        default_window_size = 5 if window_size is None else window_size
        self.window_size = default_window_size

        start_date = "01-03-2022" if start_date is None else start_date
        end_date = "24-03-2022" if end_date is None else end_date

        # validate base df with required cols
        validate_dataset(self.df, required_cols=["Date", "Adj Close"], min_rows=2)
        self.get_date_data(start_date, end_date)
        sma = self.calculate_sma()
        # res = self.format_data(sma)

        if isinstance(sma, pd.DataFrame):
            chart_name = self.plot_chart(sma)
            return {"img": chart_name}

        else:
            return {"error": sma}
