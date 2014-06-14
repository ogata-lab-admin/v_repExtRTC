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
int spawnRobotRTC(std::string& key, std::string& arg);
int spawnRangeRTC(std::string& key, std::string& arg);
int spawnCameraRTC(std::string& key, std::string& arg);
void startRTCs();
void stopRTCs();
void tickRTCs(const float interval);
int killRTC(const std::string& key);
int killAllRTC();
int syncRTC(const std::string& fullpath);
int getSyncRTCs(std::vector<std::string>& strList);
