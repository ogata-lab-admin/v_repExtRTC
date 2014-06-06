#pragma once 


#include "Tasks.h"
#include "RobotRTCContainer.h"

bool initRTM();
bool exitRTM();
int spawnRobotRTC(std::string& key);
void startRTCs();
void stopRTCs();
void tickRTCs(const float interval);

