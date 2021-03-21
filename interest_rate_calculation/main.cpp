#include <iostream>
#include <math.h>
using namespace std;

class IntRateCalculator{

    public:
        double m_rate;

        IntRateCalculator(double rate) {
            m_rate = rate;
        }

        double singlePeriod(double value) 
        {
            double f = value * (1 + m_rate);
            return f;
        }

        double multiPeriod(double value, int t) 
        {
            double f = value * pow((1 + m_rate), t);
            return f;
        }

        double continuousPeriod(double value, int t) 
        {
            double f = value * exp(m_rate * t);
            return f;
        }

};

int main(){
    IntRateCalculator irCalculator(0.08);

    cout << "Single Period: " << irCalculator.singlePeriod(100) << endl;
    cout << "Multi-Period: " << irCalculator.multiPeriod(100, 2) << endl;
    cout << "Continuous: " << irCalculator.continuousPeriod(100, 2) << endl;

    return 0;
}

