#include "solvers.h"
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cassert>
#include <iostream>
#include <math.h>

class ThreadPoolSolver::FunctionPool{
    private:

        std::queue<std::pair<int, int> > ranges_queue;
        std::mutex *m_lock;
        std::condition_variable *m_data_condition;
        std::atomic<bool> *m_accept_functions;
        Board *parentPtr;
        
        void update(std::pair<int, int> range){
            for (int i = range.first; i < range.second; i++){
                parentPtr->updateCell(i);
            }
        }

    public:
        FunctionPool(Board *parent) : ranges_queue(), parentPtr(parent){
            m_accept_functions = new std::atomic<bool> (true);
            m_data_condition = new std::condition_variable();
            m_lock = new std::mutex();
        }

        void push(std::pair<int, int> range){
            std::unique_lock<std::mutex> lock(*m_lock);
            ranges_queue.push(range);
            // when we send the notification immediately, the consumer will try to get the lock , so unlock asap
            lock.unlock();
            m_data_condition->notify_one();
        }

        void done(){
            std::unique_lock<std::mutex> lock(*m_lock);
            *m_accept_functions = false;
            lock.unlock();
            // when we send the notification immediately, the consumer will try to get the lock , so unlock asap
            m_data_condition->notify_all();
            //notify all waiting threads.
        }


        void infinite_loop_func(){
            std::pair<int, int> range; 
            while (true){
                {
                    std::unique_lock<std::mutex> lock(*m_lock);
                    m_data_condition->wait(lock, [this]() {return !ranges_queue.empty() || !(*m_accept_functions); });
                    if (!(*m_accept_functions) && ranges_queue.empty())
                    {
                        //lock will be release automatically.
                        //finish the thread loop and let it join in the main thread.
                        return;
                    }
                    range = ranges_queue.front();
                    ranges_queue.pop();
                    //release the lock
                }
                update(range);
            }
        }
};

ThreadPoolSolver::ThreadPoolSolver(Board *parent){
    this->parentPtr = parent;
}

void ThreadPoolSolver::solve(int numSteps, bool verbose, int numWorkers, int chunksize){
    utimer tp("thread_solver");
    
    if(verbose){
        parentPtr->printBoard();
    }

    int N = parentPtr->N;
    int M = parentPtr->M;

    for (int i = 0; i < numSteps; i++) {
        
        parentPtr->swapBoards();
        
        FunctionPool funcPool = FunctionPool(parentPtr);

        std::vector<std::thread> threadPool;

        int start = 0;

        while (start < N * M){
            int stop = start + chunksize;
            if (stop > N * M){
                stop = N * M;
            }

            std::pair<int,int> range = {start,stop};
            funcPool.push(range);

            start = stop;
        }

        funcPool.done();

        for (int i = 0; i < numWorkers; i++){
            threadPool.push_back(std::thread(&FunctionPool::infinite_loop_func, &funcPool));
        }

        for (unsigned int i = 0; i < threadPool.size(); i++){
            threadPool.at(i).join();
        }

        if(verbose){
            parentPtr->printBoard();
        }
    }
}
