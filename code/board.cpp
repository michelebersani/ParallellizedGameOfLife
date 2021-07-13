#include<iostream>
#include <unistd.h>
#include <vector>
#include "utimer.cpp"

class Board{

    public:

        int N, M, presentBoard;
        bool verbose;
        std::vector<std::vector<int>> cells;

        virtual void updateRule(int centre, int up, int down, int left, int right, int upLeft, int upRight, int downLeft, int downRight, int j) = 0;

        void swapBoards(){
            // Update index when moving to next timestep: 1 -> 0 -> 1 -> 0 -> ...
            presentBoard = presentBoard ? 0 : 1;
        }

        void updateCell(int j, int N, int M){
            int pastBoard =  presentBoard ? 0 : 1;
            int row = j/M;
            int col = j%M;

            int upIndex = (row == 0) ? (N - 1)*M + col : (row - 1)*M + col;
            int downIndex = (row == N - 1) ? col : (row + 1)*M + col;
            int leftIndex = (col == 0) ? (row+1)*M - 1 : (row)*M + col - 1;
            int rightIndex = (col == M - 1) ? (row)*M : (row)*M + col + 1;
            int upLeftIndex = upIndex - col + leftIndex % M; 
            int upRightIndex= upIndex - col + rightIndex % M; 
            int downLeftIndex = downIndex - col + leftIndex % M;
            int downRightIndex = downIndex - col + rightIndex % M;
            
            int centre = cells[pastBoard][j];
            int up = cells[pastBoard][upIndex];
            int down = cells[pastBoard][downIndex];
            int left = cells[pastBoard][leftIndex];
            int right = cells[pastBoard][rightIndex];
            int upLeft = cells[pastBoard][upLeftIndex];
            int upRight = cells[pastBoard][upRightIndex];
            int downLeft = cells[pastBoard][downLeftIndex];
            int downRight = cells[pastBoard][downRightIndex]; 
            
            updateRule(centre, up, down, left, right, upLeft, upRight, downLeft, downRight, j);
        }

        void printBoard(int N, int M){

            for (int i=0; i<M + 2; ++i){
                std::cout<<"\r";
            }

            for (int i=0; i<M; ++i){
                std::cout<<"-";
            }

            std::cout<<"\n";
            
            for (int i=0; i<N; ++i){
                for (int j=0; j<M; ++j){
                    if((cells[presentBoard][j + M*i] != 1) && (cells[presentBoard][j + M*i] != 0))
                        std::cout<<"ERROR";
                    if(cells[presentBoard][j + M*i] ){
                        std::cout<<"x";
                    }else{
                        std::cout<<" ";
                    }
                }
                std::cout<<"\n";
            }

            for (int i=0; i<M; ++i){
                std::cout<<"-";
            }
            std::cout<<"\n";
        }

        void sequentialSolver(int num_steps){
            utimer tp("seq");
            for (int i=0; i< num_steps; i++) {
                
                swapBoards();
                for(int j = 0; j< N*M; j++){
                    updateCell(j, N, M);
                }
                if(verbose){
                    printBoard(N, M);
                    sleep(1);
                }
            }
        }
};