#include "OnlineSLAM.h"
#include "NormalNoise.h"

#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <limits>

#define POSITION_NOISE 1
#define MAP_NOISE 10
#define MOTION_NOISE 1
#define MEASUREMENT_NOISE 10

OnlineSLAM::OnlineSLAM(Orientation orientation, double headingVariance, shared_ptr<NoisyMap> map) :
  mOrientation(orientation),
  mHeadingSigma(sqrt(headingVariance)),
  mMap(map),
  mDimension(2 * (1 + map->obstacleCount())),
  mOmega(ublas::zero_matrix<double>(mDimension, mDimension)),
  mXi(ublas::zero_vector<double>(mDimension)),
  mMu(ublas::zero_vector<double>(mDimension)),
  mMotion(ublas::zero_vector<double>(2)),
  mPositionWeight(1.0 / POSITION_NOISE),
  mMapWeight(1.0 / MAP_NOISE),
  mMotionWeight(1.0 / MOTION_NOISE),
  mMeasurementWeight(1.0 / MEASUREMENT_NOISE) {

  mXi(0) = mPositionWeight * mOrientation.x;
  mXi(1) = mPositionWeight * mOrientation.y;
  ublas::subrange(mOmega, 0, 2, 0, 2) = mPositionWeight * ublas::identity_matrix<double>(2);

  ublas::matrix<double> mapBlock = mMapWeight * ublas::identity_matrix<double>(2);
  for (int i = 0; i < map->obstacleCount(); i++) {
    unsigned int m = 2 * (i + 1);

    Coordinate coord = map->getObstacle(i)->location;
    double dx = mMapWeight * (coord.x - mOrientation.x);
    double dy = mMapWeight * (coord.y - mOrientation.y);
    mXi(0)  -= dx;
    mXi(1)  -= dy;
    mXi(m)   = dx;
    mXi(m+1) = dy;

    ublas::subrange(mOmega, 0,   2, 0,   2) +=  mapBlock;
    ublas::subrange(mOmega, m, m+2, 0,   2)  = -mapBlock;
    ublas::subrange(mOmega, 0,   2, m, m+2)  = -mapBlock;
    ublas::subrange(mOmega, m, m+2, m, m+2)  =  mapBlock;
  }

  mMu = ublas::prod(inverse(mOmega), mXi);
}

OnlineSLAM::~OnlineSLAM() {}

void OnlineSLAM::advance(double turn, double speed) {
  Orientation prior = mOrientation;
  mOrientation.advance(turn, speed);
  mMotion(0) = mOrientation.x - prior.x;
  mMotion(1) = mOrientation.y - prior.y;
}

void OnlineSLAM::update(shared_ptr<Observation> observation) {
  map<int, ublas::vector<double> > centers = processObservation(observation);

  expand();

  ublas::vector<double> motionIncrementVector = mMotionWeight * mMotion;
  ublas::subrange(mXi, 0, 2) -= motionIncrementVector;
  ublas::subrange(mXi, 2, 4) += motionIncrementVector;

  ublas::matrix<double> motionIncrementMatrix = mMotionWeight * ublas::identity_matrix<double>(2);
  ublas::subrange(mOmega, 0, 2, 0, 2) += motionIncrementMatrix;
  ublas::subrange(mOmega, 2, 4, 2, 4) += motionIncrementMatrix;
  ublas::subrange(mOmega, 0, 2, 2, 4) -= motionIncrementMatrix;
  ublas::subrange(mOmega, 2, 4, 0, 2) -= motionIncrementMatrix;

  for (map<int, ublas::vector<double> >::iterator it = centers.begin(), e = centers.end(); it != e; it++) {
    unsigned int i = it->first;
    unsigned int m = 2 * (2 + i);

    ublas::vector<double> measurementIncrementVector = mMeasurementWeight * it->second;
    ublas::subrange(mXi, 2,   4) -= measurementIncrementVector;
    ublas::subrange(mXi, m, m+2) += measurementIncrementVector;

    ublas::matrix<double> measurementIncrementMatrix = mMeasurementWeight * ublas::identity_matrix<double>(2);
    ublas::subrange(mOmega, 2,   4, 2,   4) += measurementIncrementMatrix;
    ublas::subrange(mOmega, m, m+2, m, m+2) += measurementIncrementMatrix;
    ublas::subrange(mOmega, 2,   4, m, m+2) -= measurementIncrementMatrix;
    ublas::subrange(mOmega, m, m+2, 2,   4) -= measurementIncrementMatrix;
  }

  contract();

  refineModel(observation);
}

