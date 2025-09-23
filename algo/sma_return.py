import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

class SMA_Return:
   #def sma_return(df, window_size=5):
   def __init__(self, df):
      self.df: pd.DataFrame = df

   def date_formatter(self, date):
      date_format = "%d-%m-%Y"
      return pd.to_datetime(date.lstrip(), format=date_format)

   def calculate_sma(self, data: pd.DataFrame):

      # calculate SMA for each date
      # formula = sum of the inclusive date and the previous n dates where n = window - 1

      date_range_data = data["Adj Close"].tolist()
      window = self.window_size
      dates = data.index.tolist()

      # set all to NaN
      data["SMA"] = np.nan

      # get latest index
      # we're calculating until here
      end_date_index = len(date_range_data)

      # start index
      calculate_start_index = window # starts the index n_window to calculate the historical sma

      sma_list = []

      # we -1 because thats the first index that SMA can be calculated
      # due to us needing n_window value inclusive of the date itself
      for i in range(calculate_start_index - 1, end_date_index):
         sma = 0

         # starting point of the rolling window, ending at i
         for j in range((i - window) + 1, i): 
            if sma == 0:
               sma += date_range_data[j] + date_range_data[j+1]
            elif j != i:
               sma += date_range_data[j+1]
            else:
               sma += date_range_data[i]
         sma_list.append(sma / window)
         # print(sma)
      for idx, i in enumerate(sma_list, calculate_start_index - 1):
         data.loc[dates[idx], "SMA"] = i

      return data

   def get_date_data(self, start_date = "01-03-2022", end_date = "24-03-2022"):
      # ensure all three date object is to be the same type
      self.df["Date"] = pd.to_datetime(self.df["Date"])
      start_date = self.date_formatter(start_date)
      end_date = self.date_formatter(end_date)

      date_range_filtering = (self.df["Date"] >= start_date) & (self.df["Date"] <= end_date)
      return self.df.loc[date_range_filtering].copy()

   def format_data(self, sma: pd.DataFrame):
      formatted = sma[["Date", "Adj Close", "SMA"]]
      res = formatted.to_dict(orient="records")

      return res
   # def plot_chart(self, sma_data):
   #    df = pd.DataFrame(sma_data)

   #    plt.figure(figsize=(10,5))
   #    plt.plot(df["Date"], df["Adj Close"], label="Adj Close", marker="o")
   #    plt.plot(df["Date"], df["SMA"], label="SMA (window=5)", linestyle="--", color="orange")

   #    plt.title("Stock Prices with 5-Day SMA")
   #    plt.xlabel("Date")
   #    plt.ylabel("Price")
   #    plt.legend()
   #    plt.grid(True)
   #    plt.xticks(rotation=45)
   #    plt.tight_layout()
   #    plt.show()

   def sma_return(self, start_date = None, end_date = None, window_size=None):
      default_window_size = 5 if window_size is None else window_size
      self.window_size = default_window_size

      start_date = "01-03-2022" if start_date is None else start_date
      end_date = "24-03-2022" if end_date is None else end_date

      date_data = self.get_date_data(start_date, end_date)
      sma = self.calculate_sma(date_data)
      res = self.format_data(sma)
      # self.plot_chart(sma)
      
      data_dict = {
         "sma_data": res,
         "sma_window": self.window_size
      }
      return data_dict
        