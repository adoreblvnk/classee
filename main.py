from fastapi import FastAPI, HTTPException, Request
from fastapi.responses import HTMLResponse
from fastapi.templating import Jinja2Templates
import uvicorn
import pandas as pd
from pathlib import Path
from algo.max_prof import MaxProf
from algo.sma_return import SMA_Return
from algo.up_down_runs import analyze_up_down_runs

# load dataset
DATASET = pd.read_csv(Path(__file__).parent / "data" / "TSLA.csv")

app = FastAPI()
templates = Jinja2Templates(directory="templates")


@app.get("/", response_class=HTMLResponse)
def root(request: Request):
    try:
        max_profit = MaxProf().maxProfit(DATASET.copy())
        sma_return_data = SMA_Return.sma_return(DATASET.copy())
        up_down_runs_data = analyze_up_down_runs(DATASET.copy())

        return templates.TemplateResponse(
            "index.html",
            {
                "request": request,
                "title": "Stock Market Analysis",
                "max_profit": max_profit,
                "sma_return": sma_return_data,
                "up_down_runs": {
                    "longest_up": up_down_runs_data[0],
                    "longest_down": up_down_runs_data[1],
                },
            },
        )
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


if __name__ == "__main__":
    uvicorn.run("main:app", host="127.0.0.1", port=8000, reload=True)
