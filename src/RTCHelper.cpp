#include "rtm/Manager.h"
#include <iostream>
#include "VREPRTC.h"
#include "RangeRTC.h"
#include "RobotRTC.h"
#include "RTCHelper.h"
//#include "v_repExtRTC.h"
#include "v_repLib.h"

using namespace RTC;

ManagerRunner* pRunner;
void MyModuleInit(RTC::Manager* manager);

int ManagerRunner::svc() {
  int argc = 1;
  char* argv[1] = {"vrep"};
  
  RTC::Manager* manager;
  manager = RTC::Manager::init(argc, argv);
  manager->init(argc, argv);
  manager->setModuleInitProc(MyModuleInit);
  manager->activateManager();
  manager->runManager(false);
}

void MyModuleInit(RTC::Manager* manager)
{
  RobotRTCInit(manager);
  RangeRTCInit(manager);
  VREPRTCInit(manager);
  RTC::RtcBase* comp;

  // Create a component automatically
  comp = manager->createComponent("VREPRTC");
  if (comp==NULL)
  {
    std::cerr << "Component create failed." << std::endl;
    abort();
  }

  return;
}

bool initRTM() {
  pRunner = new ManagerRunner();
  return true;
}

bool exitRTM() {
  std::cout << "Shutting Down RTC::Manager......" << std::endl;
  RTC::Manager& manager = RTC::Manager::instance();
  manager.shutdown();
  //manager.terminate();
  std::cout << "Waiting RTC::Manager......" << std::endl;
  //manager.join();
  delete pRunner;
  
  return false;
}

#include <vector>
#include <string>
#include <sstream>

static int indent = 0;
static simInt getChildren(simInt h, std::vector<simInt>& jointHandles, std::vector<std::string>& jointNames, simInt typeFilter) {
  indent += 1;
  int i = 0;
  simInt r = 0;
  while (1) {
    r = simGetObjectChild(h, i++);
    if (r < 0) {
      indent -= 1;
      return 0;
    }
    std::string name = simGetObjectName(r);
    simInt type = simGetObjectType(r);
    if (type == typeFilter) {
      jointHandles.push_back(r);
      jointNames.push_back(name);
    }
    getChildren(r, jointHandles, jointNames, typeFilter);
  }
  indent -= 1;
  return 0;
}

int spawnRangeRTC(std::string& key) {
  std::cout << " spawning RTC (objectName = " << key << ")" << std::endl;
  simInt objHandle = simGetObjectHandle(key.c_str());
  if (objHandle == -1) {
    std::cout << " failed to get object handle." << std::endl;
    returnQueue.returnReturn(Return(Return::ERROR));
    return -1;
  }

  std::vector<simInt> sensorHandles;
  std::vector<std::string> sensorNames;
  getChildren(objHandle, sensorHandles, sensorNames, sim_object_proximitysensor_type);

  if (sensorHandles.size() !=1 || sensorNames.size() != 1) {
    std::cout << " failed to get object handle." << std::endl;
    returnQueue.returnReturn(Return(Return::ERROR));
    return -1;
  }

  
  simInt bufSize = 4096;
  simInt tubeHandle = simTubeOpen(0, (key+"_HOKUYO").c_str(), bufSize, false);
  if (tubeHandle < 0) {
    std::cout << " can not open Tube to " << key << std::endl;
    returnQueue.returnReturn(Return(Return::ERROR));
  }

  std::ostringstream arg_oss;
  arg_oss << "RangeRTC?" 
	  << "exec_cxt.periodic.type=" << "SynchExtTriggerEC" << "&"
	  << "conf.default.objectName=" << key << "&"
    ///<< "conf.default.activeJointNames=" << names << "&"
	  << "conf.__innerparam.objectHandle=" << objHandle << "&"
	  << "conf.__innerparam.tubeHandle=" << tubeHandle<< "&"
	  << "conf.__innerparam.bufSize=" << bufSize;
  RTObject_impl* cmp = RTC::Manager::instance().createComponent(arg_oss.str().c_str());
  robotContainer.push(cmp->getObjRef());
  return 0;
}


int spawnRobotRTC(std::string& key) {
  std::cout << " spawning RTC (objectName = " << key << ")" << std::endl;
  simInt objHandle = simGetObjectHandle(key.c_str());
  if (objHandle == -1) {
    std::cout << " failed to get object handle." << std::endl;
    returnQueue.returnReturn(Return(Return::ERROR));
    return -1;
  }

  std::vector<simInt> jointHandles;
  std::vector<std::string> jointNames;
  getChildren(objHandle, jointHandles, jointNames, sim_object_joint_type);

  std::ostringstream name_oss;
  for (int i = 0;i < jointNames.size();i++) {
    name_oss << jointNames[i];
    if (i != jointNames.size()-1) { // If not the last object,
      name_oss << ",";
    }
  }
  std::string names = name_oss.str();

  std::ostringstream handle_oss;
  for (int i = 0;i < jointHandles.size();i++) {
    handle_oss << jointHandles[i];
    if (i != jointHandles.size()-1) { // If not the last object,
      handle_oss << ",";
    }
  }
  std::string handles = handle_oss.str();
  std::cout << "jointNames = " << names << std::endl;;
  std::cout << "jointHandles = " << handles << std::endl;;

  std::ostringstream arg_oss;
  arg_oss << "RobotRTC?" 
	  << "exec_cxt.periodic.type=" << "SynchExtTriggerEC" << "&"
	  << "conf.default.objectName=" << key << "&"
    	  << "conf.default.controlledJointNames=" << names << "&"
	  << "conf.default.observedJointNames=" << names << "&"
	  << "conf.__innerparam.objectHandle=" << objHandle << "&"
	  << "conf.__innerparam.allNames=" << names << "&"
	  << "conf.__innerparam.allHandles=" << handles;
  RTObject_impl* cmp = RTC::Manager::instance().createComponent(arg_oss.str().c_str());
  robotContainer.push(cmp->getObjRef());
  return 0;
}

void startRTCs() {
  robotContainer.start();
}

void stopRTCs() {
  robotContainer.stop();
}

void tickRTCs(const float interval) {
  robotContainer.tick(interval);
}
