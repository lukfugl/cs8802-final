#include "GuardModel.h"

#include <functional>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

using namespace std::placeholders;
namespace ublas = boost::numeric::ublas;

GuardModel::GuardModel() {}

GuardModel::~GuardModel() {}

KalmanFilter::KalmanFilter(Orientation observation) :
  mMeasurementCount(1),
  X(10),
  I(10),
  P(1000 * I),
  H(ublas::zero_matrix<double>(6, 10)),
  R(ublas::zero_matrix<double>(6, 6)),
  F(ublas::zero_matrix<double>(10, 10)) {

  ublas::identity_matrix<double> I2(2);

  // x y 0 0 0 0 0 0 0 0
  X(0) = observation.x;
  X(1) = observation.y;

  // 1 0 0 0 0 0 0 0 0 0
  // 0 1 0 0 0 0 0 0 0 0
  // 0 0 0 0 0 0 1 0 0 0
  // 0 0 0 0 0 0 0 1 0 0
  // 0 0 0 0 0 0 0 0 1 0
  // 0 0 0 0 0 0 0 0 0 1
  ublas::subrange(H, 0, 2, 0, 2) = I2;
  ublas::subrange(H, 2, 4, 6, 8) = I2;
  ublas::subrange(H, 4, 6, 8, 10) = I2;

  // 1 0 0 0 0 0
  // 0 1 0 0 0 0
  // 0 0 5 0 0 0
  // 0 0 0 5 0 0
  // 0 0 0 0 1 0
  // 0 0 0 0 0 1
  ublas::subrange(R, 0, 2, 0, 2) = I2;
  ublas::subrange(R, 2, 4, 2, 4) = 5 * I2;
  ublas::subrange(R, 4, 6, 4, 6) = I2;

  // the (*) elements will change during iteration
  // 1  0  *  0 -1  0  0  0  0  0
  // 0  1  0  *  0 -1  0  0  0  0
  // 0  0  *  0 -1  0  0  0  0  0
  // 0  0  0  *  0 -1  0  0  0  0
  // 0  0  1  0  0  0  0  0  0  0
  // 0  0  0  1  0  0  0  0  0  0
  // 0  0  0  0  0  0  1  0  0  0
  // 0  0  0  0  0  0  0  1  0  0
  // 0  0  0  0  0  0  0  0  1  0
  // 0  0  0  0  0  0  0  0  0  1
  ublas::subrange(F, 0, 2, 0, 2) = I2;
  ublas::subrange(F, 4, 6, 2, 4) = I2;
  ublas::subrange(F, 0, 2, 4, 6) = -I2;
  ublas::subrange(F, 2, 4, 4, 6) = -I2;
  ublas::subrange(F, 6, 8, 6, 8) = I2;
  ublas::subrange(F, 8, 10, 8, 10) = I2;
}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::update(Orientation observation) {
  ublas::vector<double> z(2);
  z(0) = observation.x;
  z(1) = observation.y;

  if (mMeasurementCount < 3) {
    // second measurement. calculate d1 and then put the measurement in x
    ublas::subrange(X, 4, 6) = ublas::subrange(X, 2, 4);
    ublas::subrange(X, 2, 4) = z - ublas::subrange(X, 0, 2);
    ublas::subrange(X, 0, 2) = z;
    mMeasurementCount++;
    return;
  }

  // "measuring" c,r,w means calculating what I think they'd be if my
  // prediction is correct
  ublas::vector<double> measurement(6);
  ublas::subrange(measurement, 0, 2) = z;

  ublas::vector<double> p = ublas::subrange(X, 0, 2);
  ublas::vector<double> d1 = ublas::subrange(X, 2, 4);
  ublas::vector<double> d2 = ublas::subrange(X, 4, 6);

  measurement(5) = acos(ublas::inner_prod(d1, d2) / (ublas::norm_2(d1) * ublas::norm_2(d2))); // omega
  ublas::vector<double> chord = d1 + d2;
  double chordLength = ublas::norm_2(chord);
  ublas::vector<double> chordUnit = chord / chordLength;
  ublas::vector<double> crossChord(2);
  crossChord(0) = chordUnit(1);
  crossChord(1) = -chordUnit(0);
  if (ublas::inner_prod(crossChord, d2) > 0) crossChord *= -1;
  measurement(4) = chordLength / (2 * sin(measurement(5))); // radius
  ublas::subrange(measurement, 2, 4) = p - chord / 2 + measurement(4) * cos(measurement(5)) * crossChord; // center

  // measurement update
  ublas::vector<double> y = measurement - prod(H, X);
  ublas::matrix<double> S0 = ublas::prod(P, ublas::trans(H));
  ublas::matrix<double> S = ublas::prod(H, S0) + R;
  ublas::matrix<double> K0 = ublas::prod(ublas::trans(H), inverse(S));
  ublas::matrix<double> K = ublas::prod(P, K0);
  X += ublas::prod(K, y);
  printf("new guard estimate: %.3f, %.3f\n", X(0), X(1));
  P = ublas::prod((I - ublas::prod(K, H)), P);

  // prediction
  d1 = ublas::subrange(X, 2, 4); // necessary?
  d2 = ublas::subrange(X, 4, 6);
  double norm = 2 / ublas::inner_prod(d1, d1);
  double c = norm * ublas::inner_prod(d1, d2);

  ublas::subrange(F, 0, 2, 2, 4) = ublas::subrange(F, 2, 4, 2, 4) = c * ublas::identity_matrix<double>(2);

  ublas::vector<double> left = ublas::zero_vector<double>(10);
  ublas::subrange(left, 0, 2) = ublas::subrange(left, 2, 4) = norm * d1;

  ublas::vector<double> right = ublas::zero_vector<double>(10);
  ublas::subrange(right, 2, 4) = d2 - c * d1;
  ublas::subrange(right, 4, 6) = d1;

  ublas::matrix<double> J = F + outer_prod(left, right);

  X = ublas::prod(F, X);
  P = ublas::prod(P, ublas::trans(J));
  P = ublas::prod(J, P);
}

