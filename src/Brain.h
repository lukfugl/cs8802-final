#ifndef __BRAIN_H
#define __BRAIN_H

#include "NoisyMap.h"
#include "CoupledEMSensor.h"
#include "CoupledForwardSensor.h"
#include "Orientation.h"
#include "ParticleFilter.h"

#include <memory>
#include <random>

using namespace std;

class Brain {
  public:
  Brain(shared_ptr<NoisyMap> map,
      shared_ptr<CoupledEMSensor> emSensor,
      shared_ptr<CoupledForwardSensor> forwardSensor);
  ~Brain();

  void decide(double *turn, double *speed);

  private:
  double uniform(double min, double max);

  shared_ptr<NoisyMap> mMap;
  shared_ptr<CoupledEMSensor> mEMSensor;
  shared_ptr<CoupledForwardSensor> mForwardSensor;
  Orientation mOrientation;
  bool mCalibrated;
  shared_ptr<ParticleFilter> mSwarm;
  default_random_engine mRNG;
  uniform_real_distribution<double> mUniform;
};

#endif
