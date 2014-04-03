#include "Environment.h"

Environment::Environment() {}
Environment::Environment(std::string filename) {}
Environment::~Environment() {}

unsigned int Environment::spawnObject(double x, double y) {
  mCenters.push_back(Coordinate(x, y));
  return 0;
}

unsigned int Environment::spawnGuard(double x, double y) {
  mGuards.push_back(Coordinate(x, y));
  return 0;
}

unsigned int Environment::centerCount() {
  return mCenters.size();
}

unsigned int Environment::guardCount() {
  return mGuards.size();
}

Environment::Coordinate Environment::getCenter(int index) {
  return mCenters.at(index);
}

Environment::Coordinate Environment::getGuard(int index) {
  return mGuards.at(index);
}
