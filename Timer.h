#ifndef REACHABILITY_TIMER_H
#define REACHABILITY_TIMER_H


#include <chrono>
#include <utility>

class Timer {
public:
    template<typename F, typename... Args>
    static long long getExecutionTime(F &&f, Args &&... args);

    Timer();

    void reset();

    void setMaxTimeSecond(long long t);

    bool isKilledWithinTime(long long t);

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    long long max_time_second;
};

template<typename F, typename... Args>
long long Timer::getExecutionTime(F &&f, Args &&...args) {
    auto time1 = std::chrono::high_resolution_clock::now();
    std::forward<F>(f)(std::forward<Args>(args)...);
    auto time2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long long, std::nano> d = time2 - time1;
    return d.count();
}


#endif //REACHABILITY_TIMER_H
