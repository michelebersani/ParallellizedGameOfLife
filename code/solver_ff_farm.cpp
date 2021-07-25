#include "solvers.h"
#include <ff/ff.hpp>
#include <ff/farm.hpp>

using namespace ff;

ffFarmSolver::ffFarmSolver(Board *parent){
    this->parentPtr = parent;
}

struct Task_t {
    Task_t(Board *parentPtr, std::pair<int,int> range): parentPtr(parentPtr), range(range){}
    Board *parentPtr;
    std::pair<int,int> range;
};


struct Emitter: ff_monode_t<Task_t> {

    bool verbose;
    Board *parentPtr;
    int size, numSteps, start, chunksize, firstChunksize;
    std::queue <std::pair <int,int> > ranges;

    Emitter(int numSteps, Board* parent, bool verbose)
            : numSteps(numSteps), parentPtr(parent), verbose(verbose), start(0){
                size = parentPtr->N * parentPtr->M;
            }

    void setup_iteration (int numWorkers) {
        start = 0;
        chunksize = firstChunksize;
        while (start < size){
            for (int i = 0; i < numWorkers && start < size; i++){
                if (start + chunksize < size){
                    ranges.push(std::pair<int, int> (start, start + chunksize));
                    // std::cout<< start << std::endl;
                }
                else{
                    ranges.push(std::pair<int, int> (start, size));
                    // std::cout<< "Ended with" << start << std::endl;
                }
                start += chunksize;
            }
            chunksize = std::max(chunksize / 2, 10);
        }
        if(verbose){
            parentPtr->printBoard();
        }

        parentPtr->swapBoards();
    };


    Task_t *svc(Task_t *in) {
        int numWorkers = get_num_outchannels();
        firstChunksize = size / 2 / numWorkers;
        setup_iteration(numWorkers);
        while (!ranges.empty()){
            std::pair<int,int> range = ranges.front();
            ranges.pop();
            Task_t *task = new Task_t(parentPtr, range);
            ff_send_out(task);
        }
        if (--numSteps){
            return GO_ON;
        }
        return EOS;
    }
};


struct Worker: ff_node_t<Task_t> {
    Task_t *svc(Task_t *in) {
        for (int i = in->range.first; i < in->range.second; i++){
            in->parentPtr->updateCell(i);
        }
        return in;
    }
};


void ffFarmSolver::solve(int numSteps, bool verbose, int numWorkers){
    utimer tp("ff_solver");
    
    ff_Farm<> farm([&]() {
		std::vector<std::unique_ptr<ff_node> > W;
		for(size_t i=0 ; i < numWorkers; i++)
		    W.push_back(make_unique<Worker>());
		return W;
	} () );

	Emitter E(numSteps, parentPtr, verbose);        // creating the Emitter
	farm.add_emitter(E);      // replacing the default emitter
	farm.remove_collector();  // removing the default collector
	farm.wrap_around();       // adding feedback channels between Workers and the Emitter
	
    farm.set_scheduling_ondemand();

	if (farm.run_and_wait_end()<0) {
	    error("running farm\n");
	    return;
	}
    if(verbose){
        parentPtr->printBoard();
    }
}