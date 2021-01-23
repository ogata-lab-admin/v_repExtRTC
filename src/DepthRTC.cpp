// -*- C++ -*-
/*!
 * @file  DepthRTC.cpp
 * @brief Simulator Robot RTC for VREP simulator
 * @date 2014/06/05
 * @author Yuki Suga (ysuga@ysuga.net)
 * @copyright 2014, Ogata Laboratory, Waseda University
 */

#include "DepthRTC.h"
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include "simLib.h"
// Module specification
// <rtc-template block="module_spec">
static const char* depthrtc_spec[] =
  {
    "implementation_id", "DepthRTC",
    "type_name",         "DepthRTC",
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
    //"conf.default.offset", "0,0,0,0,0,0",
    //    "conf.default.objectHandle", "-1",
    "conf.default.angularResolution", "0.9948", 
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
DepthRTC::DepthRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_pointCloudOut("pointCloud", m_pointCloud)
    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
DepthRTC::~DepthRTC()
{
}



RTC::ReturnCode_t DepthRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers

  // Set OutPort buffer
  addOutPort("pointCloud", m_pointCloudOut);

  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("objectName", m_objectName, "none");
  //bindParameter("geometry_offset", m_offsetStr, "0,0,0,0,0,0");
  // </rtc-template>
  bindParameter("angularResolution", m_angularResolution, "0.9948");
  std::cout << " - Initializing Depth: " << m_properties.getProperty("conf.default.objectName") <<  std::endl;

  std::string objhandle = m_properties.getProperty("conf.__innerparam.objectHandle");
  std::istringstream iss1(objhandle);
  iss1 >> m_objectHandle;
  return RTC::RTC_OK;
}

 
RTC::ReturnCode_t DepthRTC::onFinalize()
{
  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t DepthRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DepthRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t DepthRTC::onActivated(RTC::UniqueId ec_id)
{
  std::cout << " - Activating DepthRTC: " << m_objectName << std::endl;
  simInt resolution[2]; // x, y
  if (simGetVisionSensorResolution(m_objectHandle, resolution) < 0) {
    std::cout << " -- Resolution Request Failed." << std::endl;
    return RTC::RTC_ERROR;
  }

  std::cout << " -- Depth Resolution Width = " << resolution[0] << std::endl;
  std::cout << " -- Depth Resolution Height= " << resolution[1] << std::endl;
  m_width = resolution[0];
  m_height = resolution[1];
  m_pointCloud.points.length(resolution[0] * resolution[1]);
  return RTC::RTC_OK;
}


RTC::ReturnCode_t DepthRTC::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

RTC::ReturnCode_t DepthRTC::onExecute(RTC::UniqueId ec_id)
{
  float* auxValues=NULL;
  int* auxValuesCount=NULL;
  float minDepth, maxDepth;
  if (simCheckVisionSensor(m_objectHandle, sim_handle_all, &auxValues,&auxValuesCount)>=0) {
    if ((auxValuesCount[0]>0)||(auxValuesCount[1]>=15)) {
      minDepth = auxValues[4];
      maxDepth = auxValues[9];
    }
    simReleaseBuffer((char*)auxValues);
    simReleaseBuffer((char*)auxValuesCount);
  }

  simFloat* pBuffer = simGetVisionSensorDepthBuffer(m_objectHandle);
  if (pBuffer == NULL) {
    std::cout << " -- ERROR, DepthRTC::No image received, but no error" << std::endl;
    return RTC::RTC_OK;
  }

  simFloat* pImgBuffer = simGetVisionSensorImage(m_objectHandle);
  if (pImgBuffer == NULL) {
    std::cout << " -- ERROR, DepthRTC::No image received, but no error" << std::endl;
    simReleaseBuffer((simChar*)pBuffer);
    return RTC::RTC_OK;
  }

  
  float time = simGetSimulationTime();
  long sec = floor(time);
  long nsec = (time - sec) * 1000*1000*1000;
  m_pointCloud.tm.sec = sec;
  m_pointCloud.tm.nsec = nsec;

  for (int i = 0;i < m_height;i++) {
    for (int j = 0;j < m_width;j++) {
      int index = i*m_width + j; 
      double vertical_angle = (i - m_height/2) * m_angularResolution;
      double horizontal_angle = (j - m_width/2) * m_angularResolution;
      float depth = pBuffer[index];
      double sinV = sin(vertical_angle);
      double cosV = cos(vertical_angle);
      double sinH = sin(horizontal_angle);
      double cosH = cos(horizontal_angle);
      m_pointCloud.points[index].point.x = depth * cosV * cosH;
      m_pointCloud.points[index].point.y = depth * cosV * sinH;
      m_pointCloud.points[index].point.z = depth * sinV;

      int img_index = i*m_width*3 + j*3; 
      m_pointCloud.points[index].colour.r = static_cast<unsigned char>(pBuffer[img_index+2] * 255);
      m_pointCloud.points[index].colour.g = static_cast<unsigned char>(pBuffer[img_index+1] * 255);
      m_pointCloud.points[index].colour.b = static_cast<unsigned char>(pBuffer[img_index+0] * 255);
    }
  }
  simReleaseBuffer((simChar*)pBuffer);
  simReleaseBuffer((simChar*)pImgBuffer);
  m_pointCloudOut.write();

  
  
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
  m_depth.tm.sec = sec;
  m_depth.tm.nsec = nsec;
  if(ray_size != m_depth.depths.length()) {
    m_depth.depths.length(ray_size);
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
      m_depth.config.minAngle = angle;
    } else if (i == ray_size-1) {
      m_depth.config.maxAngle = angle;
      m_depth.config.angularRes = (m_depth.config.maxAngle - m_depth.config.minAngle) / (ray_size-1);
    }
    m_depth.depths[i] = distance;
  }

  m_depthOut.write();
  */
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t DepthRTC::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DepthRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DepthRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DepthRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t DepthRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void DepthRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(depthrtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<DepthRTC>,
                             RTC::Delete<DepthRTC>);
  }
  
};


