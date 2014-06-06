#include "rtm/Manager.h"
#include <iostream>
#include "VREPRTC.h"
#include "RobotRTC.h"
#include "RTCHelper.h"
#include "coil/Task.h"
#include "v_repExtRTC.h"
#include "v_repLib.h"

using namespace RTC;

void MyModuleInit(RTC::Manager* manager)
{
  RobotRTCInit(manager);
  VREPRTCInit(manager);
  RTC::RtcBase* comp;

  // Create a component
  comp = manager->createComponent("VREPRTC");
  if (comp==NULL)
  {
    std::cerr << "Component create failed." << std::endl;
    abort();
  }
  // Example
  // The following procedure is examples how handle RT-Components.
  // These should not be in this function.

  // Get the component's object reference
//  RTC::RTObject_var rtobj;
//  rtobj = RTC::RTObject::_narrow(manager->getPOA()->servant_to_reference(comp));

  // Get the port list of the component
//  PortServiceList* portlist;
//  portlist = rtobj->get_ports();

  // getting port profiles
//  std::cout << "Number of Ports: ";
//  std::cout << portlist->length() << std::endl << std::endl; 
//  for (CORBA::ULong i(0), n(portlist->length()); i < n; ++i)
//  {
//    PortService_ptr port;
//    port = (*portlist)[i];
//    std::cout << "Port" << i << " (name): ";
//    std::cout << port->get_port_profile()->name << std::endl;
//    
//    RTC::PortInterfaceProfileList iflist;
//    iflist = port->get_port_profile()->interfaces;
//    std::cout << "---interfaces---" << std::endl;
//    for (CORBA::ULong i(0), n(iflist.length()); i < n; ++i)
//    {
//      std::cout << "I/F name: ";
//      std::cout << iflist[i].instance_name << std::endl;
//      std::cout << "I/F type: ";
//      std::cout << iflist[i].type_name << std::endl;
//      const char* pol;
//      pol = iflist[i].polarity == 0 ? "PROVIDED" : "REQUIRED";
//      std::cout << "Polarity: " << pol << std::endl;
//    }
//    std::cout << "---properties---" << std::endl;
//    NVUtil::dump(port->get_port_profile()->properties);
//    std::cout << "----------------" << std::endl << std::endl;
//  }

  return;
}


class ManagerRunner : public coil::Task {
public:
  ManagerRunner() {
    open(0);
  }

  virtual int open(void* arg) {
    activate();
    return 0;
  }

  virtual int svc() {
  int argc = 1;
  //const char arg[5] = {'v', 'r', 'e', 'p', 0};
  char* argv[1] = {"vrep"};

  RTC::Manager* manager;
  manager = RTC::Manager::init(argc, argv);

  // Initialize manager
  manager->init(argc, argv);

  // Set module initialization proceduer
  // This procedure will be invoked in activateManager() function.
  manager->setModuleInitProc(MyModuleInit);

  // Activate manager and register to naming service
  manager->activateManager();

  // run the manager in blocking mode
  // runManager(false) is the default.
  manager->runManager(false);

  // If you want to run the manager in non-blocking mode, do like this
  // manager->runManager(true);
  std::cout << "exitting srv" << std::endl;
  }    
};


ManagerRunner* pRunner;

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
static simInt getChildren(simInt h, std::vector<simInt>& jointHandles, std::vector<std::string>& jointNames) {
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
    if (type == sim_object_joint_type) {
      //for (int j = 0;j < indent;j++) {
      //std::cout << " ";
      //}
      //std::cout << name << ": " << r << std::endl;
      jointHandles.push_back(r);
      jointNames.push_back(name);
    }
    getChildren(r, jointHandles, jointNames);
  }
  indent -= 1;
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
  getChildren(objHandle, jointHandles, jointNames);

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
	  << "conf.default.activeJointNames=" << names << "&"
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
