﻿using ReFined.Common;
using ReFined.Libraries;
using ReFined.KH2.Menus;
using ReFined.KH2.Information;
using ReFined.KH2.InGame;

using Binarysharp.MSharp;

namespace ReFined.KH2.Functions
{
    internal class Boot
    {
        public static void Initialization()
        {
            var _versionString = Variables.IS_LITE ? "Re:Freshed" : "Re:Fined";

            try
            {
                Configuration.Initialize();

                Terminal.Log("Welcome to " + _versionString + " v5.00!", 0);
                Terminal.Log("Please be patient while " + _versionString + " initializes.", 0);

                var _configIni = new INI("reFined.cfg");

                Terminal.Log("Initializing SharpHook...", 0);
                Variables.SharpHook = new MemorySharp(Hypervisor.Process);

                Terminal.Log("Initializing Configuration...", 0);
                Variables.RESET_PROMPT = Convert.ToBoolean(_configIni.Read("resetPrompt", "Kingdom Hearts II"));

                Terminal.Log("Unlocking Memory Regions...", 0);
                Hypervisor.UnlockBlock(0x360000);

                Terminal.Log("Locating Function Signatures...", 0);

                Message.OffsetMenu = Hypervisor.FindSignature(Variables.FUNC_SetMenuType);
                Message.OffsetInfo = Hypervisor.FindSignature(Variables.FUNC_ShowInformation);
                Message.OffsetObtained = Hypervisor.FindSignature(Variables.FUNC_ShowObatined);

                Message.OffsetSetSLWarning = Hypervisor.FindSignature(Variables.FUNC_SetSLWarning);
                Message.OffsetShowSLWarning = Hypervisor.FindSignature(Variables.FUNC_ShowSLWarning);
                Message.OffsetSetCampWarning = Hypervisor.FindSignature(Variables.FUNC_SetCampWarning);
                Message.OffsetShowCampWarning = Hypervisor.FindSignature(Variables.FUNC_ShowCampWarning);

                Critical.OffsetCampMenu = Hypervisor.FindSignature(Variables.FUNC_ExecuteCampMenu);

                Sound.OffsetSound = Hypervisor.FindSignature(Variables.FUNC_PlaySFX);

                Terminal.Log("Locating Hotfix Signatures...", 0);

                Continuous.LIMITER_OFFSET = (ulong)Hypervisor.FindSignature(Variables.HFIX_Framelimiter);
                Continuous.PROMPT_OFFSET = (ulong)Hypervisor.FindSignature(Variables.HFIX_ContPrompts);
                
                Critical.INVT_OFFSET = (ulong)Hypervisor.FindSignature(Variables.HFIX_InventoryReset);
                Critical.WARP_OFFSET = (ulong)Hypervisor.FindSignature(Variables.HFIX_WarpContinue);
                Critical.CMD_OFFSET = (ulong)Hypervisor.FindSignature(Variables.HFIX_CommandNavigation);

                Critical.WARP_FUNCTION = Hypervisor.Read<byte>(Critical.WARP_OFFSET, 0x05);
                Critical.INVT_FUNCTION = Hypervisor.Read<byte>(Critical.INVT_OFFSET, 0x07);
                
                Terminal.Log("Locating Hotfix Signatures for the Menus...", 0);

                Variables.HFIX_ConfigOffsets.Add((ulong)Hypervisor.FindSignature(Variables.HFIX_ConfigFirst));
                Variables.HFIX_ConfigOffsets.Add((ulong)Hypervisor.FindSignature(Variables.HFIX_ConfigSecond));
                Variables.HFIX_ConfigOffsets.Add((ulong)Hypervisor.FindSignature(Variables.HFIX_ConfigThird));
                Variables.HFIX_ConfigOffsets.Add((ulong)Hypervisor.FindSignature(Variables.HFIX_ConfigFourth));
                Variables.HFIX_ConfigOffsets.Add((ulong)Hypervisor.FindSignature(Variables.HFIX_ConfigFifth));
                Variables.HFIX_ConfigOffsets.Add((ulong)Hypervisor.FindSignature(Variables.HFIX_ConfigSixth));

                Variables.INTRO_MENU = new Intro();
                Variables.CONFIG_MENU = new Config();
                Variables.CONTINUE_MENU = new Continue();

                if (!Variables.IS_LITE)
                {
                    Terminal.Log("Initializing Extra Options...", 0);

                    var _entAudio = new Config.Entry()
                    {
                        Count = 3,
                        Title = 0x01BD,

                        Buttons = new List<ushort>()
                        {
                            0x01C4,
                            0x01C5,
                            0x01D9
                        },

                        Descriptions = new List<ushort>()
                        {
                            0x01D0,
                            0x01D1,
                            0x01DA
                        }
                    };
                    var _entMusic = new Config.Entry()
                    {
                        Count = 2,
                        Title = 0x01BF,

                        Buttons = new List<ushort>()
                        {
                            0x01C6,
                            0x01C7
                        },

                        Descriptions = new List<ushort>()
                        {
                            0x01D2,
                            0x01D3
                        }
                    };
                    var _entEnemy = new Config.Entry()
                    {
                        Count = 2,
                        Title = 0x01BD,

                        Buttons = new List<ushort>()
                        {
                            0x01C8,
                            0x01C9
                        },

                        Descriptions = new List<ushort>()
                        {
                            0x01D4,
                            0x01D5
                        }
                    };

                    Variables.CONFIG_MENU.Children.Insert(8, _entEnemy);
                    Variables.CONFIG_MENU.Children.Insert(8, _entMusic);
                    Variables.CONFIG_MENU.Children.Insert(8, _entAudio);
                }


                Variables.Source = new CancellationTokenSource();
                Variables.Token = Variables.Source.Token;

                Variables.Initialized = true;

                Terminal.Log(_versionString + " initialized with no errors!", 0);
            }

            catch (Exception ERROR)
            {
                Terminal.Log(ERROR);
                Terminal.Log(_versionString + " terminated with an exception!", 1);
                Environment.Exit(-1);
            }
        }
    }
}