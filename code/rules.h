#ifndef RULES_H
#define RULES_H

class UpdateRule{
    public:
        UpdateRule(){};
        virtual int operator() (int centre, int up, int down, int left, int right, int upLeft, int upRight, int downLeft, int downRight) = 0;
};

class GameOfLifeRule : public UpdateRule{
    public:
        int operator() (int centre, int up, int down, int left, int right, int upLeft, int upRight, int downLeft, int downRight);
};

#endif