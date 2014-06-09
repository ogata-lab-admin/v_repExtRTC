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


/*
 * Methods corresponding to IDL attributes and operations
 */
ssr::RETURN_VALUE SimulatorSVC_impl::loadProject(const char* path)
{
	ssr::RETURN_VALUE result;
  // Please insert your code here and remove the following warning pragma
#ifndef WIN32
  #warning "Code missing in function <ssr::RETURN_VALUE SimulatorSVC_impl::loadProject(const char* path)>"
#endif
  return result;
}

ssr::RETURN_VALUE SimulatorSVC_impl::start()
{
  ssr::RETURN_VALUE result;
  Task task(Task::START);
  taskQueue.pushTask(task);
  Return retval = returnQueue.waitReturn();
  result = ssr::RETVAL_OK;
  return result;
}

ssr::RETURN_VALUE SimulatorSVC_impl::pause()
{
  ssr::RETURN_VALUE result;
  Task task(Task::START);
  taskQueue.pushTask(task);
  Return retval = returnQueue.waitReturn();
  result = ssr::RETVAL_OK;
  return result;
}

ssr::RETURN_VALUE SimulatorSVC_impl::resume()
{
	ssr::RETURN_VALUE result;
  // Please insert your code here and remove the following warning pragma
#ifndef WIN32
  #warning "Code missing in function <ssr::RETURN_VALUE SimulatorSVC_impl::resume()>"
#endif
  return result;
}

ssr::RETURN_VALUE SimulatorSVC_impl::stop()
{
  ssr::RETURN_VALUE result;
  Task task(Task::STOP);
  taskQueue.pushTask(task);
  Return retval = returnQueue.waitReturn();
  result = ssr::RETVAL_OK;
  return result;
}

ssr::RETURN_VALUE SimulatorSVC_impl::spawnRobotRTC(const char* objectName, const char* arg)
{
  ssr::RETURN_VALUE result;
  Task task(Task::SPAWNROBOT);
  task.key = objectName;
  taskQueue.pushTask(task);
  Return retval = returnQueue.waitReturn();
  result = ssr::RETVAL_OK;
  return result;
}

ssr::RETURN_VALUE SimulatorSVC_impl::spawnRangeRTC(const char* objectName, const char* arg)
{
  std::cout << "SPAWNRANGE:" << objectName << std::endl;
  ssr::RETURN_VALUE result;
  Task task(Task::SPAWNRANGE);
  task.key = objectName;
  taskQueue.pushTask(task);
  Return retval = returnQueue.waitReturn();
  result = ssr::RETVAL_OK;
  return result;
}

ssr::RETURN_VALUE SimulatorSVC_impl::killRobotRTC(const char* objectName)
{
	ssr::RETURN_VALUE result;
  // Please insert your code here and remove the following warning pragma
#ifndef WIN32
  #warning "Code missing in function <ssr::RETURN_VALUE SimulatorSVC_impl::killRobotRTC(const char* objectName)>"
#endif
  return result;
}

ssr::RETURN_VALUE SimulatorSVC_impl::getObjectHandle(const char* objectName, ssr::OBJECT_HANDLE& handle)
{
	ssr::RETURN_VALUE result;
  // Please insert your code here and remove the following warning pragma
#ifndef WIN32
  #warning "Code missing in function <ssr::RETURN_VALUE SimulatorSVC_impl::getObjectHandle(const char* objectName, OBJECT_HANDLE& handle)>"
#endif
  return result;
}

ssr::RETURN_VALUE SimulatorSVC_impl::getObjectPose(ssr::OBJECT_HANDLE handle, RTC::Pose3D& pose)
{
	ssr::RETURN_VALUE result;
  // Please insert your code here and remove the following warning pragma
#ifndef WIN32
  #warning "Code missing in function <ssr::RETURN_VALUE SimulatorSVC_impl::getObjectPose(OBJECT_HANDLE handle, RTC::Pose3D& pose)>"
#endif
  return result;
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



