/**
* @file	IniImpl.h
* @author	Longwei Lai<lailongwei@126.com>
* @date	2016/08/21
* @version	1.0
*
* @brief
*/
#ifdef __LLBC_CORE_CONFIG_INI_H__

__LLBC_NS_BEGIN

template <>
inline LLBC_Variant LLBC_IniSection::GetValue(const LLBC_String &key, LLBC_Variant defaultValue) const
{
    _Values::const_iterator it = _values.find(key);
    if (it == _values.end())
    {
        LLBC_SetLastError(LLBC_ERROR_NOT_FOUND);
        return defaultValue;
    }

    LLBC_SetLastError(LLBC_ERROR_SUCCESS);
    return it->second;
}

template <>
inline bool LLBC_IniSection::GetValue(const LLBC_String &key, bool defaultValue) const
{
    return this->GetValue(key, LLBC_Variant(defaultValue));
}

template <>
inline sint8 LLBC_IniSection::GetValue(const LLBC_String &key, sint8 defaultValue) const
{
    return this->GetValue(key, LLBC_Variant(defaultValue));
}

template <>
inline uint8 LLBC_IniSection::GetValue(const LLBC_String &key, uint8 defaultValue) const
{
    return this->GetValue(key, LLBC_Variant(defaultValue));
}

template <>
inline sint16 LLBC_IniSection::GetValue(const LLBC_String &key, sint16 defaultValue) const
{
    return this->GetValue(key, LLBC_Variant(defaultValue));
}

template <>
inline uint16 LLBC_IniSection::GetValue(const LLBC_String &key, uint16 defaultValue) const
{
    return this->GetValue(key, LLBC_Variant(defaultValue));
}

template <>
inline sint32 LLBC_IniSection::GetValue(const LLBC_String &key, sint32 defaultValue) const
{
    return this->GetValue(key, LLBC_Variant(defaultValue));
}

template <>
inline uint32 LLBC_IniSection::GetValue(const LLBC_String &key, uint32 defaultValue) const
{
    return this->GetValue(key, LLBC_Variant(defaultValue));
}

template <>
inline sint64 LLBC_IniSection::GetValue(const LLBC_String &key, sint64 defaultValue) const
{
    return this->GetValue(key, LLBC_Variant(defaultValue));
}

template <>
inline uint64 LLBC_IniSection::GetValue(const LLBC_String &key, uint64 defaultValue) const
{
    return this->GetValue(key, LLBC_Variant(defaultValue));
}

template <>
inline float LLBC_IniSection::GetValue(const LLBC_String &key, float defaultValue) const
{
    return this->GetValue(key, LLBC_Variant(defaultValue));
}

template <>
inline double LLBC_IniSection::GetValue(const LLBC_String &key, double defaultValue) const
{
    return this->GetValue(key, LLBC_Variant(defaultValue));
}

template <>
inline LLBC_String LLBC_IniSection::GetValue(const LLBC_String &key, LLBC_String defaultValue) const
{
    return this->GetValue(key, LLBC_Variant(defaultValue));
}

template <>
inline std::string LLBC_IniSection::GetValue(const LLBC_String &key, std::string defaultValue) const
{
    return this->GetValue(key, LLBC_Variant(LLBC_String(defaultValue))).AsCStr();
}

template <typename _ValueType>
inline _ValueType LLBC_IniSection::GetValue(const LLBC_String &key, _ValueType defaultValue) const
{
    LLBC_SetLastError(LLBC_ERROR_NOT_IMPL);
    return defaultValue;
}

template <>
inline int LLBC_IniSection::SetValue(const LLBC_String &key, const LLBC_Variant &value, const LLBC_String &comment)
{
    if (key.empty())
    {
        LLBC_SetLastError(LLBC_ERROR_ARG);
        return LLBC_FAILED;
    }

    _Values::iterator valIt = _values.find(key);
    if (valIt != _values.end())
        _values.erase(key);
    else
        _keys.push_back(key);

    _values.insert(std::make_pair(key, value));

    _comments.erase(key);
    _comments.insert(std::make_pair(key, comment));

    return LLBC_OK;
}

template <typename _ValueType>
inline int LLBC_IniSection::SetValue(const LLBC_String &key, const _ValueType &value, const LLBC_String &comment)
{
    return this->SetValue(key, LLBC_Variant(value), comment);
}

inline void LLBC_Ini::EndLine(LLBC_String &str)
{
#if LLBC_TARGET_PLATFORM_WIN32
        str.append(LLBC_CRLF_A);
#elif LLBC_TARGET_PLATFORM_IPHONE || LLBC_TARGET_PLATFORM_MAC
        str.append(1, LLBC_CR_A);
#else
        str.append(1, LLBC_LF_A);
#endif
}

template <typename _ValueType>
inline _ValueType LLBC_Ini::GetValue(const LLBC_String &sectionName,
                            const LLBC_String &key,
                            _ValueType defaultValue) const
{
    const LLBC_IniSection *section = GetSection(sectionName);
    if (section == NULL)
        return defaultValue;

    return section->GetValue<_ValueType>(key, defaultValue);
}

template <typename _ValueType>
inline int LLBC_Ini::SetValue(const LLBC_String &sectionName,
                              const LLBC_String &key,
                              const _ValueType &value,
                              const LLBC_String &comment)
{
    LLBC_IniSection *section = GetSection(sectionName);
    if (section == NULL)
        return LLBC_FAILED;

    return section->SetValue<_ValueType>(key, value, comment);
}

__LLBC_NS_END

#endif // __LLBC_CORE_CONFIG_INI_H__
