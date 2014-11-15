// -*- C++ -*-
/*!
 * @file  GyroRTC.cpp
 * @brief Simulator Robot RTC for VREP simulator
 * @date 2014/06/05
 * @author Yuki Suga (ysuga@ysuga.net)
 * @copyright 2014, Ogata Laboratory, Waseda University
 */

#include "GyroRTC.h"
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include <v_repLib.h>
// Module specification
// <rtc-template block="module_spec">
static const char* gyrortc_spec[] =
  {
    "implementation_id", "GyroRTC",
    "type_name",         "GyroRTC",
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
    //"conf.default.maxGyro", "30.0",
    //"conf.default.minGyro", "0.3",
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
GyroRTC::GyroRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_gyroOut("gyro", m_gyro)
    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
GyroRTC::~GyroRTC()
{
}



RTC::ReturnCode_t GyroRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers

  // Set OutPort buffer
  addOutPort("gyro", m_gyroOut);

  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("objectName", m_objectName, "none");
  //bindParameter("geometry_offset", m_offsetStr, "0,0,0,0,0,0");
  //bindParameter("maxGyro", m_maxGyro, "30.0");
  //bindParameter("minGyro", m_minGyro, "0.3");
  // </rtc-template>


  std::cout << " - Initializing GyroRTC: " << m_properties.getProperty("conf.default.objectName") << std::endl;

  std::string tubehandle = m_properties.getProperty("conf.__innerparam.tubeHandle");
  std::istringstream iss0(tubehandle);
  iss0 >> m_tubeHandle;
  std::cout << " -- TubeHandle = " << m_tubeHandle << std::endl;
  std::string objhandle = m_properties.getProperty("conf.__innerparam.objectHandle");
  std::istringstream iss1(objhandle);
  iss1 >> m_objectHandle;
  std::string buflength = m_properties.getProperty("conf.__innerparam.bufSize");
  std::istringstream iss2(buflength);
  iss2 >> m_bufferSize;
  m_pBuffer = new uint8_t[m_bufferSize];
  
  return RTC::RTC_OK;
}

 
RTC::ReturnCode_t GyroRTC::onFinalize()
{
  delete[] m_pBuffer;
  m_pBuffer = NULL;
  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t GyroRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t GyroRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t GyroRTC::onActivated(RTC::UniqueId ec_id)
{
  std::cout << " - Activating GyroRTC: " << m_objectName << std::endl;
  return RTC::RTC_OK;
}

RTC::ReturnCode_t GyroRTC::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


union float_byte {
  float float_value;
  unsigned char byte_value[4];
};


RTC::ReturnCode_t GyroRTC::onExecute(RTC::UniqueId ec_id)
{
  simInt bufSize;
  simChar* pBuffer = simTubeRead(m_tubeHandle, &bufSize);
  if (pBuffer == NULL) {
    return RTC::RTC_OK;
  }
  float_byte buffer;
  int data_size = bufSize / 4;
  if (data_size != 3) {
    std::cout << "[GyroRTC] Invalid data size (" << data_size << "!=3)" << std::endl;
    return RTC::RTC_OK;
  }

  float time = simGetSimulationTime();
  long sec = floor(time);
  long nsec = (time - sec) * 1000*1000*1000;
  m_gyro.tm.sec = sec;
  m_gyro.tm.nsec = nsec;

  float x[3];
  for(int i = 0;i < 3;i++) {
    for(int j = 0;j < 4;j++) {
      buffer.byte_value[j] = pBuffer[i*4+j];
    }
    x[i] = buffer.float_value;
  }
  m_gyro.data.avx = x[0];
  m_gyro.data.avy = x[1];
  m_gyro.data.avz = x[2];
  m_gyroOut.write();
  simReleaseBuffer((simChar*)pBuffer);
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t GyroRTC::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t GyroRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t GyroRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t GyroRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t GyroRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void GyroRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(gyrortc_spec);
    manager->registerFactory(profile,
                             RTC::Create<GyroRTC>,
                             RTC::Delete<GyroRTC>);
  }
  
};


