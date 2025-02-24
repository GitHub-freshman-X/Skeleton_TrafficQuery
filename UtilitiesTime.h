#ifndef __UTILITIESTIME_H__
#define __UTILITIESTIME_H__
#define _CRT_SECURE_NO_WARNINGS

#include <chrono>

class Timer{
public:
    using fmilliseconds=std::chrono::duration<double, std::milli>;
    void start() {t1 = std::chrono::steady_clock::now();}
    void stop() {t2 = std::chrono::steady_clock::now();}
    std::chrono::steady_clock::duration::rep difference() {return (t2-t1).count();}
    double diff_in_ms() {return std::chrono::duration_cast<fmilliseconds>(t2-t1).count();}
    
private:
    std::chrono::time_point<std::chrono::steady_clock> t1, t2;
};

#endif // __UTILITIESTIME_H__