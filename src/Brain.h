#ifndef __BRAIN_H
#define __BRAIN_H

#include "NoisyMap.h"
#include "CoupledEMSensor.h"
#include "CoupledForwardSensor.h"

#include <memory>

using namespace std;

class Brain {
  public:
  Brain(shared_ptr<NoisyMap> map,
      shared_ptr<CoupledEMSensor> emSensor,
      shared_ptr<CoupledForwardSensor> forwardSensor);
  ~Brain();

  void decide(double *turn, double *speed);

  private:
  shared_ptr<NoisyMap> mMap;
  shared_ptr<CoupledEMSensor> mEMSensor;
  shared_ptr<CoupledForwardSensor> mForwardSensor;
};

#endif
