// -*- C++ -*-
/*!
 *
 * THIS FILE IS GENERATED AUTOMATICALLY!! DO NOT EDIT!!
 *
 * @file SimulatorSkel.cpp 
 * @brief Simulator server skeleton wrapper
 * @date Wed Jun  4 09:30:09 2014 
 *
 */

#include "SimulatorSkel.h"

#if defined ORB_IS_TAO
#  include "SimulatorC.cpp"
#  include "SimulatorS.cpp"
#elif defined ORB_IS_OMNIORB
#  include "SimulatorSK.cc"
#  include "SimulatorDynSK.cc"
#elif defined ORB_IS_MICO
#  include "Simulator.cc"
#  include "Simulator_skel.cc"
#elif defined ORB_IS_ORBIT2
#  include "Simulator-cpp-stubs.cc"
#  include "Simulator-cpp-skels.cc"
#elif defined ORB_IS_RTORB
#  include "OpenRTM-aist-decls.h"
#  include "Simulator-common.c"
#  include "Simulator-stubs.c"
#  include "Simulator-skels.c"
#  include "Simulator-skelimpl.c"
#else
#  error "NO ORB defined"
#endif

// end of SimulatorSkel.cpp
