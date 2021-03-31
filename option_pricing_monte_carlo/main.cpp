#include <iostream>
#include <math.h>
#include <vector>
#include <random>
using namespace std;

class RandomWalk{
    public:
        RandomWalk(double nstep, double start, double time){
            m_nstep = nstep;
            m_start = start;
            m_time = time;
        };

        double rand_normal(double mean, double stddev){
            //Box muller method
            static double n2 = 0.0;
            static int n2_cached = 0;
            if (!n2_cached)
            {
                double x, y, r;
                do
                {
                    x = 2.0*rand()/RAND_MAX - 1;
                    y = 2.0*rand()/RAND_MAX - 1;

                    r = x*x + y*y;
                }
                while (r == 0.0 || r > 1.0);
                {
                    double d = sqrt(-2.0*log(r)/r);
                    double n1 = x*d;
                    n2 = y*d;
                    double result = n1*stddev + mean;
                    n2_cached = 1;
                    return result;
                }
            }
            else
            {
                n2_cached = 0;
                return n2*stddev + mean;
            }
        }

        vector<double> generate_price(double mu, double std){
            vector<double> walk;
            double prev = m_start;

            double dt = m_time/m_nstep;

            for (int i=0; i<m_nstep; i++){
                double r = rand_normal(0,1);
                double current = prev * exp(mu * dt + std * sqrt(dt) * r);
                walk.push_back(current);
                prev = current;
            }

            return walk;

        }

        double european_option(double ret, double std, double strike, int sim_n, string direction){

            if (direction != "call" && direction != "put"){
                return 0.0;
            }
            double prev = m_start;

            double dt = m_time/m_nstep;
            double payoff = 0;
            double current = m_start;

            for (int j=0; j<sim_n; j++){
                current = m_start;
                for (int i=0; i<m_nstep; i++){
                    double r = rand_normal(0,1);
                    current = current * exp(( ret - 0.5 * pow(std, 2) ) * dt + std * sqrt(dt) * r);
                }

                if (direction == "call"){
                    if (current > strike){
                        payoff += current - strike;
                        //cout << current - strike << endl;
                    }
                }else if (direction == "put"){
                    if (current < strike){
                        payoff += strike - current;
                        //cout << strike - current << endl;
                    }
                }
                
            }
            return (payoff/(sim_n*1.0))/exp(ret*m_time);

        }

        double asian_option(double ret, double std, double strike, int days, int sim_n, string direction){

            if (direction != "call" && direction != "put"){
                return 0.0;
            }
            double current = m_start;
            double prices = 1;
            double dt = m_time/(m_nstep*1.0);
            double payoff = 0;

            vector<double> payoffs;

            for (int j=0; j<sim_n; j++){
                current = m_start;
                prices = 1;
                vector<double> payoffs;
                for (int i=0; i<m_nstep; i++){
                    double r = rand_normal(0,1);
                    current = current * exp(( ret - 0.5 * pow(std, 2) ) * dt + std * sqrt(dt) * r);

                    if (i >= m_nstep - days){
                        prices *= current;
                    }
                }

                if (direction == "call"){
                    prices = pow(prices, 1.0/days);
                    payoff += max(prices-strike, 0.0);
                }else if (direction == "put"){
                    prices = pow(prices, 1.0/days);
                    payoff += max(strike-prices, 0.0);
                }

            }

            return (payoff/(sim_n*1.0));

        }

    private:
        int m_nstep;
        double m_time;
        double m_start;
};

int main(){
    RandomWalk genPrice(255, 100.0, 1.0);

    //monte carlo prices with Geometric Brownian Motion
    //vector<double> prices = genPrice.generate_price(0.1, 0.2);

    //european call
    string direction = "call";
    double price_1 = genPrice.european_option(0.05, 0.2, 100.0, 5000, direction);

    //european call
    direction = "put";
    double price_2 = genPrice.european_option(0.05, 0.2, 100.0, 5000, direction);

    //american call
    direction = "call";
    double price_3 = genPrice.asian_option(0.05, 0.2, 100.0, 20, 5000, direction);

    //american put
    direction = "put";
    double price_4 = genPrice.asian_option(0.05, 0.2, 100.0, 20, 5000, direction);

    //for (int i=0; i<prices.size(); i++){
    //    cout << "time step " << i << " : " << prices[i] << endl;
    //}

    cout << "european option call: " << price_1 << endl;
    cout << "european option put: " << price_2 << endl;
    cout << "asian option call: " << price_3 << endl;
    cout << "asian option put: " << price_4 << endl;

    return 0;
}