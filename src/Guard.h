#ifndef __GUARD_H
#define __GUARD_H

#include "Coordinate.h"

class Guard {
  public:
  Guard(Coordinate location, double heading) :
    location(location),
    heading(heading) {}

  Coordinate location;
  double heading;
};

#endif
