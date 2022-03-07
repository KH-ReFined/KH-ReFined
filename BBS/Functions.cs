/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR BBS!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.IO;
using System.Text;
using System.Linq;
using System.Threading;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Runtime.InteropServices;

using System.Windows.Forms;

using AxaFormBase;
using ReFixed.Forms;

using DiscordRPC;

namespace ReFixed
{
    public class Functions
    {
        /*
            Initialization:

            Serves only to unlock memory regions for now.
        */
        public static void Initialization()
        {
            Variables.CancelSource = new CancellationTokenSource();
            Variables.TaskToken = Variables.CancelSource.Token;

            Hypervisor.UnlockBlock(0x023394);

            Variables.Initialized = true;
        }

        /*
            CheckTitle:

            Checks certain points in RAM to see if the player is in the Title Screen.
            Returns **true** if so, returns **false** otherwise. 
        */
        public static bool CheckTitle() =>
            Hypervisor.Read<byte>(Variables.WorldAddress) == 0xFF
            || Hypervisor.Read<byte>(Variables.WorldAddress) == 0x00
            || Hypervisor.Read<byte>(Variables.WorldAddress + 0x01) == 0xFF
            || Hypervisor.Read<byte>(0x1098D02D) == 0x00;

        /*
            ResetGame:

            Triggers a soft-reset if the proper input is given.
            The input is sought in Execute().

            INPUT: L1 + R1 + START + SELECT.
        */
        public static void ResetGame()
        {
            if (!CheckTitle() && !Variables.Debounce)
            {
                Hypervisor.Write<byte>(Variables.LimiterAddress + 0x0C, 0x01);
                Variables.Debounce = true;
            }
        }

        /*
            FinisherPrompt:

            Allow finishers to be renamed through a pop-up window, bound to Triangle.
        */
        public static void FinisherPrompt()
        {
            // Fetch the Status Menu pointer.
            var _statusPointer = Hypervisor.Read<ulong>(Variables.StatusAddress);
            var _commandPointer = Hypervisor.Read<ulong>(Variables.CommandAddress);

            // If the Status Menu is open, and it's pointer fetched:
            if (_statusPointer > 0 || _commandPointer > 0)
            {
                // Fetch the Finisher Menu pointer.
                var _finishPointer =
                    _statusPointer > 0
                        ? Hypervisor.Read<ulong>(_statusPointer + 0xC8, true)
                        : Hypervisor.Read<ulong>(_commandPointer + 0xF0, true);

                // If the Finisher Menu is open, and it's pointer fetched:
                if (_finishPointer > 0)
                {
                    // Read the input, and the finisher that is currently selected.
                    // "Selected" means "Hovering", not "Active".
                    var _inputRead = Hypervisor.Read<ushort>(Variables.InputAddress);

                    var _selectedFinisher =
                        _statusPointer > 0
                            ? Hypervisor.Read<byte>(_finishPointer + 0x8E, true)
                            : Hypervisor.Read<ulong>(_finishPointer + 0xD8, true);
                    _selectedFinisher =
                        _statusPointer > 0
                            ? _selectedFinisher
                            : Hypervisor.Read<byte>(_selectedFinisher + 0x8E, true);

                    // If the debounce is not active, and Triangle is pressed:
                    if ((_inputRead & 0x1000) == 0x1000 && !Variables.Debounce)
                    {
                        // Activate debounce.
                        Variables.Debounce = true;

                        // Using the input form we made specifically for this:
                        using (InputText _inForm = new InputText())
                        {
                            // Literally halt the game.
                            BaseSimpleForm.theInstance.suspend();

                            // Set the form position to be the center of the game form, and show the dialog.
                            _inForm.StartPosition = FormStartPosition.CenterParent;
                            var _result = _inForm.ShowDialog(BaseSimpleForm.theInstance);

                            // If the result of said dialog is OK:
                            if (_result == DialogResult.OK)
                            {
                                // Fetch the text and turn it to a byte array.
                                var _textArray = Encoding.ASCII.GetBytes(_inForm.FinisherName);

                                // Make an array that will be 0x10 bytes long when finished.
                                var _fillerArray = new List<byte>();

                                // Add the text to the new array, then fill it out to be 0x10 in size.
                                _fillerArray.AddRange(_textArray);
                                _fillerArray.AddRange(new byte[0x10 - _textArray.Length]);

                                // Write the array to the chosen finisher.
                                Hypervisor.WriteArray(
                                    Variables.NameAddress + (ulong)(0x26 * _selectedFinisher),
                                    _fillerArray.ToArray()
                                );
                            }

                            // Resume the game.
                            BaseSimpleForm.theInstance.resume();
                        }
                    }
                    // Otherwise, if debounce is active and Triangle is NOT pressed:
                    // Deactivate debounce.
                    else if ((_inputRead & 0x1000) != 0x1000 && Variables.Debounce)
                        Variables.Debounce = false;
                }
            }
        }

