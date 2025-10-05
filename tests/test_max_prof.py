import pandas as pd
import pytest
from algo.max_prof import MaxProf

# sample data for testing
@pytest.fixture
def sample_data():
    return pd.DataFrame({
        "Date": pd.to_datetime([
            "2023-01-01", "2023-01-02", "2023-01-03", "2023-01-04",
            "2023-01-05", "2023-01-06", "2023-01-07", "2023-01-08"
        ]),
        "Adj Close": [105, 100, 110, 115, 120, 118, 122, 125]
    })

def test_max_profit_with_window(sample_data):
    """test max profit calculation with specific window size"""
    mp = MaxProf()
    result = mp.maxProfit(sample_data, window_size=5)
    # expected: max profit in any 5-day window is 20
    assert result["max_profit"] == 20
    assert result["best_window_start_date"] == pd.to_datetime("2023-01-01")
    assert result["best_window_end_date"] == pd.to_datetime("2023-01-05")

def test_max_profit_no_window(sample_data):
    """test max profit using entire dataset"""
    mp = MaxProf()
    result = mp.maxProfit(sample_data)
    # expected: total profit over all increasing days
    assert result["max_profit"] == 27  # 10 + 5 + 5 + 4 + 3
    assert result["best_window_start_date"] == pd.to_datetime("2023-01-01")
    assert result["best_window_end_date"] == pd.to_datetime("2023-01-08")

def test_max_profit_decreasing_prices():
    """test with continuously decreasing prices (no profit possible)"""
    df = pd.DataFrame({
        "Date": pd.to_datetime(["2023-01-01", "2023-01-02", "2023-01-03"]),
        "Adj Close": [100, 90, 80]
    })
    mp = MaxProf()
    result = mp.maxProfit(df)
    assert result["max_profit"] == 0

def test_max_profit_empty_dataframe():
    """test with empty dataframe (should raise error)"""
    df = pd.DataFrame({"Date": [], "Adj Close": []})
    mp = MaxProf()
    with pytest.raises(ValueError, match="dataframe is empty"):
        mp.maxProfit(df)

def test_max_profit_single_row():
    """test with single row (should raise error)"""
    df = pd.DataFrame({"Date": ["2023-01-01"], "Adj Close": [100]})
    mp = MaxProf()
    with pytest.raises(ValueError, match="dataframe must have at least 2 rows"):
        mp.maxProfit(df)
