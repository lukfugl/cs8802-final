#include "Simulator.h"
#include "Bond.h"
#include "NoisyMap.h"
#include "EMSensor.h"
#include "ForwardSensor.h"
#include "NormalNoise.h"

#include <random>

Simulator::Simulator(string filename) :
  mEnvironment(new Environment),
  mTerminated(false),
  mSuccess(false)
{
  mEnvironment->loadMap(filename);
  spawnBond();
}

Simulator::~Simulator() {}

bool Simulator::terminated() {
  return mTerminated;
}

bool Simulator::success() {
  return mSuccess;
}

bool Simulator::goalReached() {
  double x = mBond->getX();
  double y = mBond->getY();
  DropZone targetZone = mEnvironment->getTargetZone();
  return targetZone.minX <= x && x <= targetZone.maxX &&
         targetZone.minY <= y && y <= targetZone.maxY;
}

void Simulator::advance() {
  printf("\t%.6f\t%.6f\t%.6f", mBond->getX(), mBond->getY(), mBond->getHeading()); // true bond position
  for (int i = 0; i < mEnvironment->guardCount(); i++) {
    shared_ptr<Guard> guard = mEnvironment->getGuard(i);
    printf("\t%.6f\t%.6f", guard->orientation.x, guard->orientation.y); // true guard position
  }

  double turn = 0, speed = 0;
  mBrain->decide(&turn, &speed);

  double bx = mBond->getX();
  double by = mBond->getY();
  double theta = mBond->getHeading() + turn;

  double dx = cos(theta);
  double dy = sin(theta);
  for (int j = 0; j < mEnvironment->obstacleCount(); j++) {
    shared_ptr<Obstacle> obstacle = mEnvironment->getObstacle(j);
    double rx = obstacle->location.x - bx;
    double ry = obstacle->location.y - by;
    double dco = dx * rx + dy * ry;
    if (dco <= 0) continue; // obstacle behind bond

    double cor = rx * rx + ry * ry + speed * speed - obstacle->radius * obstacle->radius;
    if (2 * dco * speed < cor) continue; // missed

    // collided; move to spot of collision and terminate (don't return early;
    // guard might detect too!)
    speed = cor / (2 * dco);
    printf("collided!\n");
    mTerminated = true;
    mSuccess = false;
    break;
  }

  mBond->advance(turn, speed);

  if (goalReached()) {
    printf("reached goal!\n");
    mTerminated = true;
    mSuccess = true;
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
    printf("detected!\n");
    mTerminated = true;
    mSuccess = false;
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

  shared_ptr<NoisyMap> trueMap(new NoisyMap(mEnvironment));
  shared_ptr<NoisyMap> noisyMap(new NoisyMap(mEnvironment, mapNoise));

  shared_ptr<EMSensor> rawEMSensor(new EMSensor(mEnvironment));
  rawEMSensor->setDistanceNoiseModel(emRangeNoise);
  rawEMSensor->setHeadingNoiseModel(emHeadingNoise);
  shared_ptr<CoupledEMSensor> emSensor(new CoupledEMSensor(mBond, rawEMSensor));

  shared_ptr<ForwardSensor> rawForwardSensor(new ForwardSensor(trueMap));
  rawForwardSensor->setNoiseModel(forwardNoise);
  shared_ptr<CoupledForwardSensor> forwardSensor(new CoupledForwardSensor(mBond, rawForwardSensor));

  mBrain = shared_ptr<Brain>(new Brain(noisyMap, emSensor, forwardSensor));
}
