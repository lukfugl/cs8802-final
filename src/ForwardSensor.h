#ifndef __FORWARD_SENSOR_H
#define __FORWARD_SENSOR_H

#include "NoisyMap.h"
#include "NoiseModel.h"

using namespace std;

class ForwardSensor {
  public:
  static const double NONE;

  ForwardSensor(shared_ptr<NoisyMap> map);
  ~ForwardSensor();

  void sense(double at[3], double *reading, unsigned int granularity=10);
  void setNoiseModel(shared_ptr<NoiseModel> noiseModel);

  private:
  shared_ptr<NoisyMap> mMap;
  shared_ptr<NoiseModel> mNoiseModel;
};

#endif
