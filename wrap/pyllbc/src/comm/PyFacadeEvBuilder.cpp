/**
 * @file    PyFacadeEvBuilder.cpp
 * @author  Longwei Lai<lailongwei@126.com>
 * @date    2014/11/05
 * @version 1.0
 *
 * @brief
 */

#include "pyllbc/common/Export.h"

#include "pyllbc/comm/PyFacadeEvBuilder.h"

namespace
{
    typedef pyllbc_FacadeEvBuilder This;
}

PyObject *This::_evCls = NULL;

PyObject *This::_attrSvc = PyString_FromString("_svc");
PyObject *This::_attrSessionId = PyString_FromString("_session_id");
PyObject *This::_attrIdleTime = PyString_FromString("_idletime");
PyObject *This::_attrConnected = PyString_FromString("_connected");
PyObject *This::_attrReason = PyString_FromString("_reason");
PyObject *This::_attrLocalIp = PyString_FromString("_local_ip");
PyObject *This::_attrLocalPort = PyString_FromString("_local_port");
PyObject *This::_attrPeerIp = PyString_FromString("_peer_ip");
PyObject *This::_attrPeerPort = PyString_FromString("_peer_port");
PyObject *This::_attrIsListen = PyString_FromString("_islisten");
PyObject *This::_attrSocket = PyString_FromString("_socket");
PyObject *This::_attrPacket = PyString_FromString("_packet");
PyObject *This::_attrReportLayer = PyString_FromString("_report_layer");
PyObject *This::_attrReportLevel = PyString_FromString("_report_level");
PyObject *This::_attrReportMsg = PyString_FromString("_report_msg");
PyObject *This::_attrOpcode = PyString_FromString("_opcode");
PyObject *This::_attrDestroyedFromSvc = PyString_FromString("_destroyed_from_service");
PyObject *This::_attrErrNo = PyString_FromString("_errno");
PyObject *This::_attrSubErrNo = PyString_FromString("_sub_errno");

PyObject *pyllbc_FacadeEvBuilder::BuildInitializeEv(PyObject *svc)
{
    return This::CreateEv(svc);
}

PyObject *pyllbc_FacadeEvBuilder::BuildDestroyEv(PyObject *svc)
{
    return This::CreateEv(svc);
}

PyObject *pyllbc_FacadeEvBuilder::BuildStartEv(PyObject *svc)
{
    return This::CreateEv(svc);
}

PyObject *pyllbc_FacadeEvBuilder::BuildStopEv(PyObject *svc)
{
    return This::CreateEv(svc);
}

PyObject *pyllbc_FacadeEvBuilder::BuildUpdateEv(PyObject *svc)
{
    return This::CreateEv(svc);
}

PyObject *pyllbc_FacadeEvBuilder::BuildIdleEv(PyObject *svc, int idleTime)
{
    PyObject *ev = This::CreateEv(svc);
    This::SetAttr(ev, This::_attrIdleTime, idleTime / 1000.0);

    return ev;
}

PyObject *pyllbc_FacadeEvBuilder::BuildSessionCreateEv(PyObject *svc, const LLBC_SessionInfo &si)
{
    PyObject *ev = This::CreateEv(svc);
    This::SetAttr(ev, This::_attrSocket, static_cast<int>(si.GetSocket()));
    This::SetAttr(ev, This::_attrSessionId, si.GetSessionId());
    This::SetAttr(ev, This::_attrIsListen, si.IsListenSession());

    This::SetAttr(ev, This::_attrLocalIp, si.GetLocalAddr().GetIpAsString());
    This::SetAttr(ev, This::_attrLocalPort, si.GetLocalAddr().GetPort());

    This::SetAttr(ev, This::_attrPeerIp, si.GetPeerAddr().GetIpAsString());
    This::SetAttr(ev, This::_attrPeerPort, si.GetPeerAddr().GetPort());

    return ev;
}

