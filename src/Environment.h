#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

#include "DropZone.h"
#include "Obstacle.h"
#include "Guard.h"

#include <string>
#include <vector>

class Bond;

using namespace std;

class Environment {
  public:
  Environment();
  ~Environment();

  void loadMap(string filename);
  unsigned int spawnDropZone(double minX, double minY, double maxX, double maxY);
  unsigned int spawnTargetZone(double minX, double minY, double maxX, double maxY);
  unsigned int spawnObject(double x, double y, double radius);
  shared_ptr<Guard> spawnGuard(double x, double y, double h, bool ccw=false);

  unsigned int dropZoneCount();
  unsigned int obstacleCount();
  unsigned int guardCount();

  DropZone getDropZone(int index);
  DropZone getTargetZone();
  shared_ptr<Obstacle> getObstacle(int index);
  shared_ptr<Guard> getGuard(int index);
  double getGuardSpeedMean();
  double getGuardSpeedSigma();
  double getGuardTurningSigma();
  double getGuardTurningMean();
  double getMapSigma();
  double getForwardSigma();
  double getEMRangeSigma();
  double getEMHeadingSigma();

  private:
  vector<shared_ptr<Obstacle> > mObstacles;
  vector<shared_ptr<Guard> > mGuards;
  vector<DropZone> mDropZones;
  DropZone mTargetZone;
  double mGuardSpeedMean;
  double mGuardSpeedSigma;
  double mGuardTurningMean;
  double mGuardTurningSigma;
  double mMapSigma;
  double mForwardSigma;
  double mEMRangeSigma;
  double mEMHeadingSigma;
};

#endif
