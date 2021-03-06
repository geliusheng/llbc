/**
 * @file    StreamImpl.h
 * @author  Longwei Lai<lailongwei@126.com>
 * @date    2012/1/5
 * @version 1.0
 *
 * @brief   LLBC library LLBC_Stream class define.
 */
#ifdef __LLBC_COM_STREAM_H__

#include "llbc/common/Template.h"
#include "llbc/common/Endian.h"

__LLBC_NS_BEGIN

inline LLBC_Stream::LLBC_Stream()
: _buf(NULL)
, _pos(0)
, _size(0)

, _endian(LLBC_DefaultEndian)

, _attach(false)
{
}

inline LLBC_Stream::LLBC_Stream(const LLBC_Stream &rhs)
: _buf(NULL)
, _pos(0)
, _size(0)

, _endian(LLBC_DefaultEndian)

, _attach(false)
{
    this->Assign(rhs);
}

inline LLBC_Stream::LLBC_Stream(const LLBC_Stream &rhs, bool attach)
: _buf(NULL)
, _pos(0)
, _size(0)

, _endian(LLBC_DefaultEndian)

, _attach(false)
{
    if (attach)
        this->Attach(rhs);
    else
        this->Assign(rhs);
}

inline LLBC_Stream::LLBC_Stream(size_t size)
{
    if (size > 0)
    {
        _buf = calloc(size, 1);
        ASSERT(_buf && "LLBC_Stream object alloc memory from heap failed");
    }
    else
    {
        _buf = NULL;
    }

    _pos = 0;
    _size = size;

    _endian = LLBC_DefaultEndian;

    _attach = false;
}

inline LLBC_Stream::LLBC_Stream(void *buf, size_t len)
: _buf(NULL)
, _pos(0)
, _size(0)

, _endian(LLBC_DefaultEndian)

, _attach(false)
{
    this->Attach(buf, len);
}

inline LLBC_Stream::~LLBC_Stream()
{
    if (!_attach)
        LLBC_XFree(_buf);
}

inline void LLBC_Stream::Attach(const LLBC_Stream &rhs)
{
    if (!_attach)
        LLBC_XFree(_buf);

    _buf = rhs._buf;
    _pos = rhs._pos;
    _size = rhs._size;

    _endian = rhs._endian;

    _attach = true;
}

inline void LLBC_Stream::Attach(void *buf, size_t len)
{
    if ((buf && len == 0) || (!buf && len > 0))
        return;

    if (!_attach)
        LLBC_XFree(_buf);

    _buf = buf;
    _pos = 0;
    _size = len;

    _attach = true;
}

inline void LLBC_Stream::Assign(const LLBC_Stream &rhs)
{
    if (!_attach)
        LLBC_XFree(_buf);

    if (rhs._buf)
    {
        _buf = malloc(rhs._size);
        memcpy(_buf, rhs._buf, rhs._size);
    }
    else
    {
        _buf = NULL;
    }

    _pos = rhs._pos;
    _size = rhs._size;

    _endian = rhs._endian;

    _attach = false;
}

inline void LLBC_Stream::Assign(void *buf, size_t len)
{
    if (!_attach)
        LLBC_XFree(_buf);

    if (buf && len > 0)
    {
        _buf = malloc(len);
        memcpy(_buf, buf, len);

        _size = len;
    }
    else
    {
        _buf = NULL;
        _size = 0;
    }

    _pos = 0;

    _attach = false;
}

inline void *LLBC_Stream::Detach()
{
    void *tmp = _buf;

    _buf = NULL;
    _pos = 0;
    _size = 0;

    _attach = false;

    return tmp;
}

inline bool LLBC_Stream::IsAttach() const
{
    return _attach;
}

inline void LLBC_Stream::SetAttachAttr(bool attach)
{
    _attach = attach;
}

inline int LLBC_Stream::GetEndian() const
{
    return _endian;
}

