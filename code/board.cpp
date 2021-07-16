#include<iostream>
#include"board.h"

Board::Board(int N, int M, UpdateRule &rule) : N(N), M(M), rule(rule){
    initialState = std::vector<int> (N*M);
    presentBoard = std::vector<int> (N*M);
    pastBoard = std::vector<int> (N*M);
}

void Board::setUpdateRule(UpdateRule& rule){
    this->rule = rule;
}

void Board::setInitialState(std::vector<std::vector<int>> &matrix){
    initialState = std::vector<int>();
    for(auto && v : matrix){
        initialState.insert(initialState.end(), v.begin(), v.end());
    }
}

void Board::reset(){
    presentBoard = initialState;
}

void Board::swapBoards(){
    presentBoard.swap(pastBoard);
}

void Board::updateCell(int j){

    int row = j/M;
    int col = j%M;

    int upIndex = (row == 0) ? (N - 1) * M + col : (row - 1) * M + col;
    int downIndex = (row == N - 1) ? col : (row + 1) * M + col;
    int leftIndex = (col == 0) ? (row + 1) * M - 1 : row * M  + col - 1;
    int rightIndex = (col == M - 1) ? row *M : row * M + col + 1;
    int upLeftIndex = upIndex - col + leftIndex % M; 
    int upRightIndex= upIndex - col + rightIndex % M; 
    int downLeftIndex = downIndex - col + leftIndex % M;
    int downRightIndex = downIndex - col + rightIndex % M;
    
    int centre = pastBoard[j];
    int up = pastBoard[upIndex];
    int down = pastBoard[downIndex];
    int left = pastBoard[leftIndex];
    int right = pastBoard[rightIndex];
    int upLeft = pastBoard[upLeftIndex];
    int upRight = pastBoard[upRightIndex];
    int downLeft = pastBoard[downLeftIndex];
    int downRight = pastBoard[downRightIndex]; 
    
    presentBoard[j] = rule(centre, up, down, left, right, upLeft, upRight, downLeft, downRight);
}

void Board::printBoard(){

    for (int i=0; i<M + 2; ++i){
        std::cout<<"\r";
    }

    for (int i=0; i<M; ++i){
        std::cout<<"-";
    }

    std::cout<<"\n";
    
    for (int i=0; i<N; ++i){
        for (int j=0; j<M; ++j){
            int x = presentBoard[j + M*i];
            if(x == 0){
                std::cout<<" ";
            }
            else{
                std::cout<<x;
            }
        }
        std::cout<<"\n";
    }

    for (int i=0; i<M; ++i){
        std::cout<<"-";
    }
    std::cout<<"\n";
}