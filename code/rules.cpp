#include "rules.h"

short int GameOfLifeRule::operator() (short int centre, short int up, short int down, short int left, short int right, short int upLeft, short int upRight, short int downLeft, short int downRight){

    short int sum = up + right + left + down + downLeft + downRight + upLeft + upRight; 
    return (sum == 3 || (centre == 1 && sum == 2));
}