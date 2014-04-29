#ifndef __COUPLED_FORWARD_SENSOR_H
#define __COUPLED_FORWARD_SENSOR_H

#include "ForwardSensor.h"
#include "Bond.h"

using namespace std;

class CoupledForwardSensor {
  public:
  CoupledForwardSensor(shared_ptr<Bond> bond, shared_ptr<ForwardSensor> sensor);
  ~CoupledForwardSensor();

  void sense(double *reading, unsigned int granularity=10);

  private:
  shared_ptr<Bond> mBond;
  shared_ptr<ForwardSensor> mSensor;
};

#endif
