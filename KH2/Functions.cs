/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 2 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.IO;
using System.Text;
using System.Linq;
using System.Diagnostics;
using System.Runtime.InteropServices;

using DiscordRPC;

namespace ReFixed
{
	public class Functions
	{
        public static bool IsTitle()
        {
            var _roomCheck = Hypervisor.Read<uint>(Variables.RoomAddress);
            var _softCheck = Hypervisor.Read<uint>(Variables.TitleBackAddress);
            var _titleCheck = Hypervisor.Read<uint>(Variables.TitleFlagAddress);

            return _roomCheck == 0x00FFFFFF || _roomCheck == 0x00000101 || _titleCheck == 0x01 || _softCheck == 0x01;
        }

        public static void ProcessRPC()
        {
            var _healthValue = Hypervisor.Read<byte>(0x024BC74A);
            var _magicValue = Hypervisor.Read<byte>(0x024BC8CA);
            var _levelValue = Hypervisor.Read<byte>(0x00445061);
            var _formValue = Hypervisor.Read<byte>(0x00446086);

            var _stringState = string.Format("Level {0} | Form: {1}", _levelValue, Variables.FormText.ElementAtOrDefault(_formValue));
            var _stringDetail = string.Format("HP: {0} | MP: {1}", _healthValue, _magicValue);

            var _worldID = Hypervisor.Read<byte>(Variables.RoomAddress);
            var _battleFlag = Hypervisor.Read<byte>(0x024AA5B6);

            var _timeValue = Math.Floor(Hypervisor.Read<int>(0x00444FA6) / 60F);
            var _timeMinutes = Math.Floor((_timeValue % 3600F) / 60F);
            var _timeHours = Math.Floor(_timeValue / 3600F);

            var _timeText = string.Format("In-Game Time: {0}", string.Format("{0}:{1}", _timeHours.ToString("00"), _timeMinutes.ToString("00")));

            var _diffValue = Hypervisor.Read<byte>(Variables.DifficultyAddress);

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

            if (!IsTitle())
            {
                Variables.RichClient.SetPresence(new RichPresence
                {
                    Details = _stringDetail,
                    State = _stringState,
                    Assets = new Assets
                    {
                        LargeImageKey = Variables.WorldImages.ElementAtOrDefault(_worldID),
                        LargeImageText = _timeText,
                        SmallImageKey = Variables.BattleImages.ElementAtOrDefault(_battleFlag),
                        SmallImageText = Variables.ModeText.ElementAtOrDefault(_diffValue)
                    },
                    
                    Buttons = _rpcButtons
                });
            }

            else
            {
                Variables.RichClient.SetPresence(new RichPresence
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
				});
            }
        }

