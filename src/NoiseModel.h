#ifndef __NOISE_MODEL_H
#define __NOISE_MODEL_H

class NoiseModel {
  public:
  virtual ~NoiseModel() {}
  virtual double noisyValue(double value) { return value; }
};

#endif
