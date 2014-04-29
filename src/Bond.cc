#include "Bond.h"

#include <math.h>

Bond::Bond(double x, double y, double heading) :
  mX(x),
  mY(y),
  mHeading(heading) {}

Bond::~Bond() {}

void Bond::advance(double turn, double speed) {
  mHeading += turn;
  mHeading -= 2 * M_PI * floor(mHeading / (2 * M_PI));
  mX += speed * cos(mHeading);
  mY += speed * sin(mHeading);
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
