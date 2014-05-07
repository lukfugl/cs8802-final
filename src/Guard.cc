#include "Guard.h"
#include "Bond.h"

#include <math.h>

using namespace std;

Guard::Guard(double x, double y, double heading, bool ccw) :
  orientation(x, y, heading),
  ccw(ccw),
  mSpeedNoiseModel(new NoiseModel),
  mSpeedMean(5),
  mTurningNoiseModel(new NoiseModel),
  mTurningMean(0) {}

void Guard::advance() {
  double step = mSpeedNoiseModel->noisyValue(mSpeedMean);
  double turn = mTurningNoiseModel->noisyValue(mTurningMean);
  if (!ccw) turn *= -1;
  orientation.advance(turn, step);
}

void Guard::setSpeedNoiseModel(shared_ptr<NoiseModel> noiseModel) {
  mSpeedNoiseModel = noiseModel;
}

void Guard::setSpeedMean(double speed) {
  mSpeedMean = speed;
}

void Guard::setTurningNoiseModel(shared_ptr<NoiseModel> noiseModel) {
  mTurningNoiseModel = noiseModel;
}

void Guard::setTurningMean(double turning) {
  mTurningMean = turning;
}

void Guard::setSightRange(double range) {
  mSightRange = range;
}

bool Guard::detect(shared_ptr<Bond> bond) {
  double dx = orientation.x - bond->getX();
  double dy = orientation.y - bond->getY();
  return sqrt(dx * dx + dy * dy) <= mSightRange;
}