        public static void HandleMagicSort()
        {
            // Fetch the status of the room. This will be necessary.
            var _loadRead = Hypervisor.Read<byte>(Variables.LoadAddress);

            // Fetch the input and read the pointer to the second CM.
            var _inputRead = Hypervisor.Read<ushort>(Variables.InputAddress);
            var _menuPointer = Hypervisor.Read<ulong>(Variables.MagicAddresses[0]);

            /*
                So the magic menu resets on each room transition.
                To prevent this, I check if the room is reloaded, and
                if so, just override it again.

                This is why we store the menu in memory when we shift it.
                This is why I hate my life.

                But eh, whatever.
            */

            // If the room is reloaded, and menu memory is not empty:
            if (_loadRead == 0x01 && Variables.RoomLoad && _menuPointer != 0x00)
            {
                // Write back the memorized magic menu.
                if (Variables.MagicStoreMemory != null)
                    Hypervisor.WriteArray(Variables.MagicAddresses[1], Variables.MagicStoreMemory);
                
                Variables.RoomLoad = false;
            }

            // Otherwise, if the room ISN'T loaded: Declare as such.
            else if (_loadRead == 0x00 && !Variables.RoomLoad)
                Variables.RoomLoad = true;

            // If there is a secondary CM, and the room is loaded:
            if (_menuPointer != 0x00 && _loadRead == 0x01)
            {
                // Read the secondary CM's type.
                var _menuRead = Hypervisor.Read<byte>(_menuPointer, true);

                // If the Magic Menu is the second CM to be shown (ID: 0x01):
                if (_menuRead == 0x01)
                {
                    // Fetch the current magic index and the max index.
                    var _magicIndex = Hypervisor.Read<byte>(Variables.MagicAddresses[2]);
                    var _magicMax = Hypervisor.Read<byte>(_menuPointer + 0x10, true);

                    // Set to 0x01 if it's going up, set to 0x02 if it's going down.
                    var _inputCheck = (_inputRead & 0x01001) == 0x1001 ? 0x01 : (_inputRead & 0x4001) == 0x4001 ? 0x02 : 0x00;
                    var _triggerCheck = (_inputRead & 0x01) == 0x01;

					// Read the instruction.
					var _insCheck = Hypervisor.Read<byte>(Variables.ExeAddress + Variables.SelectAddresses[0], true);

                    // If L2 is being held down:
                    if (_triggerCheck && _insCheck != 0x90)
                    {
                        // NOP out command selection, so it does not interfere with our input.
                        for (int _ins = 0; _ins < Variables.SelectAddresses.Length; _ins++)
                            Hypervisor.WriteArray(Variables.ExeAddress + Variables.SelectAddresses[_ins], new byte[] { 0x90, 0x90, 0x90 }, true);
                    }

                    else if (!_triggerCheck && _insCheck == 0x90)
                    {
                        // Revert the NOP'd instructions.
                        for (int _ins = 0; _ins < Variables.SelectAddresses.Length; _ins++)
                            Hypervisor.WriteArray(Variables.ExeAddress + Variables.SelectAddresses[_ins], Variables.SelectInstructions[_ins], true);
                    }

                    // If debounce is not active, and input is proper:
                    if (!Variables.Debounce && _inputCheck != 0x00)
                    {
                        // Activate debounce.
                        Variables.Debounce = true;

                        // Calculate the pointer and the bounds of the menu.
                        var _magicPointer = (0x02 * _magicIndex);
                        var _magicBounds = _magicPointer + (_inputCheck == 0x01 ? -0x02 : 0x02);

                        // Fetch the subject magic and the switchee.
                        var _subjectMagic = Hypervisor.Read<ushort>(Variables.MagicAddresses[1] + (ulong)_magicPointer);
						var _targetMagic = _magicBounds >= 0 ? Hypervisor.Read<ushort>(Variables.MagicAddresses[1] + (ulong)_magicBounds) : (ushort)0x0000;

                        // If the move would be in bounds, and if the switchee is not "nothing":
                        if (_targetMagic != 0x0000)
                        {
                            // Make the switch
                            Hypervisor.Write<ushort>(Variables.MagicAddresses[1] + (ulong)_magicPointer, _targetMagic);
                            Hypervisor.Write<ushort>(Variables.MagicAddresses[1] + (ulong)_magicBounds, _subjectMagic);
                        }

                        // Move the magic index.
                        Hypervisor.Write<int>(Variables.MagicAddresses[2], _magicIndex + (_inputCheck == 0x01 ? -0x01 : 0x01));
                        Hypervisor.Write<ushort>(Variables.MagicAddresses[2] + 0x04, _subjectMagic);

                        // Read the entirety of the magic menu, and save it to memory.
                        Variables.MagicStoreMemory = Hypervisor.ReadArray(Variables.MagicAddresses[1], _magicMax * 0x02);  
                    }

                    // Otherwise: If debounce is active and input is improper; deactivate debounce.
                    else if (Variables.Debounce && _inputCheck == 0x00)
                        Variables.Debounce = false;
                }
            }
        }

