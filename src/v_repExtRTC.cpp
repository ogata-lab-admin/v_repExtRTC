// Copyright: 2014, Ogata-laboratory, Waseda University.
// Author: Yuki Suga (ysuga@ysuga.net)
// URL: http://ogata-lab.jp
// This file is distributed under the GNU Public License version 3

#include "v_repExtRTC.h"
#include "simLib.h"
#include <iostream>
#include <thread>


#include "scriptFunctionData.h"
// For RTC 
#include "RTCHelper.h"

static const char* version_str = "2.0.0";
#define PLUGIN_NAME "ExtRTC"

#ifdef _WIN32
	#include <direct.h>
	#include <shlwapi.h>
	#pragma comment(lib, "Shlwapi.lib")
#endif /* _WIN32 */

#if defined (__linux) || defined (__APPLE__)
	#include <string.h>
#include <unistd.h>
	#define _stricmp(x,y) strcasecmp(x,y)
#endif

#define PLUGIN_VERSION 12

LIBRARY vrepLib; // the V-REP library that we will dynamically load and bind


struct MenuStatus {
  enum STATUS {
    STANDBY,
    MENU_ITEM_SELECTED,
    MODEL_NAME_DIALOG_SHOWN,
    ARGUMENT_DIALOG_SHOWN,
  };

  STATUS status;
  int (*spawnRTCMethod)(std::string&, std::string&);
  std::string dialogMessage;
  simInt dialogHandle;
  std::string modelName;
  std::string argument;
  std::map<simInt, std::function<void(void)>> callbacks;
};
static MenuStatus g_menuStatus;




static void addMenuItem(const std::string& itemName, int(*spawnRTCMethod)(std::string&,std::string&), void(*luaCB)(SScriptCallBack*)) {
  simInt itemHandle;
  simAddModuleMenuEntry("RTCPlugin", 1, &itemHandle);
  simSetModuleMenuItemState(itemHandle, 1, ("add" + itemName + "RTC").c_str());
  //callbacks_[itemHandle] = cb;
  g_menuStatus.callbacks[itemHandle] = [itemName, spawnRTCMethod]() {
    std::cout << "[simExtRTC] " << itemName << " Menu Selected" << std::endl;
    g_menuStatus.status = g_menuStatus.MENU_ITEM_SELECTED;
    g_menuStatus.spawnRTCMethod = spawnRTCMethod;
    g_menuStatus.dialogMessage = "Input " + itemName + " Model Name";
  };
  auto cmdName = "simExtRTC.add" + itemName + "RTC";
  simRegisterScriptCallbackFunction((cmdName + "@" + PLUGIN_NAME).c_str(),
				    ("number retval=" + cmdName + "(string modelName,string rtcArg)").c_str(), 
				    luaCB);
}


#define CONCAT(x,y,z) x y z
#define strConCat(x,y,z)    CONCAT(x,y,z)

const int inArgs_ADD_RTC[]={
    2,
    sim_script_arg_string,0,
    sim_script_arg_string,0
};


static void LUA_ADD_RTC_CALLBACK(SScriptCallBack* cb, const std::string& itemName, int(*spawnRTCMethod)(std::string&,std::string&)) {
  std::cout << "[simExtRTC] add"<<itemName<<"RTC() called" << std::endl;
  CScriptFunctionData D;
  int handle=-1;
  auto cmd = "simExtRTC.add" + itemName + "RTC";
  if (D.readDataFromStack(cb->stackID,inArgs_ADD_RTC,inArgs_ADD_RTC[0], cmd.c_str())) {
    std::vector<CScriptFunctionDataItem>* inData=D.getInDataPtr();
    auto buf = inData->at(0).stringData[0];
    std::string modelName = buf;
    std::cout << "[simExtRTC] modelName is " << modelName << std::endl;

    buf = inData->at(1).stringData[0];
    std::string argument = buf;
    std::cout << "[simExtRTC] argument is " << argument << std::endl;

    spawnRTCMethod(modelName, argument);
  } else {
    std::cout << "[simExtRTC] readDataFromStack failed." << std::endl;
  }  

}

static void LUA_ADD_ROBOT_RTC_CALLBACK(SScriptCallBack* cb) {
  LUA_ADD_RTC_CALLBACK(cb, "Robot", spawnRobotRTC);
}

