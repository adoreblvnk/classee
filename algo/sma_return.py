import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


class SMA_Return:
    # def sma_return(df, window_size=5):
    def __init__(self, df):
        self.df: pd.DataFrame = df

    def make_chart_dir(self):
        import os

        os.makedirs("chart", exist_ok=True)

    def date_formatter(self, date):
        date_format = "%d-%m-%Y"
        return pd.to_datetime(date.lstrip(), format=date_format)

    def calculate_sma(self, data: pd.DataFrame):

        window = self.window_size
        dates = data.index
        date_len = len(dates)

        if window < 0 or window > date_len:
            return f"Window size should be > 0 and <= {len(dates)}"
        
        if date_len <= 2:
            return f"Please enter a larger range of dates"

        prices = data["Adj Close"].to_numpy()

        # set all to NaN
        data["SMA"] = np.nan

        # get first_run price
        run_data = 0

        # sum of first run of previous n window
        for i in prices[:window]:
            run_data += i

        # lookup the first date according to window, average it and set the SMA accordingingly
        data.loc[dates[window - 1], "SMA"] = run_data / window

        # start from second window since we had already calculated the first window
        # formula = subsequent_runs + current window - previous window
        for i in range(window, date_len):
            run_data += prices[i] - prices[i - window]
            data.loc[dates[i], "SMA"] = run_data / window

        return data

    def get_date_data(self, start_date, end_date):
        # ensure all three date object is to be the same type
        self.df["Date"] = pd.to_datetime(self.df["Date"])
        start_date = self.date_formatter(start_date)
        end_date = self.date_formatter(end_date)

        date_range_filtering = (self.df["Date"] >= start_date) & (
            self.df["Date"] <= end_date
        )
        return self.df.loc[date_range_filtering].copy()

    # def format_data(self, sma: pd.DataFrame):
    #     formatted = sma[["Date", "Adj Close", "SMA"]]
    #     res = formatted.to_dict(orient="records")

    #     return res

    def plot_chart(self, sma_data):
        df = pd.DataFrame(sma_data)

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

    def sma_return(self, start_date=None, end_date=None, window_size=None):
        default_window_size = 5 if window_size is None else window_size
        self.window_size = default_window_size

        start_date = "01-03-2022" if start_date is None else start_date
        end_date = "24-03-2022" if end_date is None else end_date

        date_data = self.get_date_data(start_date, end_date)
        sma = self.calculate_sma(date_data)
        # res = self.format_data(sma)

        if type(sma) == pd.DataFrame:
            self.plot_chart(sma)
            return {"img": "chart.png"}

        else:
            return {"error": sma}
