#include "Brain.h"
#include "Observation.h"

Brain::Brain(shared_ptr<NoisyMap> map, shared_ptr<CoupledEMSensor> emSensor, shared_ptr<CoupledForwardSensor> forwardSensor) :
  mMap(map),
  mEMSensor(emSensor),
  mForwardSensor(forwardSensor),
  mOrientation(0, 0, 0),
  mCalibrated(false),
  mSwarm(new ParticleFilter(map, 500)),
  mGuardModel(new GuardModel()),
  mUniform(0, 1) {}

Brain::~Brain() {}

void Brain::decide(double *turn, double *speed) {
  shared_ptr<Observation> observation(new Observation(mEMSensor, 32, mForwardSensor, 32));

  // update first
  Orientation priorBelief = believedOrientation();
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

  // predict guard motion. priorBelief and currentBelief are used to correct
  // beliefs before update: i.e. if I though a guard was at X when I thought I
  // was at priorBelief, I should now think the guard is at X' = f(X,
  // priorBelief, currentBelief) given I think I'm at currentBelief. also,
  // currentBelief is used to interpret the ranges and bearings from the
  // observation into positions
  Orientation currentBelief = believedOrientation();
  mGuardModel->update(observation, priorBelief, currentBelief);

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
