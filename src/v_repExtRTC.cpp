// Copyright: 2014, Ogata-laboratory, Waseda University.
// Author: Yuki Suga (ysuga@ysuga.net)
// URL: http://ogata-lab.jp
// This file is distributed under the GNU Public License version 3

#include "v_repExtRTC.h"
#include "v_repLib.h"
#include <iostream>
// For RTC 
#include "RTCHelper.h"

#ifdef _WIN32
	#include <shlwapi.h>
	#pragma comment(lib, "Shlwapi.lib")
#endif /* _WIN32 */

#if defined (__linux) || defined (__APPLE__)
	#include <string.h>
#include <unistd.h>
	#define _stricmp(x,y) strcasecmp(x,y)
#endif

#define PLUGIN_VERSION 1

LIBRARY vrepLib; // the V-REP library that we will dynamically load and bind



// This is the plugin start routine (called just once, just after the plugin was loaded):
VREP_DLLEXPORT unsigned char v_repStart(void* reservedPointer,int reservedInt)
{
	// Dynamically load and bind V-REP functions:
	// ******************************************
	// 1. Figure out this plugin's directory:
	char curDirAndFile[1024];
#ifdef _WIN32
	GetModuleFileName(NULL,curDirAndFile,1023);
	PathRemoveFileSpec(curDirAndFile);
#elif defined (__linux) || defined (__APPLE__)
	getcwd(curDirAndFile, sizeof(curDirAndFile));
#endif
	std::string currentDirAndPath(curDirAndFile);
	// 2. Append the V-REP library's name:
	std::string temp(currentDirAndPath);
#ifdef _WIN32
	temp+="\\v_rep.dll";
#elif defined (__linux)
	temp+="/libv_rep.so";
#elif defined (__APPLE__)
	temp+="/libv_rep.dylib";
#endif /* __linux || __APPLE__ */
	// 3. Load the V-REP library:
	vrepLib=loadVrepLibrary(temp.c_str());
	if (vrepLib==NULL)
	{
		std::cout << "Error, could not find or correctly load the V-REP library. Cannot start 'RTCSkeleton' plugin.\n";
		return(0); // Means error, V-REP will unload this plugin
	}
	if (getVrepProcAddresses(vrepLib)==0)
	{
		std::cout << "Error, could not find all required functions in the V-REP library. Cannot start 'RTCSkeleton' plugin.\n";
		unloadVrepLibrary(vrepLib);
		return(0); // Means error, V-REP will unload this plugin
	}
	// ******************************************

	// Check the version of V-REP:
	// ******************************************
	int vrepVer;
	simGetIntegerParameter(sim_intparam_program_version,&vrepVer);
	if (vrepVer<20604) // if V-REP version is smaller than 2.06.04
	{
		std::cout << "Sorry, your V-REP copy is somewhat old. Cannot start 'RTCSkeleton' plugin.\n";
		unloadVrepLibrary(vrepLib);
		return(0); // Means error, V-REP will unload this plugin
	}
	// ******************************************

	// Here you could handle various initializations
	// Here you could also register custom Lua functions or custom Lua constants
	// etc.

	initRTM();
	simulatorClock.setSimulationTimeStep(simGetSimulationTimeStep());

	return(PLUGIN_VERSION); // initialization went fine, we return the version number of this plugin (can be queried with simGetModuleName)
}

// This is the plugin end routine (called just once, when V-REP is ending, i.e. releasing this plugin):
VREP_DLLEXPORT void v_repEnd()
{
	// Here you could handle various clean-up tasks
        exitRTM();
	unloadVrepLibrary(vrepLib); // release the library
	std::cout << "exiting v_repEnd" << std::endl;
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

// This is the plugin messaging routine (i.e. V-REP calls this function very often, with various messages):
VREP_DLLEXPORT void* v_repMessage(int message,int* auxiliaryData,void* customData,int* replyData)
{ // This is called quite often. Just watch out for messages/events you want to handle
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

