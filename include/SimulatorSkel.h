// -*- C++ -*-
/*!
 *
 * THIS FILE IS GENERATED AUTOMATICALLY!! DO NOT EDIT!!
 *
 * @file SimulatorSkel.h 
 * @brief Simulator server skeleton header wrapper code
 * @date Mon Jun  9 16:25:48 2014 
 *
 */

#ifndef _SIMULATORSKEL_H
#define _SIMULATORSKEL_H



#include <rtm/config_rtc.h>
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION

#if   defined ORB_IS_TAO
#  include "SimulatorC.h"
#  include "SimulatorS.h"
#elif defined ORB_IS_OMNIORB
#  if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#    undef USE_stub_in_nt_dll
#  endif
#  include "Simulator.hh"
#elif defined ORB_IS_MICO
#  include "Simulator.h"
#elif defined ORB_IS_ORBIT2
#  include "/Simulator-cpp-stubs.h"
#  include "/Simulator-cpp-skels.h"
#elif defined ORB_IS_RTORB
#  include "Simulator.h"
#else
#  error "NO ORB defined"
#endif

#endif // _SIMULATORSKEL_H
