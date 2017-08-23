//
// Created by Christofer on 2017-08-23.
//

#include <iostream>
#include <cmath>

#include "WorkQueue.h"


int main() {

    // Workload (determine if number is prime)
    auto work = [](int value) -> int {

        auto isPrime = [](int n) {
            for (int i = 2; i <= sqrt(n) + 1; ++i) {
                if (n % i == 0)
                    return false;
            }
            return true;
        };

        int result = -1;

        if (isPrime(value)) {
            result = value;
        }

        return result;
    };

    // Number of batches to run and their total time to execute
    int n_batches = 10;
    double totalTime = 0;

    for (int j = 0; j < n_batches; ++j) {
        std::cout << "Initiating work queue..." << std::endl;
        WorkQueue<int, int> workQueue(4, work);


        std::cout << "Uploading tasks..." << std::endl;

        // Add tasks to queue and record the time it takes for them to execute
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 1; i < 1000000; ++i) {
            workQueue.addToQueue(i);
        }

        workQueue.close();

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Work complete!" << std::endl;
        std::cout << "Operation took: " << duration.count() << " ms" << std::endl;
        totalTime += duration.count();

        // Print the output of the current batch
        auto output = std::move(workQueue.getOutput());

        while (!output.empty()) {
            auto value = output.front();
            output.pop();
            if (value > 0)
                std::cout << value << ", ";
        }

        std::cout << std::endl << std::endl;
    }

    std::cout << "Average time: " << totalTime / n_batches << " ms" << std::endl;

    return 0;
}