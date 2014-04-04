#include "BioSensor.h"
#include <math.h>

BioSensor::BioSensor(shared_ptr<Environment> environment) :
  mEnvironment(environment),
  mDistanceNoiseModel(new NoiseModel),
  mHeadingNoiseModel(new NoiseModel) {}

BioSensor::~BioSensor() {}

unsigned int BioSensor::sense(double at[3], double *readings, unsigned int maxReadings) {
  unsigned int guardCount = mEnvironment->guardCount();
  if (guardCount > maxReadings) guardCount = maxReadings;
  for (int i = 0; i < guardCount; i++) {
    Coordinate location = mEnvironment->getGuard(i).location;
    double dx = location.x;
    double dy = location.y;
    readings[2*i] = mDistanceNoiseModel->noisyValue(sqrt(dx * dx + dy * dy));
    readings[2*i+1] = mHeadingNoiseModel->noisyValue(atan2(dy, dx));
    if (readings[2*i+1] < 0)
      readings[2*i+1] += 2 * M_PI;
  }
  return guardCount;
}

void BioSensor::setDistanceNoiseModel(shared_ptr<NoiseModel> noiseModel) {
  mDistanceNoiseModel = noiseModel;
}

void BioSensor::setHeadingNoiseModel(shared_ptr<NoiseModel> noiseModel) {
  mHeadingNoiseModel = noiseModel;
}
