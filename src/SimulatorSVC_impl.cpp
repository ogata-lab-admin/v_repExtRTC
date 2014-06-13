// -*-C++-*-
/*!
 * @file  SimulatorSVC_impl.cpp
 * @brief Service implementation code of Simulator.idl
 *
 */

#include "SimulatorSVC_impl.h"
#include "RTCHelper.h"

extern TaskQueue taskQueue;
extern ReturnQueue returnQueue;
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

ssr::RETURN_VALUE SimulatorSVC_impl::killRobotRTC(const char* objectName)
{

  taskQueue.pushTask(Task(Task::KILLRTC, objectName));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::killAllRobotRTC()
{
  std::cout << "SimulatorSVC_impl::killAllRobotRTC" << std::endl;
  taskQueue.pushTask(Task(Task::KILLALLRTC));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::getObjectPose(const char* objectName, RTC::Pose3D& pose)
{
  taskQueue.pushTask(Task(Task::GETOBJPOSE, objectName));
  return returnCheck(returnQueue.waitReturn());
}


ssr::RETURN_VALUE SimulatorSVC_impl::setObjectPose(const char* objectName, const RTC::Pose3D& pose)
{
  taskQueue.pushTask(Task(Task::SETOBJPOSE, objectName));
  return returnCheck(returnQueue.waitReturn());
}

ssr::RETURN_VALUE SimulatorSVC_impl::synchronizeRTC(const char* rtcFullPath) {
  taskQueue.pushTask(Task(Task::SYNCRTC, rtcFullPath));
  return returnCheck(returnQueue.waitReturn());
}

// End of example implementational code

/*
 * Example implementational code for IDL interface ssr::SimulatedRobot
 */
SimulatedRobotSVC_impl::SimulatedRobotSVC_impl()
{
  // Please add extra constructor code here.
}


SimulatedRobotSVC_impl::~SimulatedRobotSVC_impl()
{
  // Please add extra destructor code here.
}


/*
 * Methods corresponding to IDL attributes and operations
 */
ssr::RETURN_VALUE SimulatedRobotSVC_impl::getJointTagNames(ssr::StringSeq_out tags)
{
	ssr::RETURN_VALUE result;
  // Please insert your code here and remove the following warning pragma
#ifndef WIN32
  #warning "Code missing in function <ssr::RETURN_VALUE SimulatedRobotSVC_impl::getJointTagNames(StringSeq_out tags)>"
#endif
  return result;
}



// End of example implementational code



