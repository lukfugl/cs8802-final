#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

#include "DropZone.h"
#include "Coordinate.h"
#include "Guard.h"

#include <string>
#include <vector>

using namespace std;

class Environment {
  public:
  Environment();
  ~Environment();

  void loadMap(string filename);
  unsigned int spawnDropZone(double minX, double minY, double maxX, double maxY);
  unsigned int spawnTargetZone(double minX, double minY, double maxX, double maxY);
  unsigned int spawnObject(double x, double y);
  unsigned int spawnGuard(double x, double y, double h);

  unsigned int dropZoneCount();
  unsigned int centerCount();
  unsigned int guardCount();

  DropZone getDropZone(int index);
  DropZone getTargetZone();
  Coordinate getCenter(int index);
  Guard getGuard(int index);
  double getGuardSpeedMean();
  double getGuardSpeedSigma();
  double getGuardTurningSigma();
  double getMapSigma();
  double getForwardSigma();
  double getBioRangeSigma();
  double getBioHeadingSigma();

  private:
  vector<Coordinate> mCenters;
  vector<Guard> mGuards;
  vector<DropZone> mDropZones;
  DropZone mTargetZone;
  double mGuardSpeedMean;
  double mGuardSpeedSigma;
  double mGuardTurningSigma;
  double mMapSigma;
  double mForwardSigma;
  double mBioRangeSigma;
  double mBioHeadingSigma;
};

#endif
