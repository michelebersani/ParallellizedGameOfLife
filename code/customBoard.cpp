#include<algorithm>
#include <stdlib.h>
#include "board.cpp"

class GameOfLifeBoard: public Board {
    
    public:

        GameOfLifeBoard(int _N, int _M, int seed, bool _verbose){
            N = _N;
            M = _M;
            verbose = _verbose;
            presentBoard = 0;

            cells = std::vector<std::vector<int>> (2, std::vector<int> (N*M) );
            std::srand(seed);
            std::generate(cells[0].begin(), cells[0].end(), []() {return (std::rand())%2;});
            if(verbose){
                printBoard(N,M);
            }
        }

        void updateRule(int centre, int up, int down, int left, int right, int upLeft, int upRight, int downLeft, int downRight, int j){
            
            int sum = up + right + left + down + downLeft + downRight + upLeft + upRight;
            
            if(sum == 3 || (centre == 1 && sum == 2)){
                cells[presentBoard][j] = 1;
            }
            else{
                cells[presentBoard][j] = 0;
            }
        }
};