        /*
            FrameOverride:

            Overwrites the frame limiter, and the instruction forcing it, according
            to the framerate chosen by the player.

            So, the same sort of shit as KH2?
            Exactly!
        */
        public static void FrameOverride()
        {
            // Calculate the instruction address.
            var _instructionAddress = Variables.ExeAddress + Variables.InstructionAddress;

            // Fetch the framerate, and the first byte of the instruction.
            var _framerateRead = Hypervisor.Read<byte>(Variables.FramerateAddress);
            var _instructionRead = Hypervisor.Read<byte>(_instructionAddress, true);

            // If the framerate is set to 30FPS, and the limiter is NOP'd out: Rewrite the instruction.
            if (_framerateRead == 0x00 && _instructionRead == 0x90)
            {
                Hypervisor.UnlockBlock(_instructionAddress, true);
                Hypervisor.WriteArray(_instructionAddress, Variables.LimiterInstruction, true);
            }
            // Otherwise, if the framerate is not set to 30FPS, and the limiter is present:
            else if (_framerateRead != 0x00 && _instructionRead != 0x90)
            {
                // NOP the instruction.
                Hypervisor.UnlockBlock(_instructionAddress, true);
                Hypervisor.WriteArray(_instructionAddress, Variables.LimiterRemoved, true);

                // Set the current limiter to be off.
                Hypervisor.Write<byte>(Variables.LimiterAddress, 0x00);
            }
        }

        /*
            AudioSwap:

            Enforces English or Japanese Audio depending on player preference.
            This is detemined by the **Vibration** option at the Camp Menu.

            This function is reliant on a patch.
        */
        public static void AudioSwap()
        {
            if (Hypervisor.Read<byte>(Variables.VibrationAddress) == 0x00)
                Hypervisor.WriteString(0x023394, "jp");
            else
                Hypervisor.WriteString(0x023394, "en");
        }

        /*
            GenerateSave:

            Only to be triggered by AutosaveEngine(), generate and write a save to
            both RAM and ROM portions, effectively saving the game.
        */
        public static void GenerateSave()
        {
            // Prepare the pointers.
            var _pointerBase = Hypervisor.Read<ulong>(Variables.InformationPointer);
            var _pointerSecond = Hypervisor.Read<ulong>(_pointerBase + 0x10, true);

            // Prepare the strings.
            var _saveName = "XB1-BBS-99";
            var _savePath = Hypervisor.ReadTerminate(_pointerBase + 0x40, true) + "\\KHBbSFM.png";

            // Calculate the Unix Date.
            var _currDate = DateTime.Now;
            var _unix = new DateTime(1970, 1, 1);
            var _writeDate = Convert.ToUInt64((_currDate - _unix).TotalSeconds);

            // Prepare the variables for Save Info.
            var _saveSlot = 0;
            var _saveInfoLength = 0x158;
            var _saveDataLength = 0x13C00;

            var _saveInfoStartRAM = _pointerSecond + 0x168;
            var _saveDataStartRAM = _pointerSecond + 0x1C270;

            var _saveInfoStartFILE = 0x1C8;
            var _saveDataStartFILE = 0x1C2D0;

            // Read the save from RAM.
            var _saveData = Hypervisor.ReadArray(Variables.SaveAddress, 0x11E50);

            // Read the save slot.
            var _saveSlotRAM = Hypervisor.ReadArray(
                _saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot),
                0x11,
                true
            );

            // If the file does not bear a save; terminate the operation.
            if (!Encoding.Default.GetString(_saveSlotRAM).Contains("BBS"))
                return;

            // Seek out the physical slot of the save to make.
            while (
                _saveSlotRAM[0] != 0x00
                && !Encoding.Default.GetString(_saveSlotRAM).Contains("BBS-99")
            )
            {
                _saveSlot++;
                _saveSlotRAM = Hypervisor.ReadArray(
                    _saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot),
                    0x11,
                    true
                );
            }

            // Calculate the checksums.
            var _checkData = Extensions.CalculateCRC32(new MemoryStream(_saveData));

