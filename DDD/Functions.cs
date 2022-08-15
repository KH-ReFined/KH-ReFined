/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR DDD!
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

namespace ReFixed
{
    public class Functions
    {
        // FOR TESTING! REVERT TO 0xFF WHEN IN PRODUCTION!
        static byte LANGUAGE = 0;
        
        static byte[] SYSBAR_HEADER;

        static bool[] DEBOUNCE = new bool[] { false, false, false, false, false };

        /*
            Initialization:

            Serves to initialize stuffs.
        */
        public static void Initialization()
        {
            Helpers.Log("Initializing Re:Fixed...", 0);

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
            var _checkByte = Hypervisor.Read<byte>(Variables.ADDR_SystemBAR);
            
			if (_checkByte == 0x40)
            {
                var _strSize = Hypervisor.Read<int>(Variables.ADDR_SystemBAR + 0x18);
                SYSBAR_HEADER = Hypervisor.ReadArray(Variables.ADDR_SystemBAR, _strSize);

                var _vibrationText = SYSBAR_HEADER.FindValue<uint>(0x140003A0) - 0x02;
                var _vibrationOnDesc = SYSBAR_HEADER.FindValue<uint>(0x25010580) - 0x02;
                var _vibrationOffDesc = SYSBAR_HEADER.FindValue<uint>(0x00000580) - 0x02;

                var _txtOffset = Hypervisor.Read<ushort>(Variables.ADDR_SystemBAR + _vibrationText);
                var _onOffset = Hypervisor.Read<ushort>(Variables.ADDR_SystemBAR + _vibrationOnDesc);
                var _offOffset = Hypervisor.Read<ushort>(Variables.ADDR_SystemBAR + _vibrationOffDesc);

                var _fetchText = Strings.DropString[LANGUAGE];

                if (_offOffset != _onOffset)
                {
                    Hypervisor.Write(Variables.ADDR_SystemBAR + _vibrationOffDesc, _onOffset);

                    _offOffset = _onOffset;

                    Hypervisor.WriteString(Variables.ADDR_SystemBAR + _txtOffset, _fetchText[0], false, true);
                    Hypervisor.WriteString(Variables.ADDR_SystemBAR + _onOffset, _fetchText[1], false, true);
                }
            }
        }

        /*
            ResetGame:

            Triggers a soft-reset if the proper input is given.
            The input is sought in Execute().

            INPUT: L1 + R1 + START + SELECT.
        */
        public static void ResetGame()
        {
            var _buttRead = Hypervisor.Read<ushort>(Variables.ADDR_Input);

            if (_buttRead == 0x0C09 && !DEBOUNCE[0])
            {
                Helpers.Log("Initiating a Soft Reset.", 0);

                Hypervisor.Write<byte>(Variables.ADDR_Reset, 0x01);

                DEBOUNCE[0] = true;
            }

            else if (_buttRead != 0x0C09 && DEBOUNCE[0])
                DEBOUNCE[0] = false;
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
                        Helpers.Log("Enabling the Drop Gauge!", 0);
                        Hypervisor.Write<byte>(Hypervisor.PureAddress + Variables.ADDR_DrawINST, 0x01, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_TimeINST, Variables.INST_DropTimer, true);
                    }
                break;
                
