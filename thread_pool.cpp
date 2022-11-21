#include "thread_pool.hpp"
#include <iostream>

ThreadPool::ThreadPool(u_int n) : num_threads_(n)
{
    InitThreads();
}

ThreadPool::~ThreadPool()
{
    std::unique_lock<std::mutex> latch(work_mu_);
    is_active_ = false;
    cv_work_.notify_all();
    latch.unlock();

    for (auto &t : threads_)
        t.join();
}

void ThreadPool::Wait()
{
    std::unique_lock<std::mutex> lock(work_mu_);
    cv_done_.wait(lock, [this]() { return work_.empty() && (busy_ == 0); });
}

u_int ThreadPool::NumThreads()
{
    return num_threads_;
}

void ThreadPool::InitThreads()
{
    threads_.resize(num_threads_);
    for (u_int i = 0; i < num_threads_; i++)
        threads_.emplace_back(&ThreadPool::DoWork, this);
}

void ThreadPool::DoWork()
{
    while (is_active_)
    {
        std::unique_lock<std::mutex> latch(work_mu_);

        // Now wait for a signal that there's work. If woken (spuriously or legitimately), go back to sleep unless
        //   1. The threadpool is set to inactive; or
        //   2. There is work in the queue.
        cv_work_.wait(latch, [this]() { return !work_.empty() || !is_active_; });

        if (!is_active_)
            break;

        auto f = work_.front();
        work_.pop_front();

        busy_++;

        latch.unlock();

        f();

        latch.lock();
        busy_--;

        cv_done_.notify_one();
    }
}
