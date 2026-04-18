#pragma once
#include <chrono>

class Timer {
    public:
        Timer();
        ~Timer();

        void stop() const;
    
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> startTimepoint;
};