                case 0x01:
                    if (_funcRead != 0x90)
                    {
                        Helpers.Log("Disabling the Drop Gauge!", 0);
                        Hypervisor.Write<byte>(Hypervisor.PureAddress + Variables.ADDR_DrawINST, 0x00, true);
                        Hypervisor.WriteArray(Hypervisor.PureAddress + Variables.ADDR_TimeINST, new byte[] { 0x90, 0x90, 0x90, 0x90 }, true);
                    }
                break;
            }
        }

                /*
            FrameOverride:

            Overwrites the frame limiter, and the instruction forcing it, according
            to the framerate chosen by the player.

            So, the same sort of shit as KH2 and BBS?
            Exactly!
        */
        public static void FrameOverride()
        {
            var _nullArray = new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

            // Calculate the instruction address.
            var _instructionAddress = Hypervisor.PureAddress + Variables.ADDR_LimiterINST;

            // Fetch the framerate, and the first byte of the instruction.
            var _framerateRead = Hypervisor.Read<byte>(Variables.ADDR_Framerate);
            var _instructionRead = Hypervisor.Read<byte>(_instructionAddress, true);

            // If the framerate is set to 30FPS, and the limiter is NOP'd out: Rewrite the instruction.
            if (_framerateRead == 0x00 && _instructionRead == 0x90)
            {
                Helpers.Log("30FPS Detected! Restoring the Framelimiter.", 0);
                Hypervisor.WriteArray(_instructionAddress, Variables.INST_FrameLimiter, true);
            }

            // Otherwise, if the framerate is not set to 30FPS, and the limiter is present:
            else if (_framerateRead != 0x00 && _instructionRead != 0x90)
            {
                Helpers.Log("60FPS Detected! Destroying the Framelimiter.", 0);

                // NOP the instruction.
                Hypervisor.WriteArray(_instructionAddress, _nullArray, true);

                // Set the current limiter to be off.
                Hypervisor.Write<byte>(Variables.ADDR_Limiter, 0x01);
            }
        }

        /*
            DiscordEngine:

            Handle the Discord Rich Presence of Re:Fixed.
            To be executed on a separate thread.

        public static void DiscordEngine()
        {
            var _healthValue = Hypervisor.Read<byte>(Variables.ADDR_SoraHP);
            var _levelValue = Hypervisor.Read<byte>(0x3237DA);

            var _charValue = Hypervisor.Read<byte>(Variables.ADDR_SoraForm);

            var _stringState = string.Format(
                "Level {0} | Character: {1}",
                _levelValue,
                Variables.CHRDictionary.ElementAtOrDefault(_formValue)
            );

            var _stringDetail = string.Format("HP: {0}", 0, _magicValue);

            var _worldID = Hypervisor.Read<byte>(0x2CBF0A);
            var _battleFlag = Hypervisor.Read<byte>(0x323782);

            var _timeValue = Math.Floor(Hypervisor.Read<int>(0x00444FA6) / 60F);
            var _timeMinutes = Math.Floor((_timeValue % 3600F) / 60F);
            var _timeHours = Math.Floor(_timeValue / 3600F);

            var _timeText = string.Format("In-Game Time: {0}", string.Format("{0}:{1}", _timeHours.ToString("00"), _timeMinutes.ToString("00")));
            var _diffValue = Hypervisor.Read<byte>(0x2CBF08);

            var _rpcButtons = new DiscordRPC.Button[]
            {
                new DiscordRPC.Button
                {
                    Label = "== Powered by Re:Fixed ==",
                    Url = "https://github.com/TopazTK/KH-ReFixed"
                },

                new DiscordRPC.Button
                {
                    Label = "== Icons by Televo ==",
                    Url = "https://github.com/Televo/kingdom-hearts-recollection"
                }
            };

            if (!CheckTitle())
            {
                Variables.DiscordClient.SetPresence(
                    new RichPresence
                    {
                        Details = _stringDetail,
                        State = _stringState,
                        Assets = new Assets
                        {
                            LargeImageKey = Variables.WRLDictionary.ElementAtOrDefault(_worldID),
                            LargeImageText = _timeText,
                            SmallImageKey = Variables.BTLDictionary.ElementAtOrDefault(_battleFlag),
                            SmallImageText = Variables.MDEDictionary.ElementAtOrDefault(_diffValue)
                        },
                        Buttons = _rpcButtons
                    }
                );
            }

            else
            {
                Variables.DiscordClient.SetPresence(
                    new RichPresence
                    {
                        Details = "On the Title Screen",
                        State = null,
                        Assets = new Assets
                        {
                            LargeImageKey = "title",
                            SmallImageKey = null,
                            SmallImageText = null
                        },
                        Buttons = _rpcButtons
                    }
                );
            }
        }
        */
        

        /*
            Execute:

            Executes the main logic within Re:Fixed.
        */
        public static void Execute()
        {
            #region High Priority
            if (!Variables.Initialized)
                Initialization();

            FrameOverride();
            ResetGame();
            #endregion

            #region Mid Priority
            TextAdjust();
            DropToggle();
            #endregion

            #region Tasks

            #endregion
        }
    }
}
