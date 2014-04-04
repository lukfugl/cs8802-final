#include "Environment.h"
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

  const Json::Value obstacles = root["obstacles"];
  for (int i = 0; i < obstacles.size(); i++) {
    double x = obstacles[i].get("x", 0).asDouble();
    double y = obstacles[i].get("y", 0).asDouble();
    spawnObject(x, y);
  }

  const Json::Value guards = root["guards"];
  for (int i = 0; i < guards.size(); i++) {
    double x = guards[i].get("x", 0).asDouble();
    double y = guards[i].get("y", 0).asDouble();
    double h = guards[i].get("heading", 0).asDouble();
    bool ccw = guards[i].get("ccw", true).asBool();
    spawnGuard(x, y, h, ccw);
  }

  if (root.isMember("guardBehavior")) {
    const Json::Value guardBehavior = root["guardBehavior"];
    mGuardSpeedMean = guardBehavior.get("speedMean", 5).asDouble();
    mGuardSpeedSigma = guardBehavior.get("speedSigma", 0).asDouble();
    mGuardTurningMean = guardBehavior.get("turningMean", 0).asDouble();
    mGuardTurningSigma = guardBehavior.get("turningSigma", 0).asDouble();
  }

  if (root.isMember("informationNoise")) {
    const Json::Value informationNoise = root["informationNoise"];
    mMapSigma = informationNoise.get("mapSigma", 0).asDouble();
    mForwardSigma = informationNoise.get("forwardSigma", 0).asDouble();
    mBioRangeSigma = informationNoise.get("bioRangeSigma", 0).asDouble();
    mBioHeadingSigma = informationNoise.get("bioHeadingSigma", 0).asDouble();
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

unsigned int Environment::spawnObject(double x, double y) {
  mObstacles.push_back(Coordinate(x, y));
  return 0;
}

unsigned int Environment::spawnGuard(double x, double y, double h, bool ccw) {
  mGuards.push_back(Guard(Coordinate(x, y), h, ccw));
  return 0;
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

Coordinate Environment::getObstacle(int index) {
  return mObstacles.at(index);
}

Guard Environment::getGuard(int index) {
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

double Environment::getBioRangeSigma() {
  return mBioRangeSigma;
}

double Environment::getBioHeadingSigma() {
  return mBioHeadingSigma;
}
