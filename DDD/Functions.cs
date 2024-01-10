/*
==================================================
      KINGDOM HEARTS - REFINED FOR DDD!
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
        // FOR TESTING! REVERT TO 0xFF WHEN IN PRODUCTION!
        static byte LANGUAGE = 0;
        
        static byte[] SYSBAR_HEADER;

		static byte SAVE_ROOM;
        static byte SAVE_WORLD;
        static byte SAVE_ITERATOR;

        static bool[] DEBOUNCE = new bool[] { false, false, false, false, false };

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

            Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_TimeINST, true);
            Hypervisor.UnlockBlock(Hypervisor.PureAddress + Variables.ADDR_DrawINST, true);

            Hypervisor.UnlockBlock(Variables.ADDR_VoicePath);

            var _documentsPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            var _saveDir = Path.Combine(_documentsPath, "Kingdom Hearts/Save Data/");

            EPIC_INIT:
            if (Directory.Exists(_saveDir))
            {
                string[] _epicDirs = Directory.GetDirectories(_saveDir, "*", SearchOption.TopDirectoryOnly);

                if (_epicDirs.Length == 0x00)
                goto EPIC_INIT; 

                foreach (var _str in _epicDirs)
                {
                    var _folderName = new DirectoryInfo(_str).Name;
                    Directory.CreateDirectory(Path.Combine(_documentsPath, "Kingdom Hearts/Configuration/" + _folderName));

                    Helpers.Log("Detected and Created directories for ID: " + _folderName, 0);
                }
            }

            else
                goto EPIC_INIT;

            Variables.Initialized = true;

            Helpers.Log("Re:Fined initialized with no errors!", 0);
        }

		public static bool CheckTitle() => Hypervisor.Read<byte>(Variables.ADDR_World) == 0x00;

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

                var _fetchDrop = Strings.DropString[LANGUAGE];

                if (_offOffset != _onOffset)
                {
                    Hypervisor.Write(Variables.ADDR_SystemBAR + _vibrationOffDesc, _onOffset);

                    _offOffset = _onOffset;

                    Hypervisor.WriteString(Variables.ADDR_SystemBAR + _txtOffset, _fetchDrop[0], false, true);
                    Hypervisor.WriteString(Variables.ADDR_SystemBAR + _onOffset, _fetchDrop[1], false, true);
                }

                var _subtitleText = SYSBAR_HEADER.FindValue<uint>(0x000903A0) - 0x02;
                var _subtitleOn = SYSBAR_HEADER.FindValue<uint>(0x1C010430) - 0x02;
                var _subtitleOff = SYSBAR_HEADER.FindValue<uint>(0x1D000430) - 0x02;
                var _subtitleOnDesc = SYSBAR_HEADER.FindValue<uint>(0x300104F0) - 0x02;
                var _subtitleOffDesc = SYSBAR_HEADER.FindValue<uint>(0x310004F0) - 0x02;

                _txtOffset = Hypervisor.Read<ushort>(Variables.ADDR_SystemBAR + _subtitleText);
                _onOffset = Hypervisor.Read<ushort>(Variables.ADDR_SystemBAR + _subtitleOn);
                _offOffset = Hypervisor.Read<ushort>(Variables.ADDR_SystemBAR + _subtitleOff);

                var _onDescOffset = Hypervisor.Read<ushort>(Variables.ADDR_SystemBAR + _subtitleOnDesc);
                var _offDescOffset = Hypervisor.Read<ushort>(Variables.ADDR_SystemBAR + _subtitleOffDesc);

                if (Variables.DualAudio)
                {
                    var _fetchDA = Strings.DualAudio[LANGUAGE];
                    var _jpOffset = Strings.JPOffset[LANGUAGE];

                    if (_onDescOffset != _offDescOffset)
                    {
                        Hypervisor.Write<ushort>(Variables.ADDR_SystemBAR + _subtitleOff, (ushort)(_offOffset + _jpOffset));
                        _offOffset += _jpOffset;

                        Hypervisor.Write(Variables.ADDR_SystemBAR + _subtitleOffDesc, _onDescOffset);

                        _offDescOffset = _onDescOffset;

                        Hypervisor.WriteString(Variables.ADDR_SystemBAR + _txtOffset, _fetchDA[0], false, true);
                        Hypervisor.WriteString(Variables.ADDR_SystemBAR + _onOffset, _fetchDA[1], false, true);
                        Hypervisor.WriteString(Variables.ADDR_SystemBAR + _offOffset, _fetchDA[2], false, true);

                        Hypervisor.WriteString(Variables.ADDR_SystemBAR + _onDescOffset, _fetchDA[3], false, true);
                        Hypervisor.WriteArray(Variables.ADDR_SystemBAR + (ulong)(_onDescOffset + _fetchDA[3].Length * 0x02), new byte[] {0x26, 0xE0});
                        Hypervisor.WriteString(Variables.ADDR_SystemBAR + (ulong)(_onDescOffset + _fetchDA[3].Length * 0x2 + 0x02), _fetchDA[4], false, true);
                    }
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
			if (!CheckTitle())
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
            AudioSwap:
        */
        public static void AudioSwap()
        {
            var _toggleRead = Hypervisor.Read<byte>(Variables.ADDR_Subtitles);
            var _audioRead = Hypervisor.Read<byte>(Variables.ADDR_VoicePath);

            switch (_toggleRead)
            {
                case 0x01:
                    if (_audioRead == 0x65)
                    {
                        Helpers.Log("Dual Audio switching to Japanese", 0);
                        Hypervisor.WriteArray(Variables.ADDR_VoicePath, new byte[] { 0x6A, 0x70 });
                    }
                break;
                
                case 0x00:
                    if (_audioRead != 0x65)
                    {
                        Helpers.Log("Dual Audio switching to English", 0);
                        Hypervisor.WriteArray(Variables.ADDR_VoicePath, new byte[] { 0x65, 0x6E });
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
            GenerateSave:

            Only to be triggered by AutosaveEngine(), generate and write a save to
            both RAM and ROM portions, effectively saving the game.
        */
        public static void GenerateSave()
        {
            // Prepare the pointers.
            var _pointerBase = Hypervisor.Read<ulong>(Variables.PINT_SaveInformation);
            var _pointerSecond = Hypervisor.Read<ulong>(_pointerBase + 0x10, true);

            // Prepare the strings.
            var _saveName = "CTR-P-AKH28-98";
            var _savePath = Hypervisor.ReadTerminate(_pointerBase + 0x40, true) + "\\KH3DHD.png";

            // Calculate the Unix Date.
            var _currDate = DateTime.Now;
            var _unix = new DateTime(1970, 1, 1);
            var _writeDate = Convert.ToUInt64((_currDate - _unix).TotalSeconds);

            // Prepare the variables for Save Info.
            var _saveSlot = 0;
            var _saveInfoLength = 0x158;
            var _saveDataLength = 0x16600;

            var _saveInfoStartRAM = _pointerSecond + 0x168;
            var _saveDataStartRAM = _pointerSecond + 0x8670;

            var _saveInfoStartFILE = 0x1C8;
            var _saveDataStartFILE = 0x86D0;

            var _saveSkip = false;

            // Read the save from RAM.
            var _saveData = Hypervisor.ReadArray(Variables.ADDR_TrueData, _saveDataLength - 0x90);

            // Read the save slot.
            var _saveSlotRAM = Hypervisor.ReadArray(_saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot), 0x11, true);

            // If the file does not bear a save; terminate the operation.
            if (!Encoding.Default.GetString(_saveSlotRAM).Contains("AKH28"))
            {
                Helpers.Log("File does not bare a save! Autosave aborted to stop corruption!", 1);
                return;
            }

            else if (!Encoding.Default.GetString(_saveSlotRAM).Contains("AKH28-00"))
                _saveSkip = true;

            // Seek out the physical slot of the save to make.
            while (_saveSlotRAM[0] != 0x00 && !Encoding.ASCII.GetString(_saveSlotRAM).Contains("AKH28-98"))
            {
                _saveSlot++;
                _saveSlotRAM = Hypervisor.ReadArray(_saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot), 0x11, true);
            }

            #region RAM Save
            // Fetch the address for the save info.
            var _saveInfoAddrRAM = _saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot);
            var _saveDataAddrRAM = _saveDataStartRAM + (ulong)(_saveDataLength * _saveSlot + (_saveSkip ? _saveDataLength : 0));

            // Write out the save information.
            Hypervisor.WriteArray(_saveInfoAddrRAM, Encoding.Default.GetBytes(_saveName), true);

            // Write the date in which the save was made.
            Hypervisor.Write(_saveInfoAddrRAM + 0x40, _writeDate, true);
            Hypervisor.Write(_saveInfoAddrRAM + 0x48, _writeDate, true);

            // Write the length of the save.
            Hypervisor.Write(_saveInfoAddrRAM + 0x50, _saveDataLength, true);

            // Read the header.
            var _saveHeader = Hypervisor.ReadArray(Variables.ADDR_SaveData, 0x90);

            // Write the header.
            Hypervisor.WriteArray(_saveDataAddrRAM, _saveHeader, true);

            // Overwrite descriptor values.
            var _timeRead = Hypervisor.Read<uint>(0x326CC6);
            var _charRead = Hypervisor.Read<byte>(0x2CBF06);
            var _munnyRead = Hypervisor.Read<uint>(0x2D9162);
            var _worldRead = Hypervisor.Read<byte>(Variables.ADDR_World);
            var _roomRead = Hypervisor.Read<byte>(Variables.ADDR_World + 0x01);

            Hypervisor.Write<uint>(_saveDataAddrRAM + 0x10, _timeRead, true);
            Hypervisor.Write<uint>(_saveDataAddrRAM + 0x14, _munnyRead, true);
            Hypervisor.Write<uint>(_saveDataAddrRAM + 0x1F, _charRead, true);
            Hypervisor.Write<byte>(_saveDataAddrRAM + 0x43, _worldRead, true);
            Hypervisor.Write<byte>(_saveDataAddrRAM + 0x44, _roomRead, true);

            // Write the save.
            Hypervisor.WriteArray(_saveDataAddrRAM + 0x90, _saveData, true);
            #endregion

            #region File Save

            // Fetch the address for the save info.
            var _saveInfoAddr = _saveInfoStartFILE + _saveInfoLength * _saveSlot;
            var _saveDataAddr = _saveDataStartFILE + _saveDataLength * _saveSlot + (_saveSkip ? _saveDataLength : 0);

            // Create the writer.
            using (var _stream = new FileStream(_savePath, FileMode.Open))
            using (var _write = new BinaryWriter(_stream))
            {
                // Write out the save information.
                _stream.Position = _saveInfoAddr;
                _write.Write(Encoding.ASCII.GetBytes(_saveName));

                // The date in which the save was made.
                _stream.Position = _saveInfoAddr + 0x40;
                _write.Write(_writeDate);
                _stream.Position = _saveInfoAddr + 0x48;
                _write.Write(_writeDate);

                // The length of the save.
                _stream.Position = _saveInfoAddr + 0x50;
                _write.Write(_saveDataLength);

                // Write the header.
                _stream.Position = _saveDataAddr;
                _write.Write(_saveHeader);

                // Overwrite the descriptor values.
                _stream.Position = _saveDataAddr + 0x10;
                _write.Write(_timeRead);
                _stream.Position = _saveDataAddr + 0x14;
                _write.Write(_munnyRead);
                _stream.Position = _saveDataAddr + 0x1F;
                _write.Write(_charRead);
                _stream.Position = _saveDataAddr + 0x43;
                _write.Write(_worldRead);
                _stream.Position = _saveDataAddr + 0x44;
                _write.Write(_roomRead);
                
                // Write, the save.
                _stream.Position = _saveDataAddr + 0x90;
                _write.Write(_saveData);
            }
            #endregion

            // Play a sound, dictating that the save was a success!
            if (Variables.sfxToggle)
                Helpers.PlaySFX(Variables.SaveSFXPath);
        }

        /*
            AutosaveEngine:

            As the name suggests, handle the logic behind Autosave functionality.
        */
        public static void AutosaveEngine()
        {
            var _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

            var _worldCheck = Hypervisor.Read<byte>(Variables.ADDR_World);
            var _roomCheck = Hypervisor.Read<byte>(Variables.ADDR_World + 0x01);

            if (!CheckTitle() && _loadRead == 0x01)
            { 
                Thread.Sleep(100);

                var _battleRead = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag);
                var _cutsceneRead = Hypervisor.Read<byte>(Variables.ADDR_CutsceneFlag);

                _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);

                var _saveableBool = Variables.saveToggle && _battleRead == 0x00 && _loadRead == 0x01 && _cutsceneRead == 0x00 && _worldCheck >= 0x03;

                if (_saveableBool)
                {
                    if (SAVE_WORLD != _worldCheck)
                    {
                        Helpers.Log("World condition met! Writing Autosave...", 0);

                        GenerateSave();
                        SAVE_ITERATOR = 0;
                    }

                    else if (SAVE_ROOM != _roomCheck)
                    {
                        SAVE_ITERATOR++;

                        if (SAVE_ITERATOR == 3)
                        {
                            Helpers.Log("Room condition met! Writing Autosave...", 0);

                            GenerateSave();
                            SAVE_ITERATOR = 0;
                        }
                    }

                    SAVE_WORLD = _worldCheck;
                    SAVE_ROOM = _roomCheck;
                }
            }
        }

        /*
            DiscordEngine:

            Handle the Discord Rich Presence of Re:Fined.
            To be executed on a separate thread.
        */
        public static void DiscordEngine()
        {
            var _levelValue = Hypervisor.Read<byte>(0x3237DA);
            var _charValue = Hypervisor.Read<byte>(0x2CBF06);

            var _charPoint = Hypervisor.Read<ulong>(Variables.PINT_CharHealth) + 0x71C;
            var _healthValue = Hypervisor.Read<byte>(_charPoint, true);

            var _stringState = string.Format(
                "Level {0} | Character: {1}",
                _levelValue,
                (_charValue & 1) == 1 ? "Riku" : "Sora"
            );

            var _stringDetail = string.Format("HP: {0}", _healthValue);

            var _worldID = Hypervisor.Read<byte>(0x2CBF0A);
            var _battleFlag = Hypervisor.Read<byte>(0x323782);

            var _timeValue = Hypervisor.Read<int>(0x326CC6);
            var _timeMinutes = Math.Floor((_timeValue % 3600F) / 60F);
            var _timeHours = Math.Floor(_timeValue / 3600F);

            var _timeText = string.Format("In-Game Time: {0}", string.Format("{0}:{1}", _timeHours.ToString("00"), _timeMinutes.ToString("00")));
            var _diffValue = Hypervisor.Read<byte>(0x2CBF08);

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
                    }
                );
            }
        }
        

        /*
            Execute:

            Executes the main logic within Re:Fined.
        */
        public static void Execute()
        {
            try
            {
                #region High Priority
                if (!Variables.Initialized)
                    Initialization();

                FrameOverride();
                ResetGame();
                FixExit();
                #endregion

                #region Mid Priority
                TextAdjust();
                AudioSwap();
                DropToggle();
                #endregion
            
                #region Tasks
                if (Variables.ASTask == null)
                {
                    Variables.ASTask = Task.Factory.StartNew(

                        delegate ()
                        {
                            while (!Variables.Token.IsCancellationRequested)
                            {
                                AutosaveEngine();
                                Thread.Sleep(5);
                            }
                        },

                        Variables.Token
                    );
                }

                if (Variables.DCTask == null && Variables.rpcToggle)
                {
                    Variables.DCTask = Task.Factory.StartNew(

                        delegate ()
                        {
                            while (!Variables.Token.IsCancellationRequested)
                            {
                                DiscordEngine();
                                Thread.Sleep(5);
                            }
                        },

                        Variables.Token
                    );
                }
                #endregion
            }
            
            catch (Exception _caughtEx)
            {
                Helpers.LogException(_caughtEx);
                Helpers.Log("Re:Fined terminated with an exception!", 1);
                Environment.Exit(-1);
            }
        }
    }
}
