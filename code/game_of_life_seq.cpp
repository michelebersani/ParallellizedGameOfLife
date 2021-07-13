#include "customBoard.cpp"

bool verbose = false;

int main(int argc, char * argv[]) {

    if(argc != 5) {
        std::cout << "Usage is: " << argv[0] << " N M number_step seed" << std::endl;
        return(-1);
    }

    int N = atoi(argv[1]);
    int M = atoi(argv[2]);
    int numSteps = atoi(argv[3]);
    int seed = atoi(argv[4]);

    GameOfLifeBoard *board = new GameOfLifeBoard(N, M, seed, verbose);
    board->sequentialSolver(numSteps);

    return (0);
}