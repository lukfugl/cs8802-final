#include "Brain.h"

Brain::Brain(shared_ptr<NoisyMap> map, shared_ptr<CoupledEMSensor> emSensor, shared_ptr<CoupledForwardSensor> forwardSensor) :
  mMap(map),
  mEMSensor(emSensor),
  mForwardSensor(forwardSensor) {}

Brain::~Brain() {}

void Brain::decide(double *heading, double *speed) {
  // space for readings
  int emReadingCount = 0;
  double *emReadings = new double[256];
  double *forwardReading = new double[10];

  // for auto-cleanup
  std::unique_ptr<double[]> mgdEMReadings(emReadings);
  std::unique_ptr<double[]> mgdForwardReading(forwardReading);

  // collect readings
  emReadingCount = mEMSensor->sense(emReadings, 256 / 2);
  mForwardSensor->sense(forwardReading, 10);

  // TODO: actually choose a heading an speed based on prior state + senses
  *heading = 0;
  *speed = 0;
}
