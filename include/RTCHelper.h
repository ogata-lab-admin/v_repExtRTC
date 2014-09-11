#pragma once 
#include <coil/Task.h>

#include "Tasks.h"
#include "RobotRTCContainer.h"

class SimulatorClock {
 private:
  float time;
  float timeStep;
 public:

 SimulatorClock() : time(0.0), timeStep(0.05) {}
  ~SimulatorClock() {}

 public:
  float getSimulationTime() {return time;}
  void setSimulationTime(const float time) {
    this->time = time;
  }

  float getSimulationTimeStep() {return timeStep;}
  void setSimulationTimeStep(const float timeStep) {
    this->timeStep = timeStep;
  }
};

class ManagerRunner : public coil::Task {
public:
  ManagerRunner() {
    open(0);
  }

  virtual int open(void* arg) {
    activate();
    return 0;
  }

  virtual int svc();
};



bool initRTM();
bool exitRTM();
int spawnRobotRTC(std::string& key, std::string& arg);
int spawnRangeRTC(std::string& key, std::string& arg);
int spawnCameraRTC(std::string& key, std::string& arg);
int spawnAccelerometerRTC(std::string& key, std::string& arg);
int spawnGyroRTC(std::string& key, std::string& arg);
int spawnDepthRTC(std::string& key, std::string& arg);
void startRTCs();
void stopRTCs();
void tickRTCs(const float interval);
int killRTC(const std::string& key);
int killAllRTC();
int syncRTC(const std::string& fullpath);
int getSyncRTCs(std::vector<std::string>& strList);


extern SimulatorClock simulatorClock;
