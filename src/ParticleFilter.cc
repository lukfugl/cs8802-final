#include "ParticleFilter.h"

ParticleFilter::ParticleFilter(shared_ptr<NoisyMap> map) :
  mMap(map) {
}

ParticleFilter::~ParticleFilter() {}

void ParticleFilter::advance(double turn, double speed) {
  // TODO
}

void ParticleFilter::filter(shared_ptr<Observation> observation) {
  // TODO
}

bool ParticleFilter::converged() {
  // TODO
  return false;
}

Orientation ParticleFilter::mean() {
  // TODO
  return Orientation(0, 0, 0);
}
