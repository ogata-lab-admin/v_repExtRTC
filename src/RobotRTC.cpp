// -*- C++ -*-
/*!
 * @file  RobotRTC.cpp
 * @brief Simulator Robot RTC for VREP simulator
 * @date 2014/06/05
 * @author Yuki Suga (ysuga@ysuga.net)
 * @copyright 2014, Ogata Laboratory, Waseda University
 */

#include "RobotRTC.h"
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include "simLib.h"
// Module specification
// <rtc-template block="module_spec">
static const char* robotrtc_spec[] =
  {
    "implementation_id", "RobotRTC",
    "type_name",         "RobotRTC",
    "description",       "Simulator Robot RTC",
    "version",           "1.0.1",
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
    m_currentPositionOut("currentPosition", m_currentPosition)

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
  addOutPort("currentPosition", m_currentPositionOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("objectName", m_objectName, "none");
  //bindParameter("objectHandle", m_objectHandle, "-1");
  bindParameter("controlledJointNames", m_controlledJointNames, "[]");
  bindParameter("observedJointNames", m_observedJointNames, "[]");
  // </rtc-template>

  RTC_DEBUG(("[simExtRTC.RobotRTC] Initializing Robot RTC(%s)", m_properties.getProperty("conf.default.objectName")));
  std::vector<std::string> keys;
  std::vector<int32_t> values;
  
  std::string names = m_properties.getProperty("conf.__innerparam.allNames");
  RTC_DEBUG(("[simExtRTC.RobotRTC]  - All Joint Name is %s", names));
  std::stringstream nss(names);
  std::string token;
  while(std::getline(nss, token, ',')) {
    std::stringstream trimmer;
    trimmer << token;
    token.clear();
    trimmer >> token;
    //std::cout << " key = " << token << std::endl;
    keys.push_back(token);
  }

  std::string handles = m_properties.getProperty("conf.__innerparam.allHandles");
  RTC_DEBUG(("[simExtRTC.RobotRTC]  - All Joint Handle is %s", handles));
  std::stringstream hss(handles);
  int32_t buf;
  while(std::getline(hss, token, ',')) {
    std::stringstream trimmer;
    trimmer << token;
    token.clear();
    trimmer >> buf;
    //std::cout << " value = " << buf << std::endl;
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

  RTC_DEBUG(("[simExtRTC.RobotRTC] Activating RobotRTC(%s)", m_objectName));
  // td::cout << " - Activating RobotRTC(" << m_objectName << ")" << std::endl;
  m_controlledJointHandle.clear();
  m_observedJointHandle.clear();
  updateParameters("default");

  std::string names = m_controlledJointNames;
  RTC_DEBUG(("[simExtRTC.RobotRTC] RobotRTC(%s) config.controlledJointNames=%s", names));
  std::stringstream nss(names);
  std::string token;
  while(std::getline(nss, token, ',')) {
    std::stringstream trimmer;
    trimmer << token;
    token.clear();
    trimmer >> token;
    JointHandleMap::iterator i = m_jointHandleMap.find(token);
    if (i == m_jointHandleMap.end()) {
      std::cout << " -- ERROR: Can not find active joint name " << token << std::endl;
      return RTC::RTC_ERROR;
    }
    m_controlledJointHandle.push_back(i->second);
  }

  names = m_controlledJointNames;
  RTC_DEBUG(("[simExtRTC.RobotRTC] RobotRTC(%s) config.observedJointNames=%s", names));
  std::stringstream nss2(names);
  while(std::getline(nss2, token, ',')) {
    std::stringstream trimmer;
    trimmer << token;
    token.clear();
    trimmer >> token;
    JointHandleMap::iterator i = m_jointHandleMap.find(token);
    if (i == m_jointHandleMap.end()) {
      std::cout << " -- ERROR: Can not find active joint name " << token << std::endl;
      return RTC::RTC_ERROR;
    }
    m_observedJointHandle.push_back(i->second);
  }

  m_currentPosition.data.length(m_observedJointHandle.size());
  m_currentVelocity.data.length(m_observedJointHandle.size());
  m_currentForce.data.length(m_observedJointHandle.size());
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RobotRTC::onDeactivated(RTC::UniqueId ec_id)
{
	RTC_DEBUG(("[simExtRTC.RobotRTC] Deactivating RobotRTC(%s)", m_objectName));
	m_controlledJointHandle.clear();
	m_observedJointHandle.clear();
	return RTC::RTC_OK;
}


RTC::ReturnCode_t RobotRTC::onExecute(RTC::UniqueId ec_id)
{
  if (m_targetPositionIn.isNew()) {
    m_targetPositionIn.read();
    if (m_controlledJointHandle.size() != m_targetPosition.data.length()) {
		RTC_DEBUG(("[simExtRTC.RobotRTC(%s)] onExecute(): Invalid Data number of targetPosition. This RTC reuqires %d data size. But %d data is sent.",
			m_objectName, m_controlledJointHandle.size(), m_targetPosition.data.length()));
		return RTC::RTC_ERROR;
    }
    for(uint32_t i = 0;i < m_targetPosition.data.length();i++) {
      simSetJointTargetPosition(m_controlledJointHandle[i], m_targetPosition.data[i]);
    }
  }

  if (m_targetVelocityIn.isNew()) {
    m_targetVelocityIn.read();
    if (m_controlledJointHandle.size() != m_targetVelocity.data.length()) {
		RTC_DEBUG(("[simExtRTC.RobotRTC(%s)] onExecute(): Invalid Data number of targetVelocity. This RTC reuqires %d data size. But %d data is sent.",
			m_objectName, m_controlledJointHandle.size(), m_targetVelocity.data.length()));
        return RTC::RTC_ERROR;
    }
    for(uint32_t i = 0;i < m_targetVelocity.data.length();i++) {
      simSetJointTargetVelocity(m_controlledJointHandle[i], m_targetVelocity.data[i]);
    }
  }

  if (m_targetForceIn.isNew()) {
    m_targetForceIn.read();
    if (m_controlledJointHandle.size() != m_targetForce.data.length()) {
		RTC_DEBUG(("[simExtRTC.RobotRTC(%s)] onExecute(): Invalid Data number of targetForce. This RTC reuqires %d data size. But %d data is sent.",
			m_objectName, m_controlledJointHandle.size(), m_targetForce.data.length()));
        return RTC::RTC_ERROR;
    }
    for(uint32_t i = 0;i < m_targetForce.data.length();i++) {
      simSetJointForce(m_controlledJointHandle[i], m_targetForce.data[i]);
    }
  }
  
  float dt = simGetSimulationTimeStep();
  float time = simGetSimulationTime();
  long sec = floor(time);
  long nsec = (time - sec) * 1000*1000*1000;
  size_t sz = m_observedJointHandle.size();
  for(uint32_t i = 0;i < sz;i++) {
    float buf;
    if (simGetJointPosition(m_observedJointHandle[i], &buf) < 0) {
		RTC_DEBUG(("[simExtRTC.RobotRTC(%s)] onExecute(): GetJointPosition failed.", m_objectName));
		return RTC::RTC_ERROR;
    }

	/// Velocity can not acquired from CoppeliaSim.
	double old_pos = m_currentPosition.data[i];
    m_currentPosition.data[i] = buf;
    double v = (buf - old_pos) / dt;
    m_currentVelocity.data[i] = v;

    if (simJointGetForce(m_observedJointHandle[i], &buf) < 0) {
		RTC_DEBUG(("[simExtRTC.RobotRTC(%s)] onExecute(): GetJointForce failed.", m_objectName));
		return RTC::RTC_ERROR;
    }
    m_currentForce.data[i] = buf;

  }
  m_currentPosition.tm.sec = sec;
  m_currentPosition.tm.nsec = nsec;
  m_currentPositionOut.write();

  m_currentVelocity.tm.sec = sec;
  m_currentVelocity.tm.nsec = nsec;
  m_currentVelocityOut.write();

  m_currentForce.tm.sec = sec;
  m_currentForce.tm.nsec = nsec;
  m_currentForceOut.write();

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


