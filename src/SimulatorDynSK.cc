// This file is generated by omniidl (C++ backend) - omniORB_4_2. Do not edit.

#include "Simulator.hh"

OMNI_USING_NAMESPACE(omni)

static const char* _0RL_dyn_library_version = omniORB_4_2_dyn;

static ::CORBA::TypeCode::_Tracker _0RL_tcTrack(__FILE__);

static const char* _0RL_enumMember_ssr_mRETURN__VALUE[] = { "RETVAL_OK", "RETVAL_INVALID_PRECONDITION", "RETVAL_OBJECT_NOT_FOUND", "RETVAL_NOT_IMPLEMENTED", "RETVAL_UNKNOWN_ERROR" };
static CORBA::TypeCode_ptr _0RL_tc_ssr_mRETURN__VALUE = CORBA::TypeCode::PR_enum_tc("IDL:ssr/RETURN_VALUE:1.0", "RETURN_VALUE", _0RL_enumMember_ssr_mRETURN__VALUE, 5, &_0RL_tcTrack);
#if defined(HAS_Cplusplus_Namespace) && defined(_MSC_VER)
// MSVC++ does not give the constant external linkage otherwise.
namespace ssr { 
  const ::CORBA::TypeCode_ptr _tc_RETURN_VALUE = _0RL_tc_ssr_mRETURN__VALUE;
} 
#else
const ::CORBA::TypeCode_ptr ssr::_tc_RETURN_VALUE = _0RL_tc_ssr_mRETURN__VALUE;
#endif

static CORBA::TypeCode_ptr _0RL_tc_ssr_mOBJECT__HANDLE = CORBA::TypeCode::PR_alias_tc("IDL:ssr/OBJECT_HANDLE:1.0", "OBJECT_HANDLE", CORBA::TypeCode::PR_long_tc(), &_0RL_tcTrack);


#if defined(HAS_Cplusplus_Namespace) && defined(_MSC_VER)
// MSVC++ does not give the constant external linkage otherwise.
namespace ssr { 
  const ::CORBA::TypeCode_ptr _tc_OBJECT_HANDLE = _0RL_tc_ssr_mOBJECT__HANDLE;
} 
#else
const ::CORBA::TypeCode_ptr ssr::_tc_OBJECT_HANDLE = _0RL_tc_ssr_mOBJECT__HANDLE;
#endif

static CORBA::TypeCode_ptr _0RL_tc_ssr_mStringSeq = CORBA::TypeCode::PR_alias_tc("IDL:ssr/StringSeq:1.0", "StringSeq", CORBA::TypeCode::PR_sequence_tc(0, CORBA::TypeCode::PR_string_tc(0, &_0RL_tcTrack), &_0RL_tcTrack), &_0RL_tcTrack);


#if defined(HAS_Cplusplus_Namespace) && defined(_MSC_VER)
// MSVC++ does not give the constant external linkage otherwise.
namespace ssr { 
  const ::CORBA::TypeCode_ptr _tc_StringSeq = _0RL_tc_ssr_mStringSeq;
} 
#else
const ::CORBA::TypeCode_ptr ssr::_tc_StringSeq = _0RL_tc_ssr_mStringSeq;
#endif

#if defined(HAS_Cplusplus_Namespace) && defined(_MSC_VER)
// MSVC++ does not give the constant external linkage otherwise.
namespace ssr { 
  const ::CORBA::TypeCode_ptr _tc_Simulator = CORBA::TypeCode::PR_interface_tc("IDL:ssr/Simulator:1.0", "Simulator", &_0RL_tcTrack);
} 
#else
const ::CORBA::TypeCode_ptr ssr::_tc_Simulator = CORBA::TypeCode::PR_interface_tc("IDL:ssr/Simulator:1.0", "Simulator", &_0RL_tcTrack);
#endif

static void _0RL_ssr_mRETURN__VALUE_marshal_fn(cdrStream& _s, void* _v)
{
  ssr::RETURN_VALUE* _p = (ssr::RETURN_VALUE*)_v;
  *_p >>= _s;
}
static void _0RL_ssr_mRETURN__VALUE_unmarshal_fn(cdrStream& _s, void*& _v)
{
  ssr::RETURN_VALUE* _p = (ssr::RETURN_VALUE*)_v;
  *_p <<= _s;
}

void operator<<=(::CORBA::Any& _a, ssr::RETURN_VALUE _s)
{
  _a.PR_insert(_0RL_tc_ssr_mRETURN__VALUE,
               _0RL_ssr_mRETURN__VALUE_marshal_fn,
               &_s);
}

