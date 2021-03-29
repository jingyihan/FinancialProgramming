# Stock Analysis

## Moving Average
Simple moving average calculates the arithmetic average over each rolling window.

MA_t = (Price_t-n + ... Price_t) / n

## Exponential Moving Average
An exponential moving average (EMA) is a type of moving average (MA) that places a greater weight and significance on the most recent data points. 

EMA_t = Price_t * (Smoothing Factor/(1+Days)) +EMA_(t-1) * (1−(Smoothing/(1+Days)))

where: EMA=Exponential moving average
Smoothing Factor = 2

## Standard Deviation
<img src = "https://user-images.githubusercontent.com/24922489/112774843-304dd280-8ff8-11eb-989f-7815a3873a75.png" width="200"/>

## Average Daily Range

Avg_daily_range = (Price_1 - Price_0 + Price_2 - Price_1 + Price_3 - Price_2 + ... + Price_t - Price_t-1) / n

Avg_daily_range = (Price_t - Price_0) / n
