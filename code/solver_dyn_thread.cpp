#include "solvers.h"
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <cmath>
#include <queue>

class ThreadPoolSolver::Barrier {
    private:
        std::mutex m;
        std::condition_variable cv;
        int counter;
        int waiting;
        int thread_count;
        std::function<void()> comp_f;

    public:
        Barrier(int count, const std::function<void()>& on_completion) : thread_count(count), counter(0), waiting(0), comp_f(on_completion){}

        void wait(){
            std::unique_lock<std::mutex> lk(m);
            ++counter;
            ++waiting;
            cv.wait(lk, [this]{return counter >= thread_count;});
            if (waiting == thread_count)
                comp_f();
            --waiting;
            cv.notify_one();
            if(waiting == 0)
                counter = 0;
        }
};

ThreadPoolSolver::ThreadPoolSolver(Board *parent){
    this->parentPtr = parent;
}

void ThreadPoolSolver::solve(int numSteps, bool verbose, int numWorkers){
    
    utimer tp ("threadpool_solver");
    int iter = 0;
    int N = parentPtr->N;
    int M = parentPtr->M;
    int firstChunksize = N * M / 2 / numWorkers;
    std::mutex ranges_lock;
    std::queue<std::pair<int,int>> ranges;
    bool work = true;

    auto resetQueue = [&]() noexcept {
        int chunksize = firstChunksize;
        int start = 0;
        while (start < N * M){
            for (int i = 0; i < numWorkers && start < N * M; i++){
                if (start + chunksize < N * M){
                    ranges.push(std::pair<int, int> (start, start + chunksize));
                    // std::cout<< start << std::endl;
                }
                else{
                    ranges.push(std::pair<int, int> (start, N * M));
                    // std::cout<< "Ended with" << start << std::endl;
                }
                start += chunksize;
            }
            chunksize = std::max(chunksize / 2, 10);
        }
    };

    auto on_completion = [&]() noexcept {
        iter++;
        if (iter == numSteps){
            work = false;
        }
        else{
            resetQueue();
            parentPtr->swapBoards();
        }
        if(verbose){
            parentPtr->printBoard();
        }
    };

    Barrier barrier(numWorkers, on_completion);

    auto work_wrapper = [&]() noexcept {
        while (work){
            while (true){
                std::unique_lock<std::mutex> lock(ranges_lock);
                if (ranges.empty()){
                    break;
                }
                std::pair<int,int> range = ranges.front();
                ranges.pop();
                lock.unlock();
                for (int i = range.first; i < range.second; i++){
                    parentPtr->updateCell(i);
                }
            }
            barrier.wait();
        }
    };

    if(verbose){
        parentPtr->printBoard();
    }
    parentPtr->swapBoards();
    resetQueue();
    std::vector<std::thread> workers;
    
    for (int tid = 0; tid < numWorkers; tid++)
        workers.push_back(std::thread(work_wrapper));

    for (std::thread& t : workers){
        t.join();
    }
}