/**
 * @file    IniNative.cs
 * @author  Longwei Lai<lailongwei@126.com>
 * @date    --/--/--
 * @version 1.0
 *
 * @brief   llbc library native code wrapper.
 *          auto generated by tool, do not try to modify it!
 */

using System;
using System.Runtime.InteropServices;

namespace llbc
{

    /// <summary>
    /// Wrap core/config/_Ini.h file defined c functions
    /// </summary>
    internal partial class LLBCNative
    {
        [DllImport(NativeLibName, CallingConvention = CallingConvention.Cdecl)]
        public extern static IntPtr csllbc_Ini_LoadFromContent(IntPtr content, IntPtr sectionCount, IntPtr errDescLen);

        [DllImport(NativeLibName, CallingConvention = CallingConvention.Cdecl)]
        public extern static void csllbc_Ini_FreeNativeSections(IntPtr sections, int sectionCount);
    }
}
