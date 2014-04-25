#ifndef _Simulator_h
#define _Simulator_h

#include <string>
#include <memory>

#include "Environment.h"
#include "Bond.h"

class Simulator {
    public:
    Simulator(string filename);
    ~Simulator();
    
    bool terminated();
    void advance();
    
    private:
    void spawnBond();

    bool mTerminated;
    shared_ptr<Environment> mEnvironment;
    shared_ptr<Bond> mBond;
};

#endif
