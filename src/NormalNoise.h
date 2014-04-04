#include "NoiseModel.h"
#include <random>

class NormalNoise : public NoiseModel {
  public:
  NormalNoise(double sigma) :
    mDistribution(0, sigma) {}

  double noisyValue(double value) {
    return value + mDistribution(mGenerator);
  }

  private:
  default_random_engine mGenerator;
  normal_distribution<double> mDistribution;
};
