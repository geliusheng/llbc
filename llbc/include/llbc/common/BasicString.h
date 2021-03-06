/**
 * @file    BasicString.h
 * @author  Longwei Lai<lailongwei@126.com>
 * @date    2013/04/05
 * @version 1.0
 *
 * @brief
 */
#ifndef __LLBC_COM_BASIC_STRING_H__
#define __LLBC_COM_BASIC_STRING_H__

#include "llbc/common/PFConfig.h"

#include "llbc/common/Macro.h"
#include "llbc/common/Errno.h"
#include "llbc/common/Errors.h"
#include "llbc/common/Stream.h"

__LLBC_NS_BEGIN

template <typename _Elem,
          typename _Traits = std::char_traits<_Elem>,
          typename _Ax = std::allocator<_Elem> >
class LLBC_BasicString : public 
    std::basic_string<_Elem, _Traits, _Ax>
{
    typedef LLBC_BasicString<_Elem, _Traits, _Ax> _This;
    typedef std::basic_string<_Elem, _Traits, _Ax> _Base;

    typedef std::vector<LLBC_BasicString<_Elem, _Traits, _Ax> > _These;

public:
    typedef typename _Base::size_type size_type;
    typedef typename _Base::value_type value_type;

    typedef typename _Base::iterator iterator;
    typedef typename _Base::const_iterator const_iterator;

    typedef typename _Base::pointer pointer;
    typedef typename _Base::const_pointer const_pointer;

    typedef _These These;

public:
#if LLBC_TARGET_PLATFORM_NON_WIN32
    static const size_type npos = _Base::npos;
#endif

public:
    // Constructors.
    explicit LLBC_BasicString(const _Ax &al = _Ax()):_Base(al) {  }
    LLBC_BasicString(const _This &rhs):_Base(rhs) {  }
    LLBC_BasicString(const _Base &rhs):_Base(rhs) {  }
    LLBC_BasicString(const _This &rhs, size_type pos, size_type n):_Base(rhs, pos, n) {  }
    LLBC_BasicString(const _Elem *s, const _Ax &al = _Ax()):_Base(al) { if (s) _Base::append(s); }
    LLBC_BasicString(const _Elem *s, size_type n, const _Ax &al = _Ax()):_Base(s, n, al) {  }
    LLBC_BasicString(size_type n, _Elem c, const _Ax &al = _Ax()):_Base(n, c, al) {  }

    // operator =
    _This &operator =(const _This &rhs)
    {
        _Base::operator =(rhs);
        return *this;
    }

    _This &operator =(const _Elem *s)
    {
        if (LIKELY(s))
            _Base::operator =(s);
        else
            this->clear();

        return *this;
    }

    _This &operator =(const _Elem &c)
    {
        _Base::operator =(c);
        return *this;
    }

    // operator +
    _This operator +(const _Elem *s) const
    {
        _This ret = *this;
        ret.append(s);
        return ret;
    }

    _This operator +(const _Base &rhs) const
    {
        _This ret = *this;
        ret.append(rhs);
        return ret;
    }

    _This operator +(const _This &rhs) const
    {
        _This ret = *this;
        ret.append(rhs);
        return ret;
    }

    // operator +=
    _This &operator +=(const _Base &rhs)
    {
        _Base::operator +=(rhs);
        return *this;
    }

    _This &operator +=(const _This &rhs)
    {
        _Base::operator +=(rhs);
        return *this;
    }

    _This &operator +=(const _Elem *s)
    {
        if (LIKELY(s))
            _Base::operator +=(s);

        return *this;
    }

    _This &operator +=(const _Elem &c)
    {
        _Base::operator +=(c);
        return *this;
    }

    // operator ==
    bool operator ==(const _This &rhs) const
    {
        return _This::compare(rhs) == 0;
    }

    bool operator ==(const _Elem *s) const
    {
        return _This::compare(s) == 0;
    }

    bool operator ==(const _Elem &c) const
    {
        return _This::compare(c) == 0;
    }

    // operator !=
    bool operator !=(const _This &rhs) const
    {
        return !this->operator ==(rhs);
    }

    bool operator !=(const _Elem *s) const
    {
        return !this->operator ==(s);
    }

    bool operator !=(const _Elem &c) const
    {
        return !this->operator ==(c);
    }

    // operator <
    bool operator <(const _This &rhs) const
    {
        return this->compare(rhs) < 0;
    }

    // operator <=
    bool operator <=(const _This &rhs) const
    {
        return this->compare(rhs) <= 0;
    }

    // operator >
    bool operator >(const _This &rhs) const
    {
        return this->compare(rhs) > 0;
    }

    // operator >=
    bool operator >=(const _This &rhs) const
    {
        return this->compare(rhs) >= 0;
    }

    // operator []
    _Elem &operator [](size_type off)
    {
        return (*((_Base *)this))[off];
    }

    const _Elem &operator [](size_type off) const
    {
        return (*((_Base *)this))[off];
    }

    // at operations.
    _Elem &at(size_type off)
    {
        return _Base::at(off);
    }

    const _Elem &at(size_type off) const
    {
        return _Base::at(off);
    }

    // append operations.
    _This &append(const _Base &str)
    {
        _Base::append(str);
        return *this;
    }

    _This &append(const _This &str)
    {
        _Base::append(str);
        return *this;
    }

    _This &append(const _This &str, size_type pos, size_type n)
    {
        _Base::append(str, pos, n);
        return *this;
    }

    _This &append(const _Elem *s)
    {
        _Base::append(s);
        return *this;
    }

    _This &append(const _Elem *s, size_type n)
    {
        _Base::append(s, n);
        return *this;
    }

    _This &append(size_type n, _Elem c)
    {
        _Base::append(n, c);
        return *this;
    }

    _This &append(iterator first, iterator last)
    {
        _Base::append(first, last);
        return *this;
    }

    _This &append(const_pointer first, const_pointer last)
    {
        _Base::append(first, last);
        return *this;
    }

    _This &append(const_iterator first, const_iterator last)
    {
        _Base::append(first, last);
        return *this;
    }

    // assign operations.
    _This &assign(const _This &str)
    {
        _Base::assign(str);
        return *this;
    }

    _This &assign(const _This &str, size_type pos, size_type n)
    {
        _Base::assign(str, pos, n);
        return *this;
    }

    _This &assign(const _Elem *s)
    {
        _Base::assign(s);
        return *this;
    }

    _This &assign(const _Elem *s, size_type n)
    {
        _Base::assign(s, n);
        return *this;
    }

    _This &assign(const_iterator first, const_iterator last)
    {
        _Base::assign(first, last);
        return *this;
    }

    // insert operations.
    _This &insert(size_type p0, const _This &str)
    {
        _Base::insert(p0, str);
        return *this;
    }

    _This &insert(size_type p0, const _This &str, 
        size_type pos, size_type n)
    {
        _Base::insert(p0, str, pos, n);
        return *this;
    }

    _This &insert(size_type p0, const _Elem *s)
    {
        _Base::insert(p0, s);
        return *this;
    }

    _This &insert(size_type p0, const _Elem *s, size_type n)
    {
        _Base::insert(p0, s, n);
        return *this;
    }

    _This &insert(size_type p0, size_type n, _Elem c)
    {
        _Base::insert(p0, n, c);
        return *this;
    }

    iterator insert(iterator it, _Elem c)
    {
        return _Base::insert(it, c);
    }

    void insert(iterator it, size_type n, _Elem c)
    {
        _Base::insert(it, n, c);
    }

    void insert(iterator it, const_iterator first, const_iterator last)
    {
        _Base::insert(it, first, last);
    }

    // erase operations.
    _This &erase(size_type p0, size_type n = npos)
    {
        _Base::erase(p0, n);
        return *this;
    }

    iterator erase(iterator it)
    {
        return _Base::erase(it);
    }

    iterator erase(iterator first, iterator last)
    {
        return _Base::erase(first, last);
    }

    // replace operations.
    _This &replace(size_type p0, size_type n0,
        const _This &str)
    {
        _Base::replace(p0, n0, str);
        return *this;
    }

    _This &replace(size_type p0, size_type n0,
        const _This &str, size_type pos, size_type n)
    {
        _Base::replace(p0, n0, str, pos, n);
        return *this;
    }

    _This &replace(size_type p0, size_type n0,
        const _Elem *s)
    {
        _Base::replace(p0, n0, s);
        return *this;
    }

    _This &replace(size_type p0, size_type n0,
        const _Elem *s, size_type n)
    {
        _Base::replace(p0, n0, s, n);
        return *this;
    }

    _This &replace(size_type p0, size_type n0,
        size_type n, _Elem c)
    {
        _Base::replace(p0, n0, n, c);
        return *this;
    }

    _This &replace(iterator first0, iterator last0,
        const _This &str)
    {
        _Base::replace(first0, last0, str);
        return *this;
    }

    _This &replace(iterator first0, iterator last0,
        const _Elem *s)
    {
        _Base::replace(first0, last0, s);
        return *this;
    }

    _This &replace(iterator first0, iterator last0,
        const _Elem *s, size_type n)
    {
        _Base::replace(first0, last0, s, n);
        return *this;
    }

    _This &replace(iterator first0, iterator last0,
        size_type n, _Elem c)
    {
        _Base::replace(first0, last0, n, c);
        return *this;
    }

    _This &replace(iterator first0, iterator last0,
        const_iterator first, const_iterator last)
    {
        _Base::replace(first0, last0, first, last);
        return *this;
    }

    _These split(const _Elem &sep, size_type max_split = -1) const
    {
        return this->split(_This(1, sep), max_split);
    }

    // findreplace operation
    _This &findreplace(const _Elem &c1, const _Elem &c2, int count = -1)
    {
        if (c1 == c2)
            return *this;

        for (size_type i = 0; i < this->size(); i++)
        {
            if ((*this)[i] == c1)
                replace(i, 1, 1, c2);
        }

        return *this;
    }

    _This &findreplace(const _This &s1, const _This &s2, int count = -1)
    {
        if (s1 == s2)
            return *this;

        size_type found = 0;
        while ((found = find(s1, found)) != npos)
        {
            replace(found, s1.size(), s2);
            found += s2.size();
        }

        return *this;
    }

    // split operation
    _These split(const _This &sep, size_type max_split = -1, bool with_elem = false) const
    {
        _These substrs;
        if (sep.empty() || max_split == 0 || this->empty())
        {
            substrs.push_back(*this);
            return substrs;
        }

        size_type idx = 0;
        uint32 splitTimes = 0;
        for (; splitTimes < static_cast<uint32>(max_split); splitTimes++)
        {
            size_type findIdx = npos;
            if (with_elem)
            {
                for (size_t i = 0; i < sep.size(); i++)
                {
                    findIdx = this->find(sep[i], idx);
                    if (findIdx != npos)
                        break;
                }
            }
            else
            {
                findIdx = this->find(sep, idx);
            }

            if (findIdx == npos)
                break;

            substrs.push_back(this->substr(idx, findIdx - idx));
            if ((idx = findIdx + 1) == this->size())
            {
                substrs.push_back(_This());
                break;
            }
        }

        if (idx != this->size())
            substrs.push_back(this->substr(idx));

        return substrs;
    }

    _These split(const _These &seps, size_type max_split = -1) const
    {
        _These substrs;
        if (seps.empty() || max_split == 0 || this->empty())
        {
            substrs.push_back(*this);
            return substrs;
        }

        size_type idx = 0;
        uint32 splitTimes = 0;
        for (; splitTimes < static_cast<uint32>(max_split); splitTimes++)
        {
            size_type findIdx = npos;
            for (size_t i = 0; i < seps.size(); i++)
            {
                findIdx = this->find(seps[i], idx);
                if (findIdx != npos)
                    break;
            }

            if (findIdx == npos)
                break;

            substrs.push_back(this->substr(idx, findIdx - idx));
            if ((idx = findIdx + 1) == this->size())
            {
                substrs.push_back(_This());
                break;
            }
        }

        if (idx != this->size())
            substrs.push_back(this->substr(idx));

        return substrs;
    }

    // swap operations.
    void swap(_This &str)
    {
        _Base::swap(str);
    }

    // find operations.
    size_type find(const _This &str, 
        size_type pos = 0) const
    {
        return _Base::find(str, pos);
    }

    size_type find(const _Elem *s, 
        size_type pos, size_type n) const
    {
        return _Base::find(s, pos, n);
    }

    size_type find(const _Elem *s,
        size_type pos = 0) const
    {
        return _Base::find(s, pos);
    }

    size_type find(_Elem c, size_type pos = 0) const
    {
        return _Base::find(c, pos);
    }

    // Reverse find operations.
    size_type rfind(const _This &str, 
        size_type pos = npos) const
    {
        return _Base::rfind(str, pos);
    }

    size_type rfind(const _Elem *s, size_type pos,
        size_type n) const
    {
        return _Base::rfind(s, pos, n);
    }

    size_type rfind(const _Elem *s, size_type pos = npos) const
    {
        return _Base::rfind(s, pos);
    }

    size_type rfind(_Elem c, size_type pos = npos) const
    {
        return _Base::rfind(c, pos);
    }

    // find first of operations.
    size_type find_first_of(const _This &str, 
        size_type pos = 0) const
    {
        return _Base::find_first_of(str, pos);
    }

    size_type find_first_of(const _Elem *s, size_type pos,
        size_type n) const
    {
        return _Base::find_first_of(s, pos, n);
    }

    size_type find_first_of(const _Elem *s, size_type pos = 0) const
    {
        return _Base::find_first_of(s, pos);
    }

    size_type find_first_of(_Elem c, size_type pos = 0) const
    {
        return _Base::find_first_of(c, pos);
    }

    // find last of operations.
    size_type find_last_of(const _This &str,
        size_type pos = npos) const
    {
        return _Base::find_last_of(str, pos);
    }

    size_type find_last_of(const _Elem *s, size_type pos,
        size_type n) const
    {
        return _Base::find_last_of(s, pos, n);
    }

    size_type find_last_of(const _Elem *s, size_type pos = npos) const
    {
        return _Base::find_last_of(s, pos);
    }

    size_type find_last_of(_Elem c, size_type pos = npos) const
    {
        return _Base::find_last_of(c, pos);
    }

    size_type find_first_not_of(const _This &str,
        size_type pos = 0) const
    {
        return _Base::find_first_not_of(str, pos);
    }

    size_type find_first_not_of(const _Elem *s, size_type pos,
        size_type n) const
    {
        return _Base::find_first_not_of(s, pos, n);
    }

    size_type find_first_not_of(const _Elem *s, size_type pos = 0) const
    {
        return _Base::find_first_not_of(s, pos);
    }

    size_type find_first_not_of(_Elem c, size_type pos = 0) const
    {
        return _Base::find_first_not_of(c, pos);
    }

    size_type find_last_not_of(const _This &str,
        size_type pos = npos) const
    {
        return _Base::find_last_not_of(str, pos);
    }

    size_type find_last_not_of(const _Elem *s, size_type pos, 
        size_type n) const
    {
        return _Base::find_last_not_of(s, pos, n);
    }

    size_type find_last_not_of(const _Elem *s, size_type pos = npos) const
    {
        return _Base::find_last_not_of(s, pos);
    }

    size_type find_last_not_of(_Elem c, size_type pos = npos) const
    {
        return _Base::find_last_not_of(c, pos);
    }

    // substr operations.
    _This substr(size_type pos = 0, size_type n = npos) const
    {
        _Base ret = _Base::substr(pos, n);
        return _This(ret.c_str());
    }

    // tolower/toupper operations.
    _This tolower() const
    {
        const _Elem *buf = this->data();
        const size_type size = this->size();

        _This lower;
        lower.resize(size);
        for (register size_type i = 0; i < size; i++)
            if (buf[i] >= 0x41 && buf[i] <= 0x5A)
                lower[i] = buf[i] + 0x20;
            else
                lower[i] = buf[i];

        return lower;
    }

    _This toupper() const
    {
        const _Elem *buf = this->data();
        const size_type size = this->size();

        _This upper;
        upper.resize(size);
        for (register size_type i = 0; i < size; i++)
            if (buf[i] >= 0x61 && buf[i] <= 0x7a)
                upper[i] = buf[i] - 0x20;
            else
                upper[i] = buf[i];

        return upper;
    }

    // compare operations.
    int compare(const _This &str) const
    {
        return _Base::compare(str);
    }

    int compare(size_type p0, size_type n0,
        const _This &str)
    {
        return _Base::compare(p0, n0, str);
    }

    int compare(size_type p0, size_type n0,
        const _This &str, size_type pos, size_type n) const
    {
        return _Base::compare(p0, n0, str, pos, n);
    }

    int compare(const _Elem *s) const
    {
        return _Base::compare(s);
    }

    int compare(size_type p0, size_type n0,
        const _Elem *s) const
    {
        return _Base::compare(p0, n0, s);
    }

    int compare(size_type p0, size_type n0,
        const _Elem *s, size_type pos) const
    {
        return _Base::compare(p0, n0, s, pos);
    }

    int compare(const _Elem &c) const
    {
        size_type size = this->size();
        if (size == 0)
            return -1;

        _Elem thisC = this->at(0);
        return (thisC > c ? 1 : 
                    (thisC < c ? -1 :  
                        (size == 1 ? 0 : 1)));
    }

    // isalpha/isupper/islower
    static bool isalpha(const _Elem &c)
    {
        if (sizeof(_Elem) == 1)
        {
            return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
        }
        else if (sizeof(_Elem) == 2)
        {
            char ch = static_cast<char>(c);
            return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
        }
        else
        {
            return false;
        }
    }

    static bool isalpha(const _This &s)
    {
        if (s.empty())
            return false;

        for (size_t i = 0; i < s.size(); i++)
        {
            if (!isalpha(s[i]))
                return false;
        }

        return true;
    }

    bool isalpha() const
    {
        return isalpha(*this);
    }

    static bool islower(const _Elem &c)
    {
        if (sizeof(_Elem) == 1)
        {
            return 'a' <= c && c <= 'z';
        }
        else if (sizeof(_Elem) == 2)
        {
            char ch = static_cast<char>(c);
            return 'a' <= ch && ch <= 'z';
        }
        else
        {
            return false;
        }
    }

    static bool islower(const _This &s)
    {
        if (s.empty())
            return false;

        bool foundLower = false;
        for (size_type i = 0; i < s.size(); i++)
        {
            if (isupper(s[i]))
                return false;
            else if (islower(s[i]))
                foundLower = true;
        }

        return foundLower;
    }

    bool islower() const
    {
        return islower(*this);
    }

    static bool isupper(const _Elem &c)
    {
        if (sizeof(_Elem) == 1)
        {
            return 'A' <= c && c <= 'Z';
        }
        else if (sizeof(_Elem) == 2)
        {
            char ch = static_cast<char>(c);
            return 'A' <= ch && ch <= 'Z';
        }
        else
        {
            return false;
        }
    }

    static bool isupper(const _This &s)
    {
        if (s.empty())
            return false;

        bool foundUpper = false;
        for (size_type i = 0; i < s.size(); i++)
        {
            if (islower(s[i]))
                return false;
            else if (isupper(s[i]))
                foundUpper = true;
        }

        return foundUpper;
    }

    bool isupper() const
    {
        return isupper(*this);
    }

    // isdigit
    static bool isdigit(const _Elem &c)
    {
        if (sizeof(_Elem) == 1)
        {
            return '0' <=c && c <= '9';
        }
        else if (sizeof(_Elem) == 2)
        {
            char ch = reinterpret_cast<char>(c);
            return '0' <= ch && ch <= '9';
        }
        else
        {
            return false;
        }
    }

    static bool isdigit(const _This &s)
    {
        if (s.empty())
            return false;

        for (size_type i = 0; i < s.size(); i++)
        {
            if (!isdigit(s[i]))
                return false;
        }

        return true;
    }

    bool isdigit() const
    {
        return isdigit(*this);
    }

    // isspace: space[' '], carriage return['\r'], line feed['\n'], form feed['\f'], horizontal tab['\t'], vertical tab['\v']
    static bool isspace(const _Elem &c)
    {
        if (sizeof(_Elem) == 1)
        {
            return  c == ' ' || c == '\t' || c == '\v' || c == '\r' || c == '\n' || c == '\f';
        }
        else if (sizeof(_Elem) == 2)
        {
            char ch = reinterpret_cast<char>(c);
            return  ch == ' ' || ch == '\t' || ch == '\v' || ch == '\r' || ch == '\n' || ch == '\f';
        }
        else
        {
            return false;
        }
    }

    static bool isspace(const _This &s)
    {
        if (s.empty())
            return false;

        for (size_type i = 0; i < s.size(); i++)
        {
            if (!isspace((*s)[i]))
                return false;
        }

        return false;
    }

    bool isspace() const
    {
        return isspace(*this);
    }

public:
    friend std::basic_ostream<_Elem> &operator <<(std::basic_ostream<_Elem> &o, const _This &str)
    {
        o.write(str.data(), str.size());
        return o;
    }

public:
    // format operation: format
    _This &format(const _Elem *fmt, ...)
    {
        if (UNLIKELY(sizeof(_Elem) != sizeof(char)))
            return *this;

        this->clear();

        char *buf; int len;
        LLBC_FormatArg(fmt, buf, len);

        this->append(reinterpret_cast<const _Elem *>(buf), len);

        LLBC_Free(buf);

        return *this;
    }

    // format operation: append format
    _This &append_format(const _Elem *fmt, ...)
    {
        if (UNLIKELY(sizeof(_Elem) != sizeof(char)))
            return *this;

        char *buf; int len;
        LLBC_FormatArg(fmt, buf, len);

        this->append(reinterpret_cast<const _Elem *>(buf), len);

        LLBC_Free(buf);

        return *this;
    }

public:
    // strip operation: strip left. 
    _This &lstrip(const _This &chars = _This())
    {
        _This willStripChars;
        if (chars.empty())
        {
            if (sizeof(_Elem) == 1)
                willStripChars.append(reinterpret_cast<const _Elem *>(" \t\v\r\n\f"));
            else if (sizeof(_Elem) == 2)
                willStripChars.append(reinterpret_cast<const _Elem *>(L" \t\v\r\n\f"));
            else
                return *this;
        }

        _This &thisRef = *this;
        _This::size_type stripTo = 0;
        for (_This::size_type i = 0; i != thisRef.size(); i++)
        {
            bool found = false;
            const _Elem &now = thisRef[i];
            for (_This::size_type j = 0; j != willStripChars.size(); j++)
            {
                if (now == willStripChars[j])
                {
                    found = true;
                    break;
                }
            }

            if (found)
                stripTo = i + 1;
            else
                break;
        }

        if (stripTo != 0)
            this->erase(0, stripTo);

        return *this;
    }

    _This lstrip(const _This &chars = _This()) const
    {
        _This copyThis(*this);
        return copyThis.lstrip(chars);
    }

    // strip operation: strip right.
    _This &rstrip(const _This &chars = _This())
    {
        _This willStripChars;
        if (chars.empty())
        {
            if (sizeof(_Elem) == 1)
                willStripChars.append(reinterpret_cast<const _Elem *>(" \t\v\r\n\f"));
            else if (sizeof(_Elem) == 2)
                willStripChars.append(reinterpret_cast<const _Elem *>(L" \t\v\r\n\f"));
            else
                return *this;
        }

        _This &thisRef = *this;
        const long thisSize = static_cast<long>(thisRef.size());

        long stripFrom = thisSize;
        for (long i = thisSize - 1; i >= 0; i--)
        {
            bool found = false;
            const _Elem &now = thisRef[i];
            for (_This::size_type j = 0; j != willStripChars.size(); j++)
            {
                if (now == willStripChars[j])
                {
                    found = true;
                    break;
                }
            }

            if (found)
                stripFrom = i;
            else
                break;
        }

        if (stripFrom != thisSize)
            thisRef.erase(stripFrom);

        return thisRef;
    }

    _This rstrip(const _This &chars = _This()) const
    {
        _This copyThis(*this);
        return copyThis.rstrip(chars);
    }

    // strip operation: trip left & trip right
    _This &strip(const _This &chars = _This())
    {
        return this->lstrip(chars).rstrip(chars);
    }

    _This strip(const _This &chars = _This()) const
    {
        _This copyThis(*this);
        return copyThis.lstrip(chars).rstrip(chars);
    }

public:
	// escape support: escape string
	_This &escape(const _This &willbeEscapeChars, const _Elem &escapeChar)
	{
		if (this->empty())
			return *this;

		const long len = static_cast<long>(this->size());
		for (long i = len - 1; i >= 0; i--)
		{
			const _Elem &ch = (*this)[i];
			if (ch == escapeChar || 
				willbeEscapeChars.find(ch) != _This::npos)
				this->insert(i, 1, escapeChar);
		}

		return *this;
	}

	_This escape(const _This &willbeEscapeChars, const _Elem &escapeChar) const
	{
		if (this->empty())
			return *this;

		return _This(*this).escape(willbeEscapeChars, escapeChar);
	}

	// escape support: unescape string
	_This &unescape(const _Elem &escapeChar)
	{
        if (this->empty())
            return *this;

		const long len = static_cast<long>(this->size());
        for (long i = len - 1; i >= 0; i--)
        {
            const _Elem &ch = (*this)[i];
            if (ch == escapeChar)
            {
                if (i > 0 && (*this)[i - 1] == escapeChar)
                    this->erase(i--, 1);
                else
                    this->erase(i, 1);
            }
        }

        return *this;
	}

    _This unescape(const _Elem &escapeChar) const
    {
        return _This(*this).unescape(escapeChar);
    }

public:
    // utf8 support: calculate utf8 length.
    size_type length_with_utf8() const
    {
        if (UNLIKELY(sizeof(_Elem) != sizeof(char)))
            return 0;

        size_type count = 0;
        size_type bytePos = 0;
        while ((bytePos = _This::next_utf8_char_pos(bytePos)) != npos)
            count++;

        return count;
    }

    // utf8 support: substring with utf8.
    _This substr_with_utf8(size_type pos = 0, size_type n = npos) const
    {
        if (UNLIKELY(sizeof(_Elem) != sizeof(char)))
            return _This();

        size_type utf8Len = this->length_with_utf8();
        if (pos >= utf8Len || n == 0)
            return _This();

        _These substrs;
        this->split_utf8_string(static_cast<long>(pos), substrs);
        if (substrs.empty())
            return _This();

        _This str1 = *substrs.rbegin();
        utf8Len = str1.length_with_utf8();
        pos = (n == npos || n > utf8Len) ? utf8Len : n;

        substrs.clear();
        str1.split_utf8_string(static_cast<long>(pos), substrs);
        if (substrs.empty())
            return _This();

        return substrs[0];
    }

    // utf8 support: split utf8 string.
    void split_utf8_string(long charIndex, _These &strs) const
    {
        strs.clear();
        if (UNLIKELY(sizeof(_Elem) != sizeof(char)))
        {
            strs.push_back(*this);
            return;
        }
        else if (charIndex == 0)
        {
            strs.push_back(*this);
            return;
        }

        size_type utf8Count = _This::length_with_utf8();
        if (UNLIKELY(utf8Count == npos))
        {
            strs.push_back(*this);
            return;
        }

        charIndex = (charIndex < 0) ? 
            static_cast<long>(utf8Count) + charIndex : charIndex;
        if (charIndex <= 0 || charIndex >= static_cast<long>(utf8Count))
        {
            strs.push_back(*this);
            return;
        }

        size_type bytePos = 0;
        size_type charPos = 0;
        while (static_cast<long>(charPos) != charIndex)
        {
            bytePos = _This::next_utf8_char_pos(bytePos);
            charPos++;
        }

        strs.push_back(_This::substr(0, bytePos));
        strs.push_back(_This::substr(bytePos));

        return;

        strs.push_back(*this);
    }

    // utf8 support: scatter utf8 string.
    void scatter_utf8_string(_These &chars, size_type scatterCount = 0) const
    {
        chars.clear();

        if (scatterCount == 0)
            scatterCount = npos;
        else if (scatterCount != npos)
            scatterCount -= 1;

        if (scatterCount == 0)
        {
            chars.push_back(*this);
            return;
        }

        size_type curPos = 0;
        size_type prevPos = 0;
        size_type curScatterCount = 0;
        while ((curPos = this->next_utf8_char_pos(prevPos)) != npos)
        {
            chars.push_back(_This::substr(prevPos, curPos - prevPos));

            if (scatterCount != npos && ++curScatterCount >= scatterCount)
            {
                chars.push_back(_This::substr(curPos));
                break;
            }

            prevPos = curPos;
        }
    }

    // utf8 support: check string has utf8 bomb.
    bool has_utf8_bomb() const
    {
        if (sizeof(_Elem) != sizeof(char) || _This::size() < 3)
            return false;
        
        return (::memcmp(reinterpret_cast<const char *>(this->data()), 
            reinterpret_cast<const char *>("\xef\xbb\xbf"), 3) == 0) ? true : false;

    }

    // utf8 support: add utf8 bomb.
    void add_utf8_bomb()
    {
        if (sizeof(_Elem) == sizeof(char) && !this->has_utf8_bomb())
            this->insert(0, reinterpret_cast<const _Elem *>("\xef\xbb\xbf"));
    }

    // utf8 support: trim utf8 bomb.
    void trim_utf8_bomb()
    {
        if (this->has_utf8_bomb())
            _Base::erase(0, 3);
    }

public:
    void Serialize(LLBC_Stream &stream) const
    {
        if (!_Base::empty())
            stream.WriteBuffer(_Base::data(), _Base::size() * sizeof(_Elem));

        const _Elem nullElem = _Elem(0);
        stream.WriteBuffer(&nullElem, sizeof(_Elem));
    }

    bool DeSerialize(LLBC_Stream &stream)
    {
        _Base::clear();

        if (stream.GetSize() - stream.GetPos() == 0)
            return false;

        const _Elem nullElem = _Elem(0);

        _Elem elem = _Elem();
        while (stream.Read(elem))
        {
            if (elem == nullElem)
                return true;

            this->append(1, elem);
        }

        return true;
    }

    void SerializeEx(LLBC_Stream &stream) const
    {
        LLBC_STREAM_BEGIN_WRITE(stream);

        LLBC_STREAM_WRITE_EX(static_cast<uint32>(this->size() * sizeof(_Elem)));
        LLBC_STREAM_WRITE_BUF(this->data(), this->size() * sizeof(_Elem));

        LLBC_STREAM_END_WRITE();
    }

    bool DeSerializeEx(LLBC_Stream &stream)
    {
        LLBC_STREAM_BEGIN_READ(stream, bool, false);

        uint32 len = 0;
        LLBC_STREAM_READ_EX(len);

        this->resize(len);
        LLBC_STREAM_READ_BUF(const_cast<_Elem *>(this->data()), this->size() * sizeof(_Elem));

        LLBC_STREAM_END_READ_RET(true);
    }

private:
    size_type next_utf8_char_pos(size_type &beginBytePos) const
    {
        if (sizeof(_Elem) != sizeof(char))
            return npos;

        if (beginBytePos == 0 && this->has_utf8_bomb())
            beginBytePos += 3;

        if (beginBytePos == npos || beginBytePos >= this->size())
            return npos;

        size_type waitCheckCount = npos;

        // 0xxx xxxx
        // Encoding len: 1 byte.
        uint8 ch = static_cast<uint8>(_Base::at(beginBytePos));
        if ((ch & 0x80) == 0x00)
            waitCheckCount = 0;
        // 110x xxxx
        // Encoding len: 2 bytes.
        else if ((ch & 0xe0) == 0xc0)
            waitCheckCount = 1;
        // 1110 xxxx
        // Encoding len: 3 bytes.
        else if ((ch & 0xf0) == 0xe0)
            waitCheckCount = 2;
        // 1111 0xxx
        // Encoding len: 4 bytes.
        else if ((ch & 0xf8) == 0xf0)
            waitCheckCount = 3;
        // 1111 10xx
        // Encoding len: 5 bytes.
        else if ((ch & 0xfc) == 0xf8)
            waitCheckCount = 4;
        // 1111 110x
        // Encoding len: 6 bytes.
        else if ((ch & 0xfe) == 0xfc)
            waitCheckCount = 5;

        if (waitCheckCount == npos)
            return npos;

        size_type curPos = beginBytePos + 1;
        size_type endPos = curPos + waitCheckCount;
        if (endPos > _This::size())
            return npos;

        for (; curPos != endPos; curPos++)
        {
            ch = static_cast<uint8>(_Base::at(curPos));
            if ((ch & 0xc0) != 0x80)
                return npos;
        }

        return endPos;
    }
};

__LLBC_NS_END

#endif // !__LLBC_COM_BASIC_STRING_H__
