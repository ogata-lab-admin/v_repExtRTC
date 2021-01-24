// -*-C++-*-
/*!
 * @file  SimulatorSVC_impl.cpp
 * @brief Service implementation code of Simulator.idl
 *
 */

#include "SimulatorSVC_impl.h"
#include "RTCHelper.h"

using namespace simExtRTC;

extern TaskQueue taskQueue;
extern ReturnQueue returnQueue;
extern simExtRTC::SimulatorClock simulatorClock;

/*
 * Example implementational code for IDL interface ssr::Simulator
 */
SimulatorSVC_impl::SimulatorSVC_impl()
{
  // Please add extra constructor code here.
}


SimulatorSVC_impl::~SimulatorSVC_impl()
{
  // Please add extra destructor code here.
}


static ssr::RETURN_VALUE returnCheck(const Return& ret) {
  std::cout << " -- SimulatorSVC_impl::returnCheck:";
  switch(ret.value) {
  case Return::RET_OK:
    std::cout << " ... OK" << std::endl;
    return ssr::RETVAL_OK;
  default:
    std::cout << " ... FAILED" << std::endl;
    return ssr::RETVAL_UNKNOWN_ERROR;
  }
  return ssr::RETVAL_UNKNOWN_ERROR;
}

/*
 * Methods corresponding to IDL attributes and operations
 */
ssr::RETURN_VALUE SimulatorSVC_impl::loadProject(const char* path)
{
  taskQueue.pushTask(Task(Task::LOADPROJECT, path));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::start()
{
  taskQueue.pushTask(Task(Task::START));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::pause()
{
  taskQueue.pushTask(Task(Task::PAUSE));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::resume()
{
  taskQueue.pushTask(Task(Task::START));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::stop()
{
  taskQueue.pushTask(Task(Task::STOP));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::spawnRobotRTC(const char* objectName, const char* arg)
{
  taskQueue.pushTask(Task(Task::SPAWNROBOT, objectName, arg));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::spawnRangeRTC(const char* objectName, const char* arg)
{
  taskQueue.pushTask(Task(Task::SPAWNRANGE, objectName, arg));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::spawnCameraRTC(const char* objectName, const char* arg)
{
  taskQueue.pushTask(Task(Task::SPAWNCAMERA, objectName, arg));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::spawnAccelerometerRTC(const char* objectName, const char* arg)
{
  taskQueue.pushTask(Task(Task::SPAWNACCEL, objectName, arg));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::spawnGyroRTC(const char* objectName, const char* arg)
{
  taskQueue.pushTask(Task(Task::SPAWNGYRO, objectName, arg));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::spawnDepthRTC(const char* objectName, const char* arg)
{
  taskQueue.pushTask(Task(Task::SPAWNDEPTH, objectName, arg));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::spawnObjectRTC(const char* objectName, const char* arg)
{
  taskQueue.pushTask(Task(Task::SPAWNOBJECT, objectName, arg));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::killRobotRTC(const char* objectName)
{
  taskQueue.pushTask(Task(Task::KILLRTC, objectName));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::killAllRobotRTC()
{
  taskQueue.pushTask(Task(Task::KILLALLRTC));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::getObjectPose(const char* objectName, RTC::Pose3D& pose)
{
  return ssr::RETVAL_NOT_IMPLEMENTED;
  //taskQueue.pushTask(Task(Task::GETOBJPOSE, objectName));
  //return returnCheck(returnQueue.waitReturn());
}


ssr::RETURN_VALUE SimulatorSVC_impl::setObjectPose(const char* objectName, const RTC::Pose3D& pose)
{
  return ssr::RETVAL_NOT_IMPLEMENTED;
  //taskQueue.pushTask(Task(Task::SETOBJPOSE, objectName));
  //return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::synchronizeRTC(const char* rtcFullPath) {
  taskQueue.pushTask(Task(Task::SYNCRTC, rtcFullPath));
  return returnCheck(returnQueue.waitReturn());
}


ssr::RETURN_VALUE SimulatorSVC_impl::getSynchronizingRTCs(::ssr::StringSeq_out fullPaths) {
  taskQueue.pushTask(Task(Task::GETSYNCRTC));
  ssr::StringSeq* buf = new ::ssr::StringSeq();
  ssr::StringSeq& buf2 = *buf;
 // fullPaths =
  Return r = returnQueue.waitReturn();
  if (r.value == Return::RET_OK) {
    buf2.length(r.stringList.size());
    for (int i = 0;i < r.stringList.size();i++) {
		buf2[i] = ::CORBA::string_dup(r.stringList[i].c_str());
    }
  }
  fullPaths = buf;
  return returnCheck(r);
}

ssr::RETURN_VALUE SimulatorSVC_impl::getSimulationTimeStep(::CORBA::Float& timeStep) 
{
  //taskQueue.pushTask(Task(Task::GETSIMSTEP));
  //Return r = returnQueue.waitReturn();
  //  if (r.value == Return::RET_OK) {
  //    timeStep = r.floatValue;
  //  }
  timeStep = simulatorClock.getSimulationTimeStep();
  //  return returnCheck(r);
  return ssr::RETVAL_OK;
}

ssr::RETURN_VALUE SimulatorSVC_impl::getSimulationTime(::CORBA::Float& time) {
  /*
  taskQueue.pushTask(Task(Task::GETSIMTIME));
  Return r = returnQueue.waitReturn();
  if (r.value == Return::RET_OK) {
    time = r.floatValue;
  }
  */
  time = simulatorClock.getSimulationTime();
  return ssr::RETVAL_OK;
}

// End of example implementational code

/*
 * Example implementational code for IDL interface ssr::SimulatedRobot

SimulatedRobotSVC_impl::SimulatedRobotSVC_impl()
{
  // Please add extra constructor code here.
}


SimulatedRobotSVC_impl::~SimulatedRobotSVC_impl()
{
  // Please add extra destructor code here.
}



 * Methods corresponding to IDL attributes and operations

ssr::RETURN_VALUE SimulatedRobotSVC_impl::getJointTagNames(ssr::StringSeq_out tags)
{
	ssr::RETURN_VALUE result;
  // Please insert your code here and remove the following warning pragma
#ifndef WIN32
  #warning "Code missing in function <ssr::RETURN_VALUE SimulatedRobotSVC_impl::getJointTagNames(StringSeq_out tags)>"
#endif
  return result;
}
*/

// End of example implementational code



