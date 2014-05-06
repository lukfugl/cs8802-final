#ifndef _Simulator_h
#define _Simulator_h

#include <string>
#include <memory>

#include "Environment.h"
#include "Bond.h"
#include "Brain.h"

class Simulator {
    public:
    Simulator(string filename);
    ~Simulator();

    bool terminated();
    bool success();
    void advance();

    private:
    void spawnBond();
    bool goalReached();

    bool mTerminated;
    bool mSuccess;
    shared_ptr<Environment> mEnvironment;
    shared_ptr<Bond> mBond;
    shared_ptr<Brain> mBrain;
};

#endif
