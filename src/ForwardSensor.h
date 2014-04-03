#include "Environment.h"
#include "NoiseModel.h"

using namespace std;

class ForwardSensor {
  public:
  static const unsigned int GRANULARITY;
  static const double NONE;

  ForwardSensor(shared_ptr<Environment> environment);
  ~ForwardSensor();

  void sense(double at[3], double *reading);
  void setNoiseModel(shared_ptr<NoiseModel> noiseModel);

  private:
  shared_ptr<Environment> mEnvironment;
  shared_ptr<NoiseModel> mNoiseModel;
};
