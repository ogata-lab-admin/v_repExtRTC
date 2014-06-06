// -*- C++ -*-
/*!
 * @file  RobotRTC.cpp
 * @brief Simulator Robot RTC
 * @date $Date$
 *
 * $Id$
 */

#include "RobotRTC.h"

#include <string>
#include <sstream>
#include <iostream>
// Module specification
// <rtc-template block="module_spec">
static const char* robotrtc_spec[] =
  {
    "implementation_id", "RobotRTC",
    "type_name",         "RobotRTC",
    "description",       "Simulator Robot RTC",
    "version",           "1.0.0",
    "vendor",            "ysuga_net",
    "category",          "Simulator",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.objectName", "none",
    //    "conf.default.objectHandle", "-1",
    "conf.default.activeJointNames", "[]",
    // Widget
    "conf.__widget__.objectName", "text",
    //"conf.__widget__.objectHandle", "text",
    "conf.__widget__.activeJointNames", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RobotRTC::RobotRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_targetForceIn("targetForce", m_targetForce),
    m_targetVelocityIn("targetVelocity", m_targetVelocity),
    m_targetPositionIn("targetPosition", m_targetPosition),
    m_currentForceOut("currentForce", m_currentForce),
    m_currentVelocityOut("currentVelocity", m_currentVelocity),
    m_currentPositiojnOut("currentPosition", m_currentPositiojn)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RobotRTC::~RobotRTC()
{
}



RTC::ReturnCode_t RobotRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("targetForce", m_targetForceIn);
  addInPort("targetVelocity", m_targetVelocityIn);
  addInPort("targetPosition", m_targetPositionIn);
  
  // Set OutPort buffer
  addOutPort("currentForce", m_currentForceOut);
  addOutPort("currentVelocity", m_currentVelocityOut);
  addOutPort("currentPosition", m_currentPositiojnOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("objectName", m_objectName, "none");
  //bindParameter("objectHandle", m_objectHandle, "-1");
  bindParameter("activeJointNames", m_activeJointNames, "[]");
  // </rtc-template>

  std::vector<std::string> keys;
  std::vector<int32_t> values;
  
  std::string names = m_properties.getProperty("conf.__innerparam.allNames");
  std::cout << "allNames=" << names << std::endl;
  std::stringstream nss(names);
  std::string token;
  while(std::getline(nss, token, ',')) {
    std::stringstream trimmer;
    trimmer << token;
    token.clear();
    trimmer >> token;
    std::cout << " key = " << token << std::endl;
    keys.push_back(token);
  }

  std::string handles = m_properties.getProperty("conf.__innerparam.allHandles");
  std::cout << "allHandles=" << handles << std::endl;
  std::stringstream hss(handles);
  int32_t buf;
  while(std::getline(hss, token, ',')) {
    std::stringstream trimmer;
    trimmer << token;
    token.clear();
    trimmer >> buf;
    std::cout << " value = " << buf << std::endl;
    values.push_back(buf);
  }

  uint32_t size = keys.size();
  for(uint32_t i = 0;i < size;i++) {
    m_jointHandleMap.append(keys[i], values[i]);
  }

  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RobotRTC::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RobotRTC::onActivated(RTC::UniqueId ec_id)
{
  m_activeJointHandle.clear();

  std::string names = m_activeJointNames;
  std::cout << "activeJointNames=" << names << std::endl;
  std::stringstream nss(names);
  std::string token;
  while(std::getline(nss, token, ',')) {
    std::stringstream trimmer;
    trimmer << token;
    token.clear();
    trimmer >> token;
    std::cout << " key = " << token << std::endl;
    JointHandleMap::iterator i = m_jointHandleMap.find(token);
    if (i == m_jointHandleMap.end()) {
      std::cout << "Can not find active joint name " << token << std::endl;
      return RTC::RTC_ERROR;
    }
    m_activeJointHandle.push_back(i->second);
  }


  for (int i = 0;i < m_activeJointHandle.size();i++) {
    std::cout << "acgiveJointHandle:" << m_activeJointHandle[i] << std::endl;
  }
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RobotRTC::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RobotRTC::onExecute(RTC::UniqueId ec_id)
{
  std::cout << "onExecute" << std::endl;
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RobotRTC::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RobotRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void RobotRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(robotrtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<RobotRTC>,
                             RTC::Delete<RobotRTC>);
  }
  
};


