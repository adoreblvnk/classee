from fastapi import FastAPI, HTTPException, Request
from fastapi.responses import HTMLResponse
from fastapi.templating import Jinja2Templates
import uvicorn
import pandas as pd
from pathlib import Path
from algo.max_prof import MaxProf
from algo.sma_return import SMA_Return
from algo.daily_return import Daily_Return
from algo.up_down_runs import analyze_up_down_runs
import datetime

# load dataset
DATASET = pd.read_csv(Path(__file__).parent / "data" / "TSLA.csv")

app = FastAPI()
templates = Jinja2Templates(directory="templates")


@app.get("/", response_class=HTMLResponse)
def root(request: Request, window_size: int = None, date_range_start: str = None, date_range_end: str = None):
    try:
        date_range_start = date_range_start.replace("/", "-") if date_range_start else None
        date_range_end = date_range_end.replace("/", "-") if date_range_end else None

        sma_return_data = SMA_Return(DATASET.copy()).sma_return(date_range_start, date_range_end)
        up_down_runs_data = analyze_up_down_runs(DATASET.copy())
        daily_return_data = Daily_Return.daily_return(DATASET.copy())

        current_window_size = window_size if window_size is not None else len(DATASET)
        max_profit = MaxProf().maxProfit(DATASET.copy(), current_window_size)

        return templates.TemplateResponse(
            "index.html",
            {
                "request": request,
                "title": "Stock Market Analysis",
                "all_dates": DATASET["Date"].tolist(),
                "all_prices": DATASET["Adj Close"].tolist(),
                "sma_return": sma_return_data,
                "up_down_runs": up_down_runs_data, 
                "daily_return": daily_return_data,
                "max_profit": {
                    "max_profit": max_profit,
                    "max_window_size": len(DATASET),  # for validation only
                    "current_window_size": current_window_size,
                },
            },
        )
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


if __name__ == "__main__":
    uvicorn.run("main:app", host="127.0.0.1", port=8000, reload=True)
