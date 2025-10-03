import pandas as pd
import warnings

warnings.filterwarnings("ignore")


# load dataset
class Daily_Return:
    def validation(df):
        if df is None or df.empty:
            raise ValueError("DataFrame is empty or None")
        if not isinstance(df, pd.DataFrame):
            raise TypeError("Input must be a pandas DataFrame")
        if "Date" not in df.columns or "Close" not in df.columns:
            raise ValueError("DataFrame must contain 'Date' and 'Close' columns")
        if df["Close"].isnull().any():
            raise ValueError("'Close' column contains null values")
        if not pd.api.types.is_numeric_dtype(df["Close"]):
            raise TypeError("'Close' column must be numeric")
        if len(df) < 2:
            raise ValueError("DataFrame must contain at least two rows to calculate daily return")
        return True
    def test_validation(df):
        df = pd.DataFrame(df) 
        df['Daily_Return'] = df['Close'].pct_change() * 100
        df["Daily_Return"][0]=0
        df["Daily_Return"]=df["Daily_Return"].round(2)  
        df = df[["Date", "Close", "Daily_Return"]]
        return {
            "mean": float(round(df['Daily_Return'].mean(),2)),
            "std": float(round(df['Daily_Return'].std(),2)),
            "max": float(round(df['Daily_Return'].max(),2)),
            "min": float(round(df['Daily_Return'].min(),2)),
            "post_day": float(round((df['Daily_Return'] > 0).sum(),2)),
            "neutral_day":float(round((df['Daily_Return'] == 0).sum(),2)),
            "neg_day":float(round((df['Daily_Return'] < 0).sum(),2)),
            "total_trading_days": len(df),
            "win_rate":float(round((df['Daily_Return'] > 0).sum() / len(df) * 100,2)),
            "daily_return": df["Daily_Return"].tolist(),
        }

    def daily_return(df):
        try:
            Daily_Return.validation(df)
            df = pd.DataFrame(df)
            if "Close" not in df.columns:
                raise ValueError("DataFrame must contain 'Close' column")
            # ... rest of calculation
            df = pd.DataFrame(df)
        #putting data in dataframe under pandas library to make it easier to manipulate
            for x in range(len(df["Close"])):#for loop to calculate daily return
                if x == 0:
                    df.loc[x, "Daily_Return"] = 0  #for the first day, daily return is 0 since there is no previous day
                else:
                    df["Daily_Return"][x] = ((df["Close"][x] - df["Close"][x - 1]) / df["Close"][x - 1] * 100)# the formula to calculate daily return
                    #df['Daily_Return'] = df['Close'].pct_change() * 100 pandas has a built in function to calculate daily return but we are not using it here
                    # we are calculating it manually to show that we understand the concept behind it by calculating it manually
                    df["Daily_Return"] = df["Daily_Return"].round(2)# rounding to 2 decimal places for better readability
            df = df[["Date", "Close", "Daily_Return"]]#returning only the relevant columns for daily return analysis
            # calculating mean of daily return
            #there is also a built-in function in pandas to calculate mean but we are not using it here
            #we are calculating it manually to show that we understand the concept behind it by calculating it manually
            mean = 0
            for x in range(len(df["Daily_Return"])):
                mean = mean + df["Daily_Return"][x]
            mean = mean / len(df["Daily_Return"])
            # calculating standard deviation of daily return
            # formula: sqrt( sum( (x_i - mean)^2 ) / (N - 1) )
            # where N is the number of observations
            # use Bessel's correction
            # we use sample standard deviation as we are using a sample of the
            # population.
            # while it seems like we use the entire dataset (in terms of years), in
            # statistics, the true population includes the past, present, & FUTURE
            std_div = 0
            for x in range(len(df["Daily_Return"])):
                std_div = std_div + (df["Daily_Return"][x] - mean) ** 2
            std_div = (std_div / (len(df["Daily_Return"]) - 1)) ** 0.5
            # calculating max and min daily return
            # max and min are useful to know the best and worst days in terms of daily return for a stock
            # there is also a built-in function in pandas to calculate max and min but we are not using it here
            # we are calculating it manually to show that we understand the concept behind it by calculating it manually
            max = df["Daily_Return"][0]
            for x in range(len(df["Daily_Return"])):
                if df["Daily_Return"][x] > max:
                    max = df["Daily_Return"][x]

            min = df["Daily_Return"][0]
            for x in range(len(df["Daily_Return"])):
                if df["Daily_Return"][x] < min:
                    min = df["Daily_Return"][x]
            # calculating number of positive and negative days
            # positive days are the days when the daily return is greater than 0
            # negative days are the days when the daily return is less than 0
            post_day = 0
            for x in range(len(df["Daily_Return"])):
                if df["Daily_Return"][x] > 0:
                    post_day = post_day + 1
            neutral_day=0
            for x in range(len(df["Daily_Return"])):
                if df["Daily_Return"][x]==0:
                    neutral_day=neutral_day+1
            neg_day = 0
            for x in range(len(df["Daily_Return"])):
                if df["Daily_Return"][x] < 0:
                    neg_day = neg_day + 1
            # calculating win rate
            # win rate is the percentage of positive days out of total trading days
            # it is useful to know the probability of making a profit on any given day
            win_rate = post_day / len(df) * 100
            # returning all the statistics in a dictionary
            # this is useful to return multiple values from a function
            # for displaying the statistics in a web app on FASTAPI
            # and for daily return list for plotting the daily return graph so only the relevant columns are returned
            # .tolist() is used to convert the pandas series to a list for easy manipulation in the web app
            # rounding all statistics to 2 decimal places for better readability
        except Exception as e:
            return {"error": str(e)}
        return {
            "mean": float(round(mean, 2)),
            "std": float(round(std_div, 2)),
            "max": float(round(max, 2)),
            "min": float(round(min, 2)),
            "post_day": float(round(post_day,2)),
            "neutral_day": float(round(neutral_day,2)),
            "neg_day": float(round(neg_day,2)),
            "total_trading_days": len(df),
            "win_rate": float(round(win_rate, 2)),
            "daily_return": df["Daily_Return"].tolist(),
        }
if __name__ == "__main__":
    # Example usage
    data = {
        "Date": ["2023-01-01", "2023-01-02", "2023-01-03", "2023-01-04"],
        "Close": [100, 102, 101, 105],
    }
    df=pd.DataFrame(data)
    print("test result with built-in function")
    test_result=Daily_Return.test_validation(df)
    print(test_result)
    print("daily return result with manual algorithm")
    result = Daily_Return.daily_return(df)
    print(result)
