#ifndef __PARTICLE_FILTER_H__
#define __PARTICLE_FILTER_H__

#include "NoisyMap.h"
#include "Observation.h"

#include <memory>

using namespace std;

class ParticleFilter {
  public:
  ParticleFilter(shared_ptr<NoisyMap> map);
  ~ParticleFilter();

  void advance(double turn, double speed);
  void filter(shared_ptr<Observation> observation);
  bool converged();
  Orientation mean();

  private:
  shared_ptr<NoisyMap> mMap;
};

#endif
