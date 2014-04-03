#include "BioSensor.h"
#include <math.h>

BioSensor::BioSensor(shared_ptr<Environment> environment)
  : mEnvironment(environment) {}

BioSensor::~BioSensor() {}

unsigned int BioSensor::sense(double at[3], double *readings, unsigned int maxReadings) {
  unsigned int guardCount = mEnvironment->guardCount();
  if (guardCount > maxReadings) guardCount = maxReadings;
  for (int i = 0; i < guardCount; i++) {
    Environment::Coordinate guard = mEnvironment->getGuard(i);
    double dx = guard.first;
    double dy = guard.second;
    readings[2*i] = sqrt(dx * dx + dy * dy);
    readings[2*i+1] = atan2(dx, dy);
  }
  return guardCount;
}

void BioSensor::setNoiseModel(shared_ptr<NoiseModel> noiseModel) {
  mNoiseModel = noiseModel;
}