static void LUA_ADD_RANGE_RTC_CALLBACK(SScriptCallBack* cb) {
  LUA_ADD_RTC_CALLBACK(cb, "Range", spawnRangeRTC);  
}

static void LUA_ADD_CAMERA_RTC_CALLBACK(SScriptCallBack* cb) {
  LUA_ADD_RTC_CALLBACK(cb, "Camera", spawnCameraRTC);  
}

static void LUA_ADD_ACCEL_RTC_CALLBACK(SScriptCallBack* cb) {
	LUA_ADD_RTC_CALLBACK(cb, "Accelerometer", spawnAccelerometerRTC);
}

static void LUA_ADD_GYRO_RTC_CALLBACK(SScriptCallBack* cb) {
	LUA_ADD_RTC_CALLBACK(cb, "Gyro", spawnGyroRTC);
}

static void LUA_ADD_DEPTH_RTC_CALLBACK(SScriptCallBack* cb) {
	LUA_ADD_RTC_CALLBACK(cb, "Depth", spawnDepthRTC);
}

static void LUA_ADD_OBJECT_RTC_CALLBACK(SScriptCallBack* cb) {
	LUA_ADD_RTC_CALLBACK(cb, "Object", spawnObjectRTC);
}


// This is the plugin start routine (called just once, just after the plugin was loaded):
VREP_DLLEXPORT unsigned char simStart(void* reservedPointer,int reservedInt)
{
        std::cout << "[simExtRTC] simStart() called." << std::endl;
	// Dynamically load and bind V-REP functions:
	// ******************************************
	// 1. Figure out this plugin's directory:
	char curDirAndFile[1024];
#ifdef _WIN32
	_getcwd(curDirAndFile, sizeof(curDirAndFile));
#elif defined (__linux) || defined (__APPLE__)
	getcwd(curDirAndFile, sizeof(curDirAndFile));
#endif
	std::string currentDirAndPath(curDirAndFile);
	// 2. Append the V-REP library's name:
	std::string temp(currentDirAndPath);
#ifdef _WIN32
	temp+="/coppeliaSim.dll";
#elif defined (__linux)
	temp+="/licoppeliaSim.so";
#elif defined (__APPLE__)
	temp+="/libcoppeliaSim.dylib";
#endif /* __linux || __APPLE__ */
	// 3. Load the V-REP library:
	std::cout << "[simExtRTC] loading v_rep library." << std::endl;
	vrepLib = loadSimLibrary(temp.c_str());
	// vrepLib=loadVrepLibrary(temp.c_str());
	if (vrepLib==NULL)
	{
		std::cout << "[simExtRTC] Error, could not find or correctly load the V-REP library. Cannot start 'RTCSkeleton' plugin.\n";
		return(0); // Means error, V-REP will unload this plugin
	}
	// if (getVrepProcAddresses(vrepLib)==0)
	if (getSimProcAddresses(vrepLib) == 0)
	{
		std::cout << "[simExtRTC] Error, could not find all required functions in the V-REP library. Cannot start 'RTCSkeleton' plugin.\n";
		unloadSimLibrary(vrepLib);
		//unloadVrepLibrary(vrepLib);
		return(0); // Means error, V-REP will unload this plugin
	}
	// ******************************************

	// Check the version of V-REP:
	// ******************************************
	int vrepVer;
	simGetIntegerParameter(sim_intparam_program_version,&vrepVer);
	std::cout << "[simExtRTC] Version Parameter is " << vrepVer << std::endl;
	if (vrepVer<20604) // if V-REP version is smaller than 2.06.04
	{
		std::cout << "Sorry, your V-REP copy is somewhat old. Cannot start 'RTCSkeleton' plugin.\n";
		unloadSimLibrary(vrepLib);
		//unloadVrepLibrary(vrepLib);
		return(0); // Means error, V-REP will unload this plugin
	}
	// ******************************************

	// Here you could handle various initializations
	// Here you could also register custom Lua functions or custom Lua constants

	g_menuStatus.status = g_menuStatus.STANDBY;
	addMenuItem("Robot", spawnRobotRTC, LUA_ADD_ROBOT_RTC_CALLBACK);
	addMenuItem("Range", spawnRangeRTC, LUA_ADD_RANGE_RTC_CALLBACK);
	addMenuItem("Camera", spawnCameraRTC, LUA_ADD_CAMERA_RTC_CALLBACK);
	addMenuItem("Accelerometer", spawnAccelerometerRTC, LUA_ADD_ACCEL_RTC_CALLBACK);
	addMenuItem("Gyro", spawnGyroRTC, LUA_ADD_GYRO_RTC_CALLBACK);
	addMenuItem("Depth", spawnDepthRTC, LUA_ADD_DEPTH_RTC_CALLBACK);
	addMenuItem("Object", spawnObjectRTC, LUA_ADD_OBJECT_RTC_CALLBACK);
	/*
	simRegisterScriptCallbackFunction(strConCat(LUA_ADD_ROBOT_RTC_COMMAND, "@", PLUGIN_NAME),
					  strConCat("number return_status=", LUA_ADD_ROBOT_RTC_COMMAND, "(string modelName,string rtc_argument)"), 
					  LUA_ADD_ROBOT_RTC_CALLBACK);
	*/
	initRTM();
	simulatorClock.setSimulationTimeStep(simGetSimulationTimeStep());

	std::cout << "[simExtRTC] (version " << version_str << ") plugin safely loaded.\n";
	std::cout << "[simExtRTC] simStart() ended." << std::endl;
	return(PLUGIN_VERSION); // initialization went fine, we return the version number of this plugin (can be queried with simGetModuleName)
}


