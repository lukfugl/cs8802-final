#include "CoupledEMSensor.h"

CoupledEMSensor::CoupledEMSensor(shared_ptr<Bond> bond, shared_ptr<EMSensor> sensor) :
  mBond(bond),
  mSensor(sensor) {}

CoupledEMSensor::~CoupledEMSensor() {}

unsigned int CoupledEMSensor::sense(double *readings, unsigned int maxReadings) {
  double at[3];
  at[0] = mBond->getX();
  at[1] = mBond->getY();
  at[2] = mBond->getHeading();
  return mSensor->sense(at, readings, maxReadings);
}
