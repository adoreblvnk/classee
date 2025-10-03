import pandas as pd
import warnings
warnings.filterwarnings("ignore")

class Daily_Return:
    #checking if the dataset imported is valid before any calculations
    def validation(df):
        if df is None or df.empty:
            raise ValueError("DataFrame is empty or None")
        #cheching if data is empty
        if "Date" not in df.columns or "Close" not in df.columns:
            raise ValueError("DataFrame must contain 'Date' and 'Close' columns")
        #checking if there is date and closing price in dataset
        if df["Close"].isnull().any():
            raise ValueError("'Close' column contains null values")
        #checking if there is any empty entry in the data set
        if not pd.api.types.is_numeric_dtype(df["Close"]):
            raise TypeError("'Close' column must be numeric")
        #checking the data in close is in numbers or words
        if len(df) < 2:
            raise ValueError("DataFrame must contain at least two rows to calculate daily return")
        #checking if there is enough rows to calculate daily return, daily return needs at least 2 entry
        return True
    #for test case to show that the calculation of bulit in function are the same as manual calculation
    def test_validation(df):
        try:
            df = pd.DataFrame(df) 
            Daily_Return.validation(df)
            df['Daily_Return'] = df['Close'].pct_change() * 100
            #calculating daily return
            df["Daily_Return"]=df["Daily_Return"].round(2)  
            #rouding all data into 2 dp for better presentation
            df = df[ "Daily_Return"]
            df = df[1:]
        except Exception as e:
            return {"error": str(e)}
        return {
            "mean": float(round(df.mean(),2)),
            "std": float(round(df.std(),2)),
            "max": float(round(df.max(),2)),
            "min": float(round(df.min(),2)),
            "post_day": float(round((df > 0).sum(),2)),
            "neutral_day":float(round((df == 0).sum(),2)),
            "neg_day":float(round((df < 0).sum(),2)),
            "total_trading_days": len(df),
            "win_rate":float(round((df> 0).sum() / len(df) * 100,2)),
            "daily_return": df.tolist(),
        }

    def daily_return(df):
        try:
            df = pd.DataFrame(df)
            Daily_Return.validation(df)
            if "Close" not in df.columns:
                raise ValueError("DataFrame must contain 'Close' column")
            # ... rest of calculation
        #putting data in dataframe under pandas library to make it easier to manipulate
            for x in range(len(df["Close"])):#for loop to calculate daily return
                if x == 0:
                    df[['Daily_Return'][x]]=0;#setting first daily return as 0 for easy calculation removing the first row later
                else:
                    df["Daily_Return"][x] = ((df["Close"][x] - df["Close"][x - 1]) / df["Close"][x - 1] * 100)# the formula to calculate daily return
                    #df['Daily_Return'] = df['Close'].pct_change() * 100 pandas has a built in function to calculate daily return but we are not using it here
                    # we are calculating it manually to show that we understand the concept behind it by calculating it manually
                    df["Daily_Return"] = df["Daily_Return"].round(2)# rounding to 2 decimal places for better readability
            df = df[ "Daily_Return"]
            df=df[1:]
            #slicing of daily return to make sure that the first daily return value is not taken into cosideration
            #cannot just set the first daily return in to 0 as it will affect the calculation of stats later on 
            #returning only the relevant columns for daily return analysis
            # calculating mean of daily return
            #there is also a built-in function in pandas to calculate mean but we are not using it here
            #we are calculating it manually to show that we understand the concept behind it by calculating it manually
            mean = 0
            for x in range(len(df)):
                mean = mean + df[x+1]
            mean = mean / len(df)
            # calculating standard deviation of daily return
            # formula: sqrt( sum( (x_i - mean)^2 ) / (N - 1) )
            # where N is the number of observations
            # use Bessel's correction
            # we use sample standard deviation as we are using a sample of the
            # population.
            # while it seems like we use the entire dataset (in terms of years), in
            # statistics, the true population includes the past, present, & FUTURE
            std_div = 0
            for x in range(len(df)):
                std_div = std_div + (df[x+1] - mean) ** 2
            std_div = (std_div / (len(df) - 1)) ** 0.5
            # calculating max and min daily return
            # max and min are useful to know the best and worst days in terms of daily return for a stock
            # there is also a built-in function in pandas to calculate max and min but we are not using it here
            # we are calculating it manually to show that we understand the concept behind it by calculating it manually
            max = df[1]
            for x in range(len(df)):
                if df[x+1] > max:
                    max = df[x+1]

            min = df[1]
            for x in range(len(df)):
                if df[x+1] < min:
                    min = df[x+1]
            # calculating number of positive and negative days
            # positive days are the days when the daily return is greater than 0
            # negative days are the days when the daily return is less than 0
            post_day = 0
            for x in range(len(df)):
                if df[x+1] > 0:
                    post_day = post_day + 1
            neutral_day=0
            for x in range(len(df)):
                if df[x+1]==0:
                    neutral_day=neutral_day+1
            neg_day = 0
            for x in range(len(df)):
                if df[x+1] < 0:
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
        #returning all values that have been calculated for displaying of data at the front end 
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
            "daily_return": df.tolist(),
        }
if __name__ == "__main__":
    # Example usage to show that test validation and daily return have the same output
    data = {
    'Date': ['2024-06-14', '2024-06-13', '2024-06-12', '2024-06-11', '2024-06-10', '2024-06-07', '2024-06-06', '2024-06-05', '2024-06-04', '2024-06-03'],
    'Close': [178.01, 182.47, 174.95, 174.77, 169.57, 177.48, 174.91, 173.96, 174.77, 174.95]
    }
    print("test result with built-in function")
    test_result=Daily_Return.test_validation(data)
    print(test_result)
    print("daily return result with manual algorithm")
    result = Daily_Return.daily_return(data)
    print(result)
    if test_result==result:
        print(f"\nTest case validated,\n{test_result}\n is the same as \n{result}")
    else:
        print("there is a error some where")
