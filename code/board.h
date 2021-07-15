#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board;

class Solver {
    public:
        virtual void operator() (int numSteps, bool verbose, int numWorkers) = 0;
        void setParent(Board * parent);
    protected:
        Board * parentPtr;
};

class SequentialSolver : public Solver{
    public:
        SequentialSolver(){};
        void operator() (int numSteps, bool verbose, int numWorkers);
};

class ffSolver : public Solver{
    public:
        ffSolver(){};
        void operator() (int numSteps, bool verbose, int numWorkers);
};

class ThreadSolver : public Solver{
    public:
        ThreadSolver(){};
        void operator() (int numSteps, bool verbose, int numWorkers);
};

class UpdateRule{
    public:
        UpdateRule(){};
        virtual int operator() (int centre, int up, int down, int left, int right, int upLeft, int upRight, int downLeft, int downRight) = 0;
};

class GameOfLifeRule : public UpdateRule{
    public:
        int operator() (int centre, int up, int down, int left, int right, int upLeft, int upRight, int downLeft, int downRight);
};

class Board{

    private:
        friend class Solver;
        friend class SequentialSolver;
        friend class ThreadSolver;
        friend class ffSolver;

        int N, M;
        std::vector<int> presentBoard, pastBoard, initialState;
        UpdateRule &rule;
        Solver &solver;

        void swapBoards();
        void updateCell(int j);

    public:
        Board(int N, int M, Solver &solver, UpdateRule &rule);
        void setUpdateRule(UpdateRule& rule);
        void setInitialState(std::vector<std::vector<int>> &matrix);
        void reset();
        void solve(int numSteps, bool verbose, int numWorkers);
        void printBoard();
        void sequentialSolver(int num_steps);

};


#endif