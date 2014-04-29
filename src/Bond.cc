#include "Bond.h"

#include <math.h>

Bond::Bond(double x, double y, double heading) :
  mX(x),
  mY(y),
  mHeading(heading) {}

Bond::~Bond() {}

void Bond::advance(double heading, double speed) {
  mHeading = heading - 2 * M_PI * floor(heading / (2 * M_PI));
  mX += speed * cos(heading);
  mY += speed * sin(heading);
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
