#include "NoisyMap.h"
#include <stdlib.h>

using namespace std;

NoisyMap::NoisyMap(shared_ptr<Environment> environment) :
  mEnvironment(environment),
  mNoise(new NoiseModel) {
  refresh();
}

NoisyMap::NoisyMap(shared_ptr<Environment> environment, shared_ptr<NoiseModel> noise) :
  mEnvironment(environment),
  mNoise(noise) {
  refresh();
}

void NoisyMap::refresh() {
  mDropZones.clear();
  mObstacles.clear();

  for (int i = 0; i < mEnvironment->dropZoneCount(); i++) {
    mDropZones.push_back(mEnvironment->getDropZone(i));
  }

  mTargetZone = mEnvironment->getTargetZone();

  for (int i = 0; i < mEnvironment->obstacleCount(); i++) {
    shared_ptr<Obstacle> obstacle = mEnvironment->getObstacle(i);
    double x = mNoise->noisyValue(obstacle->location.x);
    double y = mNoise->noisyValue(obstacle->location.y);
    Coordinate location = Coordinate(x, y);
    obstacle = shared_ptr<Obstacle>(new Obstacle(location, obstacle->radius));
    mObstacles.push_back(obstacle);
  }
}

unsigned int NoisyMap::dropZoneCount() {
  return mDropZones.size();
}

unsigned int NoisyMap::obstacleCount() {
  return mObstacles.size();
}

DropZone NoisyMap::getDropZone(unsigned int index) {
  return mDropZones.at(index);
}

DropZone NoisyMap::getTargetZone() {
  return mTargetZone;
}

shared_ptr<Obstacle> NoisyMap::getObstacle(unsigned int index) {
  return mObstacles.at(index);
}
