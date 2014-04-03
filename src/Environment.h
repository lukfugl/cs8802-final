#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

#include <string>
#include <vector>

class Environment {
  public:
  typedef std::pair<double, double> Coordinate;

  Environment();
  Environment(std::string filename);
  ~Environment();

  unsigned int spawnObject(double x, double y);
  unsigned int centerCount();
  Coordinate getCenter(int index);

  private:
  std::vector<Coordinate> mCenters;
};

#endif
