#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <sys/types.h>
#include <thread>
#include <vector>

// Adapted from https://stackoverflow.com/a/23899225
class ThreadPool
{
  public:
    // Create threadpool with n threads.
    ThreadPool(u_int n = std::thread::hardware_concurrency());

    ~ThreadPool();

    // Add a task to be run by the pool.
    template <typename F> void Add(F &&f)
    {
        std::lock_guard<std::mutex> lock(work_mu_);
        work_.emplace_back(std::forward<F>(f));
        cv_work_.notify_one();
    }

    // Block until all work is done.
    void Wait();

    u_int NumThreads();

  private:
    void InitThreads();

    void DoWork();

  private:
    std::atomic<bool> is_active_ = true;
    const u_int num_threads_;
    // the number of busy threads
    u_int busy_ = 0;
    std::vector<std::thread> threads_;

    std::condition_variable cv_work_;
    std::condition_variable cv_done_;

    // a mutex for the work queue
    std::mutex work_mu_;
    std::deque<std::function<void()>> work_;
};

#endif