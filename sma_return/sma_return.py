import pandas as pd
from pathlib import Path
from datetime import datetime

DATASET = pd.read_csv(Path(__file__).parent.parent / "test" / "data" / "TSLA.csv")

user_input_given_window_size = input("Given window size: ")
date_input = input("Enter date: ")
date = datetime.strptime(date_input, "%d-%m-%Y").date()

DATASET['Date'] = pd.to_datetime(DATASET['Date']).dt.date
matching_row = DATASET[DATASET['Date'] == date]

if matching_row.empty:
    print("No data found for the given date.")
else:
    print("Matching row(s):")
    print(matching_row)