PyObject *pyllbc_FacadeEvBuilder::BuildSessionDestroyEv(PyObject *svc, const LLBC_SessionDestroyInfo &destroyInfo)
{
    PyObject *ev = This::CreateEv(svc);
    This::SetAttr(ev, This::_attrReason, destroyInfo.GetReason());
    This::SetAttr(ev, This::_attrSessionId, destroyInfo.GetSessionId());
    This::SetAttr(ev, This::_attrDestroyedFromSvc, destroyInfo.IsDestroyedFromService());

    This::SetAttr(ev, This::_attrLocalIp, destroyInfo.GetLocalAddr().GetIpAsString());
    This::SetAttr(ev, This::_attrLocalPort, destroyInfo.GetLocalAddr().GetPort());

    This::SetAttr(ev, This::_attrPeerIp, destroyInfo.GetPeerAddr().GetIpAsString());
    This::SetAttr(ev, This::_attrPeerPort, destroyInfo.GetPeerAddr().GetPort());

    This::SetAttr(ev, This::_attrSocket, static_cast<int>(destroyInfo.GetSocket()));

    This::SetAttr(ev, This::_attrErrNo, destroyInfo.GetErrno());
    This::SetAttr(ev, This::_attrSubErrNo, destroyInfo.GetSubErrno());

    return ev;
}

PyObject *pyllbc_FacadeEvBuilder::BuildAsyncConnResultEv(PyObject *svc, const LLBC_AsyncConnResult &result)
{
    PyObject *ev = This::CreateEv(svc);
    This::SetAttr(ev, This::_attrConnected, result.IsConnected());
    This::SetAttr(ev, This::_attrReason, result.GetReason());
    This::SetAttr(ev, This::_attrPeerIp, result.GetPeerAddr().GetIpAsString());
    This::SetAttr(ev, This::_attrPeerPort, result.GetPeerAddr().GetPort());

    return ev;
}

PyObject *pyllbc_FacadeEvBuilder::BuildProtoReportEv(PyObject *svc, const LLBC_ProtoReport &report)
{
    PyObject *ev = This::CreateEv(svc);
    This::SetAttr(ev, This::_attrReportLayer, report.GetLayer());
    This::SetAttr(ev, This::_attrReportLevel, report.GetLevel());
    This::SetAttr(ev, This::_attrReportMsg, report.GetReport());
    This::SetAttr(ev, This::_attrSessionId, report.GetSessionId());

    return ev;
}

PyObject *pyllbc_FacadeEvBuilder::BuildUnHandledPacketEv(PyObject *svc, const LLBC_Packet &llbcPacket, PyObject *packet)
{
    PyObject *ev = This::CreateEv(svc);
    This::SetAttr(ev, This::_attrSessionId, llbcPacket.GetSessionId());
    This::SetAttr(ev, This::_attrOpcode, llbcPacket.GetOpcode());
    This::SetAttr(ev, This::_attrPacket, packet);

    return ev;
}

PyObject *pyllbc_FacadeEvBuilder::CreateEv(PyObject *svc)
{
    if (UNLIKELY(!This::_evCls))
        This::_evCls = pyllbc_s_TopModule->GetObject("ServiceEvent");

    return PyObject_CallFunctionObjArgs(This::_evCls, svc, NULL);
}

void pyllbc_FacadeEvBuilder::SetAttr(PyObject *ev, PyObject *attr, bool val)
{
    PyObject *pyVal = val ? Py_True : Py_False;
    PyObject_SetAttr(ev, attr, pyVal);
}

void pyllbc_FacadeEvBuilder::SetAttr(PyObject *ev, PyObject *attr, int val)
{
    PyObject *pyVal =PyInt_FromLong(val);
    PyObject_SetAttr(ev, attr, pyVal);

    Py_DECREF(pyVal);
}

void pyllbc_FacadeEvBuilder::SetAttr(PyObject *ev, PyObject *attr, sint64 val)
{
    PyObject *pyVal = PyLong_FromLongLong(val);
    PyObject_SetAttr(ev, attr, pyVal);

    Py_DECREF(pyVal);
}

void pyllbc_FacadeEvBuilder::SetAttr(PyObject *ev, PyObject *attr, double val)
{
    PyObject *pyVal = PyFloat_FromDouble(val);
    PyObject_SetAttr(ev, attr, pyVal);

    Py_DECREF(pyVal);
}

void pyllbc_FacadeEvBuilder::SetAttr(PyObject *ev, PyObject *attr, const LLBC_String &val)
{
    PyObject *pyVal = PyString_FromStringAndSize(val.data(), val.size());
    PyObject_SetAttr(ev, attr, pyVal);
    
    Py_DECREF(pyVal);
}

void pyllbc_FacadeEvBuilder::SetAttr(PyObject *ev, PyObject *attr, PyObject *val)
{
    PyObject_SetAttr(ev, attr, val);
}