inline void LLBC_Stream::SetEndian(int endian)
{
    if (LLBC_Endian::IsValid(endian))
        _endian = endian;
}

inline void LLBC_Stream::Swap(LLBC_Stream &another)
{
    LLBC_Swap(_buf, another._buf);
    LLBC_Swap(_pos, another._pos);
    LLBC_Swap(_size, another._size);

    LLBC_Swap(_endian,  another._endian);

    LLBC_Swap(_attach, another._attach);
}

inline size_t LLBC_Stream::GetPos() const
{
    return _pos;
}

inline void LLBC_Stream::SetPos(size_t pos)
{
    ASSERT(pos <= _size);
    _pos = pos;
}

inline bool LLBC_Stream::Skip(long size)
{
    const size_t skipped = static_cast<
        size_t>(MAX(0, static_cast<long>(_pos) + size));

    if (skipped > _size)
        return false;

    this->SetPos(skipped);

    return true;
}

inline void LLBC_Stream::Fill(size_t size)
{
    if (_pos + size > _size)
    {
        if (!_attach)
        {
            Resize(_size + MAX(size, _size));
        }
        else
        {
            ASSERT(false && "stream obj attach's buf limit");
            return;
        }
    }

    memset((char *)_buf + _pos, 0, size);
    _pos += size;
}

inline size_t LLBC_Stream::GetSize() const
{
    return _size;
}

inline void *LLBC_Stream::GetBuf() const
{
    return _buf;
}

inline void *LLBC_Stream::GetBufStartWithPos() const
{
    return reinterpret_cast<
        char *>(const_cast<void *>(_buf)) + _pos;
}

inline void LLBC_Stream::Insert(size_t pos, const void *buf, size_t len)
{
    this->Replace(pos, pos, buf, len);
}

inline void LLBC_Stream::Replace(size_t n0, size_t n1, const void *buf, size_t len)
{
    ASSERT(buf && len && "LLBC_Stream::Insert() buf or len invalid!");

    // Swap n0, n1, if n0 > n1.
    if (UNLIKELY(n0 > n1))
    {
        size_t temp = n0;
        n0 = n1;
        n1 = temp;
    }

    ASSERT(n1 <= _pos && "LLBC_Stream::Replace() n1 > _pos!");

    size_t eraseLen = n1 - n0;
    if (_pos + len - eraseLen > _size)
    {
        ASSERT(!_attach && "LLBC_Stream::Insert() obj attach's buf limit");
        Resize(_size + MAX(len, _size));
    }

    uint8 *tmpBuf = NULL;
    if (eraseLen == 0)
    {
        if (n1 == _pos)
            memcpy((uint8 *)_buf + n1, buf, len);
        else
        {
            tmpBuf = (uint8 *)malloc(_pos - n1);

            memcpy(tmpBuf, (uint8 *)_buf + n1, _pos - n1);
            memcpy((uint8 *)_buf + n1, buf, len);
            memcpy((uint8 *)_buf + n1 + len, tmpBuf, _pos - n1);

            free(tmpBuf);
        }

        _pos += len;
        return;
    }

    if (eraseLen == len)
    {
        memcpy((uint8 *)_buf + n0, buf, len);
        return;
    }

    if (n1 != _pos)
    {
        tmpBuf = (uint8 *)malloc(_pos - n1);
        memcpy(tmpBuf, (uint8 *)_buf + n1, _pos - n1);
    }

    memcpy((uint8 *)_buf + n0, buf, len);

    if (tmpBuf)
    {
        memcpy((uint8 *)_buf + n0 + len, tmpBuf, _pos - n1);
        free(tmpBuf);
    }

    _pos = _pos + len - (n1 - n0);
}

