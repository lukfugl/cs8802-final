#ifndef __BOND_H
#define __BOND_H

#include "Environment.h"
#include "NoisyMap.h"
#include "EMSensor.h"
#include "ForwardSensor.h"

#include <memory>

using namespace std;

class Bond {
  public:
  Bond(double x, double y, double h,
      shared_ptr<NoisyMap> map,
      shared_ptr<EMSensor> emSensor,
      shared_ptr<ForwardSensor> forwardSensor);
  ~Bond();

  void advance();
  
  private:
  double mX;
  double mY;
  double mHeading;
  shared_ptr<NoisyMap> mMap;
  shared_ptr<EMSensor> mEMSensor;
  shared_ptr<ForwardSensor> mForwardSensor;
};

#endif