Orientation OnlineSLAM::estimate() {
  return mOrientation;
}

void OnlineSLAM::expand() {
  // expand matrix
  ublas::matrix<double> newOmega = ublas::zero_matrix<double>(mDimension + 2, mDimension + 2);
  ublas::subrange(newOmega, 0, 2, 0, 2) = ublas::subrange(mOmega, 0, 2, 0, 2);
  ublas::subrange(newOmega, 4, mDimension + 2, 0, 2) = ublas::subrange(mOmega, 2, mDimension, 0, 2);
  ublas::subrange(newOmega, 0, 2, 4, mDimension + 2) = ublas::subrange(mOmega, 0, 2, 2, mDimension);
  ublas::subrange(newOmega, 4, mDimension + 2, 4, mDimension + 2) = ublas::subrange(mOmega, 2, mDimension, 2, mDimension);
  mOmega = newOmega;

  ublas::vector<double> newXi = ublas::zero_vector<double>(mDimension + 2);
  ublas::subrange(newXi, 0, 2) = ublas::subrange(mXi, 0, 2);
  ublas::subrange(newXi, 4, mDimension + 2) = ublas::subrange(mXi, 2, mDimension);
  mXi = newXi;
}

void OnlineSLAM::contract() {
  // chunk
  ublas::matrix<double> OmegaPrime = ublas::subrange(mOmega, 2, mDimension + 2, 2, mDimension + 2);
  ublas::vector<double> XiPrime = ublas::subrange(mXi, 2, mDimension + 2);
  ublas::matrix<double> A = ublas::subrange(mOmega, 0, 2, 2, mDimension + 2);
  ublas::matrix<double> B = ublas::subrange(mOmega, 0, 2, 0, 2);
  ublas::vector<double> C = ublas::subrange(mXi, 0, 2);

  // update
  ublas::matrix<double> AtBi = ublas::prod(ublas::trans(A), inverse(B));
  mOmega = OmegaPrime - ublas::prod(AtBi, A);
  mXi = XiPrime - ublas::prod(AtBi, C);
}

map<int, ublas::vector<double> > OnlineSLAM::processObservation(shared_ptr<Observation> observation) {
  // convert forward readings (ranges) to estimated positions
  ublas::vector<double> reading(2);
  vector<ublas::vector<double> > readings;
  for (int i = 0; i < observation->forwardGranularity; i++) {
    double range = observation->forwardReading[i];
    if (range == ForwardSensor::NONE) continue;
    double heading = mOrientation.heading + (M_PI / 6) * (1 - 2.0 * i / (observation->forwardGranularity - 1));
    reading(0) = mOrientation.x + range * cos(heading);
    reading(1) = mOrientation.y + range * sin(heading);
    readings.push_back(reading);
  }

  // assign forward readings to best-match obstacles
  map<int, vector<ublas::vector<double> > > obstacleReadings;
  for (vector<ublas::vector<double> >::iterator it = readings.begin(), e = readings.end(); it != e; it++) {
    reading = *it;

    int obstacleIndex = -1;
    double bestError = numeric_limits<double>::infinity();
    for (int j = 0; j < mMap->obstacleCount(); j++) {
      double err = error(reading, mMap->getObstacle(j));
      if (obstacleIndex < 0 || err < bestError) {
        obstacleIndex = j;
        bestError = err;
      }
    }

    obstacleReadings[obstacleIndex].push_back(reading);
  }

  map<int, ublas::vector<double> > centers;
  for (map<int, vector<ublas::vector<double> > >::iterator it = obstacleReadings.begin(), e = obstacleReadings.end(); it != e; it++) {
    unsigned int i = it->first;

    shared_ptr<Obstacle> obstacle = mMap->getObstacle(i);
    vector<ublas::vector<double> >& readings = it->second;

    // convert readings into an estimate (cx, cy) of the center
    ublas::vector<double> center = ublas::zero_vector<double>(2);
    ublas::vector<double> candidate(2);
    double totalWeight = 0;
    NormalNoise noise(10);
    double tuningVariance = 10000;
    for (int k = 0; k < 5000; k++) {
      // sample from distribution around believed obstacle center
      candidate(0) = noise.noisyValue(obstacle->location.x);
      candidate(1) = noise.noisyValue(obstacle->location.y);

      // calculate likelihood of candidate given readings
      double sse = 0;
      for (int j = 0; j < readings.size(); j++) {
        reading = readings.at(j);
        sse += pow(error(reading, candidate, obstacle->radius), 2);
      }
      double weight = exp(-sse / tuningVariance);

      // add weighted candidate to estimate
      totalWeight += weight;
      center += weight * candidate;
    }

    // normalize, relativize, and save
    center /= totalWeight;
    center(0) -= mOrientation.x;
    center(1) -= mOrientation.y;
    centers[i] = center;
  }

  return centers;
}

