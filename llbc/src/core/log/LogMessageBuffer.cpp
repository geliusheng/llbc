/**
 * @file    LogMessageBuffer.cpp
 * @author  Longwei Lai<lailongwei@126.com>
 * @date    2013/11/17
 * @version 1.0
 *
 * @biref
 */

#include "llbc/common/Export.h"
#include "llbc/common/BeforeIncl.h"

#include "llbc/core/log/LogMessageBuffer.h"

#if LLBC_CFG_LOG_USING_WITH_STREAM

__LLBC_NS_BEGIN

LLBC_LogMessageBuffer::LLBC_LogMessageBuffer()
{
    _stream = NULL;
}

LLBC_LogMessageBuffer::~LLBC_LogMessageBuffer()
{
    LLBC_XDelete(_stream);
}

LLBC_LogMessageBuffer &LLBC_LogMessageBuffer::operator <<(const LLBC_String &msg)
{
    _buf.append(msg);
    if (_stream)
    {
        *_stream <<static_cast<const std::basic_string<char> &>(msg);
    }

    return *this;
}

LLBC_LogMessageBuffer &LLBC_LogMessageBuffer::operator <<(const std::basic_string<char> &msg)
{
    _buf.append(msg.c_str(), msg.length());
    if (_stream)
    {
        *_stream <<msg;
    }

    return *this;
}

LLBC_LogMessageBuffer &LLBC_LogMessageBuffer::operator <<(char *msg)
{
    return operator <<((const char *)msg);
}

LLBC_LogMessageBuffer &LLBC_LogMessageBuffer::operator <<(const char *msg)
{
    const char *actualMsg = msg;
    if (UNLIKELY(!actualMsg))
    {
        actualMsg = "null";
    }

    _buf.append(actualMsg);
    if (_stream)
    {
        *_stream <<actualMsg;
    }

    return *this;
}

LLBC_LogMessageBuffer &LLBC_LogMessageBuffer::operator <<(char msg)
{
    _buf.append(1, msg);
    if (_stream)
    {
        *_stream <<msg;
    }

    return *this;
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(bool val)
{
    return static_cast<std::basic_ostream<char> &>(*this).operator <<(val);
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(uint8 val)
{
    return static_cast<std::basic_ostream<char> &>(*this).operator <<(val);
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(sint16 val)
{
    return static_cast<std::basic_ostream<char> &>(*this).operator <<(val);
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(uint16 val)
{
    return static_cast<std::basic_ostream<char> &>(*this).operator <<(val);
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(sint32 val)
{
    return static_cast<std::basic_ostream<char> &>(*this).operator <<(val);
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(uint32 val)
{
    return static_cast<std::basic_ostream<char> &>(*this).operator <<(val);
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(sint64 val)
{
    return static_cast<std::basic_ostream<char> &>(*this).operator <<(val);
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(uint64 val)
{
    return static_cast<std::basic_ostream<char> &>(*this).operator <<(val);
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(long val)
{
    return static_cast<std::basic_ostream<char> &>(*this).operator <<(val);
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(ulong val)
{
    return static_cast<std::basic_ostream<char> &>(*this).operator <<(val);
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(float val)
{
    return static_cast<std::basic_ostream<char> &>(*this).operator <<(val);
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(double val)
{
    return static_cast<std::basic_ostream<char> &>(*this).operator <<(val);
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(ldouble val)
{
    return static_cast<std::basic_ostream<char> &>(*this).operator <<(val);
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(void *val)
{
    return static_cast<std::basic_ostream<char> &>(*this).operator <<(val);
}

std::basic_ostream<char> &LLBC_LogMessageBuffer::operator <<(ios_base_manip manip)
{
    std::basic_ostream<char> &s = *this;
    (*manip)(s);

    return s;
}

LLBC_LogMessageBuffer::operator std::basic_ostream<char> &()
{
    if (!_stream)
    {
        _stream = new std::basic_ostringstream<char>(std::ios_base::ate);
        if (!_buf.empty())
        {
            *_stream <<_buf;
        }
    }

    return *_stream;
}

const LLBC_String &LLBC_LogMessageBuffer::str(LLBC_LogMessageBuffer &)
{
    return _buf;
}

const LLBC_String &LLBC_LogMessageBuffer::str(std::basic_ostream<char> &)
{
    static_cast<std::basic_string<char> &>(_buf) = _stream->str();
    return _buf;
}

__LLBC_NS_END

#endif // LLBC_CFG_LOG_USING_WITH_STREAM

#include "llbc/common/AfterIncl.h"
