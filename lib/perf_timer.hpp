#ifndef PERF_TIMER_H
#define PERF_TIMER_H

#include <chrono>
#include <iostream>
#include <string>

class PerfTimer
{
  public:
    PerfTimer(std::string name) : name_(name), started_at_(std::chrono::high_resolution_clock::now())
    {
    }
    ~PerfTimer()
    {
        stopped_at_ = std::chrono::high_resolution_clock::now();
        auto took = std::chrono::duration_cast<std::chrono::milliseconds>(stopped_at_ - started_at_).count();
        std::cout << name_ << " took " << took << "ms" << std::endl;
    }

  private:
    std::string name_;
    std::chrono::system_clock::time_point started_at_;
    std::chrono::system_clock::time_point stopped_at_;
};

#endif