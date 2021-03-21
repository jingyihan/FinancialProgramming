#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

class StockAnalysis{

    public:

        vector<double> price;

        StockAnalysis(vector<double> hist_price){
            price = hist_price;
        }

        vector<double> cal_MA(int period){
            vector<double> ma;
            double sum = 0;
            for (int i=0; i<price.size(); i++){
                sum += price[i];
                if (i >= period){
                    ma.push_back(sum/period);
                    sum -= price[i-period];
                }
            }
            return ma;
        }

        vector<double> cal_EMA(int period, double factor){
            vector<double> ema;
            double sum = 0;
            double multiplier = factor / (period + 1);

            for (int i=0; i<price.size(); i++){
                sum += price[i];
                if (i == period){
                    ema.push_back(sum/period);
                    sum -= price[i-period];
                }
                else if (i > period){
                    double val = (1-multiplier) * ema.back() + multiplier * price[i];
                    ema.push_back(val);
                }
            }
            return ema;

        }

        double cal_dailyRange(){
            double n = price.size();

            if (price.size() < 2){
                return 0.0;
            }

            double previous = price[0];
            double sum = 0;

            for (int i=1; i<price.size(); i++){
                double range = abs(price[i]-previous);
                sum += range;
                previous = price[i];
            }

            return sum / (n-1);
        }

        double cal_Vol(){
            double sum;

            for (int i=0; i<price.size(); i++){
                sum += price[i];
            }

            double avg = sum / price.size();

            double sum_var;
            for (int i=0; i<price.size(); i++){
                sum_var += pow(price[i] - avg, 2);
            }

            double var = sqrt(sum_var/(price.size()-1));

            return var;
        }


};

int main(){
    //double p[] = {10.0, 11.0, 22.0, 12.0, 13.0, 23.0, 12.0, 32.0, 12.0, 3.0, 2.0, 22.0, 32.0};
    double p[] = {3.0, 3.5, 5, 4.48, 5.2, 6.0, 6.1, 5.5, 5.2, 5.7};
    vector<double> prices(p,p+sizeof(p)/sizeof(p[0]));

    StockAnalysis stock_cal(prices);

    cout << "Moving Average: " << endl;
    vector<double> ma_res = stock_cal.cal_MA(5);
    for (int i = 0; i < ma_res.size(); i++){
        cout << "time step: " << i << " : " << ma_res[i] << endl;
    }
    cout << "Exponential Moving Average: " << endl;
    vector<double> ema_res = stock_cal.cal_EMA(5, 2);
    for (int i = 0; i < ema_res.size(); i++){
        cout << "time step: " << i << " : " << ema_res[i] << endl;
    }
    cout << "Volatility: " << stock_cal.cal_Vol() << endl;

    cout << "Average Daily Range: " << stock_cal.cal_dailyRange() << endl;

    return 0;
}
