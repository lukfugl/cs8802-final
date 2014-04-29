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
  Bond(double x, double y, double h);
  ~Bond();

  void advance(double heading, double speed);

  double getX();
  double getY();
  double getHeading();

  private:
  double mX;
  double mY;
  double mHeading;
};

#endif
