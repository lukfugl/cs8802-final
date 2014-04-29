#ifndef __COUPLED_EM_SENSOR_H
#define __COUPLED_EM_SENSOR_H

#include "EMSensor.h"
#include "Bond.h"

using namespace std;

class CoupledEMSensor {
  public:
  CoupledEMSensor(shared_ptr<Bond> bond, shared_ptr<EMSensor> sensor);
  ~CoupledEMSensor();

  unsigned int sense(double *readings, unsigned int maxReadings);

  private:
  shared_ptr<Bond> mBond;
  shared_ptr<EMSensor> mSensor;
};

#endif
