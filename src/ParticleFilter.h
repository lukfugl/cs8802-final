#ifndef __PARTICLE_FILTER_H__
#define __PARTICLE_FILTER_H__

#include "NoisyMap.h"
#include "Observation.h"
#include "Orientation.h"

#include <memory>
#include <random>

using namespace std;

class ParticleFilter {
  public:
  ParticleFilter(shared_ptr<NoisyMap> map, unsigned int particleCount);
  ~ParticleFilter();

  void advance(double turn, double speed);
  void filter(shared_ptr<Observation> observation);
  bool converged();
  Orientation mean();
  Orientation variance();

  private:
  double uniform(double min, double max);
  double headingDistance(double heading, double targetHeading);

  shared_ptr<NoisyMap> mMap;
  vector<Orientation> mParticles;
  default_random_engine mRNG;
  uniform_real_distribution<double> mUniform;
};

#endif
