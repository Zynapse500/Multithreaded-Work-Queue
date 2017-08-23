//
// Created by Christofer on 2017-08-23.
//

#ifndef MULTITHREADED_WORK_QUEUE_WORKQUEUE_H
#define MULTITHREADED_WORK_QUEUE_WORKQUEUE_H

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>


template<typename OutType, typename InType>
class WorkQueue {
public:

    // Creates a work queue that performs 'work'
    WorkQueue(int nThreads, const std::function<OutType(InType)>& work);

    // Adds values to the work queue
    void addToQueue(const InType& value);

    // Closes the queue and terminates the threads when all existing tasks are complete
    void close();

    // Return the output of all tasks
    std::queue<OutType> getOutput() const {
        return output;
    }

private:


    // Should the threads be running?
    bool running = true;

    // List of all threads
    std::vector<std::thread> threads;

    // The tasks that need work
    std::queue<InType> tasks;
    std::mutex taskMutex;
    std::condition_variable hasTask;

    // The output of all the tasks
    std::queue<OutType> output;
    std::mutex outputMutex;
    std::condition_variable completedTask;

    // Main logic for threads:
    // The thread waits for a new item to appear in the queue
    // It then processes that item and puts it's result in the output queue
    static void waitForWork(WorkQueue<OutType, InType>& t, const std::function<OutType(InType)>& work);


};


#include "WorkQueue.tpp"



#endif //MULTITHREADED_WORK_QUEUE_WORKQUEUE_H
