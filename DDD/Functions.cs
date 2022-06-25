/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 1 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.IO;
using System.Text;
using System.Linq;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;
using System.Collections.Generic;

using DiscordRPC;

namespace ReFixed
{
    public class Functions
    {
        /*
            Variable Space!
          
            Yes, this class has one, too!
        */

        [DllImport("kernel32")]
		static extern bool AllocConsole();

        /*
            Initialization:

            Serves to initialize stuffs.
        */
        public static void Initialization()
        {
            Helpers.Log("Initializing Re:Fixed...", 0);

            if (!Directory.Exists(Path.GetTempPath() + "ReFixed"))
                Directory.CreateDirectory(Path.GetTempPath() + "ReFixed");
                
            if (!File.Exists(Variables.SaveSFXPath))
            {
                var _saveStream = File.Create(Variables.SaveSFXPath);
                Variables.SaveSFX.CopyTo(_saveStream);
            }

            Variables.Source = new CancellationTokenSource();
            Variables.Token = Variables.Source.Token;

            Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_TimeINST, true);
            Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_DrawINST, true);

            Variables.Initialized = true;

            Helpers.Log("Re:Fixed initialized with no errors!", 0);
        }

        /*
            TextAdjust:
        
            Change the text in-game to give the feel that this is an official SE mod
            and not some jank put together by a 20-year-old.
        */
        public static void TextAdjust()
        {

        }

         /*
            DropToggle:
        
            Seeking the vibration option, toggles the Drop Gauge.
        */
        public static void DropToggle()
        {
            var _toggleRead = Hypervisor.Read<byte>(Variables.ADDR_Vibration);
            var _funcRead = Hypervisor.Read<byte>(Hypervisor.PureAddress + Variables.ADDR_TimeINST, true);
            switch (_toggleRead)
            {
                case 0x00:
                    if (_funcRead == 0x90)
                    {
                        Hypervisor.Write<byte>(Hypervisor.PureAddress + Variables.ADDR_DrawINST, 0x01, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_TimeINST, Variables.INST_DropTimer, true);
                    }
                break;
                case 0x01:
                    if (_funcRead != 0x90)
                    {
                        Hypervisor.Write<byte>(Hypervisor.PureAddress + Variables.ADDR_DrawINST, 0x00, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_TimeINST, new byte[] { 0x90, 0x90, 0x90, 0x90 }, true);
                    }
                break;
            }
        }

        /*
            Execute:

            Executes the main logic within Re:Fixed.
        */
        public static void Execute()
        {
            #region High Priority
            if (!Variables.Initialized)
                Initialization();
            #endregion

            #region Mid Priority
            DropToggle();
            #endregion

            #region Tasks

            #endregion
        }
    }
}
