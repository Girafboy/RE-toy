#include "Timer.h"

Timer::Timer() : start(std::chrono::high_resolution_clock::now()), max_time_second(0ll) {}

void Timer::setMaxTimeSecond(long long int t) {
    max_time_second = t;
}

bool Timer::isKilledWithinTime(long long t) {
    return std::chrono::high_resolution_clock::now() - start > std::chrono::seconds{max_time_second - t};
}
