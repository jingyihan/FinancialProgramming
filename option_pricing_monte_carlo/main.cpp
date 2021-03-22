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
                double current = prev * exp(( mu - 0.5 * pow(std, 2) ) * dt + std * sqrt(dt) * r);
                walk.push_back(current);
                prev = current;
            }

            return walk;
        }

    private:
        int m_nstep;
        double m_time;
        double m_start;
};

int main(){
    RandomWalk genPrice(12, 100.0, 1.0);

    vector<double> prices = genPrice.generate_price(0.05, 0.01);

    for (int i=0; i<prices.size(); i++){
        cout << "time step " << i << " : " << prices[i] << endl;
    }

    return 0;
}