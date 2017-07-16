--  @file   premake5.lua
--  @author Longwei Lai<lailongwei@126.com>
--  @brief  The llbc library(included all wrap libraries) premake script define.

-- #########################################################################
-- Global compile settings

-- python tool define
local PY = string.find(_ACTION, "vs") and "$(ProjectDir)../../tools/py.exe" or "python"

-- All libraries output directory
local LLBC_OUTPUT_DIR = "../../output/" .. _ACTION

-- Some third party libraries paths define
-- Python library: format [1]: include path, [2]: lib path [3]: lib name
local PYTHON_LIB = { "/usr/local/include/python2.7", "/usr/local/lib", "python2.7" }

-- zlib library:
local ZLIB_LIB = "../../llbc/3rd_party/zlib"
-- #########################################################################

workspace ("llbc_" .. _ACTION)
    -- location define
    location ("../../build/" .. _ACTION)
    -- target directory define
    targetdir (LLBC_OUTPUT_DIR)

    -- configurations
    configurations { "release32", "debug32" }

    -- defines
    filter { "configurations:debug*" }
        defines {
            "DEBUG"
        }

    -- control symbols
    filter { "system:macosx", "language:c++" }
        symbols("On")

    -- optimize
    filter { "configurations:debug*", "language:c++", "system:not windows" }
        buildoptions {
            "-ggdb -g",
        }
    filter { "configurations:debug*", "language:not c++" }
        optimize "Debug"
    filter { "configurations:release*" }
        optimize "On"

    -- architecture
    filter { "system:linux" }
        architecture "x64"
    filter { "system:windows" }
        architecture "x86"
    filter { "system:macosx" }
        architecture "x86"

    -- characterset
    filter { "language:c++" }
        characterset "MBCS"

-- ****************************************************************************
-- llbc core library compile setting
project "llbc"
    -- language, kind
    language "c++"
    kind "SharedLib"

    -- files
    files {
        "../../llbc/**.h",
        "../../llbc/**.c",
        "../../llbc/**.cpp",
    }
    filter { "system:macosx" }
    files {
        "../../llbc/**.mm",
    }

    -- includedirs
    filter {}
    includedirs {
        ZLIB_LIB .. "/include",
        "../../llbc/include",
    }

    -- target prefix
    targetprefix "lib"

    -- links
    filter { "system:linux" }
        libdirs {
            ZLIB_LIB .. "/lib/linux"
        }
        links {
            "rt",
            "uuid",
        }

    filter { "system:windows" }
        libdirs {
            ZLIB_LIB .. "/lib/win"
        }
        links {
            "ws2_32",
            "Mswsock",
        }
    filter { "system:windows", "configurations:debug*", "architecture:x86" }
        links {
            "zlibwapi_debug",
        }
    filter { "system:windows", "configurations:release*", "architecture:x86" }
        links {
            "zlibwapi",
        }
    filter { "system:windows", "configurations:debug*", "architecture:x64" }
        links {
            "zlibwapi_debug_64",
        }
    filter { "system:windows", "configurations:release*", "architecture:x64" }
        links {
            "zlibwapi_64",
        }

    filter { "system:macosx" }
        links {
            "iconv",
        }

    -- flags
    filter { "system:not windows" }
        buildoptions {
            "-fvisibility=hidden",
        }

    -- debug target suffix define
    filter { "configurations:debug*" }
        targetsuffix "_debug"


-- ****************************************************************************
-- core library testsuite compile setting
project "testsuite"
    -- language, kind
    language "c++"
    kind "ConsoleApp"

    -- dependents
    dependson {
        "llbc",
    }

    -- files
    files {
        "../../testsuite/**.h",
        "../../testsuite/**.cpp",
    }

    -- includedirs
    includedirs {
        "../../llbc/include",
        "../../testsuite",
    }

    -- links
    libdirs { LLBC_OUTPUT_DIR }
    filter { "system:linux" }
        links {
            "dl",
        }
    filter { "system:not windows", "configurations:debug*" }
        links {
            "llbc_debug",
        }
    filter { "system:not windows", "configurations:release*" }
        links {
            "llbc",
        }

    filter { "system:windows" }
        links {
            "ws2_32",
        }
    filter { "system:windows", "configurations:debug*" }
        links {
            "libllbc_debug",
        }
    filter { "system:windows", "configurations:release*" }
        links {
            "libllbc",
        }

    -- debug target suffix define
    filter { "configurations:debug*" }
        targetsuffix "_debug"

    -- warnings
    filter { "system:not windows" }
        disablewarnings {
            "invalid-source-encoding",
        }

