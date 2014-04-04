#include "../src/NoiseModel.h"

class DeterministicNoise : public NoiseModel {
  public:
  DeterministicNoise() : mAmount(0) {}
  double noisyValue(double value) {
    mAmount += 0.1;
    return value + mAmount;
  }

  private:
  double mAmount;
};
