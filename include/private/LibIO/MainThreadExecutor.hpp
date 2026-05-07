#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class MainThreadExecutor {
public:
    void execute(std::function<void()> fn) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push(std::move(fn));
        }
        cv.notify_one();
    }

    void pump() {
        std::queue<std::function<void()>> local;
        {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [&]{ return !queue.empty(); });
            std::swap(local, queue);
        }

        while (!local.empty()) {
            local.front()();
            local.pop();
        }
    }

private:
    std::mutex mutex;
    std::condition_variable cv;
    std::queue<std::function<void()>> queue;
};
