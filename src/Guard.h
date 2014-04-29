#ifndef __GUARD_H
#define __GUARD_H

#include "NoiseModel.h"
#include "Orientation.h"
#include <memory>

using namespace std;

class Bond;

class Guard {
  public:
  Guard(double x, double y, double heading, bool ccw=true);

  void advance();
  void setSpeedNoiseModel(shared_ptr<NoiseModel> noiseModel);
  void setSpeedMean(double speed);
  void setTurningNoiseModel(shared_ptr<NoiseModel> noiseModel);
  void setTurningMean(double turning);
  void setSightRange(double range);

  bool detect(shared_ptr<Bond> bond);

  Orientation orientation;
  bool ccw;

  private:
  shared_ptr<NoiseModel> mSpeedNoiseModel;
  double mSpeedMean;
  shared_ptr<NoiseModel> mTurningNoiseModel;
  double mTurningMean;
  double mSightRange;
};

#endif
