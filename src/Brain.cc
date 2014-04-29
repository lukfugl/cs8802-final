#include "Brain.h"
#include "Observation.h"

Brain::Brain(shared_ptr<NoisyMap> map, shared_ptr<CoupledEMSensor> emSensor, shared_ptr<CoupledForwardSensor> forwardSensor) :
  mMap(map),
  mEMSensor(emSensor),
  mForwardSensor(forwardSensor) {}

Brain::~Brain() {}

void Brain::decide(double *heading, double *speed) {
  Observation observation(mEMSensor, 32, mForwardSensor, 32);

  // TODO: actually choose a heading an speed based on prior state + senses
  *heading = 0;
  *speed = 0;
}
