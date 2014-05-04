#ifndef __GUARD_MODEL_H
#define __GUARD_MODEL_H

#include "Observation.h"
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <memory>

namespace ublas = boost::numeric::ublas;

class KalmanFilter {
  public:
  KalmanFilter(Orientation observation);
  ~KalmanFilter();

  void update(Orientation observation);
  Orientation prediction();

  void correctAnchor(Orientation previous, Orientation current);

  protected:
  ublas::matrix<double> inverse(ublas::matrix<double> m);

  private:
  int mMeasurementCount;
  ublas::vector<double> X;
  ublas::identity_matrix<double> I;
  ublas::matrix<double> P, H, Ht, R, F;
};

class GuardModel {
  public:
  GuardModel();
  ~GuardModel();

  void update(shared_ptr<Observation> observation, Orientation priorAnchor, Orientation currentAnchor);

  private:
  Orientation extrapolate(Orientation anchor, double range, double bearing);
  double score(Orientation prediction, Orientation actual);

  vector<shared_ptr<KalmanFilter> > mGuards;
  Orientation mPreviousAnchor;
};

#endif
