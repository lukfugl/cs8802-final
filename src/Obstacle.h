#include "Coordinate.h"

class Obstacle {
  public:
  Obstacle(Coordinate location, double radius) :
    location(location),
    radius(radius) {}

  Coordinate location;
  double radius;
};
