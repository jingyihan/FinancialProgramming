# Stock Analysis

## Moving Average
Simple moving average calculates the arithmetic average over each rolling window.

MA_t = (Price_t-n + ... Price_t) / n

## Exponential Moving Average
An exponential moving average (EMA) is a type of moving average (MA) that places a greater weight and significance on the most recent data points. 

EMA_t = Price_t * (Smoothing Factor/(1+Days)) +EMA_(t-1) * (1−(Smoothing/(1+Days)))

where: EMA=Exponential moving average
Smoothing Factor = 2

## standard Deviation

