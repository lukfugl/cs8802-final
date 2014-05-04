#include "EMSensor.h"
#include "Orientation.h"
#include <math.h>

EMSensor::EMSensor(shared_ptr<Environment> environment) :
  mEnvironment(environment),
  mDistanceNoiseModel(new NoiseModel),
  mHeadingNoiseModel(new NoiseModel) {}

EMSensor::~EMSensor() {}

unsigned int EMSensor::sense(double at[3], double *readings, unsigned int maxReadings) {
  unsigned int guardCount = mEnvironment->guardCount();
  if (guardCount > maxReadings) guardCount = maxReadings;
  for (int i = 0; i < guardCount; i++) {
    Orientation orientation = mEnvironment->getGuard(i)->orientation;
    double dx = orientation.x - at[0];
    double dy = orientation.y - at[1];

    readings[2*i] = mDistanceNoiseModel->noisyValue(sqrt(dx * dx + dy * dy));
    readings[2*i+1] = mHeadingNoiseModel->noisyValue(atan2(dy, dx) - at[2]);
    while (readings[2*i+1] < 0)
      readings[2*i+1] += 2 * M_PI;
    while (readings[2*i+1] > 2 * M_PI)
      readings[2*i+1] -= 2 * M_PI;
  }
  return guardCount;
}

void EMSensor::setDistanceNoiseModel(shared_ptr<NoiseModel> noiseModel) {
  mDistanceNoiseModel = noiseModel;
}

void EMSensor::setHeadingNoiseModel(shared_ptr<NoiseModel> noiseModel) {
  mHeadingNoiseModel = noiseModel;
}
