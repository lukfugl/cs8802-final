#include "Simulator.h"
#include "Bond.h"
#include "NoisyMap.h"
#include "EMSensor.h"
#include "ForwardSensor.h"
#include "NormalNoise.h"

#include <random>

Simulator::Simulator(string filename) :
  mEnvironment(new Environment),
  mTerminated(false)
{
  mEnvironment->loadMap(filename);
  spawnBond();
}

Simulator::~Simulator() {}

bool Simulator::terminated() {
  return mTerminated;
}

bool Simulator::goalReached() {
  double x = mBond->getX();
  double y = mBond->getY();
  DropZone targetZone = mEnvironment->getTargetZone();
  return targetZone.minX <= x && x <= targetZone.maxX &&
         targetZone.minY <= y && y <= targetZone.maxY;
}

void Simulator::advance() {
  double heading = 0, speed = 0;
  mBrain->decide(&heading, &speed);
  mBond->advance(heading, speed);

  if (goalReached()) {
    // TODO actual goal handling
    printf("reached goal!\n");
    mTerminated = true;
    return;
  }

  bool detected = false;
  for (int i = 0; i < mEnvironment->guardCount(); i++) {
    shared_ptr<Guard> guard = mEnvironment->getGuard(i);
    guard->advance();
    if (guard->detect(mBond))
      detected = true;
  }

  if (detected) {
    // TODO actual detection handling
    printf("detected!\n");
    mTerminated = true;
  }
}

void Simulator::spawnBond() {
  default_random_engine rng;

  int i = uniform_int_distribution<int>(0, mEnvironment->dropZoneCount() - 1)(rng);
  DropZone zone = mEnvironment->getDropZone(i);

  double x = uniform_real_distribution<double>(zone.minX, zone.maxX)(rng);
  double y = uniform_real_distribution<double>(zone.minY, zone.maxY)(rng);
  double h = uniform_real_distribution<double>(2 * M_PI)(rng);
  mBond = shared_ptr<Bond>(new Bond(x, y, h));

  shared_ptr<NoiseModel> mapNoise(new NormalNoise(mEnvironment->getMapSigma()));
  shared_ptr<NoiseModel> forwardNoise(new NormalNoise(mEnvironment->getForwardSigma()));
  shared_ptr<NoiseModel> emRangeNoise(new NormalNoise(mEnvironment->getEMRangeSigma()));
  shared_ptr<NoiseModel> emHeadingNoise(new NormalNoise(mEnvironment->getEMHeadingSigma()));

  shared_ptr<NoisyMap> map(new NoisyMap(mEnvironment, mapNoise));

  shared_ptr<EMSensor> rawEMSensor(new EMSensor(mEnvironment));
  rawEMSensor->setDistanceNoiseModel(emRangeNoise);
  rawEMSensor->setHeadingNoiseModel(emHeadingNoise);
  shared_ptr<CoupledEMSensor> emSensor(new CoupledEMSensor(mBond, rawEMSensor));

  shared_ptr<ForwardSensor> rawForwardSensor(new ForwardSensor(mEnvironment));
  rawForwardSensor->setNoiseModel(forwardNoise);
  shared_ptr<CoupledForwardSensor> forwardSensor(new CoupledForwardSensor(mBond, rawForwardSensor));

  mBrain = shared_ptr<Brain>(new Brain(map, emSensor, forwardSensor));
}
