/**
 * @file    Config.h
 * @author  Longwei Lai<lailongwei@126.com>
 * @date    2013/04/29
 * @version 1.0
 *
 * @brief
 */
#ifndef __LLBC_CORE_CONFIG_CONFIG_H__
#define __LLBC_CORE_CONFIG_CONFIG_H__

#include "llbc/common/Common.h"

#include "llbc/core/json/json.h"

#include "llbc/core/variant/Variant.h"

__LLBC_NS_BEGIN

/**
 * \brief Config class simplifies the operations for json file.
 */
class LLBC_EXPORT LLBC_Config
{
public:
    LLBC_Config();
    ~LLBC_Config();

public:
    /**
     * Add json file the config.
     * @param[in] file - json file.
     * @return int - return 0 if successed, otherwise return -1, see last error.
     */
    int AddFile(const LLBC_String &file);

    /**
     * Parse and load all json files in the config.
     * @return int - return 0 if successed, otherwise return -1.
     */
    int Initialize();

    /**
     * Get error describe, when Initialize() return -1, can call
     * This method to obtain error describe.
     * @return const LLBC_String & - error describe.
     */
    const LLBC_String &GetError() const;

public:
    /**
     * Get json value.
     * @param[in] file  - file name.
     * @param[in] field - field name.
     * @return LLBC_JsonValue - the json value.
     */
    LLBC_JsonValue GetJsonValue(const LLBC_String &file, const char *field = NULL) const;

    /**
    * Get the field as std::vector<LLBC_JsonValue>.
     * @param[in] file  - file name.
     * @param[in] field - field name.
     * @return std::vector<LLBC_JsonValue> - json value array.
     */
    std::vector<LLBC_JsonValue> GetJsonValueArray(const LLBC_String &file, const char *field = NULL) const;

    /**
     * Get the field as std::map<int, LLBC_JsonValue>.
     * @param[in] file  - file name.
     * @param[in] field - field name.
     * @return std::map<int, LLBC_JsonValue> - value.
     */
    std::map<int, LLBC_JsonValue> GetIntJsonMap(const LLBC_String &file, const char *field = NULL) const;

    /**
     * Get the field as std::map<LLBC_String, LLBC_JsonValue>.
     * @param[in] file  - file name.
     * @param[in] field - field name.
     * @return std::map<LLBC_String, LLBC_JsonValue> - value.
     */
    std::map<LLBC_String, LLBC_JsonValue> GetStringJsonMap(const LLBC_String &file, const char *field = NULL) const;

    /**
     * Get variant value.
     * @param[in] file  - file name.
     * @param[in] field - field name.
     * @return LLBC_Variant - variant value.
     */
    LLBC_Variant GetVariantValue(const LLBC_String &file, const char *field) const;

    /**
     * Get the field as std::vector<LLBC_Variant>.
     * @param[in] file  - file name.
     * @param[in] field - field name.
     * @return std::vector<LLBC_Variant> - value.
     */
    std::vector<LLBC_Variant> GetVariantValueArray(const LLBC_String &file, const char *field = NULL) const;

    /**
     * Get the field as std::map<int, LLBC_Variant>.
     * @param[in] file  - file name.
     * @param[in] field - field name.
     * @return std::map<int, LLBC_Variant> - value.
     */
    std::map<int, LLBC_Variant> GetIntVariantMap(const LLBC_String &file, const char *field = NULL) const;

    /**
     * Get the field as std::map<LLBC_String, LLBC_Variant>
     * @param[in] file  - file name.
     * @param[in] field - field name.
     * @return std::map<LLBC_String, LLBC_Variant> - value.
     */
    std::map<LLBC_String, LLBC_Variant> GetStringVariantMap(const LLBC_String &file, const char *field = NULL) const;

private:
    /**
     * Build error message.
     * @param[in] file - file name.
     * @param[in] err  - error describe.
     */
    void BuildErrMsg(const LLBC_String &file, const LLBC_String &err);

private:
    LLBC_String _errMsg;
    std::map<LLBC_String, LLBC_JsonValue *> _values;
};

__LLBC_NS_END

#endif // !__LLBC_CORE_CONFIG_CONFIG_H__
