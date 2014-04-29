#ifndef __OBSERVATION_H__
#define __OBSERVATION_H__

#include "CoupledEMSensor.h"
#include "CoupledForwardSensor.h"

#include <memory>

using namespace std;

class Observation {
  public:
  Observation(shared_ptr<CoupledEMSensor> emSensor, unsigned int maxEMReadings, shared_ptr<CoupledForwardSensor> forwardSensor, unsigned int forwardGranularity) :
    forwardGranularity(forwardGranularity)
  {
    // space for readings
    emReadings = new double[maxEMReadings * 2];
    forwardReading = new double[forwardGranularity];

    // for auto-cleanup
    mManagedEMReadings = std::unique_ptr<double[]>(emReadings);
    mManagedForwardReading = std::unique_ptr<double[]>(forwardReading);

    // collect readings
    emReadingCount = emSensor->sense(emReadings, maxEMReadings);
    forwardSensor->sense(forwardReading, forwardGranularity);
  }

  ~Observation() {}

  double *emReadings;
  unsigned int emReadingCount;
  double *forwardReading;
  unsigned int forwardGranularity;

  private:
  std::unique_ptr<double[]> mManagedEMReadings;
  std::unique_ptr<double[]> mManagedForwardReading;
};

#endif
