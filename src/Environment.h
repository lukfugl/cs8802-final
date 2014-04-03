#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

#include <string>
#include <vector>

using namespace std;

class Environment {
  public:
  typedef pair<double, double> Coordinate;

  Environment();
  ~Environment();

  void loadMap(string filename);
  unsigned int spawnObject(double x, double y);
  unsigned int spawnGuard(double x, double y);

  unsigned int centerCount();
  unsigned int guardCount();
  Coordinate getCenter(int index);
  Coordinate getGuard(int index);

  private:
  vector<Coordinate> mCenters;
  vector<Coordinate> mGuards;
};

#endif
