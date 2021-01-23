// -*- C++ -*-
/*!
 * @file  AccelerometerRTC.cpp
 * @brief Simulator Robot RTC for VREP simulator
 * @date 2014/06/05
 * @author Yuki Suga (ysuga@ysuga.net)
 * @copyright 2014, Ogata Laboratory, Waseda University
 */

#include "AccelerometerRTC.h"
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include "simLib.h"
// Module specification
// <rtc-template block="module_spec">
static const char* accelerometerrtc_spec[] =
  {
    "implementation_id", "AccelerometerRTC",
    "type_name",         "AccelerometerRTC",
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
    //"conf.default.maxAccelerometer", "30.0",
    //"conf.default.minAccelerometer", "0.3",
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
AccelerometerRTC::AccelerometerRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_accelOut("accel", m_accel)
    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
AccelerometerRTC::~AccelerometerRTC()
{
}



RTC::ReturnCode_t AccelerometerRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers

  // Set OutPort buffer
  addOutPort("accel", m_accelOut);

  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("objectName", m_objectName, "none");
  //bindParameter("geometry_offset", m_offsetStr, "0,0,0,0,0,0");
  //bindParameter("maxAccelerometer", m_maxAccelerometer, "30.0");
  //bindParameter("minAccelerometer", m_minAccelerometer, "0.3");
  // </rtc-template>


  std::cout << " - Initializing AccelerometerRTC: " << m_properties.getProperty("conf.default.objectName") << std::endl;

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

 
RTC::ReturnCode_t AccelerometerRTC::onFinalize()
{
  delete[] m_pBuffer;
  m_pBuffer = NULL;
  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t AccelerometerRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AccelerometerRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t AccelerometerRTC::onActivated(RTC::UniqueId ec_id)
{
  std::cout << " - Activating AccelerometerRTC: " << m_objectName << std::endl;
  return RTC::RTC_OK;
}

RTC::ReturnCode_t AccelerometerRTC::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


union float_byte {
  float float_value;
  unsigned char byte_value[4];
};


RTC::ReturnCode_t AccelerometerRTC::onExecute(RTC::UniqueId ec_id)
{
  simInt bufSize;
  simChar* pBuffer = simTubeRead(m_tubeHandle, &bufSize);
  if (pBuffer == NULL) {
    return RTC::RTC_OK;
  }
  float_byte buffer;
  int data_size = bufSize / 4;
  if (data_size != 3) {
    std::cout << "[AccelerometerRTC] Invalid data size (" << data_size << "!=3)" << std::endl;
    return RTC::RTC_OK;
  }

  float time = simGetSimulationTime();
  long sec = floor(time);
  long nsec = (time - sec) * 1000*1000*1000;
  m_accel.tm.sec = sec;
  m_accel.tm.nsec = nsec;

  float x[3];
  for(int i = 0;i < 3;i++) {
    for(int j = 0;j < 4;j++) {
      buffer.byte_value[j] = pBuffer[i*4+j];
    }
    x[i] = buffer.float_value;
  }
  m_accel.data.ax = x[0];
  m_accel.data.ay = x[1];
  m_accel.data.az = x[2];
  m_accelOut.write();
  simReleaseBuffer((simChar*)pBuffer);
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t AccelerometerRTC::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AccelerometerRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AccelerometerRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AccelerometerRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t AccelerometerRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void AccelerometerRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(accelerometerrtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<AccelerometerRTC>,
                             RTC::Delete<AccelerometerRTC>);
  }
  
};


