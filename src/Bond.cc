#include "Bond.h"

Bond::Bond(double x, double y, double heading,
    shared_ptr<NoisyMap> map,
    shared_ptr<EMSensor> emSensor,
    shared_ptr<ForwardSensor> forwardSensor) :
  mX(x),
  mY(y),
  mHeading(heading),
  mMap(map),
  mEMSensor(emSensor),
  mForwardSensor(forwardSensor) {}

Bond::~Bond() {}

void Bond::advance() {
}

double Bond::getX() {
  return mX;
}

double Bond::getY() {
  return mY;
}

double Bond::getHeading() {
  return mHeading;
}
