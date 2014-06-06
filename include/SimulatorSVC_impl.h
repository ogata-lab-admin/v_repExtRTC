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
   ssr::RETURN_VALUE killRobotRTC(const char* objectName);
  ssr::RETURN_VALUE getObjectHandle(const char* objectName, ssr::OBJECT_HANDLE& handle);
  ssr::RETURN_VALUE getObjectPose(ssr::OBJECT_HANDLE handle, RTC::Pose3D& pose);

};

/*!
 * @class SimulatedRobotSVC_impl
 * Example class implementing IDL interface ssr::SimulatedRobot
 */
class SimulatedRobotSVC_impl
 : public virtual POA_ssr::SimulatedRobot,
   public virtual PortableServer::RefCountServantBase
{
 private:
   // Make sure all instances are built on the heap by making the
   // destructor non-public
   //virtual ~SimulatedRobotSVC_impl();

 public:
  /*!
   * @brief standard constructor
   */
   SimulatedRobotSVC_impl();
  /*!
   * @brief destructor
   */
   virtual ~SimulatedRobotSVC_impl();

   // attributes and operations
  ssr::RETURN_VALUE getJointTagNames(ssr::StringSeq_out tags);

};



#endif // SIMULATORSVC_IMPL_H


