// -*- C++ -*-
/*!
 * @file  VREPRTC.cpp
 * @brief VREP Dynamic Simulator Interface RTC
 * @date $Date$
 *
 * $Id$
 */

#include "VREPRTC.h"

// Module specification
// <rtc-template block="module_spec">
static const char* vreprtc_spec[] =
  {
    "implementation_id", "VREPRTC",
    "type_name",         "VREPRTC",
    "description",       "VREP Dynamic Simulator Interface RTC",
    "version",           "1.0.0",
    "vendor",            "ysuga.net",
    "category",          "Simulator",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.debug", "0",
    // Widget
    "conf.__widget__.debug", "text",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
VREPRTC::VREPRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_simulatorPortPort("simulatorPort")

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
VREPRTC::~VREPRTC()
{
}



RTC::ReturnCode_t VREPRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  
  // Set OutPort buffer
  
  // Set service provider to Ports
  m_simulatorPortPort.registerProvider("Simulator", "ssr::Simulator", m_simulator);
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  addPort(m_simulatorPortPort);
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("debug", m_debug, "0");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t VREPRTC::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t VREPRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t VREPRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t VREPRTC::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t VREPRTC::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t VREPRTC::onExecute(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t VREPRTC::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t VREPRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t VREPRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t VREPRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t VREPRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void VREPRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(vreprtc_spec);
    manager->registerFactory(profile,
                             RTC::Create<VREPRTC>,
                             RTC::Delete<VREPRTC>);
  }
  
};


