#include "Environment.h"
#include "NormalNoise.h"

#include <stdlib.h>
#include <fstream>

#include "../vendor/jsoncpp/include/json/json.h"

using namespace std;

Environment::Environment() {}
Environment::~Environment() {}

void Environment::loadMap(string filename) {
  Json::Value root;

  Json::Reader reader;
  ifstream file(filename);
  if (!reader.parse(file, root)) {
    printf("Failed to parse configuration\n%s", reader.getFormatedErrorMessages().c_str());
    exit(1);
  }

  const Json::Value dropZones = root["dropZones"];
  for (int i = 0; i < dropZones.size(); i++) {
    double minX = dropZones[i].get("minX", 0).asDouble();
    double minY = dropZones[i].get("minY", 0).asDouble();
    double maxX = dropZones[i].get("maxX", 0).asDouble();
    double maxY = dropZones[i].get("maxY", 0).asDouble();
    spawnDropZone(minX, minY, maxX, maxY);
  }

  if (root.isMember("targetZone")) {
    const Json::Value targetZone = root["targetZone"];
    double minX = targetZone.get("minX", 0).asDouble();
    double minY = targetZone.get("minY", 0).asDouble();
    double maxX = targetZone.get("maxX", 0).asDouble();
    double maxY = targetZone.get("maxY", 0).asDouble();
    spawnTargetZone(minX, minY, maxX, maxY);
  }

  double obstacleRadius = root.get("obstacleRadius", 10).asDouble();
  const Json::Value obstacles = root["obstacles"];
  for (int i = 0; i < obstacles.size(); i++) {
    double x = obstacles[i].get("x", 0).asDouble();
    double y = obstacles[i].get("y", 0).asDouble();
    spawnObject(x, y, obstacleRadius);
  }

  double guardSightRange;
  if (root.isMember("guardBehavior")) {
    const Json::Value guardBehavior = root["guardBehavior"];
    mGuardSpeedMean = guardBehavior.get("speedMean", 5).asDouble();
    mGuardSpeedSigma = guardBehavior.get("speedSigma", 0).asDouble();
    mGuardTurningMean = guardBehavior.get("turningMean", 0).asDouble();
    mGuardTurningSigma = guardBehavior.get("turningSigma", 0).asDouble();
    guardSightRange = guardBehavior.get("sightRange", 8).asDouble();
  }
  else {
    mGuardSpeedMean = 5;
    mGuardSpeedSigma = 0;
    mGuardTurningMean = 0;
    mGuardTurningSigma = 0;
    guardSightRange = 8;
  }

  shared_ptr<NoiseModel> speedNoise(new NormalNoise(mGuardSpeedSigma));
  shared_ptr<NoiseModel> turningNoise(new NormalNoise(mGuardTurningSigma));

  const Json::Value guards = root["guards"];
  for (int i = 0; i < guards.size(); i++) {
    double x = guards[i].get("x", 0).asDouble();
    double y = guards[i].get("y", 0).asDouble();
    double h = guards[i].get("heading", 0).asDouble();
    bool ccw = guards[i].get("ccw", true).asBool();
    shared_ptr<Guard> guard = spawnGuard(x, y, h, ccw);
    guard->setSpeedMean(mGuardSpeedMean);
    guard->setSpeedNoiseModel(speedNoise);
    guard->setTurningMean(mGuardTurningMean);
    guard->setTurningNoiseModel(turningNoise);
    guard->setSightRange(guardSightRange);
  }

  if (root.isMember("informationNoise")) {
    const Json::Value informationNoise = root["informationNoise"];
    mMapSigma = informationNoise.get("mapSigma", 0).asDouble();
    mForwardSigma = informationNoise.get("forwardSigma", 0).asDouble();
    mEMRangeSigma = informationNoise.get("emRangeSigma", 0).asDouble();
    mEMHeadingSigma = informationNoise.get("emHeadingSigma", 0).asDouble();
  }
}

unsigned int Environment::spawnDropZone(double minX, double minY, double maxX, double maxY) {
  DropZone dropZone;
  dropZone.minX = minX;
  dropZone.minY = minY;
  dropZone.maxX = maxX;
  dropZone.maxY = maxY;
  mDropZones.push_back(dropZone);
  return 0;
}

unsigned int Environment::spawnTargetZone(double minX, double minY, double maxX, double maxY) {
  mTargetZone.minX = minX;
  mTargetZone.minY = minY;
  mTargetZone.maxX = maxX;
  mTargetZone.maxY = maxY;
  return 0;
}

unsigned int Environment::spawnObject(double x, double y, double radius) {
  Coordinate location(x, y);
  shared_ptr<Obstacle> obstacle(new Obstacle(location, radius));
  mObstacles.push_back(obstacle);
  return 0;
}

shared_ptr<Guard> Environment::spawnGuard(double x, double y, double h, bool ccw) {
  shared_ptr<Guard> guard(new Guard(x, y, h, ccw));
  mGuards.push_back(guard);
  return guard;
}

unsigned int Environment::dropZoneCount() {
  return mDropZones.size();
}

unsigned int Environment::obstacleCount() {
  return mObstacles.size();
}

unsigned int Environment::guardCount() {
  return mGuards.size();
}

DropZone Environment::getDropZone(int index) {
  return mDropZones.at(index);
}

DropZone Environment::getTargetZone() {
  return mTargetZone;
}

shared_ptr<Obstacle> Environment::getObstacle(int index) {
  return mObstacles.at(index);
}

shared_ptr<Guard> Environment::getGuard(int index) {
  return mGuards.at(index);
}

double Environment::getGuardSpeedMean() {
  return mGuardSpeedMean;
}

double Environment::getGuardSpeedSigma() {
  return mGuardSpeedSigma;
}

double Environment::getGuardTurningMean() {
  return mGuardTurningMean;
}

double Environment::getGuardTurningSigma() {
  return mGuardTurningSigma;
}

double Environment::getMapSigma() {
  return mMapSigma;
}

double Environment::getForwardSigma() {
  return mForwardSigma;
}

double Environment::getEMRangeSigma() {
  return mEMRangeSigma;
}

double Environment::getEMHeadingSigma() {
  return mEMHeadingSigma;
}
