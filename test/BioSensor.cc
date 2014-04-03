#include "../src/Environment.h"
#include "../src/BioSensor.h"
#include <math.h>

int main(int argc, char **argv) {
  shared_ptr<Environment> env(new Environment);
  BioSensor sensor(env);

  double at[3] = { 0.0, 0.0, 0.0 };
  double readings[16];
  double expected[16];
  unsigned int readingCount;

  // empty environment, should sense nothing
  readingCount = sensor.sense(at, readings, 8);
  if (readingCount != 0) {
    printf("\texpected 0 readings, got %d\n", readingCount);
  }

  for (int i = 0; i < readingCount; i++) {
    if (fabs(readings[2*i] - expected[2*i]) > 0.000001 || fabs(readings[2*i+1] - expected[2*i+1]) > 0.000001) {
      printf("\texpected reading <%.8f, %.8f> at %d, got <%.8f, %.8f>\n",
          expected[2*i], expected[2*i+1], i,
          readings[2*i], readings[2*i+1]);
    }
  }

  // add guard, should sense it
  env->spawnGuard(15.0, 15.0);

  readingCount = sensor.sense(at, readings, 8);
  if (readingCount != 1) {
    printf("\texpected 1 reading, got %d\n", readingCount);
  }

  expected[0] = sqrt(2) * 15; // distance
  expected[1] = M_PI / 4; // heading

  for (int i = 0; i < readingCount; i++) {
    if (fabs(readings[2*i] - expected[2*i]) > 0.000001 || fabs(readings[2*i+1] - expected[2*i+1]) > 0.000001) {
      printf("\texpected reading <%.8f, %.8f> at %d, got <%.8f, %.8f>\n",
          expected[2*i], expected[2*i+1], i,
          readings[2*i], readings[2*i+1]);
    }
  }

  return 0;
}
