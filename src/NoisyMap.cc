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
    Environment::Coordinate obstacle = mEnvironment->getCenter(i);
    double x = mNoise->noisyValue(obstacle.first);
    double y = mNoise->noisyValue(obstacle.second);
    mObstacles.push_back(Environment::Coordinate(x, y));
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

Environment::Coordinate NoisyMap::getObstacle(unsigned int index) {
  return mObstacles.at(index);
}
