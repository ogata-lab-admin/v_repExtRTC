#include <iostream>

#include "RobotRTCContainer.h"
RobotRTCContainer robotContainer;

RobotRTCHolder::RobotRTCHolder(RTC::RTObject_ptr pRTC, const std::string& name, RTC::ExecutionContextBase* pEC /*=NULL*/) {
  std::cout << " - Registering RTC(" << name << ") in RTCHolder" << std::endl;
  m_pRTC = RTC::RTObject::_duplicate(pRTC);
  m_objectName = name;
  m_pECBase = pEC;
  if (pEC) {
    m_simulatorRTC = false;
    m_pEC = OpenRTM::ExtTrigExecutionContextService::_narrow(pEC->getObjRef());
    if (!m_pEC) {
      std::cout << " -- Given Extra Execution Context is not ExtTrigExecutionContext. Failed." << std::endl;
    }
  } else {
    m_simulatorRTC = true;
    std::cout << " -- ExtraEC is nil. Default EC[0] is used." << std::endl;
    RTC::ExecutionContext_ptr p = m_pRTC->get_context(0);
    m_pEC = OpenRTM::ExtTrigExecutionContextService::_narrow(p);
    if (!m_pEC) {
      std::cout << " -- Execution Context [0] is not ExtTrigExecutionContext. Failed." << std::endl;
    }
  }
  init();
}

RobotRTCHolder::RobotRTCHolder(const RobotRTCHolder& h) {
  m_pRTC = RTC::RTObject::_duplicate(h.m_pRTC);
  m_pEC = OpenRTM::ExtTrigExecutionContextService::_duplicate(h.m_pEC);
  m_pECBase = h.m_pECBase;
  m_objectName = h.m_objectName;
  m_simulatorRTC = h.m_simulatorRTC;
  init();
}

RobotRTCHolder::~RobotRTCHolder() {
  if (m_pRTC) {
    ::CORBA::release(m_pRTC);
  }
}

void RobotRTCHolder::init() {
    /**
    RTC::ExecutionContextList_var ecList = m_pRTC->get_contexts();
    for(int i = 0;i < ecList->length();i++) {
      OpenRTM::ExtTrigExecutionContextService p = 
	OpenRTM::ExtTrigExecutionContextService::_narrow(ecList[i]);
      if(p->is_nil()) {
	std::cout << "Context " << i << " is not exttrigec." << std::endl;
      } else {
	m_pEC = p;
      }
      }**/
  m_Interval = 1.0 / m_pEC->get_rate();
}

void RobotRTCHolder::start() {
  std::cout << " - Starting RobotRTC(" << m_objectName << ")" << std::endl;
  m_Interval = 1.0 / m_pEC->get_rate();
  m_Counter = 0;
  std::cout << " -- EC interval = " << m_Interval << std::endl;
  m_pEC->tick();
  RTC::ReturnCode_t r = m_pEC->activate_component(m_pRTC);
  if (r != RTC::RTC_OK) { 
    std::cout << " -- Failed." << std::endl;
    std::cout << " -- ActivatingComponent returns " << r << std::endl;
  } else {
    std::cout << " -- Succeeded." << std::endl;
  }

}


void RobotRTCHolder::stop() {
  std::cout << " - Stopping RobotRTC(" << m_objectName << ")" << std::endl;
  m_pEC->deactivate_component(m_pRTC);
  m_pEC->tick();
  m_Counter = 0;
}

void RobotRTCHolder::tick(const float interval) {
    m_Counter += interval;
    if (m_Counter >= m_Interval) {
      m_Counter = 0;
      m_pEC->tick();
    }
  }

int RobotRTCHolder::kill() {
  if (m_pRTC->exit() != RTC::RTC_OK) {
    return -1;
  } 
  return 0;
}

