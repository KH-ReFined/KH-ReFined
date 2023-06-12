/*
==================================================
      KINGDOM HEARTS - RE:FINED FOR 2 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using System.Text.RegularExpressions;

using Binarysharp.MSharp;
using System.Runtime.InteropServices;

namespace ReFined
{
    public static class Additions
    {
        /// <summary>
        /// Shows the Information Bar in-game, with the given text.
        /// </summary>
        /// <param name="StringID">The ID of the text to be shown.</param>
        public static void ShowInformation(ushort StringID)
        {
            if (!Operations.CheckTitle())
            {
                var _pointString = Operations.FetchPointerMSG(Variables.PINT_SystemBAR, StringID);
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
                var _pointString = Operations.FetchPointerMSG(Variables.PINT_SystemBAR, StringID);
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