        public static void HandleTutorialSkip()
        {            
            var _skipBool = !Variables.SkipRoxas && !Variables.SkipComplete;

            if (IsTitle() && !_skipBool)
            {
                Variables.SkipRoxas = false;
                Variables.SkipComplete = false;
            }

            var _vibRead = Hypervisor.Read<ushort>(Variables.ConfigAddress) & 0x01;
            var _diffRead = Hypervisor.Read<byte>(Variables.DifficultyAddress);

            if (_skipBool && !IsTitle())
            {
                switch (_vibRead)
                {
                    case 0x01:
                        Variables.SkipComplete = true;
                        break;
                    case 0x00:
                        Variables.SkipRoxas = true;
                        Variables.SkipComplete = false;
                        Hypervisor.Write<ushort>(Variables.ConfigAddress, (ushort)(_vibRead + 0x01));
                        break;
                }
            }

            if (Variables.SkipRoxas && !Variables.SkipComplete)
            {
                var _worldCheck = Hypervisor.Read<byte>(Variables.RoomAddress);
                var _roomCheck = Hypervisor.Read<byte>(Variables.RoomAddress + 0x01);
                var _eventCheck = Hypervisor.Read<byte>(Variables.RoomAddress + 0x04);

                if (_worldCheck == 0x02 && _roomCheck == 0x01 && _eventCheck != 0x34)
                {
                    Hypervisor.Write<uint>(Variables.RoomAddress, 0x322002);
			        Hypervisor.Write<uint>(Variables.RoomAddress + 0x04, 0x01);
			        Hypervisor.Write<uint>(Variables.RoomAddress + 0x08, 0x01);

                    Hypervisor.Write<uint>(0x444832, 0x1FF00001);
                    Hypervisor.Write<uint>(0x444832 + 0x04, 0x00000000);
                }

                if (_worldCheck == 0x02 && _roomCheck == 0x20 && _eventCheck == 0x9A)
                {
                    Hypervisor.Write<uint>(Variables.RoomAddress, 0x001702);
			        Hypervisor.Write<uint>(Variables.RoomAddress + 0x04, (0x02 << 10) + 0x02);
			        Hypervisor.Write<uint>(Variables.RoomAddress + 0x08, 0x02);

                    Hypervisor.Write<byte>(Variables.InventoryFlagAddress, 0x9F);
                    Hypervisor.WriteArray(Variables.StoryFlagAddress, Variables.StoryFlagArray);

                    if (_diffRead == 0x03) 
                    {
                        Hypervisor.Write<byte>(0x445056, 0x18);
                        Hypervisor.Write<byte>(0x445056 + 0x01, 0x18);
                        Hypervisor.WriteArray(0x4450A6, new byte[] { 0x89, 0x01, 0x88, 0x01, 0xA5, 0x01, 0x94, 0x01, 0x97, 0x01, 0x97, 0x01, 0x95, 0x01, 0x52, 0x00, 0x8A, 0x00, 0x9E, 0x00 });
                    }

                    else
                    {
                        Hypervisor.Write<byte>(0x445056, 0x1E);
                        Hypervisor.Write<byte>(0x445056 + 0x01, 0x1E);
                        Hypervisor.WriteArray(0x4450A6, new byte[] { 0x52, 0x00, 0x8A, 0x00, 0x9E, 0x00 });
                    }
                    
                    Hypervisor.Write<byte>(0x446262, 0x04);
                    
                    Hypervisor.Write<byte>(0x446262 + 0x08, 0x06);
                    Hypervisor.Write<byte>(0x446262 + 0x0A, 0x40);
                    Hypervisor.Write<byte>(0x446262 + 0x0D, 0x02);

                    Variables.SkipRoxas = false;
                    Variables.SkipComplete = true;
                }
            }
        }

        public static void SeekReset()
        {
            var _inputRead = Hypervisor.Read<ushort>(Variables.InputAddress);

            if (_inputRead == 0x090C)
                Hypervisor.Write<byte>(Variables.TitleBackAddress, 0x01);
        }

        /*
            So you may be asking:
            "Topaz, why the fuck is this not an ASM Overwrite, and is an IL Hack?"

            Well you see, KH2 took a lazy approach for limiting the cutscenes to 30FPS.
            They just enable the framelimiter for the *whole game* instead of just limitting
            the cutscene like KH1 does. So if I overwrite that function in ASM, it will
            completely disable the 30FPS function. That is a big no-no since some do use it.

            So, this is the best solution to this problem. Just write a function to NOP
            that instruction whilst in 60FPS or above, but recover it once 30FPS is selected.

            These solutions have also paved the way to fixing the L2+Pad input problem of Magic Sort.
        */

