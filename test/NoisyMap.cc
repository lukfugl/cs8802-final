#include "../src/Environment.h"
#include "../src/NoisyMap.h"
#include "DeterministicNoise.h"

#include <math.h>

int main(int argc, char **argv) {
  shared_ptr<Environment> env(new Environment);
  env->spawnDropZone(0, 0, 5, 5);
  env->spawnTargetZone(35, 35, 40, 40);
  env->spawnObject(10, 10, 4);
  env->spawnObject(20, 30, 4);
  env->spawnGuard(5, 35, 0);

  shared_ptr<NoiseModel> noise(new DeterministicNoise);

  NoisyMap map(env, noise);

  double expected[4];

  // should see the drop zone without noise
  unsigned int dropZoneCount = map.dropZoneCount();
  if (dropZoneCount != 1) {
    printf("\texpected 1 drop zone, got %d\n", dropZoneCount);
  }

  expected[0] = 0.0;
  expected[1] = 0.0;
  expected[2] = 5.0;
  expected[3] = 5.0;

  for (int i = 0; i < dropZoneCount; i++) {
    DropZone dropZone = map.getDropZone(i);
    if (fabs(dropZone.minX - expected[4*i]) > 0.000001 ||
        fabs(dropZone.minY - expected[4*i+1]) > 0.000001 ||
        fabs(dropZone.maxX - expected[4*i+2]) > 0.000001 ||
        fabs(dropZone.maxY - expected[4*i+3]) > 0.000001) {
      printf("\texpected drop zone <%.8f, %.8f, %.8f, %.8f> at %d, got <%.8f, %.8f, %.8f, %.8f>\n",
        expected[4*i], expected[4*i+1], expected[4*i+2], expected[4*i+3], i,
        dropZone.minX, dropZone.minY, dropZone.maxX, dropZone.maxY);
    }
  }

  // should see the target zone without noise
  expected[0] = 35.0;
  expected[1] = 35.0;
  expected[2] = 40.0;
  expected[3] = 40.0;

  DropZone targetZone = map.getTargetZone();
  if (fabs(targetZone.minX - expected[0]) > 0.000001 ||
      fabs(targetZone.minY - expected[1]) > 0.000001 ||
      fabs(targetZone.maxX - expected[2]) > 0.000001 ||
      fabs(targetZone.maxY - expected[3]) > 0.000001) {
    printf("\texpected target zone <%.8f, %.8f, %.8f, %.8f>, got <%.8f, %.8f, %.8f, %.8f>\n",
      expected[0], expected[1], expected[2], expected[3],
      targetZone.minX, targetZone.minY, targetZone.maxX, targetZone.maxY);
  }

  // should include both obstacles with noise, but no guard
  unsigned int obstacleCount = map.obstacleCount();
  if (obstacleCount != 2) {
    printf("\texpected 2 obstacles, got %d\n", obstacleCount);
  }

  expected[0] = 10.1;
  expected[1] = 10.2;
  expected[2] = 20.3;
  expected[3] = 30.4;

  for (int i = 0; i < obstacleCount; i++) {
    shared_ptr<Obstacle> obstacle = map.getObstacle(i);
    if (fabs(obstacle->location.x - expected[2*i]) > 0.000001 ||
        fabs(obstacle->location.y - expected[2*i+1]) > 0.000001) {
      printf("\texpected obstacle <%.8f, %.8f> at %d, got <%.8f, %.8f>\n",
        expected[2*i], expected[2*i+1], i, obstacle->location.x, obstacle->location.y);
    }
  }

  // should reuse the same noise for the same values
  for (int i = 0; i < obstacleCount; i++) {
    shared_ptr<Obstacle> obstacle = map.getObstacle(i);
    if (fabs(obstacle->location.x - expected[2*i]) > 0.000001 ||
        fabs(obstacle->location.y - expected[2*i+1]) > 0.000001) {
      printf("\tstill expected obstacle <%.8f, %.8f> at %d, got <%.8f, %.8f>\n",
        expected[2*i], expected[2*i+1], i, obstacle->location.x, obstacle->location.y);
    }
  }

  return 0;
}
