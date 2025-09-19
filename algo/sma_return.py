import pandas as pd
from datetime import datetime


class SMA_Return:
    #def sma_return(df, window_size=5):
    def sma_return(df, start_date = None, end_date = None, window_size=5):
        # date_input = input("Enter start date of window size: ")

        if start_date and end_date:
            df = df[(df["Date"] >= start_date) & (df["Date"] <= end_date)]
            # df = df[start_date:end_date]
            
        date_input = "29-06-2010"  # hardcoded for now
        date = datetime.strptime(date_input, "%d-%m-%Y").date()

        df["Date"] = pd.to_datetime(df["Date"]).dt.date
        matching_row = df[df["Date"] == date]

        if matching_row.empty:
            print("No data found for the given date.")
        else:
            print("Matching row(s):")
            print(matching_row)

            start_index = matching_row.index.values.astype(int)[0]
            end_index = start_index + window_size
            print("end index", end_index)

            # get next n rows
            window_data = df.loc[start_index : end_index - 1]["Adj Close"].tolist()
            sma = sum(window_data) / window_size

            return {"window_data": window_data, "sma": sma}
