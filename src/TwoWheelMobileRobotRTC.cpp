// -*- C++ -*-
/*!
 * @file  TwoWheelMobileRobotRTC.cpp
 * @brief Mobile robot kinematics component
 * @date $Date$
 *
 * $Id$
 */

#include "TwoWheelMobileRobotRTC.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "simLib.h"
// Module specification
// <rtc-template block="module_spec">
static const char* mobilerobotkinematics_spec[] =
  {
    "implementation_id", "TwoWheelMobileRobotRTC",
    "type_name",         "TwoWheelMobileRobotRTC",
    "description",       "Mobile robot kinematics component",
    "version",           "1.0.0",
    "vendor",            "Sugar Sweet Robotics",
    "category",          "Navigation",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.axleTrack", "0.331",
    "conf.default.wheelRadius", "0.09525",
    // Widget
    "conf.__widget__.axleTrack", "text",
    "conf.__widget__.wheelRadius", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
TwoWheelMobileRobotRTC::TwoWheelMobileRobotRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
   // m_currentWheelAngleIn("currentWheelAngle", m_currentWheelAngle),
    m_targetVelocityIn("targetVelocity", m_targetVelocity),
    m_updatePoseIn("updatePose", m_updatePose),
  //  m_targetWheelVelocityOut("targetWheelVelocity", m_targetWheelVelocity),
    m_currentPoseOut("currentPose", m_currentPose)

    // </rtc-template>
{
  x = y = theta = 0;
  m_oldRightWheelAngle = m_oldLeftWheelAngle = 0;
}

/*!
 * @brief destructor
 */
TwoWheelMobileRobotRTC::~TwoWheelMobileRobotRTC()
{
}



RTC::ReturnCode_t TwoWheelMobileRobotRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  // addInPort("currentWheelAngle", m_currentWheelAngleIn);
  addInPort("targetVelocity", m_targetVelocityIn);
  addInPort("updatePose", m_updatePoseIn);
  
  // Set OutPort buffer
  //addOutPort("targetWheelVelocity", m_targetWheelVelocityOut);
  addOutPort("currentPose", m_currentPoseOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("axleTrack", m_axleTrack, "0.331");
  bindParameter("wheelRadius", m_wheelRadius, "0.09525");

  bindParameter("objectName", m_objectName, "none");
  //bindParameter("objectHandle", m_objectHandle, "-1");
  bindParameter("rightWheelJointName", m_rightWheelJointName, "");
  bindParameter("leftWheelJointName", m_leftWheelJointName, "");

  // </rtc-template>

  m_updatePoseIn.addConnectorDataListener(ON_BUFFER_WRITE,
	  new UpdatePoseListener(this));

  RTC_DEBUG(("[simExtRTC.TwoWheelMobileRobotRTC] Initializing Robot RTC(%s)", m_properties.getProperty("conf.default.objectName")));
  std::vector<std::string> keys;
  std::vector<int32_t> values;

  std::string names = m_properties.getProperty("conf.__innerparam.allNames");
  RTC_DEBUG(("[simExtRTC.TwoWheelMobileRobotRTC]  - All Joint Name is %s", names));
  std::stringstream nss(names);
  std::string token;
  while (std::getline(nss, token, ',')) {
	  std::stringstream trimmer;
	  trimmer << token;
	  token.clear();
	  trimmer >> token;
	  std::cout << " key = " << token << std::endl;
	  keys.push_back(token);
  }

  std::string handles = m_properties.getProperty("conf.__innerparam.allHandles");
  RTC_DEBUG(("[simExtRTC.TwoWheelMobileRobotRTC]  - All Joint Handle is %s", handles));
  std::stringstream hss(handles);
  int32_t buf;
  while (std::getline(hss, token, ',')) {
	  std::stringstream trimmer;
	  trimmer << token;
	  token.clear();
	  trimmer >> buf;
	  //std::cout << " value = " << buf << std::endl;
	  values.push_back(buf);
  }

  uint32_t size = keys.size();
  for (uint32_t i = 0; i < size; i++) {
	  m_jointHandleMap.append(keys[i], values[i]);
  }



  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t TwoWheelMobileRobotRTC::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TwoWheelMobileRobotRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TwoWheelMobileRobotRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t TwoWheelMobileRobotRTC::onActivated(RTC::UniqueId ec_id)
{

	RTC_DEBUG(("[simExtRTC.TwoWheelMobileRobotRTC] Activating RobotRTC(%s)", m_objectName));
	// td::cout << " - Activating RobotRTC(" << m_objectName << ")" << std::endl;
	m_controlledJointHandle.clear();
	m_observedJointHandle.clear();
	updateParameters("default");

	std::cout << " - rightWheel: " << m_rightWheelJointName << std::endl;
	std::cout << " - leftWheel: " << m_leftWheelJointName << std::endl;

	auto i = m_jointHandleMap.find(m_rightWheelJointName);
	if (i == m_jointHandleMap.end()) {
		RTC_ERROR(("ERROR: Can not find active joint name %s", m_rightWheelJointName.c_str()));
		return RTC::RTC_ERROR;
	}
	m_rightWheelJointHandle = i->second;

	i = m_jointHandleMap.find(m_leftWheelJointName);
	if (i == m_jointHandleMap.end()) {
		RTC_ERROR(("ERROR: Can not find active joint name %s", m_leftWheelJointName.c_str()));
		return RTC::RTC_ERROR;
	}
	m_leftWheelJointHandle = i->second;
	m_oldRightWheelAngle = m_oldLeftWheelAngle = 0;

	x = y = theta = 0;
	return RTC::RTC_OK;
}


RTC::ReturnCode_t TwoWheelMobileRobotRTC::onDeactivated(RTC::UniqueId ec_id)
{
	RTC_DEBUG(("[simExtRTC.TwoWheelMobileRobotRTC] Deactivating TwoWheelMobileRobotRTC(%s)", m_objectName));
	return RTC::RTC_OK;
}



RTC::ReturnCode_t TwoWheelMobileRobotRTC::onExecute(RTC::UniqueId ec_id)
{
	const float dt = simGetSimulationTimeStep();
	const float time = simGetSimulationTime();
	const long sec = floor(time);
	const long nsec = (time - sec) * 1000 * 1000 * 1000;
	RTC::Time tm;
	tm.sec = sec;
	tm.nsec = nsec;

	const double axleTrack = getAxleTrack();
	const double wheelRadius = getWheelRadius();

	if (m_targetVelocityIn.isNew()) {
		m_targetVelocityIn.read();

		const double v_buf = m_targetVelocity.data.va * axleTrack / 2;
		const double vr = m_targetVelocity.data.vx + v_buf;
		const double vl = m_targetVelocity.data.vx - v_buf;

		const double omegaR = vr / wheelRadius;
		const double omegaL = vl / wheelRadius;

		simSetJointTargetVelocity(m_rightWheelJointHandle, omegaR);
		simSetJointTargetVelocity(m_leftWheelJointHandle, omegaL);
	}

	float rightWheelAngle, leftWheelAngle;
	if (simGetJointPosition(m_rightWheelJointHandle, &rightWheelAngle) < 0) {
		RTC_DEBUG(("[simExtRTC.RobotRTC(%s)] onExecute(): GetJointPosition failed.", m_objectName));
		return RTC::RTC_ERROR;
	}
	if (simGetJointPosition(m_leftWheelJointHandle, &leftWheelAngle) < 0) {
		RTC_DEBUG(("[simExtRTC.RobotRTC(%s)] onExecute(): GetJointPosition failed.", m_objectName));
		return RTC::RTC_ERROR;
	}

	const double x = getX();
	const double y = getY();
	const double theta = getTheta();

	double deltaR = rightWheelAngle - m_oldRightWheelAngle;
	if (deltaR > M_PI) { deltaR -= 2 * M_PI; }
	else if (deltaR < -M_PI) { deltaR += 2 * M_PI; }
	double deltaL = leftWheelAngle - m_oldLeftWheelAngle;
	if (deltaL > M_PI) { deltaL -= 2 * M_PI; }
	else if (deltaL < -M_PI) { deltaL += 2 * M_PI; }

	double deltaTrans = (deltaR + deltaL) * wheelRadius / 2;
	double deltaTheta = (deltaR - deltaL) * wheelRadius / axleTrack;

	setX(x + deltaTrans * cos(theta + deltaTheta / 2));
	setY(y + deltaTrans * sin(theta + deltaTheta / 2));
	setTheta(theta + deltaTheta);

	m_oldRightWheelAngle = rightWheelAngle;
	m_oldLeftWheelAngle = leftWheelAngle;

	writeCurrentPose(tm);
	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t TwoWheelMobileRobotRTC::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TwoWheelMobileRobotRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t TwoWheelMobileRobotRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t TwoWheelMobileRobotRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t TwoWheelMobileRobotRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void TwoWheelMobileRobotRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(mobilerobotkinematics_spec);
    manager->registerFactory(profile,
                             RTC::Create<TwoWheelMobileRobotRTC>,
                             RTC::Delete<TwoWheelMobileRobotRTC>);
  }
  
};


