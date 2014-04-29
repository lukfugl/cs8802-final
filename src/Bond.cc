#include "Bond.h"

#include <math.h>

Bond::Bond(double x, double y, double heading) :
  mOrientation(x, y, heading) {}

Bond::~Bond() {}

void Bond::advance(double turn, double speed) {
  mOrientation.advance(turn, speed);
}

double Bond::getX() {
  return mOrientation.x;
}

double Bond::getY() {
  return mOrientation.y;
}

double Bond::getHeading() {
  return mOrientation.heading;
}
