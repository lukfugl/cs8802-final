#include "ParticleFilter.h"

#include <math.h>

ParticleFilter::ParticleFilter(shared_ptr<NoisyMap> map, unsigned int particleCount) :
  mMap(map),
  mUniform(0, 1) {
  // initialize particles
  unsigned int dropZoneCount = mMap->dropZoneCount();
  uniform_int_distribution<unsigned int> dropZone(0, dropZoneCount - 1);
  for (int i = 0; i < particleCount; i++) {
    DropZone zone = mMap->getDropZone(dropZone(mRNG));
    double x = uniform(zone.minX, zone.maxX);
    double y = uniform(zone.minY, zone.maxY);
    double heading = uniform(0, 2 * M_PI);
    mParticles.push_back(Orientation(x, y, heading));
  }
}

ParticleFilter::~ParticleFilter() {}

void ParticleFilter::advance(double turn, double speed) {
  for (vector<Orientation>::iterator i = mParticles.begin(), e = mParticles.end(); i != e; ++i)
    i->advance(turn, speed);
}

void ParticleFilter::filter(shared_ptr<Observation> observation) {
  double at[3];
  double *expectedReading = new double[observation->forwardGranularity];
  std::unique_ptr<double[]> managedExpectedReading(expectedReading);
  ForwardSensor sensor(mMap);

  double tuningVariance = 100000;

  // weight particles by likelihood
  vector<double> weights;
  double maxWeight = -1000.0;
  for (vector<Orientation>::iterator i = mParticles.begin(), e = mParticles.end(); i != e; ++i) {
    at[0] = i->x;
    at[1] = i->y;
    at[2] = i->heading;
    sensor.sense(at, expectedReading, observation->forwardGranularity);

    // likelihood of seeing observation given particle's assumed position and
    // heading
    double sse = 0.0;
    for (int j = 0; j < observation->forwardGranularity; j++)
      sse += pow(fabs(observation->forwardReading[j] - expectedReading[j]), 2);
    double weight = exp(-sse / tuningVariance);

    weights.push_back(weight);
    maxWeight = max(weight, maxWeight);
  }

  // resample particles according to weights
  vector<Orientation> resampled;
  unsigned int idx = uniform_int_distribution<unsigned int>(0, mParticles.size() - 1)(mRNG);
  double beta = 0.0;
  for (int i = 0; i < mParticles.size(); i++) {
    beta += uniform(0, 2 * maxWeight);
    while (beta > weights[idx]) {
      beta -= weights[idx];
      idx = (idx + 1) % mParticles.size();
    }
    resampled.push_back(mParticles.at(idx));
  }

  mParticles = resampled;

  Orientation m = mean();
}

bool ParticleFilter::converged() {
  // "good enough" convergence: sum of squared errors (from the mean)
  // in position is less than 10 (arbitrary, will be tweaked) and sum of
  // squared errors in heading is less than π/4 (ditto), simultaneously
  Orientation mu = mean();
  Orientation variance(0, 0, 0);
  for (vector<Orientation>::iterator i = mParticles.begin(), e = mParticles.end(); i != e; ++i) {
    variance.x += pow(i->x - mu.x, 2);
    variance.y += pow(i->y - mu.y, 2);
    variance.heading += pow(headingDistance(i->heading, mu.heading), 2);
  }
  variance.x /= mParticles.size();
  variance.y /= mParticles.size();
  variance.heading /= mParticles.size();
  return (variance.x + variance.y) < 20 && variance.heading < M_PI / 4;
}

double ParticleFilter::headingDistance(double heading, double targetHeading) {
  double delta = 2 * M_PI + heading - targetHeading;
  delta -= 2 * M_PI * floor(delta / (2 * M_PI));
  if (delta > M_PI) delta = delta - 2 * M_PI;
  return delta;
}

Orientation ParticleFilter::mean() {
  double anchorHeading = mParticles.at(0).heading;
  Orientation mean(0, 0, 0);
  for (vector<Orientation>::iterator i = mParticles.begin(), e = mParticles.end(); i != e; ++i) {
    mean.x += i->x;
    mean.y += i->y;
    mean.heading += headingDistance(i->heading, anchorHeading);
  }
  mean.x /= mParticles.size();
  mean.y /= mParticles.size();
  mean.heading /= mParticles.size();
  mean.heading += anchorHeading;
  return mean;
}

double ParticleFilter::uniform(double min, double max) {
  return min + mUniform(mRNG) * (max - min);
}
