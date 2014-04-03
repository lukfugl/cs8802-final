#include "../src/NoiseModel.h"

class ConstantNoise : public NoiseModel {
  public:
  ConstantNoise(double amount) : mAmount(amount) {}
  double noisyValue(double value) { return value + mAmount; }

  private:
  double mAmount;
};
