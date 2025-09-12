from fastapi import FastAPI, HTTPException
import uvicorn
import pandas as pd
from pathlib import Path
from max_prof.max_prof import MaxProf
from sma_return.sma_return import SMA_Return
from up_down_runs.up_down_runs import analyze_up_down_runs

# load dataset
DATASET = pd.read_csv(Path(__file__).parent / "data" / "TSLA.csv")

app = FastAPI()


@app.get("/")
def root():
    return {"message": "Hello World"}


@app.get("/max-profit")
def get_max_profit():
    try:
        return MaxProf().maxProfit(DATASET.copy())
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


@app.get("/sma-return")
def get_sma_returns():
    try:
        data = SMA_Return.sma_return(DATASET.copy())  # for now no user input yet
        return data
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


@app.get("/up-down-runs")
def get_up_down_runs():
    try:
        data = analyze_up_down_runs(DATASET.copy())
        return {"longest_up": data[0], "longest_down": data[1]}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))


if __name__ == "__main__":
    uvicorn.run("main:app", host="127.0.0.1", port=8000, reload=True)
