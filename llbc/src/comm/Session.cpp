/**
 * @file    Session.cpp
 * @author  Longwei Lai<lailongwei@126.com>
 * @date    2014/11/11
 * @version 1.0
 *
 * @brief
 */

#include "llbc/common/Export.h"
#include "llbc/common/BeforeIncl.h"

#include "llbc/comm/protocol/ProtocolStack.h"

#include "llbc/comm/Packet.h"
#include "llbc/comm/Socket.h"
#include "llbc/comm/Session.h"
#include "llbc/comm/BasePoller.h"
#include "llbc/comm/PollerType.h"
#include "llbc/comm/ServiceEvent.h"
#include "llbc/comm/IService.h"

__LLBC_NS_BEGIN

LLBC_SessionCloseInfo::LLBC_SessionCloseInfo()
: _fromSvc(false)
{
    // Get reason.
    _reason = LLBC_FormatLastError();

    // Fill errNo & subErrNo.
    _errNo = LLBC_Errno;
    if (LLBC_HasSubErrorNo(_errNo))
        _subErrNo = LLBC_GetSubErrorNo();
    else
        _subErrNo = LLBC_ERROR_SUCCESS;
}

LLBC_SessionCloseInfo::LLBC_SessionCloseInfo(char *reason)
: _fromSvc(true)
, _reason(reason)

, _errNo(LLBC_ERROR_SUCCESS)
, _subErrNo(LLBC_ERROR_SUCCESS)
{
}

LLBC_SessionCloseInfo::LLBC_SessionCloseInfo(int errNo, int subErrNo)
: _fromSvc(false)
{
    // Format error.
    if (!LLBC_ERROR_TYPE_IS_LIBRARY(errNo))
    {
        _errNo = errNo;
        _subErrNo = subErrNo;
        _reason = LLBC_StrErrorEx(errNo, subErrNo);
    }
    else
    {
        _errNo = errNo;
        _subErrNo = LLBC_ERROR_SUCCESS;
        _reason = LLBC_StrError(errNo);
    }
}

LLBC_SessionCloseInfo::~LLBC_SessionCloseInfo()
{
}

bool LLBC_SessionCloseInfo::IsFromService() const
{
    return _fromSvc;
}

const LLBC_String &LLBC_SessionCloseInfo::GetReason() const
{
    return _reason;
}

int LLBC_SessionCloseInfo::GetErrno() const
{
    return _errNo;
}

int LLBC_SessionCloseInfo::GetSubErrno() const
{
    return _subErrNo;
}

LLBC_Session::LLBC_Session()
: _id(0)
, _socket(NULL)
, _sockHandle(LLBC_INVALID_SOCKET_HANDLE)

, _svc(NULL)
, _poller(NULL)

, _protoStack(NULL)
{
}

LLBC_Session::~LLBC_Session()
{
    LLBC_XDelete(_socket);
    LLBC_XDelete(_protoStack);
}

int LLBC_Session::GetId() const
{
    return _id;
}

void LLBC_Session::SetId(int id)
{
    _id = id;
}

LLBC_SocketHandle LLBC_Session::GetSocketHandle() const
{
    return _sockHandle;
}

bool LLBC_Session::IsListen() const
{
    return _socket->IsListen();
}

LLBC_Socket *LLBC_Session::GetSocket()
{
    return _socket;
}

const LLBC_Socket *LLBC_Session::GetSocket() const
{
    return _socket;
}

void LLBC_Session::SetSocket(LLBC_Socket *socket)
{
    _socket = socket;
    _sockHandle = _socket->Handle();
    _pollerType = socket->GetPollerType();
}

LLBC_IService *LLBC_Session::GetService()
{
    return _svc;
}

void LLBC_Session::SetService(LLBC_IService *svc)
{
    _svc = svc;
#if LLBC_CFG_COMM_USE_FULL_STACK
    _protoStack = _svc->CreateFullStack();
#else
    _protoStack = _svc->CreateRawStack();
#endif
    _protoStack->SetSession(this);
}

LLBC_BasePoller *LLBC_Session::GetPoller()
{
    return _poller;
}