Orientation KalmanFilter::prediction() {
  return Orientation(X(0), X(1), 0);
}

void KalmanFilter::correctAnchor(Orientation previous, Orientation current) {
  ublas::matrix<double> R(2, 2);
  R(0, 0) = R(1, 1) = cos(current.heading - previous.heading);
  R(1, 0) = sin(current.heading - previous.heading);
  R(0, 1) = -R(1, 0);

  ublas::vector<double> prev(2), curr(2);
  prev(0) = previous.x;
  prev(1) = previous.y;
  curr(0) = current.x;
  curr(1) = current.y;
  ublas::vector<double> offset = curr - ublas::prod(R, prev);

  ublas::subrange(X, 0, 2) = ublas::prod(R, ublas::subrange(X, 0, 2)) + offset;
  ublas::subrange(X, 2, 4) = ublas::prod(R, ublas::subrange(X, 2, 4));
  ublas::subrange(X, 4, 6) = ublas::prod(R, ublas::subrange(X, 4, 6));
  if (mMeasurementCount >= 3) {
    ublas::subrange(X, 6, 8) = ublas::prod(R, ublas::subrange(X, 6, 8)) + offset;
  }
}

ublas::matrix<double> KalmanFilter::inverse(ublas::matrix<double> m) {
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

struct Tuple {
  int i, j;
  double score;
  Tuple(int i, int j, double score) :
    i(i), j(j), score(score) {};
};

inline bool operator< (const Tuple& lhs, const Tuple& rhs) {
  return lhs.score < rhs.score;
}

bool filter(Tuple t, int i, int j) {
  return t.i == i || t.j == j;
}

void GuardModel::update(shared_ptr<Observation> observation, Orientation priorAnchor, Orientation currentAnchor) {
  if (mGuards.empty()) {
    for (int i = 0; i < observation->emReadingCount; i++) {
      Orientation actual = extrapolate(currentAnchor, observation->emReadings[2*i], observation->emReadings[2*i+1]);
      mGuards.push_back(shared_ptr<KalmanFilter>(new KalmanFilter(actual)));
    }
  }
  else {
    vector<Orientation> predictions;
    vector<Orientation> observations;
    for (int i = 0; i < observation->emReadingCount; i++) {
      // update previous beliefs by change in anchor
      mGuards.at(i)->correctAnchor(priorAnchor, currentAnchor);
      predictions.push_back(mGuards.at(i)->prediction());
      observations.push_back(extrapolate(currentAnchor, observation->emReadings[2*i], observation->emReadings[2*i+1]));
    }

    vector<Tuple> scores;
    for (int i = 0; i < observation->emReadingCount; i++) {
      Orientation prediction = predictions.at(i);
      for (int j = 0; j < observation->emReadingCount; j++) {
        Orientation actual = observations.at(j);
        scores.push_back(Tuple(i, j, score(prediction, actual)));
      }
    }

    vector<Tuple>::iterator current = scores.begin(), end = scores.end();
    sort(current, end);
    while (current != end) {
      int i = current->i;
      int j = current->j;
      mGuards.at(i)->update(observations.at(j));
      end = std::remove_if(current, end, std::bind(filter, _1, i, j));
    }
  }
}

Orientation GuardModel::extrapolate(Orientation anchor, double range, double bearing) {
  double heading = anchor.heading + bearing;
  double x = anchor.x + range * cos(heading);
  double y = anchor.y + range * sin(heading);
  return Orientation(x, y, 0);
}

double GuardModel::score(Orientation prediction, Orientation actual) {
  double dx = fabs(prediction.x - actual.x);
  double dy = fabs(prediction.y - actual.y);
  return pow(dx, 2) + pow(dy, 2);
}
