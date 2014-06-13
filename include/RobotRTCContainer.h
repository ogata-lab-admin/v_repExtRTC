#pragma once

#include "RobotRTC.h"
#include <string>
#include <vector>


/**
 * @brief Robot RTC holder class
 */
class RobotRTCHolder {
 private:
  std::string m_objectName;
  RTC::RTObject_ptr m_pRTC;
  OpenRTM::ExtTrigExecutionContextService_var m_pEC;
  RTC::ExecutionContextBase* m_pECBase;
  float m_Interval;
  float m_Counter;
 public:

  const std::string& getObjectName() {return m_objectName; }

 public:
 RobotRTCHolder() : m_pRTC(NULL) {}

  RobotRTCHolder(RTC::RTObject_ptr pRTC, const std::string& name, RTC::ExecutionContextBase* pEC=NULL);

  RobotRTCHolder(const RobotRTCHolder& h);

  void operator=(const RobotRTCHolder& h) {
    m_pRTC = RTC::RTObject::_duplicate(h.m_pRTC);
    m_pEC = OpenRTM::ExtTrigExecutionContextService::_duplicate(h.m_pEC);
    m_pECBase = h.m_pECBase;
    m_objectName = h.m_objectName;
    init();
  }

  ~RobotRTCHolder();

 private:
  void init();

 public:
  void start();

  void stop();

  void tick(const float interval);

  int kill();

};



/**
 * RobotRTC Container
 * @brief Robot RTC Holder Container Class.
 */
class RobotRTCContainer : public std::vector<RobotRTCHolder> {
 public:
  RobotRTCContainer() {}
  ~RobotRTCContainer() {}

 public:
  void push(RTObject_ptr pRTC, const std::string& objectName, RTC::ExecutionContextBase* pEC=NULL) {
    this->push_back(RobotRTCHolder(pRTC, objectName, pEC));
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

  int kill(const std::string& name) {
    for(RobotRTCContainer::iterator it = begin();it != end();++it) {
      if ((*it).getObjectName() == name) {
	if ((*it).kill() < 0) {
	  erase(it);
	  return -1;
	} else {
	  erase(it);
	  return 0;
	}
      }
    }
    return -1;
  }

  int killall() {
    std::cout << "container::killall" << std::endl;
    if (size() == 0) {
      std::cout << "size - zero." << std::endl;
      return 0;
    }
    for(RobotRTCContainer::iterator it = begin();it != end();++it) {
      if ((*it).kill() < 0) {
	erase(it);
	return -1;
      }
    }
    clear();
    std::cout << "cleared." << std::endl;
    return 0;
  }
};

extern RobotRTCContainer robotContainer;
