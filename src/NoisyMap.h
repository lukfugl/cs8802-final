#include "Environment.h"
#include "NoiseModel.h"

class NoisyMap {
  public:
  NoisyMap(shared_ptr<Environment> environment, shared_ptr<NoiseModel> noise);

  unsigned int dropZoneCount();
  unsigned int obstacleCount();

  DropZone getDropZone(unsigned int index);
  DropZone getTargetZone();
  shared_ptr<Obstacle> getObstacle(unsigned int index);

  private:
  shared_ptr<Environment> mEnvironment;
  shared_ptr<NoiseModel> mNoise;

  vector<DropZone> mDropZones;
  DropZone mTargetZone;
  vector<shared_ptr<Obstacle> > mObstacles;
};
