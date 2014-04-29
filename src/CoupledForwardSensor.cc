#include "CoupledForwardSensor.h"

CoupledForwardSensor::CoupledForwardSensor(shared_ptr<Bond> bond, shared_ptr<ForwardSensor> sensor) :
  mBond(bond),
  mSensor(sensor) {}

CoupledForwardSensor::~CoupledForwardSensor() {}

void CoupledForwardSensor::sense(double *reading, unsigned int granularity) {
  double at[3];
  at[0] = mBond->getX();
  at[1] = mBond->getY();
  at[2] = mBond->getHeading();
  return mSensor->sense(at, reading, granularity);
}