inline void LLBC_Stream::WriteBuffer(const void *buf, size_t len)
{
    if (UNLIKELY(!buf || len <= 0))
    {
        return;
    }
    else if (_pos + len > _size)
    {
        if (!_attach)
            Resize(_size + MAX(len, _size));
        else
        {
            ASSERT(false && "stream obj attach's buf limit");
            return;
        }
    }

    // Check memory overlapped
    ASSERT(this->OverlappedCheck(buf, len) && "LLBC_Stream::WriteBuffer() buffer overlapped!");

    memcpy((uint8 *)_buf + _pos, buf, len);
    _pos += len;
}

inline bool LLBC_Stream::ReadBuffer(void *buf, size_t len)
{
    if (len == 0)
        return true;
    else if (_pos + len > _size)
        return false;

    ASSERT(buf && "LLBC_Stream::ReadBuffer(): expect not-null buf pointer to read");

    // check memory overlapped
    ASSERT(this->OverlappedCheck(buf, len) && "LLBC_Stream::ReadBuffer() buffer overlapped!");

    memcpy(buf, (const uint8 *)_buf + _pos, len);
    _pos += len;

    return true;
}

inline void LLBC_Stream::Resize(size_t newSize)
{
    if (newSize > _size)
    {
        _buf = realloc(_buf, newSize);
        ASSERT(_buf && "alloc memory from heap fail!");

        _size = newSize;
    }
}

inline void LLBC_Stream::Cleanup()
{
    if (_buf && !_attach)
        free(_buf);

    _buf = NULL;
    _size = _pos = 0;

    _attach = false;
}

inline bool LLBC_Stream::ReadBool(bool &value)
{
    return this->ReadBuffer(&value, sizeof(bool));
}

inline bool LLBC_Stream::ReadSInt8(sint8 &value)
{
    return this->ReadBuffer(&value, sizeof(sint8));
}

inline bool LLBC_Stream::ReadUInt8(uint8 &value)
{
    return this->ReadBuffer(&value, sizeof(uint8));
}

inline bool LLBC_Stream::ReadSInt16(sint16 &value)
{
    return this->ReadRawType(value);
}

inline bool LLBC_Stream::ReadUInt16(uint16 &value)
{
    return this->ReadRawType(value);
}

inline bool LLBC_Stream::ReadSInt32(sint32 &value)
{
    return this->ReadRawType(value);
}

inline bool LLBC_Stream::ReadUInt32(uint32 &value)
{
    return this->ReadRawType(value);
}

inline bool LLBC_Stream::ReadSInt64(sint64 &value)
{
    return this->ReadRawType(value);
}

inline bool LLBC_Stream::ReadUInt64(uint64 &value)
{
    return this->ReadRawType(value);
}

inline bool LLBC_Stream::ReadLong(long &value)
{
    return this->ReadRawType(value);
}

inline bool LLBC_Stream::ReadULong(ulong &value)
{
    return this->ReadRawType(value);
}

inline bool LLBC_Stream::ReadFloat(float &value)
{
    return this->ReadRawType(value);
}

inline bool LLBC_Stream::ReadDouble(double &value)
{
    return this->ReadRawType(value);
}

inline bool LLBC_Stream::ReadPtr(void *&value)
{
    return this->ReadRawType(value);
}

inline bool LLBC_Stream::ReadBool_2(bool failRet)
{
    this->ReadBuffer(&failRet, sizeof(bool));
    return failRet;
}

inline sint8 LLBC_Stream::ReadSInt8_2(sint8 failRet)
{
    this->ReadBuffer(&failRet, sizeof(sint8));
    return failRet;
}

inline uint8 LLBC_Stream::ReadUInt8_2(uint8 failRet)
{
    this->ReadBuffer(&failRet, sizeof(uint8));
    return failRet;
}

inline sint16 LLBC_Stream::ReadSInt16_2(sint16 failRet)
{
    this->ReadRawType(failRet);
    return failRet;
}

inline uint16 LLBC_Stream::ReadUInt16_2(uint16 failRet)
{
    this->ReadRawType(failRet);
    return failRet;
}

