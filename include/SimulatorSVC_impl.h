// -*-C++-*-
/*!
 * @file  SimulatorSVC_impl.h
 * @brief Service implementation header of Simulator.idl
 *
 */

#include "BasicDataTypeSkel.h"
#include "ExtendedDataTypesSkel.h"

#include "SimulatorSkel.h"

#ifndef SIMULATORSVC_IMPL_H
#define SIMULATORSVC_IMPL_H
 
/*!
 * @class SimulatorSVC_impl
 * Example class implementing IDL interface ssr::Simulator
 */
class SimulatorSVC_impl
 : public virtual POA_ssr::Simulator,
   public virtual PortableServer::RefCountServantBase
{
 private:
   // Make sure all instances are built on the heap by making the
   // destructor non-public
   //virtual ~SimulatorSVC_impl();

 public:
  /*!
   * @brief standard constructor
   */
   SimulatorSVC_impl();
  /*!
   * @brief destructor
   */
   virtual ~SimulatorSVC_impl();

   // attributes and operations
   ssr::RETURN_VALUE loadProject(const char* path);
   ssr::RETURN_VALUE start();
   ssr::RETURN_VALUE pause();
   ssr::RETURN_VALUE resume();
   ssr::RETURN_VALUE stop();
   ssr::RETURN_VALUE spawnRobotRTC(const char* objectName, const char* arg);
   ssr::RETURN_VALUE spawnRangeRTC(const char* objectName, const char* arg);
   ssr::RETURN_VALUE spawnCameraRTC(const char* objectName, const char* arg);
   ssr::RETURN_VALUE spawnAccelerometerRTC(const char* objectName, const char* arg);
   ssr::RETURN_VALUE spawnGyroRTC(const char* objectName, const char* arg);
   ssr::RETURN_VALUE spawnObjectRTC(const char* objectName, const char* arg);
   ssr::RETURN_VALUE spawnDepthRTC(const char* objectName, const char* arg);
   ssr::RETURN_VALUE killRobotRTC(const char* objectName);
  ssr::RETURN_VALUE killAllRobotRTC();
  ssr::RETURN_VALUE getObjectPose(const char* objectName, RTC::Pose3D& pose);
  ssr::RETURN_VALUE setObjectPose(const char* objectName, const ::RTC::Pose3D& pose);
  ssr::RETURN_VALUE synchronizeRTC(const char* rtcFullPath);
  ssr::RETURN_VALUE getSynchronizingRTCs(::ssr::StringSeq_out fullPaths);
  ssr::RETURN_VALUE getSimulationTimeStep(::CORBA::Float& timeStep);
  ssr::RETURN_VALUE getSimulationTime(::CORBA::Float& time);

};

/*!
 * @class SimulatedRobotSVC_impl
 * Example class implementing IDL interface ssr::SimulatedRobot
 */
/**
class SimulatedRobotSVC_impl
 : public virtual POA_ssr::SimulatedRobot,
   public virtual PortableServer::RefCountServantBase
{
 private:
   // Make sure all instances are built on the heap by making the
   // destructor non-public
   //virtual ~SimulatedRobotSVC_impl();

 public:
   SimulatedRobotSVC_impl();
   virtual ~SimulatedRobotSVC_impl();

   // attributes and operations
  ssr::RETURN_VALUE getJointTagNames(ssr::StringSeq_out tags);

};

**/

#endif // SIMULATORSVC_IMPL_H


