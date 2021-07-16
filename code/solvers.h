#ifndef SOLVERS_H
#define SOLVERS_H

#include "utimer.cpp"
#include "board.h"

class Solver {
    public:
        virtual void solve(int numSteps, bool verbose, int numWorkers) = 0;
    protected:
        Board * parentPtr;
};


class SequentialSolver : public Solver{
    public:
        SequentialSolver(Board * parent);
        void solve(int numSteps, bool verbose, int numWorkers);
};

class ffSolver : public Solver{
    public:
        ffSolver(Board * parent);
        void solve(int numSteps, bool verbose, int numWorkers);
};

class ThreadSolver : public Solver{
    public:
        ThreadSolver(Board * parent);
        void solve(int numSteps, bool verbose, int numWorkers);
};

#endif