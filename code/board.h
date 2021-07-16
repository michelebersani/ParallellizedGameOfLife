#ifndef BOARD_H
#define BOARD_H

#include"rules.h"
#include <vector>


class Board{

    private:
        friend class Solver;
        friend class SequentialSolver;
        friend class ThreadSolver;
        friend class ffSolver;

        int N, M;
        std::vector<int> presentBoard, pastBoard, initialState;
        UpdateRule &rule;

        void swapBoards();
        void updateCell(int j);

    public:
        Board(int N, int M, UpdateRule &rule);
        void setUpdateRule(UpdateRule& rule);
        void setInitialState(std::vector<std::vector<int>> &matrix);
        void reset();
        void printBoard();

};

#endif