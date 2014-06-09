#pragma once 
#include <coil/Task.h>

#include "Tasks.h"
#include "RobotRTCContainer.h"

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
int spawnRobotRTC(std::string& key);
int spawnRangeRTC(std::string& key);
int spawnCameraRTC(std::string& key);
void startRTCs();
void stopRTCs();
void tickRTCs(const float interval);

