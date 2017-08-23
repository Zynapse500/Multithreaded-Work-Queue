//
// Created by Christofer on 2017-08-23.
//
// Template Implementation of WorkQueue
//

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "WorkQueue.h"

template<typename OutType, typename InType>
WorkQueue<OutType, InType>::WorkQueue(int nThreads, const std::function<OutType(InType)>& work) {
    // Create all threads and set their workload
    for (int i = 0; i < nThreads; ++i) {
        threads.push_back(std::move(
                std::thread(waitForWork, std::ref(*this), work))
        );
    }
}

template<typename OutType, typename InType>
void WorkQueue<OutType, InType>::addToQueue(const InType& value) {
    std::unique_lock<std::mutex> lock(taskMutex);

    tasks.push(value);

    lock.unlock();

    // Notify that a task has been added
    hasTask.notify_one();
}

template<typename OutType, typename InType>
void WorkQueue<OutType, InType>::close() {
    // Wait for all tasks to complete before closing
    while (running) {
        std::unique_lock<std::mutex> lock(taskMutex);
        completedTask.wait_for(lock, std::chrono::milliseconds(10));
        if (tasks.empty()) {
            running = false;
            hasTask.notify_all();
        }
    }

    // Merge all threads
    for (auto&& thread : threads) {
        thread.join();
    }
}

template<typename OutT, typename InT>
void WorkQueue<OutT, InT>::waitForWork(WorkQueue<OutT, InT>& t, const std::function<OutT(InT)>& work) {
    // Keep waiting while the queue is active
    while (t.running) {
        // Wait for either a new item or a close-event
        std::unique_lock<std::mutex> lock(t.taskMutex);
        t.hasTask.wait(lock, [&t]() { return !t.tasks.empty() || !t.running; });

        // Process the next item in the queue
        if (!t.tasks.empty()) {
            OutT value = std::move(t.tasks.front());
            t.tasks.pop();

            lock.unlock();

            OutT workOutput = std::move(work(value));

            std::unique_lock<std::mutex> outputLock(t.outputMutex);
            t.output.push(std::move(workOutput));
            outputLock.unlock();

            t.completedTask.notify_all();
        }
    }
}
