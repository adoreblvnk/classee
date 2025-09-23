import pandas as pd
import numpy as np

class HTMLFormatter:
   def html_format():
      pass

class SMA_Return:
   #def sma_return(df, window_size=5):
   def __init__(self, df):
      self.df: pd.DataFrame = df

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

      # start index = end index - window 
      calculate_start_index = end_date_index - window # starts the index at desired window

      sma_list = []

      for i in range(calculate_start_index, end_date_index):
         sma = 0

         # j = calculate from the window until the date itself
         for j in range(i - window, i): 
            if sma == 0:
               sma += date_range_data[j] + date_range_data[j+1]
            elif j != i:
               sma += date_range_data[j+1]
            else:
               sma += date_range_data[i]
         sma_list.append(sma / window)
         
      for idx, i in enumerate(sma_list, calculate_start_index):
         data.loc[dates[idx], "SMA"] = i

      return data

   def get_date_data(self, date_range = "07-03-2022, 18-03-2022"):
      start_date, end_date = date_range.split(",")

      date_format = "%d-%m-%Y"
      # ensure all three date object is to be the same type
      self.df["Date"] = pd.to_datetime(self.df["Date"])
      start_date = pd.to_datetime(start_date.lstrip(), format=date_format)
      end_date = pd.to_datetime(end_date.lstrip(), format=date_format)

      date_range_filtering = (self.df["Date"] >= start_date) & (self.df["Date"] <= end_date)
      return self.df.loc[date_range_filtering].copy()

   def format_data(self, sma: pd.DataFrame):
      formatted = sma[["Date", "Adj Close", "SMA"]]
      res = formatted.to_dict(orient="records")
      # formatted_data_df = pd.DataFrame(res)
      # html_table = formatted_data_df.to_html(index=False, border=2)

      # return f"""
      #          <html>
      #             <body>
      #                   {html_table}
      #             </body>
      #          </html>
      #          """

      return res
   
   def sma_return(self, start_date = None, end_date = None, window_size=5):
      self.window_size = window_size
      date_data = self.get_date_data()
      sma = self.calculate_sma(date_data)
      res = self.format_data(sma)
      
      return res
        