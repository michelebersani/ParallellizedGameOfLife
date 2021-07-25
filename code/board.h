#ifndef BOARD_H
#define BOARD_H

#include "rules.h"

class Board{

    private:
        friend class Solver;
        friend class SequentialSolver;
        friend class ThreadSolver;
        friend class ThreadPoolSolver;
        friend class FunctionPool;
        friend class ffFarmSolver;
        friend class ffParforSolver;
        friend struct Emitter;
        friend struct Worker;

        int N, M;
        short int cellStates;
        std::vector<short int> presentBoard, pastBoard, initialState;
        UpdateRule &rule;

        void swapBoards();
        void updateCell(int j);

    public:
        Board(int N, int M, short int cellStates, UpdateRule &rule);
        void setUpdateRule(UpdateRule& rule);
        void defineInitialState(std::vector<std::vector<short int>> &matrix);
        void reset();
        void printBoard();

};

#endif