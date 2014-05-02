#ifndef __ORIENTATION_H
#define __ORIENTATION_H

#include <math.h>

class Orientation {
  public:
  double x;
  double y;
  double heading;

  Orientation() : x(0), y(0), heading(0) {}
  Orientation(double x, double y, double h) : x(x), y(y), heading(h) {}
  ~Orientation() {}
  void advance(double turn, double speed) {
    heading += turn;
    heading -= 2 * M_PI * floor(heading / (2 * M_PI));
    x += speed * cos(heading);
    y += speed * sin(heading);
  }
};

#endif