inline sint32 LLBC_Stream::ReadSInt32_2(sint32 failRet)
{
    this->ReadRawType(failRet);
    return failRet;
}

inline uint32 LLBC_Stream::ReadUInt32_2(uint32 failRet)
{
    this->ReadRawType(failRet);
    return failRet;
}

inline sint64 LLBC_Stream::ReadSInt64_2(sint64 failRet)
{
    this->ReadRawType(failRet);
    return failRet;
}

inline uint64 LLBC_Stream::ReadUInt64_2(uint64 failRet)
{
    this->ReadRawType(failRet);
    return failRet;
}

inline long LLBC_Stream::ReadLong_2(long failRet)
{
    this->ReadRawType(failRet);
    return failRet;
}

inline ulong LLBC_Stream::ReadULong_2(ulong failRet)
{
    this->ReadRawType(failRet);
    return failRet;
}

inline float LLBC_Stream::ReadFloat_2(float failRet)
{
    this->ReadRawType(failRet);
    return failRet;
}

inline double LLBC_Stream::ReadDouble_2(double failRet)
{
    this->ReadRawType(failRet);
    return failRet;
}

inline void *LLBC_Stream::ReadPtr_2(void *failRet)
{
    this->ReadRawType(failRet);
    return failRet;
}

inline void LLBC_Stream::WriteBool(bool value)
{
    this->WriteBuffer(&value, sizeof(bool));
}

inline void LLBC_Stream::WriteSInt8(sint8 value)
{
    this->WriteBuffer(&value, sizeof(sint8));
}

inline void LLBC_Stream::WriteUInt8(uint8 value)
{
    this->WriteBuffer(&value, sizeof(uint8));
}

inline void LLBC_Stream::WriteSint16(sint16 value)
{
    this->WriteRawType(value);
}

inline void LLBC_Stream::WriteUInt16(uint16 value)
{
    this->WriteRawType(value);
}

inline void LLBC_Stream::WriteSInt32(sint32 value)
{
    this->WriteRawType(value);
}

inline void LLBC_Stream::WriteUInt32(uint32 value)
{
    this->WriteRawType(value);
}

inline void LLBC_Stream::WriteSInt64(sint64 value)
{
    this->WriteRawType(value);
}

inline void LLBC_Stream::WriteUInt64(uint64 value)
{
    this->WriteRawType(value);
}

inline void LLBC_Stream::WriteLong(long value)
{
    this->WriteRawType(value);
}

inline void LLBC_Stream::WriteULong(ulong value)
{
    this->WriteRawType(value);
}

inline void LLBC_Stream::WriteFloat(float value)
{
    this->WriteRawType(value);
}

inline void LLBC_Stream::WriteDouble(double value)
{
    this->WriteRawType(value);
}

inline void LLBC_Stream::WritePtr(const void *value)
{
    this->WriteRawType(value);
}

inline LLBC_Stream &LLBC_Stream::operator =(const LLBC_Stream &rhs)
{
    this->Assign(rhs);
    return *this;
}

template <typename T>
inline bool LLBC_Stream::ReadRawType(T &value)
{
    if (!this->ReadBuffer(&value, sizeof(T)))
        return false;

    if (_endian != LLBC_MachineEndian)
        LLBC_ReverseBytes(value);

    return true;
}

template <typename T>
inline void LLBC_Stream::WriteRawType(const T &value)
{
    if (_endian == LLBC_MachineEndian)
        this->WriteBuffer(&value, sizeof(T));
    else
    {
        const T reversedVal = LLBC_ReverseBytes2(value);
        this->WriteBuffer(&reversedVal, sizeof(T));
    }
}

inline bool LLBC_Stream::OverlappedCheck(const void *another, size_t len)
{
    if (!_buf)
        return true;

    return (((uint8 *)another + len <= _buf) || 
        (another >= (uint8 *)_buf + _size));
}

__LLBC_NS_END

#endif // !__LLBC_COM_STREAM_H__
