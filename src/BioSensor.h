#include "Environment.h"
#include "NoiseModel.h"

using namespace std;

class BioSensor {
  public:
  BioSensor(shared_ptr<Environment> environment);
  ~BioSensor();

  unsigned int sense(double at[3], double *readings, unsigned int maxReadings);
  void setDistanceNoiseModel(shared_ptr<NoiseModel> noiseModel);
  void setHeadingNoiseModel(shared_ptr<NoiseModel> noiseModel);

  private:
  shared_ptr<Environment> mEnvironment;
  shared_ptr<NoiseModel> mDistanceNoiseModel;
  shared_ptr<NoiseModel> mHeadingNoiseModel;
};
