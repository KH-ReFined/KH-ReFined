/*
==================================================
      KINGDOM HEARTS - RE:FINED FOR COM !
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

namespace ReFined
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
            Helpers.Log("Initializing Re:Fined...", 0);

            if (!Directory.Exists(Path.GetTempPath() + "ReFined"))
                Directory.CreateDirectory(Path.GetTempPath() + "ReFined");
                
            if (!File.Exists(Variables.SaveSFXPath))
            {
                var _saveStream = File.Create(Variables.SaveSFXPath);
                Variables.SaveSFX.CopyTo(_saveStream);
            }

            Variables.Source = new CancellationTokenSource();
            Variables.Token = Variables.Source.Token;

            Variables.Initialized = true;

            Helpers.Log("Re:Fined initialized with no errors!", 0);
        }

        /*
            TextAdjust:
        
            Change the text in-game to give the feel that this is an official SE mod
            and not some jank put together by a 20-year-old.
        */
        public static void TextAdjust()
        {
            var _textPointer = Hypervisor.Read<ulong>(Variables.PINT_SystemBAR);
            var _pintSecond = Hypervisor.Read<ulong>(_textPointer + 18);
            var _pintThird = Hypervisor.Read<ulong>(_pintSecond + 48);
            
            var _headerBegin = Hypervisor.Read<ulong>(_pintThird + 70);

            var _selectButton = Hypervisor.Read<byte>(_pintThird) + 0xB4; 
        }

        /*
            CheckTitle:

            Checks certain points in RAM to see if the player is in the Title Screen.
            Returns **true** if so, returns **false** otherwise. 
        */
        public static bool CheckTitle() => Hypervisor.Read<uint>(Variables.ADDR_Reset) == 0x00;

        /*
            ResetGame:

            Triggers a soft-reset if the proper input is given.
            The input is sought in Execute().

            INPUT: L1 + R1 + START + SELECT.
        */
        public static void ResetGame()
        {
            var _controllerRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
            var _keyboardRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);

            if (_keyboardRead == 0x0C09 || _controllerRead == 0x0C09 && !DEBOUNCE[0])
            {
                Helpers.Log("Initiating a Soft Reset.", 0);
                Hypervisor.Write<byte>(Variables.ADDR_Reset, 0x05);

                DEBOUNCE[0] = true;
            }

            else if (_keyboardRead != 0x0C09 && _controllerRead != 0x0C09 && DEBOUNCE[0])
                DEBOUNCE[0] = false;
        }

        /*
            FixExit:

            I sorta kinda unknowningly broke the Exit function in KH.
            To fix this, this function exists.
        */
        public static void FixExit()
        {
            if (CheckTitle())
            {
                var _titlePointer = Hypervisor.Read<ulong>(Variables.PINT_TitleSelect);
                var _pintSecond = Hypervisor.Read<ulong>(_titlePointer + 48);
                var _pintThird = Hypervisor.Read<ulong>(_pintSecond + 28);

                var _selectButton = Hypervisor.Read<byte>(_pintThird) + 0xB4; 

                var _inputRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);
                var _confirmRead = Hypervisor.Read<byte>(Variables.ADDR_Confirm);

                var _buttonSeek = (_confirmRead == 0x01 ? 0x2000 : 0x4000);
                var _inputValue = _inputRead & _buttonSeek;

                if (_inputValue == _buttonSeek && _selectButton == 0x03)
                {
                    Helpers.Log("Title to Exit detected! 2.5 second limit set! Initating exit...", 0);
                    Thread.Sleep(2500);

                    if (File.Exists("KINGDOM HEARTS HD 1.5+2.5 Launcher.exe"))
                    {
                        Helpers.Log("Launcher found! Launching the launcher...", 0);
                        Process.Start("KINGDOM HEARTS HD 1.5+2.5 Launcher");
                    }
                    
                    Helpers.Log("Re:Fined terminated with no errors.", 0);
                    Environment.Exit(0);
                }
            }
        }

        /*
            Execute:

            Executes the main logic within Re:Fined.
        */
        public static void Execute()
        {
            #region High Priority
            if (!Variables.Initialized)
                Initialization();

            ResetGame();
            FixExit();
            #endregion

            #region Mid Priority
            TextAdjust();
            #endregion

            #region Tasks

            #endregion
        }
    }
}