-- ****************************************************************************
-- python wrap library(pyllbc) compile setting
project "pyllbc"
    -- language, kind
    language "c++"
    kind "SharedLib"

    -- dependents
    dependson {
        "llbc",
    }

    -- files
    files {
        "../../wrap/pyllbc/**.h",
        "../../wrap/pyllbc/**.cpp",
    }

    -- includedirs
    includedirs {
        PYTHON_LIB[1],

        "../../wrap/pyllbc/Python2.7.8/Include",

        "../../llbc/include",

        "../../wrap/pyllbc/include",
        "../../wrap/pyllbc",
    }

    -- some defines
    filter { "action:vs2013 or vs2015 or vs2017" }
    defines { "HAVE_ROUND" }

    -- prebuild commands
    filter {}
    prebuildcommands {
        PY .. " ../../tools/building_script/py_prebuild.py pyllbc",
    }

    -- target name, target prefix, extension
    targetname "llbc"
    targetprefix ""
    filter { "system:windows" }
        targetextension ".pyd"
    filter {}

    -- links 
    libdirs { 
        LLBC_OUTPUT_DIR,
    }
    filter { "system:linux" }
        libdirs {
            PYTHON_LIB[2],
        }
        links {
            PYTHON_LIB[3],
        }
    filter { "system:linux", "configurations:debug*" }
        links {
            "llbc_debug",
        }
    filter { "system:linux", "configurations:release*" }
        links {
            "llbc",
        }

    filter { "system:windows", "architecture:x86" }
        libdirs {
            "../../wrap/pyllbc/Python2.7.8/Libs/Win/32",
        }
    filter { "system:windows", "architecture:x64" }
        libdirs {
            "../../wrap/pyllbc/Python2.7.8/Libs/Win/64",
        }

    filter { "system:windows", "configurations:debug*" }
        links {
            "python27_d",
            "libllbc_debug",
        }
    filter { "system:windows", "configurations:release*" }
        links {
            "python27",
            "libllbc",
        }

    -- flags
    filter { "system:not windows" }
        buildoptions {
            "-fvisibility=hidden",
        }

    -- debug target suffix define
    filter { "configurations:debug" }
        targetsuffix "_debug"

-- ****************************************************************************
-- csharp wrap library(csllbc) native library compile setting
project "csllbc_native"
    -- language, kind
    language "c++"
    kind "SharedLib"

    -- dependents
    dependson {
        "llbc",
    }

    -- library suffix
    targetprefix "lib"

    -- files
    files {
        "../../wrap/csllbc/native/**.h",
        "../../wrap/csllbc/native/**.cpp",
    }

    -- includedirs
    includedirs {
        "../../llbc/include",
        "../../wrap/csllbc/native/include",
    }

    -- links
    libdirs {
        LLBC_OUTPUT_DIR,
    }
    filter { "system:windows", "configurations:debug*" }
        links {
            "libllbc_debug",
        }
    filter { "system:windows", "configurations:release*" }
        links {
            "libllbc",
        }
    filter { "system:not windows", "configurations:debug*" }
        links {
            "llbc_debug",
        }
    filter { "system:not windows", "configurations:release*" }
        links {
            "llbc",
        }

    -- flags
    filter { "system:not windows" }
        buildoptions {
            "-fvisibility=hidden",
        }

    -- debug target suffix define
    filter { "configurations:debug*" }
        targetsuffix "_debug"

    -- disable warnings
    filter { "system:not windows" }
        disablewarnings {
            "attributes"
        }

-- ****************************************************************************
-- csharp wrap library(csllbc) compile setting
project "csllbc"
    -- language, kind
    kind "SharedLib"
    language "c#"

    -- files
    files {
        "../../wrap/csllbc/csharp/**.cs",
    }

    -- dependents
    dependson {
        "llbc",
        "csllbc_native",
    }

    -- set unsafe flag
    clr "Unsafe"

    -- prebuild commands
    prebuildcommands {
        PY .. " ../../wrap/csllbc/csharp/script_tools/gen_native_code.py",
        PY .. " ../../wrap/csllbc/csharp/script_tools/gen_errno_code.py",
    }

    -- postbuild commands
    filter { 'system:not windows' }
        postbuildcommands {
            PY .. " ../../wrap/csllbc/csharp/script_tools/gen_dll_cfg.py ../../output/" .. _ACTION,
        }

    -- defines
    filter { "system:linux" }
        defines {
            "CSLLBC_TARGET_PLATFORM_LINUX",
        }
    filter { "system:windows" }
        defines {
            "CSLLBC_TARGET_PLATFORM_WIN32",
        }

    -- links
    filter {}
    links {
        "System",
        "System.Net",
        "System.Core",
    }

-- ****************************************************************************
-- csharp wrap library(csllbc) testsuite compile setting
project "csllbc_testsuite"
    -- language, kind
    kind "ConsoleApp"
    language "c#"

    -- dependents
    dependson {
        "llbc",
        "csllbc_native",
        "csllbc",
    }

    -- files
    files {
        "../../wrap/csllbc/testsuite/**.cs",
    }

    -- links
    links {
        "System",
        "System.Net",
        "System.Core",
        "csllbc",
    }


