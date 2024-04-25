/*
==================================================
      KINGDOM HEARTS - RE:FINED FOR 2 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.Runtime.InteropServices;
using BSharpConvention = Binarysharp.MSharp.Assembly.CallingConvention.CallingConventions;

namespace ReFined
{
    public static class Additions
    {
        /// <summary>
        /// Shows a Warning Window, with the given raw text.
        /// Must be in the Save/Load Menu.
        /// </summary>
        /// <param name="Input">The text to be shown.</param>
        public static void ShowSLWarning(short StringID)
        {
            if (!Operations.CheckTitle())
            {
                Variables.SharpHook[(IntPtr)0x320AA0].Execute(StringID);
                Variables.SharpHook[(IntPtr)0x3208E0].Execute();
                Variables.SharpHook[(IntPtr)0x2F3F80].Execute(BSharpConvention.MicrosoftX64, 0x05, 0x00);
            }
        }

        /// <summary>
        /// Shows a Warning Window, with the given text ID.
        /// Must be in a Menu.
        /// </summary>
        /// <param name="StringID">The ID of the text to be shown.</param>
        /// <param name="Type">The menu to fall back after confirmation.</param>
        public static void ShowCampWarning(short StringID, int Type)
        {
            if (!Operations.CheckTitle())
            {
                var _currentMenu = Hypervisor.Read<int>(0x6877DE);
                Hypervisor.Write(0x689202, _currentMenu);

                Variables.SharpHook[(IntPtr)0x3047D0].ExecuteJMP(BSharpConvention.MicrosoftX64, StringID, 0x0000);
                Variables.SharpHook[(IntPtr)0x304620].Execute();
                Variables.SharpHook[(IntPtr)0x2F3F80].Execute(BSharpConvention.MicrosoftX64, Type, 0x00);
            }
        }

        /// <summary>
        /// Shows a Warning Window, with the given raw text.
        /// Must be in a Menu.
        /// </summary>
        /// <param name="Input">The text to be shown.</param>
        /// <param name="Type">The menu to fall back after confirmation.</param>
        public static void ShowCampWarningRAW(string Input, int Type)
        {
            if (!Operations.CheckTitle())
            {
                var _convString = Input.ToKHSCII();
                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x800000, _convString, true);

                var _currentMenu = Hypervisor.Read<int>(0x6877DE);
                Hypervisor.Write(0x689202, _currentMenu);

                Variables.SharpHook[(IntPtr)0x304890].ExecuteJMP(BSharpConvention.MicrosoftX64, (long)(Hypervisor.PureAddress + 0x800000), 0x0000);
                Variables.SharpHook[(IntPtr)0x304620].Execute();
                Variables.SharpHook[(IntPtr)0x2F3F80].Execute(BSharpConvention.MicrosoftX64, Type, 0x00);
            }
        }

        /// <summary>
        /// Shows the Information Bar in-game, with the given text.
        /// </summary>
        /// <param name="StringID">The ID of the text to be shown.</param>
        public static void ShowInformation(ushort StringID)
        {
            if (!Operations.CheckTitle())
            {
                var _pointString = Operations.FetchPointerMSG(Variables.PINT_SystemMSG, StringID);
                Variables.SharpHook[(IntPtr)0x179310].Execute(_pointString);
            }
        }

        /// <summary>
        /// Shows the Information Bar in-game, with the given raw text.
        /// </summary>
        /// <param name="String">The text to be shown.</param>
        public static void ShowInformationRAW(string Input)
        {
            if (!Operations.CheckTitle())
            {
                var _convString = Input.ToKHSCII();
                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x800000, _convString, true);

                Variables.SharpHook[(IntPtr)0x179310].Execute((long)(Hypervisor.PureAddress + 0x800000));
            }
        }

        /// <summary>
        /// Shows the Small Obtained Window in-game, with the given text.
        /// </summary>
        /// <param name="StringID">The ID of the text to be shown.</param>
        public static void ShowSmallObtained(ushort StringID)
        {
            if (!Operations.CheckTitle())
            {
                var _pointString = Operations.FetchPointerMSG(Variables.PINT_SystemMSG, StringID);
                Variables.SharpHook[(IntPtr)0x1571D0].Execute(_pointString);
            }
        }

        /// <summary>
        /// Shows the Small Obtained Window in-game, with the given raw text.
        /// </summary>
        /// <param name="String">The text to be shown.</param>
        public static void ShowSmallObtainedRAW(string Input)
        {
            if (!Operations.CheckTitle())
            {
                var _convString = Input.ToKHSCII();
                Hypervisor.WriteArray(Hypervisor.PureAddress + 0x800000, _convString, true);
                Variables.SharpHook[(IntPtr)0x1571D0].Execute((long)(Hypervisor.PureAddress + 0x800000));
            }
        }

        /// <summary>
        /// Plays a sound effect according to the ID given.
        /// </summary>
        /// <param name="SoundID">The ID of the sound to be played.</param>
        public static void PlaySFX(int SoundID)
        {
            Variables.SharpHook[(IntPtr)0x1DADF0].Execute(SoundID);
        }
    }
}
