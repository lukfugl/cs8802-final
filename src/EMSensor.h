#ifndef __EM_SENSOR_H
#define __EM_SENSOR_H

#include "Environment.h"
#include "NoiseModel.h"

using namespace std;

class EMSensor {
  public:
  EMSensor(shared_ptr<Environment> environment);
  ~EMSensor();

  unsigned int sense(double at[3], double *readings, unsigned int maxReadings);
  void setDistanceNoiseModel(shared_ptr<NoiseModel> noiseModel);
  void setHeadingNoiseModel(shared_ptr<NoiseModel> noiseModel);

  private:
  shared_ptr<Environment> mEnvironment;
  shared_ptr<NoiseModel> mDistanceNoiseModel;
  shared_ptr<NoiseModel> mHeadingNoiseModel;
};

#endif
