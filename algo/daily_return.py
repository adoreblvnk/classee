import pandas as pd
from pathlib import Path
# load dataset
DATASET = pd.read_csv(Path(__file__).parent.parent / "data" / "TSLA.csv")
df = pd.DataFrame(DATASET)
for x in range(len(df['Close'])):
            if x == 0:
                df[['Daily_Return'][x]]=0;
            else:
             df['Daily_Return'][x]=(df['Close'][x]-df['Close'][x-1])/df['Close'][x-1]*100;

df=df[['Date', 'Close', 'Daily_Return']]

stats = {
            'Mean Return (%)': df['Daily_Return'].mean(),
            'Std Deviation (%)': df['Daily_Return'].std(),
            'Max Gain (%)': df['Daily_Return'].max(),
            'Max Loss (%)': df['Daily_Return'].min(),
            'Positive Days': (df['Daily_Return'] > 0).sum(),
            'Negative Days': (df['Daily_Return'] < 0).sum(),
            'Total Trading Days': len(df),
            'Win Rate (%)': (df['Daily_Return'] > 0).sum() / len(df) * 100
        }
for key, value in stats.items():
            print(f"{key}: {value:.2f}")