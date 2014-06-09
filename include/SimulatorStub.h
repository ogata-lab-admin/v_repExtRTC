// -*- C++ -*-
/*!
 *
 * THIS FILE IS GENERATED AUTOMATICALLY!! DO NOT EDIT!!
 *
 * @file SimulatorStub.h 
 * @brief Simulator client stub header wrapper code
 * @date Fri Jun  6 16:44:30 2014 
 *
 */

#ifndef _SIMULATORSTUB_H
#define _SIMULATORSTUB_H



#include <rtm/config_rtc.h>
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#if   defined ORB_IS_TAO
#  include "SimulatorC.h"
#elif defined ORB_IS_OMNIORB
#  if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#    undef USE_stub_in_nt_dll
#  endif
#  include "Simulator.hh"
#elif defined ORB_IS_MICO
#  include "Simulator.h"
#elif defined ORB_IS_ORBIT2
#  include "Simulator-cpp-stubs.h"
#elif defined ORB_IS_RTORB
#  include "Simulator.h"
#else
#  error "NO ORB defined"
#endif

#endif // _SIMULATORSTUB_H
