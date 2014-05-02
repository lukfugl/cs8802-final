#include "Brain.h"
#include "Observation.h"

Brain::Brain(shared_ptr<NoisyMap> map, shared_ptr<CoupledEMSensor> emSensor, shared_ptr<CoupledForwardSensor> forwardSensor) :
  mMap(map),
  mEMSensor(emSensor),
  mForwardSensor(forwardSensor),
  mOrientation(0, 0, 0),
  mCalibrated(false),
  mSwarm(new ParticleFilter(map, 500)),
  mUniform(0, 1) {}

Brain::~Brain() {}

void Brain::decide(double *turn, double *speed) {
  shared_ptr<Observation> observation(new Observation(mEMSensor, 32, mForwardSensor, 32));

  // update first
  if (!mCalibrated) {
    // continue calibration
    mSwarm->filter(observation);
    if (mSwarm->converged()) {
      mOrientation = mSwarm->mean();
      mCalibrated = true;
    }
  }
  else {
    // TODO: update SLAM
  }

  // then choose movement
  if (!mCalibrated) {
    // just jiggle around
    *turn = uniform(0, 2 * M_PI);
    *speed = uniform(0.5, 1.5);
    mSwarm->advance(*turn, *speed);
  }
  else {
    // TODO: plan
    *turn = 0;
    *speed = 0;
    mOrientation.advance(*turn, *speed);
  }
}

Orientation Brain::believedOrientation() {
  if (mCalibrated) {
    return mOrientation;
  }
  else {
    return mSwarm->mean();
  }
}

double Brain::uniform(double min, double max) {
  return min + mUniform(mRNG) * (max - min);
}
