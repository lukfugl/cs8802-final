#include "Brain.h"
#include "Observation.h"

Brain::Brain(shared_ptr<NoisyMap> map, shared_ptr<CoupledEMSensor> emSensor, shared_ptr<CoupledForwardSensor> forwardSensor) :
  mMap(map),
  mEMSensor(emSensor),
  mForwardSensor(forwardSensor),
  mCalibrated(false),
  mSwarm(new ParticleFilter(map, 500)),
  mSlam(NULL),
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
      mSlam = shared_ptr<OnlineSLAM>(new OnlineSLAM(mSwarm->mean(), mSwarm->variance().heading, mMap));
      mCalibrated = true;
    }
  }
  else {
    mSlam->update(observation);
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
    *turn = uniform(0, M_PI / 2);
    *speed = uniform(0.5, 1.0);
    mSwarm->advance(*turn, *speed);
  }
  else {
    // TODO: plan
    *turn = uniform(0, M_PI / 6);
    *speed = uniform(0.5, 1.0);
    mSlam->advance(*turn, *speed);
  }
}

Orientation Brain::believedOrientation() {
  if (mCalibrated) {
    return mSlam->estimate();
  }
  else {
    return mSwarm->mean();
  }
}

double Brain::uniform(double min, double max) {
  return min + mUniform(mRNG) * (max - min);
}
