#ifndef __ONLINE_SLAM_H
#define __ONLINE_SLAM_H

#include "Orientation.h"
#include "NoisyMap.h"
#include "Observation.h"
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <map>
#include <memory>

namespace ublas = boost::numeric::ublas;

class OnlineSLAM {
  public:
  OnlineSLAM(Orientation orientation, double headingVariance, shared_ptr<NoisyMap> map);
  ~OnlineSLAM();

  void advance(double turn, double speed);
  void update(shared_ptr<Observation> observation);
  Orientation estimate();

  private:
  void expand();
  void contract();
  map<int, ublas::vector<double> > processObservation(shared_ptr<Observation> observation);
  void refineModel(shared_ptr<Observation> observation);
  void refineHeading(shared_ptr<Observation> observation);
  ublas::matrix<double> inverse(ublas::matrix<double> m);
  double error(ublas::vector<double> reading, shared_ptr<Obstacle> obstacle);
  double error(ublas::vector<double> reading, ublas::vector<double> center, double radius);

  Orientation mOrientation;
  double mHeadingSigma;
  shared_ptr<NoisyMap> mMap;
  unsigned int mDimension;
  ublas::matrix<double> mOmega;
  ublas::vector<double> mXi;
  ublas::vector<double> mMu;
  ublas::vector<double> mMotion;
  double mPositionWeight;
  double mMapWeight;
  double mMotionWeight;
  double mMeasurementWeight;
};

#endif
