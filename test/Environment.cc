#include "../src/Environment.h"

int main(int argc, char **argv) {
  Environment env;
  env.loadMap("map.json");

  // loads obstacles
  if (env.centerCount() != 2) {
    printf("\texpected 2 obstacles, got %d\n", env.centerCount());
  }

  double expected[4] = { 50, 50, 30, 60 };
  for (int i = 0; i < env.centerCount(); i++) {
    Environment::Coordinate center = env.getCenter(i);
    if (expected[2*i] != center.first || expected[2*i+1] != center.second) {
      printf("\texpected <%f, %f> at %d obstacles, got <%f, %f>\n", expected[2*i], expected[2*i+1], i, center.first, center.second);
    }
  }

  // loads guards
  expected[0] = 40;
  expected[1] = 70;
  expected[2] = 20;
  expected[3] = 30;

  if (env.guardCount() != 2) {
    printf("\texpected 2 guards, got %d\n", env.guardCount());
  }

  for (int i = 0; i < env.guardCount(); i++) {
    Environment::Coordinate guard = env.getGuard(i);
    if (expected[2*i] != guard.first || expected[2*i+1] != guard.second) {
      printf("\texpected <%f, %f> at %d obstacles, got <%f, %f>\n", expected[2*i], expected[2*i+1], i, guard.first, guard.second);
    }
  }

  return 0;
}
