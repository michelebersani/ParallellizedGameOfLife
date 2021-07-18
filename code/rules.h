#ifndef RULES_H
#define RULES_H

#include<vector>

class UpdateRule{
    public:
        UpdateRule(){};
        // virtual short int operator() (std::vector <short int> neighbourhood) = 0;
        virtual short int operator() (short int centre, short int up, short int down, short int left, short int right, short int upLeft, short int upRight, short int downLeft, short int downRight) = 0;
};

class GameOfLifeRule : public UpdateRule{
    public:
        short int operator() (short int centre, short int up, short int down, short int left, short int right, short int upLeft, short int upRight, short int downLeft, short int downRight);
};

#endif