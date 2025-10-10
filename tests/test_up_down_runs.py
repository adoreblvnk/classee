import pandas as pd
import pytest
from algo.up_down_runs import analyze_up_down_runs

@pytest.fixture
def sample_stock_data():
    """Provides a diverse sample DataFrame for run testing."""
    data = {
        "Date": pd.to_datetime([
            "2023-01-01", "2023-01-02", "2023-01-03", "2023-01-04", # Down run (2)
            "2023-01-05", "2023-01-06", "2023-01-07", "2023-01-08", # Up run (3)
            "2023-01-09", # Flat day
            "2023-01-10", "2023-01-11"  # Down run (1)
        ]),
        "Close": [105, 102, 100, 100, 105, 110, 115, 112, 112, 110, 108]
    }
    return pd.DataFrame(data)

def test_up_down_runs_standard_scenario(sample_stock_data):
    """
    Tests the run analysis on a typical dataset with mixed runs.
    """
    result = analyze_up_down_runs(sample_stock_data)
    
    # Expected: Longest up run is 3 days, longest down is 2 days.
    assert result["longest_up"]["length"] == 3
    assert result["longest_up"]["start_date"] == "2023-01-04"
    assert result["longest_up"]["end_date"] == "2023-01-07"
    
    assert result["longest_down"]["length"] == 2
    assert result["longest_down"]["start_date"] == "2023-01-01"
    assert result["longest_down"]["end_date"] == "2023-01-03"
    
    assert result["total_upward_runs"] == 1
    assert result["total_downward_runs"] == 3 # three separate down runs

def test_up_down_runs_all_upward():
    """
    Tests a scenario where prices only increase.
    """
    data = {
        "Date": pd.to_datetime(["2023-01-01", "2023-01-02", "2023-01-03", "2023-01-04"]),
        "Close": [100, 102, 105, 110]
    }
    df = pd.DataFrame(data)
    result = analyze_up_down_runs(df)
    
    assert result["longest_up"]["length"] == 3
    assert result["longest_down"]["length"] == 0
    assert result["total_upward_runs"] == 1
    assert result["total_downward_runs"] == 0

def test_up_down_runs_alternating_days():
    """
    Tests a scenario with no runs longer than 1 day.
    """
    data = {
        "Date": pd.to_datetime(["2023-01-01", "2023-01-02", "2023-01-03", "2023-01-04"]),
        "Close": [100, 102, 101, 103]
    }
    df = pd.DataFrame(data)
    result = analyze_up_down_runs(df)
    
    assert result["longest_up"]["length"] == 1
    assert result["longest_down"]["length"] == 1
    assert result["total_upward_runs"] == 2
    assert result["total_downward_runs"] == 1

def test_up_down_runs_with_flat_days(sample_stock_data):
    """
    Ensures that days with no price change correctly break streaks.
    """
    # The fixture already contains flat days.
    # The down run from 2023-01-03 is length 2, not 3, because of the flat day.
    # The up run from 2023-01-04 is length 3, and is broken by a down day.
    # A new down run starts after the second flat day.
    result = analyze_up_down_runs(sample_stock_data)
    
    # Asserting the main scenario again confirms flat days are handled correctly.
    assert result["longest_up"]["length"] == 3
    assert result["longest_down"]["length"] == 2