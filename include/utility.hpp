#ifndef __UTILITY_HPP__
#define __UTILITY_HPP__

#include <string>
#include <ctime>
#include <iostream>
#include <iomanip>
namespace secureindex{

    class TimerCalc
    {

    public:

        explicit TimerCalc(const std::string & desc_):desc(desc_)
            {
                start = clock();
            }

        void stop()
            {
                end = clock();
                std::cout<<"Timer: "<<desc<<"\t"
                         <<std::setiosflags(std::ios::fixed)
                         <<std::setprecision(10)
                         <<(double)(end - start)/CLOCKS_PER_SEC<<" secs"<<std::endl;
            }
        ~TimerCalc()
            {
            }
        
    private:
        
        std::string desc;
        clock_t start;
        clock_t end;
    };
            
}
#endif
