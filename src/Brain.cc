#include "Brain.h"
#include "Observation.h"

Brain::Brain(shared_ptr<NoisyMap> map, shared_ptr<CoupledEMSensor> emSensor, shared_ptr<CoupledForwardSensor> forwardSensor) :
  mMap(map),
  mEMSensor(emSensor),
  mForwardSensor(forwardSensor),
  mOrientation(0, 0, 0),
  mCalibrated(false) {}

Brain::~Brain() {}

void Brain::decide(double *turn, double *speed) {
  Observation observation(mEMSensor, 32, mForwardSensor, 32);

  // TODO: actually choose a turn and speed based on prior state + senses
  *turn = 0;
  *speed = 0;

  // update believed heading/location
  mOrientation.advance(*turn, *speed);
}