#include "../src/Environment.h"

int main(int argc, char **argv) {
  Environment env;
  env.loadMap("test/map.json");

  double expected[12];

  // loads drop zones
  if (env.dropZoneCount() != 3) {
    printf("\texpected 3 drop zones, got %d\n", env.dropZoneCount());
  }

  expected[0] = 0;
  expected[1] = 0;
  expected[2] = 10;
  expected[3] = 10;
  expected[4] = 0;
  expected[5] = 90;
  expected[6] = 10;
  expected[7] = 100;
  expected[8] = 90;
  expected[9] = 0;
  expected[10] = 100;
  expected[11] = 10;

  for (int i = 0; i < env.dropZoneCount(); i++) {
    DropZone dropZone = env.getDropZone(i);
    if (expected[4*i] != dropZone.minX || expected[4*i+1] != dropZone.minY || expected[4*i+2] != dropZone.maxX || expected[4*i+3] != dropZone.maxY) {
      printf("\texpected drop zone <%f, %f, %f, %f> at %d, got <%f, %f, %f, %f>\n",
          expected[4*i], expected[4*i+1], expected[4*i+2], expected[4*i+3], i,
          dropZone.minX, dropZone.minY, dropZone.maxX, dropZone.maxY);
    }
  }

  // loads target zone
  expected[0] = 90;
  expected[1] = 90;
  expected[2] = 100;
  expected[3] = 100;

  DropZone targetZone = env.getTargetZone();
  if (expected[0] != targetZone.minX || expected[1] != targetZone.minY || expected[2] != targetZone.maxX || expected[3] != targetZone.maxY) {
    printf("\texpected target zone <%f, %f, %f, %f>, got <%f, %f, %f, %f>\n",
        expected[0], expected[1], expected[2], expected[3],
        targetZone.minX, targetZone.minY, targetZone.maxX, targetZone.maxY);
  }

  // loads obstacles
  if (env.obstacleCount() != 2) {
    printf("\texpected 2 obstacles, got %d\n", env.obstacleCount());
  }

  expected[0] = 50;
  expected[1] = 50;
  expected[2] = 30;
  expected[3] = 60;

  for (int i = 0; i < env.obstacleCount(); i++) {
    shared_ptr<Obstacle> obstacle = env.getObstacle(i);
    if (expected[2*i] != obstacle->location.x || expected[2*i+1] != obstacle->location.y) {
      printf("\texpected <%f, %f> at %d obstacles, got <%f, %f>\n", expected[2*i], expected[2*i+1], i, obstacle->location.x, obstacle->location.y);
    }
  }

  // loads guards
  expected[0] = 40;
  expected[1] = 70;
  expected[2] = 4.71;

  expected[3] = 20;
  expected[4] = 30;
  expected[5] = 2.09;

  if (env.guardCount() != 2) {
    printf("\texpected 2 guards, got %d\n", env.guardCount());
  }

  for (int i = 0; i < env.guardCount(); i++) {
    shared_ptr<Guard> guard = env.getGuard(i);
    Coordinate location = guard->location;
    if (expected[3*i] != location.x || expected[3*i+1] != location.y || expected[3*i+2] != guard->heading) {
      printf("\texpected <%f, %f, %f> at %d obstacles, got <%f, %f, %f>\n",
        expected[3*i], expected[3*i+1], expected[3*i+2], i,
        location.x, location.y, guard->heading);
    }
  }

  if (!env.getGuard(0)->ccw) {
    printf("\texpected first guard to be ccw, was not\n");
  }

  if (env.getGuard(1)->ccw) {
    printf("\texpected second guard not to be ccw, was\n");
  }

  // loads guard behavior
  expected[0] = 5;
  expected[1] = 0.2;
  expected[2] = 0.3;
  expected[3] = 0.05;

  double guardSpeedMean = env.getGuardSpeedMean();
  double guardSpeedSigma = env.getGuardSpeedSigma();
  double guardTurningMean = env.getGuardTurningMean();
  double guardTurningSigma = env.getGuardTurningSigma();
  if (expected[0] != guardSpeedMean || expected[1] != guardSpeedSigma || expected[2] != guardTurningMean || expected[3] != guardTurningSigma) {
    printf("\texpected guard parameters <%f, %f, %f, %f>, got <%f, %f, %f, %f>\n",
        expected[0], expected[1], expected[2], expected[3],
        guardSpeedMean, guardSpeedSigma, guardTurningMean, guardTurningSigma);
  }

  // loads noise parameters
  expected[0] = 5;
  expected[1] = 2;
  expected[2] = 2;
  expected[3] = 0.2;

  double mapSigma = env.getMapSigma();
  double forwardSigma = env.getForwardSigma();
  double bioRangeSigma = env.getBioRangeSigma();
  double bioHeadingSigma = env.getBioHeadingSigma();
  if (expected[0] != mapSigma || expected[1] != forwardSigma || expected[2] != bioRangeSigma || expected[3] != bioHeadingSigma) {
    printf("\texpected noise parameters <%f, %f, %f, %f>, got <%f, %f, %f, %f>\n",
        expected[0], expected[1], expected[2], expected[3],
        mapSigma, forwardSigma, bioRangeSigma, bioHeadingSigma);
  }

  return 0;
}
