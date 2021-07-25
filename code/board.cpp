#include<iostream>
#include <algorithm>
#include <cstdlib>
#include"board.h"

Board::Board(int N, int M, short int cellStates, UpdateRule &rule) : N(N), M(M), cellStates(cellStates), rule(rule){
    std::vector<short int> initialState;
    presentBoard = std::vector<short int> (N*M);
    pastBoard = std::vector<short int> (N*M);
}

void Board::setUpdateRule(UpdateRule& rule){
    this->rule = rule;
}

void Board::defineInitialState(std::vector<std::vector<short int>> &matrix){
    initialState = std::vector<short int>();
    for(auto && v : matrix){
        initialState.insert(initialState.end(), v.begin(), v.end());
    }
}

void Board::reset(){
    if (initialState.empty()){
        initialState = std::vector<short int>(N*M);
        std::generate(initialState.begin(), initialState.end(), [this](){return (std::rand())%(this->cellStates);});
    }
    else{
        presentBoard = initialState;
    }
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
    int rightIndex = (col == M - 1) ? row * M : row * M + col + 1;
    
    presentBoard[j] = rule(
            pastBoard[j], pastBoard[upIndex], pastBoard[downIndex],
            pastBoard[leftIndex], pastBoard[rightIndex],
            pastBoard[upIndex - col + leftIndex % M], pastBoard[upIndex - col + rightIndex % M],
            pastBoard[downIndex - col + leftIndex % M], pastBoard[downIndex - col + rightIndex % M]
        );

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
            short int x = presentBoard[j + M*i];
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