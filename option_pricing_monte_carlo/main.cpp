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

        double european_option(double ret, double std, double interest, double strike, int sim_n, string direction){

            if (direction != "call" && direction != "put"){
                return 0.0;
            }
            double prev = m_start;

            double dt = m_time/m_nstep;
            double payoff = 0;
            double current = m_start;

            for (int j=0; j<sim_n; j++){
                prev = m_start;
                for (int i=0; i<m_nstep; i++){
                    double r = rand_normal(0,1);
                    current = prev * exp(( ret - 0.5 * pow(std, 2) ) * dt + std * sqrt(dt) * r);
                    prev = current;
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
            return (payoff/(sim_n*1.0))/exp(interest*m_time);

        }

        double american_option(double ret, double std, double interest, double strike, int sim_n, string direction){

            if (direction != "call" && direction != "put"){
                return 0.0;
            }
            double prev = m_start;

            double dt = m_time/m_nstep;
            double payoff = 0;

            vector<double> payoffs;

            for (int j=0; j<sim_n; j++){
                prev = m_start;
                for (int i=0; i<m_nstep; i++){
                    double r = rand_normal(0,1);
                    double current = prev * exp(( ret - 0.5 * pow(std, 2) ) * dt + std * sqrt(dt) * r);
                    prev = current;

                    if (direction == "call"){
                        if (current > strike){
                            payoffs.push_back(current - strike);
                        }
                    }else if (direction == "put"){
                        if (current < strike){
                            payoffs.push_back(strike - current);
                        }
                    }
                    
                }
                int n = payoffs.size();
                if (n!=0){
                    payoff += accumulate( payoffs.begin(), payoffs.end(), 0.0) / n; 
                }
            }

            return (payoff/(sim_n*1.0))/exp(interest*m_time);

        }

    private:
        int m_nstep;
        double m_time;
        double m_start;
};

int main(){
    RandomWalk genPrice(250, 100.0, 1.0);

    //monte carlo prices with Geometric Brownian Motion
    //vector<double> prices = genPrice.generate_price(0.1, 0.2);

    //european call
    string direction = "call";
    double price_1 = genPrice.european_option(0.05, 0.2, 0.03, 100.0, 1000, direction);

    //european call
    direction = "put";
    double price_2 = genPrice.european_option(0.05, 0.2, 0.03, 100.0, 1000, direction);

    //american call
    direction = "call";
    double price_3 = genPrice.american_option(0.05, 0.2, 0.03, 100.0, 1000, direction);

    //american put
    direction = "put";
    double price_4 = genPrice.american_option(0.05, 0.2, 0.03, 100.0, 1000, direction);

    //for (int i=0; i<prices.size(); i++){
    //    cout << "time step " << i << " : " << prices[i] << endl;
    //}

    cout << "european option call: " << price_1 << endl;
    cout << "european option put: " << price_2 << endl;
    cout << "american option call: " << price_3 << endl;
    cout << "american option put: " << price_4 << endl;

    return 0;
}