        public static void OverrideLimiter()
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

        public static void OverrideText()
        {
            #region Roxas Story Option
                var _roxasCheck = Hypervisor.Read<byte>(Variables.TitleTextAddresses[1]);

                if (_roxasCheck != 0x46)
                {
                    var _buttOffset = Hypervisor.Read<uint>(Variables.TitleButtonAddress);
                    Hypervisor.Write<uint>(Variables.TitleButtonAddress, _buttOffset + 0x01);

                    for (int i = 0; i < Variables.TitleStrings.Length; i++)
                        Hypervisor.WriteArray(Variables.TitleTextAddresses[i], Variables.TitleStrings[i].ToKHSCII());
                }
            #endregion

            #region Limit Text
                var _raveText = "Rave{0x00}End";
                var _arsCheck = Hypervisor.Read<byte>(Variables.LimitAddresses[0]);

                var _secAccumilator = 0;

                if (_arsCheck != 0x2E)
                {
                    for (int i = 0; i < Variables.LimitAddresses.Length; i += 2)
                    {
                        // Write the text.
                        Hypervisor.WriteArray(Variables.LimitAddresses[i], Variables.LimitStrings[_secAccumilator].ToKHSCII());
                        Hypervisor.WriteArray(Variables.LimitAddresses[i + 1], Variables.LimitStrings[_secAccumilator].ToKHSCII());
                        
                        // Increase the accumilator for the text array.
                        _secAccumilator++;
                    }

                    // Since "Sonic Blade" is longer than "Sonic Rave", update the offsets for the RCs.
                    Hypervisor.Write<uint>(0x255CFFE, 0x01B42F);
                    Hypervisor.Write<uint>(0x255D006, 0x01B434);
                    Hypervisor.Write<uint>(0x255CE46, 0x01AA4B);

                    // Write the RCs text.
                    Hypervisor.WriteArray(0x2572571, _raveText.ToKHSCII());
                }
            #endregion

            #region Dual-Audio Toggle
                for (int i = 0; i < Variables.AudioStrings.Length; i++)
                {
                    Hypervisor.Write<int>(Variables.AudioOffsetAddresses[i], Variables.AudioOffsets[i]);
                    Hypervisor.WriteArray(Variables.AudioTextAddresses[i], Variables.AudioStrings[i].ToKHSCII()); 
                }
            #endregion
        }

        public static void OverrideShortcuts()
        {
            var _confirmRead = Hypervisor.Read<byte>(Variables.ConfirmAddress);
            var _shortRead = Hypervisor.Read<ushort>(Variables.ShortcutStartAddress);


            if (_confirmRead == 0x00 && _shortRead != 0x02BA)
            {
                Hypervisor.UnlockBlock(Variables.ShortcutStartAddress);
                Hypervisor.Write<ushort>(Variables.ShortcutStartAddress, 0x02BA);
                Hypervisor.Write<ushort>(Variables.ShortcutStartAddress + 0x06, 0x02AB);
            }

            else if (_confirmRead == 0x01 && _shortRead != 0x02AB)
            {
                Hypervisor.UnlockBlock(Variables.ShortcutStartAddress);
                Hypervisor.Write<ushort>(Variables.ShortcutStartAddress, 0x02AB);
                Hypervisor.Write<ushort>(Variables.ShortcutStartAddress + 0x06, 0x02BA);
            }
        }

        public static void CreateAutosave()
        {
            // Prepare the pointers.
            var _pointerBase = Hypervisor.Read<ulong>(Variables.InformationPointer);
            var _pointerSecond = Hypervisor.Read<ulong>(_pointerBase + 0x10, true);

            // Prepare the strings.
            var _saveName = "BISLPM-66675FM-98";
            var _savePath = Hypervisor.ReadTerminate(_pointerBase + 0x40, true) + "\\KHIIFM.png";

            // Calculate the Unix Date.
            var _currDate = DateTime.Now;
            var _unix = new DateTime(1970, 1, 1);
            var _writeDate = Convert.ToUInt64((_currDate - _unix).TotalSeconds);

            // Prepare the variables for Save Info.
            var _saveSlot = 0;
            var _saveInfoLength = 0x158;
            var _saveDataLength = 0x10FC0;

            var _saveInfoStartRAM = _pointerSecond + 0x168;
            var _saveDataStartRAM = _pointerSecond + 0x19630;

            var _saveInfoStartFILE = 0x1C8;
            var _saveDataStartFILE = 0x19690;

            // Read the save from RAM.
            var _saveData = Hypervisor.ReadArray(Variables.SaveAddress, _saveDataLength);

            // Read the save slot.
            var _saveSlotRAM = Hypervisor.ReadArray(_saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot), 0x11, true);

