#pragma once 
#include <coil/Task.h>

#include "Tasks.h"
#include "RobotRTCContainer.h"

namespace simExtRTC {

	class SimulatorClock {
	private:
		float time;
		float timeStep;
	public:

		SimulatorClock() : time(0.0), timeStep(0.05) {}
		~SimulatorClock() {}

	public:
		float getSimulationTime() { return time; }
		void setSimulationTime(const float time) {
			this->time = time;
		}

		float getSimulationTimeStep() { return timeStep; }
		void setSimulationTimeStep(const float timeStep) {
			this->timeStep = timeStep;
		}
	};

	class ManagerRunner : public coil::Task {
	public:
		ManagerRunner() {
			open(0);
			endflag = false;
		}

		volatile bool endflag;
		virtual ~ManagerRunner() {
			//manager->shutdown();
			//manager->join();
			endflag = true;
		}

		virtual int open(void* arg) {
			std::cout << " - Activating ManagerRunner" << std::endl;
			activate();
			return 0;
		}

		virtual int svc();

		RTC::Manager* manager;
	};



	bool initRTM();
	bool exitRTM();
	int spawnRobotRTC(const std::string& key, const std::string& arg);
	int spawnTwoWheelMobileRobotRTC(const std::string& key, const std::string& arg);
	int spawnRangeRTC(const std::string& key, const std::string& arg);
	int spawnCameraRTC(const std::string& key, const std::string& arg);
	int spawnAccelerometerRTC(const std::string& key, const std::string& arg);
	int spawnGyroRTC(const std::string& key, const std::string& arg);
	int spawnDepthRTC(const std::string& key, const std::string& arg);
	int spawnObjectRTC(const std::string& key, const std::string& arg);
	void startRTCs();
	void stopRTCs();
	void tickRTCs(const float interval);
	int killRTC(const std::string& key);
	int killAllRTC();
	int syncRTC(const std::string& fullpath);
	int getSyncRTCs(std::vector<std::string>& strList);



}


extern simExtRTC::SimulatorClock simulatorClock;
