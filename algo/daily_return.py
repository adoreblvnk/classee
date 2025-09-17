import pandas as pd
import warnings
warnings.filterwarnings('ignore')
# load dataset
class Daily_Return:
    
    def daily_return(df):
        df = pd.DataFrame(df)
        for x in range(len(df['Close'])):
                if x == 0:
                    df[['Daily_Return'][x]]=0;
                else:
                    df['Daily_Return'][x]=(df['Close'][x]-df['Close'][x-1])/df['Close'][x-1]*100;
        df=df[['Date','Close', 'Daily_Return']]
        mean=0
        for x in range(len(df['Daily_Return'])):
            mean=mean+df['Daily_Return'][x]
        mean=mean/len(df['Daily_Return'])
        
        std_div=0
        for x in range(len(df['Daily_Return'])):
            std_div=std_div+(df['Daily_Return'][x]-mean)**2
        std_div=(std_div/len(df['Daily_Return']))**0.5
        
        max=df['Daily_Return'][0]
        for x in range(len(df['Daily_Return'])):
            if df['Daily_Return'][x]>max:
                max=df['Daily_Return'][x]
        
        min=df['Daily_Return'][0]
        for x in range(len(df['Daily_Return'])):
            if df['Daily_Return'][x]<min:
                min=df['Daily_Return'][x]
        post_day=0
        for x in range(len(df['Daily_Return'])):
            if df['Daily_Return'][x]>0:
                post_day=post_day+1
        neg_day=0
        for x in range(len(df['Daily_Return'])):
            if df['Daily_Return'][x]<0:
                neg_day=neg_day+1
        win_rate=post_day/len(df)*100
        return {
                'mean': float(round(mean,2)),
                'std': float(round(std_div,2)),
                'max': float(round(max,2)),
                'min': float(round(min,2)),
                'post_day': post_day,
                'neg_day': neg_day,
                'total_trading_days': len(df),
                'win_rate': float(round(win_rate,2)),
                "daily_return": df['Daily_Return'].tolist(),
            }