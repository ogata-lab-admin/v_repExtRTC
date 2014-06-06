#pragma once

#include "RobotRTC.h"
#include <vector>

class RobotRTCHolder {
 private:
  RTC::RTObject_ptr m_pRTC;
  OpenRTM::ExtTrigExecutionContextService_var m_pEC;
  float m_Interval;
  float m_Counter;
 public:
 RobotRTCHolder() : m_pRTC(NULL) {}

  RobotRTCHolder(RTC::RTObject_ptr pRTC) {
    m_pRTC = RTC::RTObject::_duplicate(pRTC);
    init();
  }

  RobotRTCHolder(const RobotRTCHolder& h) {
    m_pRTC = RTC::RTObject::_duplicate(h.m_pRTC);
    init();
  }

  void operator=(const RobotRTCHolder& h) {
    m_pRTC = RTC::RTObject::_duplicate(h.m_pRTC);
    init();
  }

  ~RobotRTCHolder() {}
 private:
  void init() {
    RTC::ExecutionContext_ptr p = m_pRTC->get_context(0);
    m_Interval = 1.0 / p->get_rate();
  }

 public:
  void start() {
    RTC::ExecutionContext_ptr p = m_pRTC->get_context(0);
    m_Interval = 1.0 / p->get_rate();
    m_Counter = 0;
    m_pEC = OpenRTM::ExtTrigExecutionContextService::_narrow(p);
  }

  void stop() {
    m_Counter = 0;
  }

  void tick(const float interval) {
    m_Counter += interval;
    if (m_Counter > m_Interval) {
      m_Counter = 0;
      m_pEC->tick();
    }
  }
};

class RobotRTCContainer : public std::vector<RobotRTCHolder> {
 public:
  RobotRTCContainer() {}
  ~RobotRTCContainer() {}

 public:
  void push(RTObject_ptr pRTC) {
    this->push_back(RobotRTCHolder(pRTC));
  }

  void start() {
    for(RobotRTCContainer::iterator it = begin();it != end();++it) {
      (*it).start();
    }
  }

  void stop() {
    for(RobotRTCContainer::iterator it = begin();it != end();++it) {
      (*it).stop();
    }
  }

  void tick(const float interval) {
    for(RobotRTCContainer::iterator it = begin();it != end();++it) {
      (*it).tick(interval);
    }
  }
    
};

extern RobotRTCContainer robotContainer;
