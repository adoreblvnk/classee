import pandas as pd
import pytest
from algo.daily_return import Daily_Return

@pytest.fixture
def sample_stock_data():
    """provides a sample dataframe for testing"""
    data = {
        "Date": pd.to_datetime(["2023-01-01", "2023-01-02", "2023-01-03", "2023-01-04", "2023-01-05"]),
        "Close": [100, 102, 99, 101, 101]
    }
    return pd.DataFrame(data)

def test_daily_return_with_valid_data(sample_stock_data):
    """tests calculation with a typical dataset using the manual daily_return method"""
    result = Daily_Return.daily_return(sample_stock_data)
    
    # expected values are based on the manual calculations in the old code
    assert result["daily_return"] == [2.0, -2.94, 2.02, 0.0]
    assert result["mean"] == 0.27
    assert result["std"] == 2.34
    assert result["max"] == 2.02
    assert result["min"] == -2.94
    assert result["post_day"] == 2
    assert result["neutral_day"] == 1
    assert result["neg_day"] == 1
    assert result["total_return_days"] == 4
    assert result["win_rate"] == 50.0

def test_manual_vs_pandas_implementation(sample_stock_data):
    """compares the manual implementation against the pandas-based one"""
    manual_result = Daily_Return.daily_return(sample_stock_data)
    pandas_result = Daily_Return.test_validation(sample_stock_data)
    
    # we expect the core metrics to be the same
    assert manual_result["mean"] == pandas_result["mean"]
    assert manual_result["std"] == pandas_result["std"]
    assert manual_result["max"] == pandas_result["max"]
    assert manual_result["min"] == pandas_result["min"]
    assert manual_result["daily_return"] == pandas_result["daily_return"]

def test_calculate_with_insufficient_data():
    """tests with a dataframe that is too small to calculate returns"""
    data = {"Date": ["2023-01-01"], "Close": [100]}
    df = pd.DataFrame(data)
    result = Daily_Return.daily_return(df)
    
    assert "error" in result
    assert result["error"] == "dataframe must have at least 2 rows"

def test_calculate_with_missing_columns():
    """tests with a dataframe missing the required 'Close' column"""
    data = {"Date": ["2023-01-01", "2023-01-02"], "Open": [100, 102]}
    df = pd.DataFrame(data)
    result = Daily_Return.daily_return(df)
    
    assert "error" in result
    assert "missing required columns" in result["error"]

def test_calculate_with_zero_prices():
    """tests with a dataframe containing a zero price, which should result in inf return"""
    data = {
        "Date": pd.to_datetime(["2023-01-01", "2023-01-02", "2023-01-03"]),
        "Close": [100, 0, 100]
    }
    df = pd.DataFrame(data)
    result = Daily_Return.daily_return(df)
    
    # with the fix, the code should produce 0.0 instead of 'inf'
    assert result["daily_return"][1] == 0.0
    assert result["max"] == 0.0