-- ****************************************************************************
-- luasrc library(liblua) compile setting
local LUA_SRC_PATH = "../../wrap/lullbc/lua"
project "lullbc_lualib"
    -- language, kind
    language "c++"
    kind "SharedLib"

    -- files
    files {
        LUA_SRC_PATH .. "/*.h",
        LUA_SRC_PATH .. "/*.c",
    }
    removefiles {
        LUA_SRC_PATH .. "/lua.c",
        LUA_SRC_PATH .. "/luac.c",
    }

    -- defines
    defines {
        "LUA_COMPAT_5_1",
        "LUA_COMPAT_5_2",
    }
    filter { "system:windows" }
        defines { "LUA_BUILD_AS_DLL" }
    filter {}

    -- target name, target prefix
    targetname "lua"
    targetprefix "lib"

    -- debug target suffix define
    filter { "configurations:debug" }
        targetsuffix "_debug"

-- lua executable compile setting
local LUA_SRC_PATH = "../../wrap/lullbc/lua"
project "lullbc_luaexec"
    -- language, kind
    language "c++"
    kind "ConsoleApp"

    -- files
    files {
        LUA_SRC_PATH .. "/*.h",
        LUA_SRC_PATH .. "/lua.c",
    }

    -- defines
    defines {
        "LUA_COMPAT_5_1",
        "LUA_COMPAT_5_2",
    }

    -- dependents
    dependson {
        "lullbc_lualib"
    }

    -- links 
    libdirs { 
        LLBC_OUTPUT_DIR,
    }
    filter { "configurations:debug*", "system:windows" }
        links { "liblua_debug" }
    filter {}
    filter { "configurations:release*", "system:windows" }
        links { "liblua" }
    filter {}
    filter { "configurations:debug*", "system:not windows" }
        links { "lua_debug" }
    filter {}
    filter { "configurations:release*", "system:not windows" }
        links { "lua" }
    filter {}
 
    -- target name, target prefix
    targetname "lua"

    -- debug target suffix define
    filter { "configurations:debug" }
        targetsuffix "_debug"

-- lua wrap library(lullbc) compile setting
-- import lualib_setting
package.path = package.path .. ";" .. "../../wrap/lullbc/?.lua"
local LUALIB_SETTING = require "lualib_setting"
local LUALIB_INCL_PATH = LUALIB_SETTING.use_setting and LUALIB_SETTING.lua_path[1] or LUA_SRC_PATH
local LUALIB_LIB_DIR = LUALIB_SETTING.use_setting and LUALIB_SETTING.lua_path[2] or LLBC_OUTPUT_DIR
project "lullbc"
    -- language, kind
    language "c++"
    kind "SharedLib"

    -- dependents
    dependson {
        "llbc",
        "lullbc_lualib",
        "lullbc_luaexec",
    }

    -- files
    files {
        "../../wrap/lullbc/lua/*.h",

        "../../wrap/lullbc/include/**.h",
        "../../wrap/lullbc/src/**.h",
        "../../wrap/lullbc/src/**.c",
        "../../wrap/lullbc/src/**.cpp",

        "../../wrap/lullbc/script/**.lua",
    }

    -- includedirs
    includedirs {
        LUALIB_INCL_PATH,
        "../../llbc/include",
        "../../wrap/lullbc/include",
        "../../wrap/lullbc",
    }

    -- defines
    filter { "system:windows", "action:vs2013 and vs2015 and vs2017" }
    defines {
        "HAVE_ROUND",
    }
    filter {}

    -- prebuild commands
    filter { "configurations:debug*" }
        prebuildcommands {
            PY .. " ../../tools/building_script/lu_prebuild.py lullbc debug",
        }
        postbuildcommands {
            PY .. string.format(' ../../tools/building_script/lu_postbuild.py %s %s "%s"', "lullbc", "debug", LLBC_OUTPUT_DIR),
        }
    filter {}
    filter { "configurations:release*" }
        prebuildcommands {
            PY .. " ../../tools/building_script/lu_prebuild.py lullbc release",
        }
        postbuildcommands {
            PY .. string.format(' ../../tools/building_script/lu_postbuild.py %s %s "%s"', "lullbc", "release", LLBC_OUTPUT_DIR),
        }
    filter {}

    -- target name, target prefix, extension
    targetname "_lullbc"
    targetprefix ""

    -- links 
    libdirs { 
        LLBC_OUTPUT_DIR,
        LUALIB_LIB_DIR,
    }
    filter { "configurations:debug*", "system:windows" }
        links {
            "libllbc_debug",
            LUALIB_SETTING.use_setting and LUALIB_SETTING.lua_path[3] or "liblua_debug",
        }
    filter {}
    filter { "configurations:debug*", "system:not windows" }
        links {
            "llbc_debug",
            LUALIB_SETTING.use_setting and LUALIB_SETTING.lua_path[3] or "lua_debug",
        }
    filter {}
    filter { "configurations:release*", "system:windows" }
        links {
            "libllbc",
            LUALIB_SETTING.use_setting and LUALIB_SETTING.lua_path[3] or "liblua",
        }
    filter {}
    filter { "configurations:release*", "system:not windows" }
        links {
            "llbc",
            LUALIB_SETTING.use_setting and LUALIB_SETTING.lua_path[3] or "lua",
        }
    filter {}

    -- flags
    filter { "system:not windows" }
        buildoptions {
            "-fvisibility=hidden",
        }
    filter {}

    -- debug target suffix define
    filter { "configurations:debug" }
        targetsuffix "_debug"
    filter {}

