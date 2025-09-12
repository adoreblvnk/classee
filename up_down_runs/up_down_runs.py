import pandas as pd
import mplfinance as mpf
import numpy as np 

# Loading dataset

# CSV format: Date,Open,High,Low,Close,Volume
df = pd.read_csv("TSLA.csv")

# Ensuring Date column is datetime type and sort by date
df['Date'] = pd.to_datetime(df['Date'])
df = df.sort_values('Date')
df.set_index('Date', inplace=True)

# Date range
df = df['2022-01-01':'2022-03-24']

# Calculate the daily changes (close - prev close)
df['Change'] = df['Close'].diff()

# Label each day as Up (+1), Down (-1), or No Change (0)
df['Direction'] = df['Change'].apply(lambda x: 1 if x > 0 else (-1 if x < 0 else 0))

# Count runs (consecutive streaks of +- 1)
runs = []
current_dir = 0
current_length = 0

for d in df['Direction']:
    if d == current_dir and d != 0: # continuing a run
        current_length += 1
    elif d != 0: # new run starts 
        current_dir = d
        current_length = 1
    else: # no change day
        current_dir = 0
        current_length = 0
    runs.append(current_length)

df['Run_Length'] = runs

# Find longest upward and downard runs
longest_up = df[df['Direction'] == 1]['Run_Length'].max()
longest_down = df[df['Direction'] == -1]['Run_Length'].max()

print("Longest Upward Run: ", longest_up)
print("Longest Downward Run: ", longest_down)

# Visualisation
# Preparing markers for mplfinance

# Upward runs: mark with green
up_marker = df['Close'].copy()
up_marker[df['Direction'] != 1] = np.nan

# Downard runs: mark with red
down_marker = df['Close'].copy()
down_marker[df['Direction'] != -1] = np.nan

# Create addplot objects
ap_up = mpf.make_addplot(up_marker, type = 'scatter', markersize = 50, marker = '^', color = 'green')
ap_down = mpf.make_addplot(down_marker, type = 'scatter', markersize = 50, marker = 'v', color = 'red')

# Plot candlestick
mpf.plot(
    df,
    type='candle',
    style='yahoo',
    volume=True,
    addplot=[ap_up, ap_down],
    title="TSLA stock Upward and Downward Runs",
    ylabel="Price ($)",
    ylabel_lower="Volume",
    mav=(12,26)
)