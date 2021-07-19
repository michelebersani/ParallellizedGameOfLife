#ifndef SOLVERS_H
#define SOLVERS_H

#include "utimer.cpp"
#include "board.h"

class Solver {
    public:
        virtual void solve(int numSteps, bool verbose, int numWorkers, int chunksize) = 0;
    protected:
        Board * parentPtr;
};


class SequentialSolver : public Solver{
    public:
        SequentialSolver(Board * parent);
        void solve(int numSteps, bool verbose, int numWorkers, int chunksize);
};

class ffSolver : public Solver{
    public:
        ffSolver(Board * parent);
        void solve(int numSteps, bool verbose, int numWorkers, int chunksize);
};

class ThreadSolver : public Solver{
    public:
        ThreadSolver(Board * parent);
        void solve(int numSteps, bool verbose, int numWorkers, int chunksize);
};

class ThreadPoolSolver : public Solver{
    public:
        ThreadPoolSolver(Board * parent);
        void solve(int numSteps, bool verbose, int numWorkers, int chunksize);
    private:
        class FunctionPool;
};

#endif