void OnlineSLAM::refineModel(shared_ptr<Observation> observation) {
  // estimate
  mMu = ublas::prod(inverse(mOmega), mXi);

  // update orientation
  mOrientation.x = mMu(0);
  mOrientation.y = mMu(1);

  // re-estimate heading
  refineHeading(observation);

  // update noisy map
  for (int i = 0; i < mMap->obstacleCount(); i++) {
    unsigned int m = 2 * (1 + i);
    shared_ptr<Obstacle> obstacle = mMap->getObstacle(i);
    obstacle->location.x = mMu(m);
    obstacle->location.y = mMu(m+1);
  }
}

void OnlineSLAM::refineHeading(shared_ptr<Observation> observation) {
  if (mHeadingSigma <= 0) return;

  double at[3];
  at[0] = mOrientation.x;
  at[1] = mOrientation.y;

  double *expectedReading = new double[observation->forwardGranularity];
  std::unique_ptr<double[]> managedExpectedReading(expectedReading);
  ForwardSensor sensor(mMap);
  NormalNoise noise(mHeadingSigma);

  double tuningVariance = 10000;
  double momentZero = 0, momentOne = 0, momentTwo = 0;
  for (int k = 0; k < 5000; k++) {
    // sample from distribution around believed heading
    at[2] = noise.noisyValue(mOrientation.heading);

    // calculate likelihood of candidate given readings
    sensor.sense(at, expectedReading, observation->forwardGranularity);
    double sse = 0.0;
    for (int j = 0; j < observation->forwardGranularity; j++)
      sse += pow(fabs(observation->forwardReading[j] - expectedReading[j]), 2);
    double weight = exp(-sse / tuningVariance);

    // add weighted candidate to estimate
    momentZero += weight;
    momentOne += weight * at[2];
    momentTwo += weight * at[2] * at[2];
  }

  mOrientation.heading = momentOne / momentZero;
  double discriminant = momentTwo / momentZero - pow(mOrientation.heading, 2);
  if (discriminant < 0) discriminant = 0;
  mHeadingSigma = sqrt(discriminant);
}

ublas::matrix<double> OnlineSLAM::inverse(ublas::matrix<double> m) {
  // This code is based on http://adorio-research.org/wordpress/?p=4560
  unsigned int size = m.size1();

  // first, computes the upper triangular Cholesky factorization (assumes a
  // positive definite matrix).
  ublas::matrix<double> upper = ublas::zero_matrix<double>(size, size);
  for (int i = 0; i < size; i++) {
    double s = 0;
    for (int k = 0; k < i; k++)
      s += pow(upper(k, i), 2);
    double d = m(i, i) - s;
    if (fabs(d) < 0) d = 0;
    upper(i, i) = sqrt(d);
    for (int j = i + 1; j < size; j++) {
      double s = 0;
      for (int k = 0; k < i; k++)
        s += upper(k, i) * upper(k, j);
      if (fabs(s) < 1.0e-5) s = 0;
      upper(i, j) = (m(i, j) - s) / upper(i, i);
    }
  }

  // now, use that to compute inverse given that decomposition
  ublas::matrix<double> inverse = ublas::zero_matrix<double>(size, size);
  for (int j = size - 1; j >= 0; j--) {
    double q = 1 / upper(j, j);
    double s = 0;
    for (int k = j + 1; k < size; k++)
      s += upper(j, k) * inverse(j, k);
    inverse(j, j) = q * (q - s);
    for (int i = j - 1; i >= 0; i--) {
      double s = 0;
      for (int k = i + 1; k < size; k++)
        s += upper(i, k) * inverse(k, j);
      inverse(j, i) = inverse(i, j) = -s / upper(i, i);
    }
  }

  return inverse;
}

double OnlineSLAM::error(ublas::vector<double> reading, shared_ptr<Obstacle> obstacle) {
  ublas::vector<double> center(2);
  center(0) = obstacle->location.x;
  center(1) = obstacle->location.y;
  return error(reading, center, obstacle->radius);
}

double OnlineSLAM::error(ublas::vector<double> reading, ublas::vector<double> center, double radius) {
  return ublas::norm_2(center - reading) - radius;
}
