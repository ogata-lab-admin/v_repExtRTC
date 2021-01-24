#pragma once

#include "RobotRTC.h"
#include <string>
#include <iostream>
#include <vector>

namespace simExtRTC {
	/**
	 * @brief Robot RTC holder class
	 */
	class RobotRTCHolder {
	private:
		bool m_simulatorRTC;
		std::string m_objectName;
		RTC::RTObject_ptr m_pRTC;
		OpenRTM::ExtTrigExecutionContextService_var m_pEC;
		RTC::ExecutionContextBase* m_pECBase;
		float m_Interval;
		float m_Counter;
	public:

		const std::string& getObjectName() { return m_objectName; }

		bool isSimulatorRTC() { return m_simulatorRTC; }

	public:
		RobotRTCHolder() : m_simulatorRTC(true), m_pRTC(NULL) {}

		RobotRTCHolder(RTC::RTObject_ptr pRTC, const std::string& name, RTC::ExecutionContextBase* pEC = NULL);

		RobotRTCHolder(const RobotRTCHolder& h);

		void operator=(const RobotRTCHolder& h) {
			m_pRTC = RTC::RTObject::_duplicate(h.m_pRTC);
			m_simulatorRTC = h.m_simulatorRTC;
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
		bool push(RTObject_ptr pRTC, const std::string& objectName, RTC::ExecutionContextBase* pEC = NULL) {
			RobotRTCContainer::iterator it = this->begin();
			for (; it != end(); ++it) {
				if ((*it).getObjectName() == objectName) {
					return false;
				}
			}
			this->push_back(RobotRTCHolder(pRTC, objectName, pEC));
			return true;
		}

		void start() {
			for (RobotRTCContainer::iterator it = begin(); it != end(); ++it) {
				(*it).start();
			}
		}

		void stop() {
			for (RobotRTCContainer::iterator it = begin(); it != end(); ++it) {
				(*it).stop();
			}
		}

		void tick(const float interval) {
			for (RobotRTCContainer::iterator it = begin(); it != end(); ++it) {
				(*it).tick(interval);
			}
		}

		int kill(const std::string& name) {
			std::cout << "[simExtRTC] RobotRTCContainer::killRTC(" << name << ") called." << std::endl;
			for (RobotRTCContainer::iterator it = begin(); it != end(); ++it) {
				if ((*it).getObjectName() == name) {
					std::cout << "[simExtRTC] killing RTC(" << name << ")" << std::endl;
					if ((*it).kill() < 0) {
						std::cout << "[simExtRTC] killing RTC(" << name << ") failed." << std::endl;
						erase(it);
						return -1;
					}
					else {
						std::cout << "[simExtRTC] killing RTC(" << name << ") succeeded." << std::endl;
						erase(it);
						return 0;
					}
				}
			}
			std::cout << "[simExtRTC] killing RTC(" << name << ") failed. RTC not found." << std::endl;
			return -1;
		}

		int killall() {
			std::cout << "[simExtRTC] RobotRTCContainer::killall() called." << std::endl;
			for (RobotRTCContainer::iterator it = begin(); it != end(); ++it) {
				if ((*it).kill() < 0) {
				}
			}
			clear();
			std::cout << "[simExtRTC] RobotRTCContainer::killall() exit." << std::endl;
			return 0;
		}
	};

}

extern simExtRTC::RobotRTCContainer robotContainer;
