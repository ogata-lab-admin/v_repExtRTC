// -*- C++ -*-
/*!
 * @file  RangeRTC.cpp
 * @brief Simulator Robot RTC for VREP simulator
 * @date 2014/06/05
 * @author Yuki Suga (ysuga@ysuga.net)
 * @copyright 2014, Ogata Laboratory, Waseda University
 */

#include "RangeRTC.h"
#include <string>
#include <sstream>
#include <iostream>
#include <math.h>
#include <v_repLib.h>
// Module specification
// <rtc-template block="module_spec">
static const char* rangertc_spec[] =
  {
    "implementation_id", "RangeRTC",
    "type_name",         "RangeRTC",
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
    "conf.default.geometry_offset", "0,0,0,0,0,0",
	"conf.default.maxRange", "30.0",
	"conf.default.minRange", "0.3",
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
RangeRTC::RangeRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_rangeOut("range", m_range)
    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RangeRTC::~RangeRTC()
{
}



RTC::ReturnCode_t RangeRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers

  // Set OutPort buffer
  addOutPort("range", m_rangeOut);

  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("objectName", m_objectName, "none");
  bindParameter("geometry_offset", m_offsetStr, "0,0,0,0,0,0");
  bindParameter("maxRange", m_maxRange, "30.0");
  bindParameter("minRange", m_minRange, "0.3");
  // </rtc-template>


  std::cout << " - Initializing RangeRTC: " << m_properties.getProperty("conf.default.objectName") << std::endl;

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

 
RTC::ReturnCode_t RangeRTC::onFinalize()
{
  delete[] m_pBuffer;
  m_pBuffer = NULL;
  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t RangeRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RangeRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RangeRTC::onActivated(RTC::UniqueId ec_id)
{
  std::cout << " - Activating RangeRTC: " << m_objectName << std::endl;
  std::string names = m_offsetStr;
  std::cout << " -- geometry_offset:" << names << std::endl;
  std::stringstream nss(names);
  std::string token;
  float value;
  std::vector<float> values;
  while(std::getline(nss, token, ',')) {
    std::stringstream trimmer;
    trimmer << token;
    token.clear();
    trimmer >> value;
    values.push_back(value);
  }
  if (values.size() != 6) {
    std::cout << " -- Invalid Value of the String Configuration: geometry_offset." << std::endl;
    std::cout << " -- geometry_offset must be \"x, y, z, r, p, y\"" << std::endl;
    std::cout << " -- ex., geometry_offset=2.0, 0.0, 1.0, 0.0, 0.0, 3.14159" << std::endl;
    return RTC::RTC_ERROR;
  }

  m_range.geometry.geometry.pose.position.x = values[0];
  m_range.geometry.geometry.pose.position.y = values[1];
  m_range.geometry.geometry.pose.position.z = values[2];
  m_range.geometry.geometry.pose.orientation.r = values[3];
  m_range.geometry.geometry.pose.orientation.p = values[4];
  m_range.geometry.geometry.pose.orientation.y = values[5];
  m_range.config.maxRange = m_maxRange;
  m_range.config.minRange = m_minRange;
  m_range.config.maxAngle = 0.0;
  m_range.config.minAngle = 0.0;
  return RTC::RTC_OK;
}


bool RangeRTC::isInitRangeConfig() { 
  if(m_range.config.maxAngle == m_range.config.minAngle) {
    return false;
  } 
  return true;
}


RTC::ReturnCode_t RangeRTC::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


union float_byte {
  float float_value;
  unsigned char byte_value[4];
};

RTC::ReturnCode_t RangeRTC::onExecute(RTC::UniqueId ec_id)
{
  simInt bufSize;
  simChar* pBuffer = simTubeRead(m_tubeHandle, &bufSize);
  //std::cout << "ret == " << bufSize << std::endl;
  if (pBuffer == NULL) {
    // std::cout << " - RangeData: NULL. But this is not error." << std::endl;
    return RTC::RTC_OK;
  }
  float_byte buffer;
  int data_size = bufSize / 4;
  int ray_size = data_size / 3;

  float time = simGetSimulationTime();
  long sec = floor(time);
  long nsec = (time - sec) * 1000*1000*1000;
  m_range.tm.sec = sec;
  m_range.tm.nsec = nsec;
  if(ray_size != m_range.ranges.length()) {
    m_range.ranges.length(ray_size);
  }

  bool previous_point_is_valid = false;
  double previous_angle = 0.0;
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

    const double epsilon = 0.001;
    bool current_point_is_valid = distance < epsilon ? false : true;

	if (distance > m_range.config.maxRange) {
		m_range.ranges[i] = m_range.config.maxRange;
	} else {
	    m_range.ranges[i] = distance;
	}
    //m_range.ranges[i] = distance;

    if (!isInitRangeConfig()) {
      if (previous_point_is_valid && current_point_is_valid) {
	m_range.config.angularRes = angle - previous_angle;
      }
      previous_angle = angle;
      previous_point_is_valid = current_point_is_valid;
    }

  }
  ///m_range.config.angularRes = (m_range.config.maxAngle - m_range.config.minAngle) / (ray_size-1);

  if (!isInitRangeConfig()) {
    double full_range = m_range.config.angularRes * (ray_size-1);
    m_range.config.minAngle = -full_range/2;
    m_range.config.maxAngle = full_range/2;
  }
  m_rangeOut.write();
  simReleaseBuffer((simChar*)pBuffer);
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RangeRTC::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RangeRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RangeRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RangeRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RangeRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void RangeRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(rangertc_spec);
    manager->registerFactory(profile,
                             RTC::Create<RangeRTC>,
                             RTC::Delete<RangeRTC>);
  }
  
};


