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

void Simulator::advance() {
  mBond->advance();
  for (int i = 0; i < mEnvironment->guardCount(); i++) {
    shared_ptr<Guard> guard = mEnvironment->getGuard(i);
    guard->advance();
  }
}

void Simulator::spawnBond() {
  default_random_engine rng;
  
  int i = uniform_int_distribution<int>(0, mEnvironment->dropZoneCount() - 1)(rng);
  DropZone zone = mEnvironment->getDropZone(i);
  
  double x = uniform_real_distribution<double>(zone.minX, zone.maxX)(rng);
  double y = uniform_real_distribution<double>(zone.minY, zone.maxY)(rng);
  double h = uniform_real_distribution<double>(2 * M_PI)(rng);
  
  shared_ptr<NoiseModel> mapNoise(new NormalNoise(mEnvironment->getMapSigma()));
  shared_ptr<NoiseModel> forwardNoise(new NormalNoise(mEnvironment->getForwardSigma()));
  shared_ptr<NoiseModel> emRangeNoise(new NormalNoise(mEnvironment->getEMRangeSigma()));
  shared_ptr<NoiseModel> emHeadingNoise(new NormalNoise(mEnvironment->getEMHeadingSigma()));
  
  shared_ptr<NoisyMap> map(new NoisyMap(mEnvironment, mapNoise));
  
  shared_ptr<EMSensor> emSensor(new EMSensor(mEnvironment));
  emSensor->setDistanceNoiseModel(emRangeNoise);
  emSensor->setHeadingNoiseModel(emHeadingNoise);
  
  shared_ptr<ForwardSensor> forwardSensor(new ForwardSensor(mEnvironment));
  forwardSensor->setNoiseModel(forwardNoise);
  
  mBond = shared_ptr<Bond>(new Bond(x, y, h, map, emSensor, forwardSensor));
}