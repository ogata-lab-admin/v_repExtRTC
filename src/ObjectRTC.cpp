// -*- C++ -*-
/*!
 * @file  ObjectRTC.cpp
 * @brief Simulator Robot RTC for VREP simulator
 * @date 2014/06/05
 * @author Yuki Suga (ysuga@ysuga.net)
 * @copyright 2014, Ogata Laboratory, Waseda University
 */

#include "ObjectRTC.h"
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include "simLib.h"
// Module specification
// <rtc-template block="module_spec">
static const char* objectrtc_spec[] =
  {
    "implementation_id", "ObjectRTC",
    "type_name",         "ObjectRTC",
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
    //"conf.default.geometry_offset", "0,0,0,0,0,0",
    //"conf.default.maxObject", "30.0",
    //"conf.default.minObject", "0.3",
    //    "conf.default.objectHandle", "-1",
    //"conf.default.activeJointNames", "[]",
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
ObjectRTC::ObjectRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_poseOut("pose", m_pose)
    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
ObjectRTC::~ObjectRTC()
{
}



RTC::ReturnCode_t ObjectRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers

  // Set OutPort buffer
  addOutPort("pose", m_poseOut);

  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("objectName", m_objectName, "none");
  //bindParameter("geometry_offset", m_offsetStr, "0,0,0,0,0,0");
  //bindParameter("maxObject", m_maxObject, "30.0");
  //bindParameter("minObject", m_minObject, "0.3");
  // </rtc-template>


  std::cout << " - Initializing ObjectRTC: " << m_properties.getProperty("conf.default.objectName") << std::endl;

  /*
  std::string tubehandle = m_properties.getProperty("conf.__innerparam.tubeHandle");
  std::istringstream iss0(tubehandle);
  iss0 >> m_tubeHandle;
  std::cout << " -- TubeHandle = " << m_tubeHandle << std::endl;
  */
  std::string objhandle = m_properties.getProperty("conf.__innerparam.objectHandle");
  std::istringstream iss1(objhandle);
  iss1 >> m_objectHandle;
  /*
  std::string buflength = m_properties.getProperty("conf.__innerparam.bufSize");
  std::istringstream iss2(buflength);
  iss2 >> m_bufferSize;
  m_pBuffer = new uint8_t[m_bufferSize];
  */
  return RTC::RTC_OK;
}

 
RTC::ReturnCode_t ObjectRTC::onFinalize()
{
  //delete[] m_pBuffer;
  //m_pBuffer = NULL;
  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t ObjectRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t ObjectRTC::onActivated(RTC::UniqueId ec_id)
{
  std::cout << " - Activating ObjectRTC: " << m_objectName << std::endl;
  return RTC::RTC_OK;
}

RTC::ReturnCode_t ObjectRTC::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


union float_byte {
  float float_value;
  unsigned char byte_value[4];
};


RTC::ReturnCode_t ObjectRTC::onExecute(RTC::UniqueId ec_id)
{
	simFloat position[3];
	simFloat orientation[3];

	::simGetObjectPosition(m_objectHandle, -1, position);
	::simGetObjectOrientation(m_objectHandle, -1, orientation);

	/*
	simInt bufSize;
	simChar* pBuffer = simTubeRead(m_tubeHandle, &bufSize);
  if (pBuffer == NULL) {
    return RTC::RTC_OK;
  }
  float_byte buffer;
  int data_size = bufSize / 4;
  if (data_size != 3) {
    std::cout << "[ObjectRTC] Invalid data size (" << data_size << "!=3)" << std::endl;
    return RTC::RTC_OK;
  }


  float x[3];
  for(int i = 0;i < 3;i++) {
    for(int j = 0;j < 4;j++) {
      buffer.byte_value[j] = pBuffer[i*4+j];
    }
    x[i] = buffer.float_value;
  }
  m_object.data.avx = x[0];
  m_object.data.avy = x[1];
  m_object.data.avz = x[2];
  m_objectOut.write();
  simReleaseBuffer((simChar*)pBuffer);
  */

	float time = simGetSimulationTime();
	long sec = floor(time);
	long nsec = (time - sec) * 1000 * 1000 * 1000;
	m_pose.tm.sec = sec;
	m_pose.tm.nsec = nsec;
	m_pose.data.position.x = position[0];
	m_pose.data.position.y = position[1];
	m_pose.data.position.z = position[2];
	m_pose.data.orientation.r = orientation[0];
	m_pose.data.orientation.p = orientation[1];
	m_pose.data.orientation.y = orientation[2];
	m_poseOut.write();
	return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t ObjectRTC::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t ObjectRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void ObjectRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(objectrtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<ObjectRTC>,
                             RTC::Delete<ObjectRTC>);
  }
  
};


