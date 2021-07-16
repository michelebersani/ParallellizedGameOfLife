#include "rules.h"

int GameOfLifeRule::operator() (int centre, int up, int down, int left, int right, int upLeft, int upRight, int downLeft, int downRight){

    int sum = up + right + left + down + downLeft + downRight + upLeft + upRight; 
    
    if(sum == 3 || (centre == 1 && sum == 2)){
        return 1;
    }
    else{
        return 0;
    }
}