            // Seek out the physical slot of the save to make.
            while (_saveSlotRAM[0] != 0x00 && !Encoding.Default.GetString(_saveSlotRAM).Contains("66675FM-98"))
            {
                _saveSlot++;
                _saveSlotRAM = Hypervisor.ReadArray(_saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot), 0x11, true);
            }

            // Calculate the checksums.
            var _magicArray = _saveData.Take(0x08).ToArray();
            var _dataArray = _saveData.Skip(0x0C).ToArray();

            var _checkMagic = Extensions.CalculateCRC32(_magicArray, 8, uint.MaxValue);
            var _checkData = Extensions.CalculateCRC32(_dataArray, _dataArray.Length, _checkMagic ^ uint.MaxValue);

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
                Hypervisor.WriteArray(_saveDataAddrRAM, Encoding.Default.GetBytes("KH2J"), true);
                Hypervisor.Write<uint>(_saveDataAddrRAM + 0x04, 0x3A, true);

                // Write the checksum.
                Hypervisor.Write<uint>(_saveDataAddrRAM + 0x08, _checkData, true);

                // Write, the save.
                Hypervisor.WriteArray(_saveDataAddrRAM + 0x0C, _dataArray, true);
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
                    _write.Write(_saveDataLength);
                    
                    // Write the header.
                    _stream.Position = _saveDataAddr;
                    _write.Write(Encoding.Default.GetBytes("KH2J"));
                    _stream.Position = _saveDataAddr + 0x04;
                    _write.Write(0x3A);

                    // Write the checksum.
                    _stream.Position = _saveDataAddr + 0x08;
                    _write.Write(_checkData);

                    // Write, the save.
                    _stream.Position = _saveDataAddr + 0x0C;
                    _write.Write(_dataArray);
                }
            #endregion
        }

        public static void HandleAutosave()
        {
            var _battleRead = Hypervisor.Read<byte>(0x24AA5B6);
            var _loadRead = Hypervisor.Read<byte>(Variables.LoadAddress);

            var _worldCheck = Hypervisor.Read<byte>(Variables.RoomAddress);
            var _roomCheck = Hypervisor.Read<byte>(Variables.RoomAddress + 0x01);

            // If not in the title screen, nor in a battle, and the room is loaded:
            if (!IsTitle() && _battleRead == 0x00 && _loadRead == 0x01)
            {
                // If the past WorldID is not equal to the current WorldID:
                if (Variables.SaveWorld != _worldCheck)
                { 
                    CreateAutosave();
                    Variables.SaveIterator = 0;
                }

                else if (Variables.SaveRoom != _roomCheck && _worldCheck >= 2)
                {
                    if (Variables.SaveIterator == 3)
                    {
                        CreateAutosave();
                        Variables.SaveIterator = 0;
                    }

                    else
                        Variables.SaveIterator++;
                }

                Variables.SaveWorld = _worldCheck;
                Variables.SaveRoom = _roomCheck;
            }
        }

        public static void HandleAudio()
        {
            Hypervisor.UnlockBlock(Variables.PaxFormatterAddress);
            Hypervisor.UnlockBlock(Variables.VoiceFormatterAddress);

            var _toggleCheck = Hypervisor.Read<ushort>(Variables.ConfigAddress);

            if ((_toggleCheck & 0x01) == 0x00)
            {
                var _paxBytes = Encoding.ASCII.GetBytes("obj/%s.a.jp");
                Hypervisor.WriteArray(Variables.PaxFormatterAddress, _paxBytes);
                Hypervisor.WriteArray(Variables.PaxFormatterAddress + 0x10, _paxBytes);

                var _voiceBytes = Encoding.ASCII.GetBytes("voice/jp/battle");
                Hypervisor.WriteArray(Variables.VoiceFormatterAddress, _voiceBytes);
            }

            else
            {
                var _paxBytes = Encoding.ASCII.GetBytes("obj/%s.a.us");
                Hypervisor.WriteArray(Variables.PaxFormatterAddress, _paxBytes);
                Hypervisor.WriteArray(Variables.PaxFormatterAddress + 0x10, _paxBytes);

                var _voiceBytes = Encoding.ASCII.GetBytes("voice/us/battle");
                Hypervisor.WriteArray(Variables.VoiceFormatterAddress, _voiceBytes);
            }
        }

        public static void OverwriteOBJ(string Input)
        {
            for (ulong _formIterator = 0; _formIterator < (ulong)Variables.FormNames.Length; _formIterator++)
            {
                var _formatForm = String.Format(Variables.FormNames[_formIterator], Input);
                var _convertForm = Encoding.ASCII.GetBytes(_formatForm);

                if (_convertForm.Length < 16)
                {
                    var _byteList = new List<byte>();
                    _byteList.AddRange(_convertForm);
                    _byteList.AddRange(new byte[16 - _convertForm.Length]);

                    _convertForm = _byteList.ToArray();
                }

                Hypervisor.WriteArray(Variables.ObjentryAddresses[0] + 0x60 * _formIterator, _convertForm);
            }

            var _limitFormat = String.Format("P_EX100{0}_KH1F", Input);
            var _limitConvert = Encoding.ASCII.GetBytes(_limitFormat);

            Hypervisor.WriteArray(Variables.ObjentryAddresses[2], _limitConvert);

            for (ulong _friendIterator = 0; _friendIterator < (ulong)Variables.PartyNames.Length; _friendIterator++)
            {
                var _formatFriend = String.Format(Variables.PartyNames[_friendIterator], Input);
                var _convertFriend = Encoding.ASCII.GetBytes(_formatFriend);

                if (_convertFriend.Length < 16)
                {
                    var _byteList = new List<byte>();
                    _byteList.AddRange(_convertFriend);
                    _byteList.AddRange(new byte[16 - _convertFriend.Length]);

                    _convertFriend = _byteList.ToArray();
                }

                Hypervisor.WriteArray(Variables.ObjentryAddresses[1] + 0x60 * _friendIterator, _convertFriend);
            }
        }

        public static void HandleFestivity()
        {
            var _suffixRead = Hypervisor.Read<byte>(Variables.ObjentryAddresses[0] + 0x07);
            var _dateCurrent = DateTime.Now;

            var _dateHalloweenStart = new DateTime(_dateCurrent.Year, 10, 31);
            var _dateHalloweenEnd = new DateTime(_dateCurrent.Year, 11, 03);

            var _dateChristmasStart = new DateTime(_dateCurrent.Year, 12, 24);
            var _dateChristmasEnd = new DateTime(_dateCurrent.Year, 01, 02);

            var _suffixWrite = "";

            if (_dateCurrent > _dateHalloweenStart && 
                _dateCurrent < _dateHalloweenEnd)
                _suffixWrite = "_NM";

            else if (_dateCurrent > _dateChristmasStart && 
                    _dateCurrent > _dateChristmasEnd)
                _suffixWrite = "_XM";

            else
                _suffixWrite = "";

            if (_suffixWrite != "" && _suffixRead == 0x00)
                OverwriteOBJ(_suffixWrite);

            else if (_suffixWrite == "" && _suffixRead != 0x00)
                OverwriteOBJ(_suffixWrite);
        }

        public static void Execute()
        {
            HandleAutosave();
            
            HandleAudio();
            HandleFestivity();

            SeekReset();
            HandleTutorialSkip();

            OverrideLimiter();
            HandleMagicSort();

            OverrideText();
            OverrideShortcuts();

            ProcessRPC();
        }
    }
}