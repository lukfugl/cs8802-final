#include "NoisyMap.h"
#include <stdlib.h>

using namespace std;

NoisyMap::NoisyMap(shared_ptr<Environment> environment, shared_ptr<NoiseModel> noise) :
  mEnvironment(environment),
  mNoise(noise) {

  for (int i = 0; i < mEnvironment->dropZoneCount(); i++) {
    DropZone sourceDropZone(mEnvironment->getDropZone(i));
    DropZone noisyDropZone;
    noisyDropZone.minX = mNoise->noisyValue(sourceDropZone.minX);
    noisyDropZone.minY = mNoise->noisyValue(sourceDropZone.minY);
    noisyDropZone.maxX = mNoise->noisyValue(sourceDropZone.maxX);
    noisyDropZone.maxY = mNoise->noisyValue(sourceDropZone.maxY);
    mDropZones.push_back(noisyDropZone);
  }

  DropZone sourceTargetZone(mEnvironment->getTargetZone());
  mTargetZone.minX = mNoise->noisyValue(sourceTargetZone.minX);
  mTargetZone.minY = mNoise->noisyValue(sourceTargetZone.minY);
  mTargetZone.maxX = mNoise->noisyValue(sourceTargetZone.maxX);
  mTargetZone.maxY = mNoise->noisyValue(sourceTargetZone.maxY);

  for (int i = 0; i < mEnvironment->centerCount(); i++) {
    Coordinate obstacle = mEnvironment->getCenter(i);
    double x = mNoise->noisyValue(obstacle.x);
    double y = mNoise->noisyValue(obstacle.y);
    mObstacles.push_back(Coordinate(x, y));
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

Coordinate NoisyMap::getObstacle(unsigned int index) {
  return mObstacles.at(index);
}
