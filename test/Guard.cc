#include "../src/Guard.h"
#include "ConstantNoise.h"

#include <stdio.h>
#include <math.h>

using namespace std;

void expect(Guard guard, double x, double y, double heading) {
  if (fabs(guard.location.x - x) > 0.000001 ||
      fabs(guard.location.y - y) > 0.000001 ||
      fabs(guard.heading - heading) > 0.000001) {
    printf("\texpected new configuration <%f, %f, %f>, got <%f, %f, %f>\n",
        x, y, heading,
        guard.location.x, guard.location.y, guard.heading);
  }
}

int main(int argc, char **argv) {
  Guard guard(Coordinate(10, 10), 0);

  // should update location
  guard.advance();
  expect(guard, 15, 10, 0);

  // should use heading when updating location
  guard.location.x = 10;
  guard.heading = M_PI / 2;
  guard.advance();
  expect(guard, 10, 15, M_PI / 2);

  // should use configured speed when updating location
  guard.setSpeedMean(10);

  guard.location.x = 10;
  guard.location.y = 10;
  guard.heading = 0;
  guard.advance();
  expect(guard, 20, 10, 0);

  // should apply noise when updating location
  shared_ptr<NoiseModel> speedNoise(new ConstantNoise(0.2));
  guard.setSpeedNoiseModel(speedNoise);
  guard.setSpeedMean(5);

  guard.location.x = 10;
  guard.location.y = 10;
  guard.heading = 0;
  guard.advance();
  expect(guard, 15.2, 10, 0);

  // should apply turning noise before updating location
  shared_ptr<NoiseModel> turningNoise(new ConstantNoise(M_PI / 2));
  guard.setTurningNoiseModel(turningNoise);

  guard.location.x = 10;
  guard.location.y = 10;
  guard.heading = 0;
  guard.advance();
  expect(guard, 10, 15.2, M_PI / 2);

  // should include static turning parameter when updating
  guard.setTurningMean(M_PI / 2);

  guard.location.x = 10;
  guard.location.y = 10;
  guard.heading = 0;
  guard.advance();
  expect(guard, 4.8, 10, M_PI);

  // should turn the opposite direction if ccw=false
  guard.setTurningMean(0); // will still get turning from noise
  guard.ccw = false;

  guard.location.x = 10;
  guard.location.y = 10;
  guard.heading = 0;
  guard.advance();
  expect(guard, 10, 4.8, 3 * M_PI / 2);

  return 0;
}
