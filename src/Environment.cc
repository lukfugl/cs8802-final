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
    spawnGuard(x, y);
  }
}

unsigned int Environment::spawnObject(double x, double y) {
  mCenters.push_back(Coordinate(x, y));
  return 0;
}

unsigned int Environment::spawnGuard(double x, double y) {
  mGuards.push_back(Coordinate(x, y));
  return 0;
}

unsigned int Environment::centerCount() {
  return mCenters.size();
}

unsigned int Environment::guardCount() {
  return mGuards.size();
}

Environment::Coordinate Environment::getCenter(int index) {
  return mCenters.at(index);
}

Environment::Coordinate Environment::getGuard(int index) {
  return mGuards.at(index);
}
