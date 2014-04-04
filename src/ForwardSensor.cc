#include "ForwardSensor.h"
#include <math.h>

const unsigned int ForwardSensor::GRANULARITY = 10;
const double ForwardSensor::NONE = -1.0;

ForwardSensor::ForwardSensor(shared_ptr<Environment> environment) :
  mEnvironment(environment),
  mNoiseModel(new NoiseModel) {}

ForwardSensor::~ForwardSensor() {}

const double thetaLeft = M_PI / 6;
const double thetaRight = -thetaLeft;
const double thetaStep = (thetaRight - thetaLeft) / (ForwardSensor::GRANULARITY - 1);
const double r = 4;

void ForwardSensor::sense(double at[3], double *reading) {
  for (int i = 0; i < GRANULARITY; i++)
    reading[i] = NONE;

  for (int j = 0; j < mEnvironment->obstacleCount(); j++) {
    Coordinate obstacle = mEnvironment->getObstacle(j);
    double cx = obstacle.x;
    double cy = obstacle.y;
    double rx = cx - at[0];
    double ry = cy - at[1];
    double cor = rx * rx + ry * ry - r * r;
    double theta = at[2] + thetaLeft;
    for (int i = 0; i < GRANULARITY; i++, theta += thetaStep) {
      double dx = cos(theta);
      double dy = sin(theta);
      double dco = dx * rx + dy * ry;
      double discriminant = dco * dco - cor;
      if (discriminant > 0) {
        double distance = dco - sqrt(discriminant);
        if (reading[i] < 0 || reading[i] > distance)
          reading[i] = distance;
      }
    }
  }

  for (int i = 0; i < GRANULARITY; i++)
    if (reading[i] != NONE)
      reading[i] = mNoiseModel->noisyValue(reading[i]);
}

void ForwardSensor::setNoiseModel(shared_ptr<NoiseModel> noiseModel) {
  mNoiseModel = noiseModel;
}
