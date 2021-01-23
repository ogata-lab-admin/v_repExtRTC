// -*- C++ -*-
/*!
 * @file  CameraRTC.cpp
 * @brief Simulator Robot RTC for VREP simulator
 * @date 2014/06/05
 * @author Yuki Suga (ysuga@ysuga.net)
 * @copyright 2014, Ogata Laboratory, Waseda University
 */

#include "CameraRTC.h"
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include "simLib.h"
// Module specification
// <rtc-template block="module_spec">
static const char* camerartc_spec[] =
  {
    "implementation_id", "CameraRTC",
    "type_name",         "CameraRTC",
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
    "conf.default.offset", "0,0,0,0,0,0",
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
CameraRTC::CameraRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_imageOut("image", m_image)
    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
CameraRTC::~CameraRTC()
{
}



RTC::ReturnCode_t CameraRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers

  // Set OutPort buffer
  addOutPort("image", m_imageOut);

  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("objectName", m_objectName, "none");
  //bindParameter("geometry_offset", m_offsetStr, "0,0,0,0,0,0");
  // </rtc-template>
  
  std::cout << " - Initializing Camera: " << m_properties.getProperty("conf.default.objectName") <<  std::endl;

  std::string objhandle = m_properties.getProperty("conf.__innerparam.objectHandle");
  std::istringstream iss1(objhandle);
  iss1 >> m_objectHandle;
  return RTC::RTC_OK;
}

 
RTC::ReturnCode_t CameraRTC::onFinalize()
{
  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t CameraRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t CameraRTC::onActivated(RTC::UniqueId ec_id)
{
  std::cout << " - Activating CameraRTC: " << m_objectName << std::endl;
  simInt resolution[2]; // x, y
  if (simGetVisionSensorResolution(m_objectHandle, resolution) < 0) {
    std::cout << " -- Resolution Request Failed." << std::endl;
    return RTC::RTC_ERROR;
  }

  std::cout << " -- Camera Resolution Width = " << resolution[0] << std::endl;
  std::cout << " -- Camera Resolution Height= " << resolution[1] << std::endl;
  m_image.width = resolution[0];
  m_image.height = resolution[1];
  m_image.bpp = 8*3;
  m_image.format = "bitmap";
  m_image.fDiv = 1.0;
  m_image.pixels.length(m_image.width * m_image.height * 3);
  return RTC::RTC_OK;
}


RTC::ReturnCode_t CameraRTC::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

RTC::ReturnCode_t CameraRTC::onExecute(RTC::UniqueId ec_id)
{
  simFloat* pBuffer = simGetVisionSensorImage(m_objectHandle);
  if (pBuffer == NULL) {
    std::cout << " -- ERROR, CameraRTC::No image received, but no error" << std::endl;
    return RTC::RTC_OK;
  }

  
  float time = simGetSimulationTime();
  long sec = floor(time);
  long nsec = (time - sec) * 1000*1000*1000;
  m_image.tm.sec = sec;
  m_image.tm.nsec = nsec;

  for (int i = 0;i < m_image.height;i++) {
    for (int j = 0;j < m_image.width;j++) {
      int index = i*m_image.width*3 + j*3; 
      int buffer_index = (m_image.width * (m_image.height-i-1)) * 3 + j*3;
      m_image.pixels[buffer_index + 0] = static_cast<unsigned char>(pBuffer[index+2] * 255);
      m_image.pixels[buffer_index + 1] = static_cast<unsigned char>(pBuffer[index+1] * 255);
      m_image.pixels[buffer_index + 2] = static_cast<unsigned char>(pBuffer[index+0] * 255);
    }
  }
  simReleaseBuffer((simChar*)pBuffer);
  m_imageOut.write();

  
  
  /*
  simInt bufSize;
  simChar* pBuffer = simTubeRead(m_tubeHandle, &bufSize);
  //std::cout << "ret == " << bufSize << std::endl;
  if (pBuffer == NULL) {
    std::cout << "NULL!!" << std::endl;
    return RTC::RTC_OK;
  }
  float_byte buffer;
  int data_size = bufSize / 4;
  int ray_size = data_size / 3;

  float time =  simGetSimulationTime();
  long sec = floor(time);
  long nsec = (time - sec) * 1000*1000*1000;
  m_camera.tm.sec = sec;
  m_camera.tm.nsec = nsec;
  if(ray_size != m_camera.cameras.length()) {
    m_camera.cameras.length(ray_size);
  }

  for(int i = 0;i < ray_size;i++) {
    float x[3];
    for(int j = 0;j < 3;j++) {
      for(int k = 0;k < 4;k++) {
	buffer.byte_value[k] = pBuffer[i*4*3 + j*4 + k];
      }
      x[j] = buffer.float_value;
    }
    //std::cout << "data(" << i << ")=(" << x[0] << ", " << x[1] << ", " << x[2] << ")";
    
    double angle = atan2(x[1], x[0]);
    double distance = sqrt(x[1]*x[1] + x[0]*x[0]);
    //std::cout << " --- " << distance << "/rad=" << angle << ", deg=" << angle / 3.1415926 * 180.0 << std::endl;

    if (i == 0) { //start angle
      m_camera.config.minAngle = angle;
    } else if (i == ray_size-1) {
      m_camera.config.maxAngle = angle;
      m_camera.config.angularRes = (m_camera.config.maxAngle - m_camera.config.minAngle) / (ray_size-1);
    }
    m_camera.cameras[i] = distance;
  }

  m_cameraOut.write();
  */
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t CameraRTC::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t CameraRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void CameraRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(camerartc_spec);
    manager->registerFactory(profile,
                             RTC::Create<CameraRTC>,
                             RTC::Delete<CameraRTC>);
  }
  
};