::CORBA::Boolean operator>>=(const ::CORBA::Any& _a, ssr::RETURN_VALUE& _s)
{
  return _a.PR_extract(_0RL_tc_ssr_mRETURN__VALUE,
                       _0RL_ssr_mRETURN__VALUE_unmarshal_fn,
                       &_s);
}

static void _0RL_ssr_mStringSeq_marshal_fn(cdrStream& _s, void* _v)
{
  ssr::StringSeq* _p = (ssr::StringSeq*)_v;
  *_p >>= _s;
}
static void _0RL_ssr_mStringSeq_unmarshal_fn(cdrStream& _s, void*& _v)
{
  ssr::StringSeq* _p = new ssr::StringSeq;
  *_p <<= _s;
  _v = _p;
}
static void _0RL_ssr_mStringSeq_destructor_fn(void* _v)
{
  ssr::StringSeq* _p = (ssr::StringSeq*)_v;
  delete _p;
}

void operator<<=(::CORBA::Any& _a, const ssr::StringSeq& _s)
{
  ssr::StringSeq* _p = new ssr::StringSeq(_s);
  _a.PR_insert(_0RL_tc_ssr_mStringSeq,
               _0RL_ssr_mStringSeq_marshal_fn,
               _0RL_ssr_mStringSeq_destructor_fn,
               _p);
}
void operator<<=(::CORBA::Any& _a, ssr::StringSeq* _sp)
{
  _a.PR_insert(_0RL_tc_ssr_mStringSeq,
               _0RL_ssr_mStringSeq_marshal_fn,
               _0RL_ssr_mStringSeq_destructor_fn,
               _sp);
}

::CORBA::Boolean operator>>=(const ::CORBA::Any& _a, ssr::StringSeq*& _sp)
{
  return _a >>= (const ssr::StringSeq*&) _sp;
}
::CORBA::Boolean operator>>=(const ::CORBA::Any& _a, const ssr::StringSeq*& _sp)
{
  void* _v;
  if (_a.PR_extract(_0RL_tc_ssr_mStringSeq,
                    _0RL_ssr_mStringSeq_unmarshal_fn,
                    _0RL_ssr_mStringSeq_marshal_fn,
                    _0RL_ssr_mStringSeq_destructor_fn,
                    _v)) {
    _sp = (const ssr::StringSeq*)_v;
    return 1;
  }
  return 0;
}

static void _0RL_ssr_mSimulator_marshal_fn(cdrStream& _s, void* _v)
{
  omniObjRef* _o = (omniObjRef*)_v;
  omniObjRef::_marshal(_o, _s);
}
static void _0RL_ssr_mSimulator_unmarshal_fn(cdrStream& _s, void*& _v)
{
  omniObjRef* _o = omniObjRef::_unMarshal(ssr::Simulator::_PD_repoId, _s);
  _v = _o;
}
static void _0RL_ssr_mSimulator_destructor_fn(void* _v)
{
  omniObjRef* _o = (omniObjRef*)_v;
  if (_o)
    omni::releaseObjRef(_o);
}

void operator<<=(::CORBA::Any& _a, ssr::Simulator_ptr _o)
{
  ssr::Simulator_ptr _no = ssr::Simulator::_duplicate(_o);
  _a.PR_insert(ssr::_tc_Simulator,
               _0RL_ssr_mSimulator_marshal_fn,
               _0RL_ssr_mSimulator_destructor_fn,
               _no->_PR_getobj());
}
void operator<<=(::CORBA::Any& _a, ssr::Simulator_ptr* _op)
{
  _a.PR_insert(ssr::_tc_Simulator,
               _0RL_ssr_mSimulator_marshal_fn,
               _0RL_ssr_mSimulator_destructor_fn,
               (*_op)->_PR_getobj());
  *_op = ssr::Simulator::_nil();
}

::CORBA::Boolean operator>>=(const ::CORBA::Any& _a, ssr::Simulator_ptr& _o)
{
  void* _v;
  if (_a.PR_extract(ssr::_tc_Simulator,
                    _0RL_ssr_mSimulator_unmarshal_fn,
                    _0RL_ssr_mSimulator_marshal_fn,
                    _0RL_ssr_mSimulator_destructor_fn,
                    _v)) {
    omniObjRef* _r = (omniObjRef*)_v;
    if (_r)
      _o = (ssr::Simulator_ptr)_r->_ptrToObjRef(ssr::Simulator::_PD_repoId);
    else
      _o = ssr::Simulator::_nil();
    return 1;
  }
  return 0;
}