// This is the plugin end routine (called just once, when V-REP is ending, i.e. releasing this plugin):
VREP_DLLEXPORT void simEnd()
{
  std::cout << "[simExtRTC] simEnd() called." << std::endl;
  // Here you could handle various clean-up tasks
  exitRTM();
  unloadSimLibrary(vrepLib);
  std::cout << "[simExtRTC] simEnd() exiting." << std::endl;
}


void message_pump() {
  // Task Queue Check for unsynchronized task thrown from Service Port of RT-Component.
	Task t = taskQueue.popTask();
	simInt ret;
	switch(t.value){
	case Task::START:
	  std::cout << " - Task::Starting Simulation" << std::endl;
	  ret = simStartSimulation();
	  if (ret == 0) {
	    returnQueue.returnReturn(Return(Return::RET_FAILED));
	  } else if(ret < 0) {
	    returnQueue.returnReturn(Return(Return::RET_ERROR));
	  } else {
	    returnQueue.returnReturn(Return(Return::RET_OK));
	  }
	  break;
	case Task::STOP:
	  std::cout << " - Task::Stopping Simulation" << std::endl;
	  ret = simStopSimulation();
	  if (ret == 0) {
	    returnQueue.returnReturn(Return(Return::RET_FAILED));
	  } else if(ret < 0) {
	    returnQueue.returnReturn(Return(Return::RET_ERROR));
	  } else {
	    returnQueue.returnReturn(Return(Return::RET_OK));
	  }
	  break;
	case Task::PAUSE:
	  std::cout << " - Task::Pausing Simulation" << std::endl;
	  ret = simPauseSimulation();
	  if (ret == 0) {
	    returnQueue.returnReturn(Return(Return::RET_FAILED));
	  } else if(ret < 0) {
	    returnQueue.returnReturn(Return(Return::RET_ERROR));
	  } else {
	    returnQueue.returnReturn(Return(Return::RET_OK));
	  }
	  break;
	case Task::LOADPROJECT:
	  std::cout << " - Task::Loading Project: " << t.key.c_str() << std::endl;
	  ret = simLoadScene(t.key.c_str());
	  std::cout << " -- ret = " << ret;
	  if (ret < 0) {
	    returnQueue.returnReturn(Return(Return::RET_ERROR));
	  } else {
	    returnQueue.returnReturn(Return(Return::RET_OK));
	  }
	  break;
	case Task::SPAWNROBOT:
	  std::cout << " - Task::SpawnRobot" << std::endl;
	  if (spawnRobotRTC(t.key, t.arg) < 0) {
	    returnQueue.returnReturn(Return(Return::RET_ERROR));
	  } else {
	    returnQueue.returnReturn(Return(Return::RET_OK));
	  }
	  break;
	case Task::SPAWNRANGE:
	  std::cout << " - Task::Spawnrange" << std::endl;
	  if (spawnRangeRTC(t.key, t.arg) < 0) {
	    returnQueue.returnReturn(Return(Return::RET_ERROR));
	  } else {
	    returnQueue.returnReturn(Return(Return::RET_OK));
	  }

	  break;
	case Task::SPAWNDEPTH:
	  std::cout << " - Task::Spawndepth" << std::endl;
	  if (spawnDepthRTC(t.key, t.arg) < 0) {
	    returnQueue.returnReturn(Return(Return::RET_ERROR));
	  } else {
	    returnQueue.returnReturn(Return(Return::RET_OK));
	  }

	  break;
	case Task::SPAWNACCEL:
	  std::cout << " - Task::Spawnaccel" << std::endl;
	  if (spawnAccelerometerRTC(t.key, t.arg) < 0) {
	    returnQueue.returnReturn(Return(Return::RET_ERROR));
	  } else {
	    returnQueue.returnReturn(Return(Return::RET_OK));
	  }

	  break;
	case Task::SPAWNGYRO:
	  std::cout << " - Task::Spawngyro" << std::endl;
	  if (spawnGyroRTC(t.key, t.arg) < 0) {
	    returnQueue.returnReturn(Return(Return::RET_ERROR));
	  } else {
	    returnQueue.returnReturn(Return(Return::RET_OK));
	  }

	  break;
	case Task::SPAWNCAMERA:
	  std::cout << " - Task::SpawnCamera" << std::endl;
	  if (spawnCameraRTC(t.key, t.arg) < 0) {
	    returnQueue.returnReturn(Return(Return::RET_ERROR));
	  } else {
	    returnQueue.returnReturn(Return(Return::RET_OK));
	  }

	  break;
	case Task::SPAWNOBJECT:
		std::cout << " - Task::SpawnObject" << std::endl;
		if (spawnObjectRTC(t.key, t.arg) < 0) {
			returnQueue.returnReturn(Return(Return::RET_ERROR));
		}
		else {
			returnQueue.returnReturn(Return(Return::RET_OK));
		}

		break;
	case Task::KILLRTC:
	  std::cout << " - Task::KillRTC" << std::endl;
	  if (killRTC(t.key) < 0) {
	    returnQueue.returnReturn(Return(Return::RET_ERROR));
	  } else {
	    returnQueue.returnReturn(Return(Return::RET_OK));
	  }
	  break;
	case Task::KILLALLRTC:
	  std::cout << " - Task::KillAllRTC" << std::endl;
	  if (killAllRTC() < 0) {
	    returnQueue.returnReturn(Return(Return::RET_ERROR));
	  } else {
	    returnQueue.returnReturn(Return(Return::RET_OK));
	  }
	  break;
	case Task::SYNCRTC:
	  std::cout << " - Task::SYNCRTC" << std::endl;
	  if (syncRTC(t.key) < 0) {
	    returnQueue.returnReturn(Return(Return::RET_ERROR));
	  } else {
	    returnQueue.returnReturn(Return(Return::RET_OK));
	  }
	  break;
	case Task::GETSYNCRTC:
	  std::cout << " - Task::GETSYNCRTC" << std::endl;
	  {
	    Return r(Return::RET_OK);
	    if (getSyncRTCs(r.stringList) < 0) {
	      r.value = Return::RET_ERROR;
	      returnQueue.returnReturn(r);
	    } else {
	      for(int i = 0;i < r.stringList.size();i++) {
		std::cout << " - Task: " << r.stringList[i] << std::endl;
	      }
	      returnQueue.returnReturn(r);
	    }
	  }
	  break;
	case Task::GETSIMTIME:
	  std::cout << " - Task::GETSIMTIME" << std::endl;
	  {
	    Return r(Return::RET_OK);
	    r.floatValue = simGetSimulationTime();
	    returnQueue.returnReturn(r);
	  }
	  break;
	case Task::GETSIMSTEP:
	  std::cout << " - Task::GETSIMSTEP" << std::endl;
	  {
	    Return r(Return::RET_OK);
	    r.floatValue = simGetSimulationTimeStep();
	    returnQueue.returnReturn(r);
	  }
	  break;
	case Task::GETOBJPOSE:
	  returnQueue.returnReturn(Return(Return::RET_OK));
	  break;

	case Task::SETOBJPOSE:
	  returnQueue.returnReturn(Return(Return::RET_OK));
	  break;
	  
	default:
	  //returnQueue.returnReturn(Return(Return::RET_ERROR));
	  break;
	}


}

