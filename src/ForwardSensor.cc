#include "ForwardSensor.h"
#include <math.h>

const double ForwardSensor::NONE = -1.0;

ForwardSensor::ForwardSensor(shared_ptr<NoisyMap> map) :
  mMap(map),
  mNoiseModel(new NoiseModel) {}

ForwardSensor::~ForwardSensor() {}

const double THETA_LEFT = M_PI / 6;
const double THETA_RIGHT = -THETA_LEFT;

void ForwardSensor::sense(double at[3], double *reading, unsigned int granularity) {
  double thetaStep = (THETA_RIGHT - THETA_LEFT) / (granularity - 1);

  for (int i = 0; i < granularity; i++)
    reading[i] = NONE;

  for (int j = 0; j < mMap->obstacleCount(); j++) {
    shared_ptr<Obstacle> obstacle = mMap->getObstacle(j);
    double r = obstacle->radius;
    double cx = obstacle->location.x;
    double cy = obstacle->location.y;
    double rx = cx - at[0];
    double ry = cy - at[1];
    double cor = rx * rx + ry * ry - r * r;
    double theta = at[2] + THETA_LEFT;
    for (int i = 0; i < granularity; i++, theta += thetaStep) {
      double dx = cos(theta);
      double dy = sin(theta);
      double dco = dx * rx + dy * ry;
      if (dco < 0) continue;
      double discriminant = dco * dco - cor;
      if (discriminant > 0) {
        double distance = dco - sqrt(discriminant);
        if (reading[i] < 0 || reading[i] > distance)
          reading[i] = distance;
      }
    }
  }

  for (int i = 0; i < granularity; i++)
    if (reading[i] != NONE)
      reading[i] = mNoiseModel->noisyValue(reading[i]);
}

void ForwardSensor::setNoiseModel(shared_ptr<NoiseModel> noiseModel) {
  mNoiseModel = noiseModel;
}
