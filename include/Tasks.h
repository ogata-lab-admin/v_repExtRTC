#pragma once

#include <string>
#include <queue>
#include <coil/Mutex.h>


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
  };
  int value;
  std::string key;
 public:
 Task(): value(INVALID) {}

 Task(const int& v) : value(v) {}

  Task(const Task& t) {
    this->value = t.value;
    this->key = t.key;
  }

  void operator=(const Task& t) {
    this->value = t.value;
    this->key = t.key;
  }
};

class Return {
 public:
  int value;
 public:
  enum {
    INVALID = -1,
    OK = 0,
    FAILED = -2,
    ERROR = -3,
  };
 Return() : value(INVALID) {}

 Return(const int t) : value(t) {}
    

  Return(const Return& t) {
    this->value = t.value;
  }

  void operator=(const Return& t) {
    this->value = t.value;
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
    return Return(Return::INVALID);
  }
};


extern TaskQueue taskQueue;
extern ReturnQueue returnQueue;