void LLBC_Session::SetPoller(LLBC_BasePoller *poller)
{
    _poller = poller;
}

int LLBC_Session::Send(LLBC_Packet *packet)
{
    bool removeSession;
    LLBC_MessageBlock *block;
#if LLBC_CFG_COMM_USE_FULL_STACK
    if (_protoStack->Send(packet, block, removeSession) != LLBC_OK)
#else
    if (_protoStack->SendRaw(packet, block, removeSession) != LLBC_OK)
#endif
        return removeSession ? LLBC_FAILED : LLBC_OK;

    return Send(block);
}

int LLBC_Session::Send(LLBC_MessageBlock *block)
{
    if (_socket->AsyncSend(block) != LLBC_OK)
        return LLBC_FAILED;

    // In LINUX or ANDROID platform, if use EPOLL ET mode, we must force call OnSend() one time.
#if LLBC_TARGET_PLATFORM_LINUX || LLBC_TARGET_PLATFORM_ANDROID
    if (_pollerType == LLBC_PollerType::EpollPoller)
        OnSend();
#endif

    return LLBC_OK;
}

#if LLBC_TARGET_PLATFORM_WIN32
void LLBC_Session::OnSend(LLBC_POverlapped ol)
{
    _socket->OnSend(ol);
}
#else // LLBC_TARGET_PLATFORM_NON_WIN32
void LLBC_Session::OnSend()
{
    _socket->OnSend();
}
#endif // LLBC_TARGET_PLATFORM_WIN32

#if LLBC_TARGET_PLATFORM_WIN32
void LLBC_Session::OnRecv(LLBC_POverlapped ol)
{
    _socket->OnRecv(ol);
}
#else // LLBC_TARGET_PLATFORM_NON_WIN32
void LLBC_Session::OnRecv()
{
    _socket->OnRecv();
}
#endif // LLBC_TARGET_PLATFORM_WIN32

#if LLBC_TARGET_PLATFORM_WIN32
void LLBC_Session::OnClose(LLBC_POverlapped ol, LLBC_SessionCloseInfo *closeInfo)
#else
void LLBC_Session::OnClose(LLBC_SessionCloseInfo *closeInfo)
#endif // LLBC_TARGET_PLATFORM_WIN32
{
    if (closeInfo == NULL)
        closeInfo = new LLBC_SessionCloseInfo();

    // Notify socket session closed.
    const LLBC_SocketHandle sockHandle = _socket->Handle();
#if LLBC_TARGET_PLATFORM_WIN32
    _socket->OnClose(ol);
#else
    _socket->OnClose();
#endif // LLBC_TARGET_PLATFORM_WIN32

    // Build session-destroy event and push to service.
    _svc->Push(LLBC_SvcEvUtil::BuildSessionDestroyEv(_socket->GetLocalAddress(),
                                                     _socket->GetPeerAddress(),
                                                     _socket->IsListen(),
                                                     _id,
                                                     sockHandle,
                                                     closeInfo));

    // Let poller remove self.
    _poller->RemoveSession(this);
}

void LLBC_Session::OnSent(size_t len)
{
    // TODO: For support sampler, do stuff here.
    // ... ...
}

bool LLBC_Session::OnRecved(LLBC_MessageBlock *block)
{
    bool removeSession;
    std::vector<LLBC_Packet *> packets;
#if LLBC_CFG_COMM_USE_FULL_STACK
    if (_protoStack->Recv(block, packets, removeSession) != LLBC_OK)
#else
    if (_protoStack->RecvRaw(block, packets, removeSession) != LLBC_OK)
#endif
    {
        if (removeSession)
            OnClose();

        return false;
    }

    LLBC_Packet *packet;
    for (size_t i = 0; i < packets.size(); i++)
    {
        packet = packets[i];
        packet->SetSessionId(_id);
        packet->SetLocalAddr(_socket->GetLocalAddress());
        packet->SetPeerAddr(_socket->GetPeerAddress());

        _svc->Push(LLBC_SvcEvUtil::BuildDataArrivalEv(packet));
    }

    return true;
}

__LLBC_NS_END

#include "llbc/common/AfterIncl.h"