            #region RAM Save
            // Fetch the address for the save info.
            var _saveInfoAddrRAM = _saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot);
            var _saveDataAddrRAM = _saveDataStartRAM + (ulong)(_saveDataLength * _saveSlot);

            // Write out the save information.
            Hypervisor.WriteArray(_saveInfoAddrRAM, Encoding.Default.GetBytes(_saveName), true);

            // Write the date in which the save was made.
            Hypervisor.Write<ulong>(_saveInfoAddrRAM + 0x40, _writeDate, true);
            Hypervisor.Write<ulong>(_saveInfoAddrRAM + 0x48, _writeDate, true);

            // Write the length of the save.
            Hypervisor.Write<int>(_saveInfoAddrRAM + 0x50, _saveDataLength, true);

            // Write the header.
            Hypervisor.WriteArray(_saveDataAddrRAM, Encoding.Default.GetBytes("BBSD"), true);
            Hypervisor.Write<uint>(_saveDataAddrRAM + 0x04, 0x1D, true);

            // Write the size.
            Hypervisor.Write<int>(_saveDataAddrRAM + 0x08, 0x12000, true);

            // Write the checksum.
            Hypervisor.Write<uint>(_saveDataAddrRAM + 0x0C, _checkData, true);

            // Write, the save.
            Hypervisor.WriteArray(_saveDataAddrRAM + 0x10, _saveData.Skip(0x10).ToArray(), true);
            #endregion

            #region File Save

            // Fetch the address for the save info.
            var _saveInfoAddr = _saveInfoStartFILE + _saveInfoLength * _saveSlot;
            var _saveDataAddr = _saveDataStartFILE + _saveDataLength * _saveSlot;

            // Create the writer.
            using (var _stream = new FileStream(_savePath, FileMode.Open))
            using (var _write = new BinaryWriter(_stream))
            {
                // Write out the save information.
                _stream.Position = _saveInfoAddr;
                _write.Write(Encoding.Default.GetBytes(_saveName));

                // The date in which the save was made.
                _stream.Position = _saveInfoAddr + 0x40;
                _write.Write(_writeDate);
                _stream.Position = _saveInfoAddr + 0x48;
                _write.Write(_writeDate);

                // The length of the save.
                _stream.Position = _saveInfoAddr + 0x50;
                _write.Write(0x12000);

                // Write the header.
                _stream.Position = _saveDataAddr;
                _write.Write(Encoding.Default.GetBytes("BBSD"));
                _stream.Position = _saveDataAddr + 0x04;
                _write.Write(0x1D);

                // Write the size.
                _stream.Position = _saveDataAddr + 0x08;
                _write.Write(0x11E50);

                // Write the checksum.
                _stream.Position = _saveDataAddr + 0x0C;
                _write.Write(_checkData);

                // Write, the save.
                _stream.Position = _saveDataAddr + 0x10;
                _write.Write(_saveData.Skip(0x10).ToArray());
            }
            #endregion

            Variables.SaveSFX.Play();
        }

        /*
            AutosaveEngine:

            As the name suggests, handle the logic behind Autosave functionality.
        */
        public static void AutosaveEngine()
        {
            var _vibrationRead = Hypervisor.Read<byte>(Variables.VibrationAddress);

            if (_vibrationRead == 0x01 || Variables.DualAudio)
            {
                var _battleRead = Hypervisor.Read<byte>(Variables.BattleAddress);
                var _loadRead = Hypervisor.Read<byte>(0x20D2AC);

                var _worldCheck = Hypervisor.Read<byte>(Variables.WorldAddress);
                var _roomCheck = Hypervisor.Read<byte>(Variables.WorldAddress + 0x01);

                // If not in the title screen, nor in a battle, and the room is loaded:
                if (!CheckTitle() && _battleRead == 0x00 && _loadRead == 0x01)
                {
                    // If the past WorldID is not equal to the current WorldID:
                    if (Variables.SaveWorld != _worldCheck)
                    {
                        GenerateSave();
                        Variables.SaveIterator = 0;
                    }
                    else if (Variables.SaveRoom != _roomCheck && _worldCheck >= 2)
                    {
                        if (Variables.SaveIterator == 3)
                        {
                            GenerateSave();
                            Variables.SaveIterator = 0;
                        }
                        else
                            Variables.SaveIterator++;
                    }

                    Variables.SaveWorld = _worldCheck;
                    Variables.SaveRoom = _roomCheck;
                }
            }
        }

        /*
            TextAdjust:

            Overwrite the text in certain portions of the game, to give the illusion that
            the features given are Square-made, and not some jank being made by a 20-year-old
            no life :^)
        */
        public static void TextAdjust()
        {
            if (!Variables.DualAudio)
            {
                var _basePointer = Hypervisor.Read<ulong>(Variables.SettingsPointer);
                var _secondaryPointer = Hypervisor.Read<ulong>(_basePointer + 0xA8, true);
                var _baseAddress = _secondaryPointer + 0xAEE;

                var _charRead = Hypervisor.Read<char>(_baseAddress, true);

                if (_charRead == 0x56)
                {
                    for (int i = 0; i < Variables.SaveOffsets.Length; i++)
                        Hypervisor.WriteArray(
                            _baseAddress + Variables.SaveOffsets[i],
                            Encoding.ASCII.GetBytes(Variables.SaveText[i]),
                            true
                        );
                }
            }
            
            else
            {
                var _basePointer = Hypervisor.Read<ulong>(Variables.SettingsPointer);
                var _secondaryPointer = Hypervisor.Read<ulong>(_basePointer + 0xA0, true);
                var _baseAddress = _secondaryPointer - 0x09A0;

                var _baseRead = Hypervisor.Read<uint>(_baseAddress, true);
                var _charRead = Hypervisor.Read<char>(
                    _baseAddress + Variables.AudioOffsets[0],
                    true
                );

                if (_baseRead == 0x44544340)
                    if (_charRead != 0x44)
                    {
                        Hypervisor.Write<uint>(
                            _baseAddress + 0x630,
                            (uint)Variables.AudioOffsets[3],
                            true
                        );
                        Hypervisor.Write<uint>(
                            _baseAddress + 0x63C,
                            (uint)Variables.AudioOffsets[4],
                            true
                        );

                        for (int i = 0; i < Variables.AudioOffsets.Length; i++)
                            Hypervisor.WriteArray(
                                _baseAddress + Variables.AudioOffsets[i],
                                Encoding.GetEncoding(437).GetBytes(Variables.AudioText[i]),
                                true
                            );
                    }
            }
        }

        /*
            DiscordEngine:

            Handle the Discord Rich Presence of Re:Fixed.
            To be executed on a separate thread.
        */
        public static void DiscordEngine()
        {
            var _levelValue = Hypervisor.Read<byte>(0x1098D02D);
            var _diffValue = Hypervisor.Read<byte>(0x1097ADBD);
            var _charValue = Hypervisor.Read<byte>(0x1098CF98);

            var _stringDetail = string.Format(
                "Level {0} | {1} Mode",
                _levelValue,
                Variables.ModeText.ElementAtOrDefault(_diffValue)
            );
            var _stringState = string.Format(
                "Character: {0}",
                Variables.CharText.ElementAtOrDefault(_charValue)
            );

            var _worldID = Hypervisor.Read<byte>(Variables.WorldAddress);
            var _battleFlag = Hypervisor.Read<byte>(Variables.BattleAddress);

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
                            LargeImageKey = Variables.WorldImages.ElementAtOrDefault(_worldID),
                            SmallImageKey = _battleFlag % 2 == 0 ? "safe" : "battle",
                            SmallImageText = _battleFlag % 2 == 0 ? "Safe" : "In Battle"
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

        /*
            Execute:

            Executes the main logic within Re:Fixed.
        */
        public static void Execute()
        {
            #region High Priority
            if (!Variables.Initialized)
                Initialization();

            if (Hypervisor.Read<ushort>(Variables.InputAddress) == 0x0C09)
                ResetGame();

            else
                Variables.Debounce = false;
            #endregion

            #region Mid Priority
            if (Variables.DualAudio)
                AudioSwap();

            FinisherPrompt();
            FrameOverride();
            #endregion

            #region Low Priority
            TextAdjust();
            #endregion

            #region Tasks
            if (Variables.AutoSaveTask == null)
            {
                Variables.AutoSaveTask = Task.Factory.StartNew(
                    delegate()
                    {
                        while (!Variables.TaskToken.IsCancellationRequested)
                        {
                            AutosaveEngine();
                            Thread.Sleep(5);
                        }
                    },
                    Variables.TaskToken
                );
            }

            if (Variables.DiscordTask == null)
            {
                Variables.DiscordTask = Task.Factory.StartNew(
                    delegate()
                    {
                        while (!Variables.TaskToken.IsCancellationRequested)
                        {
                            DiscordEngine();
                            Thread.Sleep(5);
                        }
                    },
                    Variables.TaskToken
                );
            }
            #endregion
        }
    }
}
