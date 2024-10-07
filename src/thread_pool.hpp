#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

class ThreadPool {
  public:
    ThreadPool(size_t num_threads = std::thread::hardware_concurrency()) : num_threads(num_threads) {
        std::cout << "nt: " << num_threads << std::endl;
        for (size_t i = 0; i < num_threads; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);

                        cv.wait(lock, [this] {
                            return !tasks.empty() || should_stop;
                        });

                        if (should_stop && tasks.empty()) {
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            should_stop = true;
        }

        cv.notify_all();

        for (auto &thread : threads) {
            thread.join();
        }
    }

    void enqueue(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace(std::move(task));
        }
        cv.notify_one();
    }

    size_t num_threads;

  private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;

    std::condition_variable cv;

    bool should_stop = false;
};

#endif
