import pandas as pd
from typing import Iterable, Optional


def validate_dataset(
    df: pd.DataFrame,
    required_cols: Optional[Iterable[str]] = None,
    min_rows: int = 2,
) -> bool:
    """validates input df used by algo modules

    args:
        df: pandas dataframe to validate
        required_cols: iterable of required column names (e.g. ['Date','Close'] or ['Date','Adj Close'])
        min_rows: minimum number of rows required in df

    returns:
        True if df passes validation, otherwise raises

    notes:
        - does not mutate df
        - checks for empty df, required cols, nulls, and numeric dtype for non-date cols
    """
    if df is None:
        raise ValueError("dataframe is None")

    if not isinstance(df, pd.DataFrame):
        raise TypeError("input must be a pandas dataframe")

    if df.empty:
        raise ValueError("dataframe is empty")

    if len(df) < min_rows:
        raise ValueError(f"dataframe must have at least {min_rows} rows")

    if required_cols:
        missing = [c for c in required_cols if c not in df.columns]
        if missing:
            raise ValueError(f"missing required columns: {missing}")

        # basic dtype/null checks for required columns
        for col in required_cols:
            if df[col].isnull().any():
                raise ValueError(f"column '{col}' contains null values")
            if col.lower() != "date":
                if not pd.api.types.is_numeric_dtype(df[col]):
                    raise TypeError(f"column '{col}' must be numeric")

    return True
