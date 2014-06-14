#pragma once

#include <string>
#include <queue>
#include <coil/Mutex.h>
#include <iostream>

class MutexBinder
{
 private:
  coil::Mutex &m_m;
 public:
 MutexBinder(coil::Mutex& m) : m_m(m) {m_m.lock();}
  ~MutexBinder() {m_m.unlock();}
};

class Task {
 public:
  enum {
    INVALID = -1,
    START = 0,
    STOP = 1,
    SPAWNROBOT = 2,
    SPAWNRANGE = 3,
    SPAWNCAMERA = 4,
    LOADPROJECT = 5,
    PAUSE = 6,
    KILLRTC = 7,
    KILLALLRTC = 8,
    SYNCRTC = 9,
    SETOBJPOSE = 10,
    GETOBJPOSE = 11,
    GETSYNCRTC = 12,
  };
  int value;
  std::string key;
  std::string arg;
 public:
 Task(): value(INVALID) {}

 Task(const int& v) : value(v) {}
 Task(const int& v, const std::string& k) : value(v), key(k) {}
 Task(const int& v, const std::string& k, const std::string& a) : value(v), key(k), arg(a) {}

  Task(const Task& t) {
    this->value = t.value;
    this->key = t.key;
    this->arg = t.arg;
  }

  void operator=(const Task& t) {
    this->value = t.value;
    this->key = t.key;
    this->arg = t.arg;
  }
};

class Return {
 public:
  int value;
 public:
  enum {
    RET_INVALID = -1,
    RET_OK = 0,
    RET_FAILED = -2,
    RET_ERROR = -3,
  };

  std::vector<std::string> stringList;
 public:

 Return() : value(RET_INVALID) {}

 Return(const int t) : value(t) {}
    

  Return(const Return& t) {
    this->value = t.value;
    std::copy(t.stringList.begin(), t.stringList.end(), back_inserter(this->stringList) );
  }

  void operator=(const Return& t) {
    this->value = t.value;
    std::copy(t.stringList.begin(), t.stringList.end(), back_inserter(this->stringList) );
  }

  
};


class TaskQueue : public std::queue<Task> 
{
 private:
  coil::Mutex m_m;
 public:
  TaskQueue() {}
  ~TaskQueue() {}

 public:
  void pushTask(const Task& t) {
    MutexBinder b(m_m);
    this->push(t);
  }

  Task popTask() {
    Task t;
    MutexBinder b(m_m);
    if(!this->empty()) {
      t = this->front();
      this->pop();
    }
    return t;
  }
};


class ReturnQueue : public std::queue<Return>
{
 private:
  coil::Mutex m_m;
 public:
  ReturnQueue() {}
  ~ReturnQueue() {}

 public:
  void returnReturn(const Return& r) {
    MutexBinder b(m_m);
    this->push(r);
  }

  Return waitReturn() {
    while(1) {
      MutexBinder b(m_m);
      if (!this->empty()) {
	Return r = this->front();
	this->pop();
	return r;
      }
    }
    return Return(Return::RET_INVALID);
  }
};


extern TaskQueue taskQueue;
extern ReturnQueue returnQueue;