static simInt dlg;

// This is the plugin messaging routine (i.e. V-REP calls this function very often, with various messages):
VREP_DLLEXPORT void* simMessage(int message,int* auxiliaryData,void* customData,int* replyData)
{
        // This is called quite often. Just watch out for messages/events you want to handle
	// Keep following 5 lines at the beginning and unchanged:
        //static bool refreshDlgFlag=true;
	int errorModeSaved;
	simGetIntegerParameter(sim_intparam_error_report_mode,&errorModeSaved);
	simSetIntegerParameter(sim_intparam_error_report_mode,sim_api_errormessage_ignore);
	void* retVal=NULL;

	// Here we can intercept many messages from V-REP (actually callbacks). Only the most important messages are listed here.
	// For a complete list of messages that you can intercept/react with, search for "sim_message_eventcallback"-type constants
	// in the V-REP user manual.
	if (message==sim_message_eventcallback_mainscriptabouttobecalled)
	{ // The main script is about to be run (only called while a simulation is running (and not paused!))
	  //  main script is called every dynamics calculation. 
	  tickRTCs(simGetSimulationTimeStep());
	}
	if (message==sim_message_eventcallback_simulationabouttostart)
	{ // Simulation is about to start
	  simulatorClock.setSimulationTimeStep(simGetSimulationTimeStep());
	  //simulatorClock.setSimulationTime(simGetSimulationTime());
	  startRTCs();
	}
	if (message == sim_message_eventcallback_menuitemselected) {
		simInt handle = auxiliaryData[0];
		simInt state = auxiliaryData[1];
		if (g_menuStatus.callbacks.count(handle) <= 0) {
		  std::cout << "[simExtRTC] Error. simMessage callback with sim_message_eventcallback_menuitemselected, but handle can not found." << std::endl;
		} else {
			std::cout << "[simExtRTC] MenuItem Selected and dialog will starts...." << std::endl;
		  g_menuStatus.callbacks[handle]();
		}
	}
	if (message == sim_message_eventcallback_beforerendering) {
		if (g_menuStatus.status == g_menuStatus.MENU_ITEM_SELECTED) {
			g_menuStatus.dialogHandle = simDisplayDialog("Input Model Object Name",
				g_menuStatus.dialogMessage.c_str(),
				sim_dlgstyle_input, "", NULL, NULL, NULL);
			g_menuStatus.status = g_menuStatus.MODEL_NAME_DIALOG_SHOWN;
		}
		else if (g_menuStatus.status == g_menuStatus.MODEL_NAME_DIALOG_SHOWN) {
			simInt ret = simGetDialogResult(g_menuStatus.dialogHandle);
			if (ret == sim_dlgret_still_open) {

			}
			else if (ret == sim_dlgret_ok) {
				char *buf = simGetDialogInput(g_menuStatus.dialogHandle);
				g_menuStatus.modelName = buf;
				simReleaseBuffer(buf);
				g_menuStatus.dialogHandle = simDisplayDialog("Input Argument of RTC", "Input Argument of createComponent method", sim_dlgstyle_input, "", NULL, NULL, NULL);
				g_menuStatus.status = g_menuStatus.ARGUMENT_DIALOG_SHOWN;
			}
			else {
				g_menuStatus.status = g_menuStatus.STANDBY;
			}
		}
		else if (g_menuStatus.status == g_menuStatus.ARGUMENT_DIALOG_SHOWN) {
			simInt ret = simGetDialogResult(g_menuStatus.dialogHandle);
			if (ret == sim_dlgret_still_open) {

			}
			else if (ret == sim_dlgret_ok) {
				char *buf = simGetDialogInput(g_menuStatus.dialogHandle);
				g_menuStatus.argument = buf;
				simReleaseBuffer(buf);
				g_menuStatus.spawnRTCMethod(g_menuStatus.modelName, g_menuStatus.argument);
				g_menuStatus.status = g_menuStatus.STANDBY;
			}
			else {
				g_menuStatus.status = g_menuStatus.STANDBY;
			}

		}
	}
	if (message==sim_message_eventcallback_simulationended)
	{ // Simulation just ended
	  stopRTCs();
	}
	simulatorClock.setSimulationTime(simGetSimulationTime());
	message_pump();
	// Keep following unchanged:
	simSetIntegerParameter(sim_intparam_error_report_mode,errorModeSaved); // restore previous settings
	return(retVal);
}

