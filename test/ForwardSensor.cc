#include "../src/Environment.h"
#include "../src/ForwardSensor.h"
#include "ConstantNoise.h"
#include <math.h>

int main(int argc, char **argv) {
  shared_ptr<Environment> env(new Environment);
  ForwardSensor sensor(env);

  double at[3] = { 0.0, 0.0, 0.0 };
  double reading[ForwardSensor::GRANULARITY];
  double expected[ForwardSensor::GRANULARITY];

  // empty environment, should sense nothing
  sensor.sense(at, reading);
  for (int i = 0; i < ForwardSensor::GRANULARITY; i++) {
    if (reading[i] != ForwardSensor::NONE) {
      printf("\texpected blank reading at %d, got %.8f\n", i, reading[i]);
    }
  }

  // non-empty environment, should sense object
  env->spawnObject(10.0, 0.0, 4.0);

  expected[0] = ForwardSensor::NONE;
  expected[1] = 8.623518165476579;
  expected[2] = 6.791635707202559;
  expected[3] = 6.2446581763283255;
  expected[4] = 6.025567478240387;
  expected[5] = 6.025567478240387;
  expected[6] = 6.2446581763283255;
  expected[7] = 6.791635707202559;
  expected[8] = 8.623518165476579;
  expected[9] = ForwardSensor::NONE;

  sensor.sense(at, reading);
  for (int i = 0; i < ForwardSensor::GRANULARITY; i++) {
    if (fabs(reading[i] - expected[i]) > 0.000001) {
      printf("%.8f\n", fabs(reading[i] - expected[i]));
      printf("\texpected reading %.6f at %d, got %.8f\n", expected[i], i, reading[i]);
    }
  }

  // off-center position, should sense off-center
  at[1] = 1.0;

  expected[0] = ForwardSensor::NONE;
  expected[1] = ForwardSensor::NONE;
  expected[2] = 8.12623419;
  expected[3] = 6.74277738;
  expected[4] = 6.25010756;
  expected[5] = 6.06300723;
  expected[6] = 6.09298721;
  expected[7] = 6.35219463;
  expected[8] = 6.98157037;
  expected[9] = ForwardSensor::NONE;

  sensor.sense(at, reading);
  for (int i = 0; i < ForwardSensor::GRANULARITY; i++) {
    if (fabs(reading[i] - expected[i]) > 0.000001) {
      printf("\texpected reading %.6f at %d, got %.8f\n", expected[i], i, reading[i]);
    }
  }

  // off-axis orientation, should sense
  at[0] = 10.0;
  at[1] = 10.0;
  at[2] = -M_PI / 2;

  expected[0] = ForwardSensor::NONE;
  expected[1] = 8.623518165476579;
  expected[2] = 6.791635707202559;
  expected[3] = 6.2446581763283255;
  expected[4] = 6.025567478240387;
  expected[5] = 6.025567478240387;
  expected[6] = 6.2446581763283255;
  expected[7] = 6.791635707202559;
  expected[8] = 8.623518165476579;
  expected[9] = ForwardSensor::NONE;

  sensor.sense(at, reading);
  for (int i = 0; i < ForwardSensor::GRANULARITY; i++) {
    if (fabs(reading[i] - expected[i]) > 0.000001) {
      printf("\texpected reading %.6f at %d, got %.8f\n", expected[i], i, reading[i]);
    }
  }

  // two objects, should sense both
  env->spawnObject(20.0, 0.0, 4.0);

  at[0] = 15.0;
  at[1] = 10.0;
  at[2] = -M_PI / 2;

  // left object
  expected[0] = 7.21674416;
  expected[1] = 7.21252908;
  expected[2] = 7.50588594;
  expected[3] = 8.29981672;
  expected[4] = ForwardSensor::NONE;
  expected[5] = ForwardSensor::NONE;
  // right object
  expected[6] = 8.29981672;
  expected[7] = 7.50588594;
  expected[8] = 7.21252908;
  expected[9] = 7.21674416;

  sensor.sense(at, reading);
  for (int i = 0; i < ForwardSensor::GRANULARITY; i++) {
    if (fabs(reading[i] - expected[i]) > 0.000001) {
      printf("\texpected reading %.6f at %d, got %.8f\n", expected[i], i, reading[i]);
    }
  }

  // both visible but overlapping, should prefer closer object
  at[0] = 5.0;
  at[1] = 12.0;
  at[2] = -M_PI / 4;

	expected[0] = ForwardSensor::NONE;
	expected[1] = ForwardSensor::NONE;
  // far object
  expected[2] = 16.86280897;
	expected[3] = 15.36277258;
	expected[4] = 15.31192304;
	expected[5] = 16.57379490;
  // near object
	expected[6] = 9.82860306;
	expected[7] = 9.15057338;
	expected[8] = 9.00405156;
	expected[9] = 9.27570219;

  sensor.sense(at, reading);
  for (int i = 0; i < ForwardSensor::GRANULARITY; i++) {
    if (fabs(reading[i] - expected[i]) > 0.000001) {
      printf("\texpected reading %.6f at %d, got %.8f\n", expected[i], i, reading[i]);
    }
  }

  // back to simple viewing, but noisy (but constant noise for testing)
  sensor.setNoiseModel(shared_ptr<NoiseModel>(new ConstantNoise(0.5)));

  at[0] = 0.0;
  at[1] = 0.0;
  at[2] = 0.0;

  expected[0] = ForwardSensor::NONE;
  expected[1] = 9.123518165476579;
  expected[2] = 7.291635707202559;
  expected[3] = 6.7446581763283255;
  expected[4] = 6.525567478240387;
  expected[5] = 6.525567478240387;
  expected[6] = 6.7446581763283255;
  expected[7] = 7.291635707202559;
  expected[8] = 9.123518165476579;
  expected[9] = ForwardSensor::NONE;

  sensor.sense(at, reading);
  for (int i = 0; i < ForwardSensor::GRANULARITY; i++) {
    if (fabs(reading[i] - expected[i]) > 0.000001) {
      printf("\texpected reading %.6f at %d, got %.8f\n", expected[i], i, reading[i]);
    }
  }

  return 0;
}
