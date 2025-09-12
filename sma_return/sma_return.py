import pandas as pd
from pathlib import Path
from datetime import datetime

class SMA_Return:
    def sma_return():
        DATASET = pd.read_csv(Path(__file__).parent.parent / "data" / "TSLA.csv")

        input_window_size = int(input("Give window size: "))
        # date_input = input("Enter start date of window size: ")
        date_input = "29-06-2010" #hardcoded for now
        date = datetime.strptime(date_input, "%d-%m-%Y").date()

        DATASET['Date'] = pd.to_datetime(DATASET['Date']).dt.date
        matching_row = DATASET[DATASET['Date'] == date]

        if matching_row.empty:
            print("No data found for the given date.")
        else:
            print("Matching row(s):")
            print(matching_row)

            start_index = matching_row.index.values.astype(int)[0]
            end_index = start_index + int(input_window_size)
            print('end index', end_index)

            #get next n rows
            window_data = DATASET.loc[start_index:end_index-1]['Adj Close'].tolist()
            sma = sum(window_data) / input_window_size
            
            return {"window_data": window_data, "sma": sma}

    
    


