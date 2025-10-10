import pandas as pd
import numpy as np
import pytest
from algo.sma_return import SMA_Return

@pytest.fixture
def sample_stock_data():
    """provides a sample dataframe for testing"""
    dates = """
                2022-03-07
                2022-03-08
                2022-03-09
                2022-03-10
                2022-03-11
                2022-03-14
                2022-03-15
                2022-03-16
                2022-03-17
                2022-03-18
                2022-03-21
                2022-03-22
                2022-03-23
                2022-03-24
            """
    adj_close = """
                    804.580017
                    824.400024
                    858.969971
                    838.299988
                    795.349976
                    766.369995
                    801.890015
                    840.229980
                    871.599976
                    905.390015
                    921.159973
                    993.979980
                    999.109985
                    1013.919983
                """

    return pd.DataFrame({
        "Date": pd.to_datetime(dates.split()),
        "Adj Close": [float(i) for i in adj_close.split()]
    }
    
)

def test_out_of_range_window(sample_stock_data):
    # tests for window size more than date length
    sma_test_obj = SMA_Return(sample_stock_data)
    res = sma_test_obj.sma_return(window_size=300)
    test_output = {"error": f"Window size should be < {len(sample_stock_data['Date'])}"}
    assert res == test_output

def test_larger_date_range(sample_stock_data):
    # tests if the minimum date range is entered
    sma_test_obj = SMA_Return(sample_stock_data)
    res = sma_test_obj.sma_return(start_date='07-03-2022', end_date='08-03-2022')
    test_output = {"error":f"Please enter a larger range of dates"}
    assert res == test_output

def test_window_len_lesser_than_date_len(sample_stock_data):
    # tests if the window len is lesser than date len
    sma_test_obj = SMA_Return(sample_stock_data)

    start_date = '07-03-2022'
    end_date = '15-03-2022'

    get_date_len = len(SMA_Return.get_date_data(sma_test_obj, start_date=start_date, end_date=end_date))

    res = sma_test_obj.sma_return(start_date='07-03-2022', end_date='15-03-2022', window_size=15)
    test_output = {"error":f"Window size should be < {get_date_len}"}
    assert res == test_output

def test_rolling_window_with_pandas(sample_stock_data):
    # tests if the sliding window algorithm is correct with pandas built in rolling func
    sma_test_obj = SMA_Return(sample_stock_data)
    sma_test_obj.window_size = 5
    
    data_filtered = sma_test_obj.get_date_data(start_date='07-03-2022', end_date='17-03-2022')
    sma_manual_calculation = sma_test_obj.calculate_sma()
    remove_na_manual = sma_manual_calculation.dropna()
    

    temp_copy_of_data = data_filtered.copy()
    temp_copy_of_data['SMA'] = np.nan
    temp_copy_of_data['SMA'] = sample_stock_data['Adj Close'].rolling(window=5).mean().round(2)
    remove_na_pandas = temp_copy_of_data.dropna()
    
    assert remove_na_manual['SMA'].tolist() == remove_na_pandas['SMA'].tolist()
    

