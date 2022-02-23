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
using System.Diagnostics;
using System.Collections.Generic;
using System.Runtime.InteropServices;

using System.Windows.Forms;

using AxaFormBase;
using ReFixed.Forms;

using DiscordRPC;

namespace ReFixed
{
	public static class Functions
	{
        public static bool IsTitle()
        {
            var _levelValue = Hypervisor.Read<byte>(0x1098D02D);
            var _worldID = Hypervisor.Read<byte>(Variables.WorldAddress);
			var _roomID = Hypervisor.Read<byte>(Variables.WorldAddress + 0x01);

            return _worldID == 0xFF || _roomID == 0xFF || _worldID == 0x00 || _levelValue == 0;
        }

		/* 
			BBS is bullshit, we need to work with pointers for a lot
			of the stuff we will be doing here.

			Be advised. Because again, this is bullshit.
		*/

		public static void ProcessRPC()
        {
            var _levelValue = Hypervisor.Read<byte>(0x1098D02D);
			var _diffValue = Hypervisor.Read<byte>(0x1097ADBD);
			var _charValue = Hypervisor.Read<byte>(0x1098CF98);

            var _stringDetail = string.Format("Level {0} | {1} Mode", _levelValue, Variables.ModeText.ElementAtOrDefault(_diffValue));
            var _stringState = string.Format("Character: {0}", Variables.CharText.ElementAtOrDefault(_charValue));

            var _worldID = Hypervisor.Read<byte>(Variables.WorldAddress);
			var _roomID = Hypervisor.Read<byte>(Variables.WorldAddress + 0x01);
            var _battleFlag = Hypervisor.Read<byte>(Variables.BattleAddress);

			if (!IsTitle())
			{
				Variables.RichClient.SetPresence(new RichPresence()
				{
					Details = _stringDetail,
					State = _stringState,
					Assets = new Assets()
					{
						LargeImageKey = Variables.WorldImages.ElementAtOrDefault(_worldID),
						SmallImageKey = _battleFlag % 2 == 0 ? "safe" : "battle",
						SmallImageText = _battleFlag % 2 == 0 ? "Safe" : "In Battle"
					},
					
					Buttons = new DiscordRPC.Button[] 
					{ 
						new DiscordRPC.Button()
						{ 
							Label = "== Powered by Re:Fixed ==", 
							Url = "https://github.com/TopazTK/KH-ReFixed" 
						},
						new DiscordRPC.Button()
						{ 
							Label = "== Icons by Televo ==", 
							Url = "https://github.com/Televo/kingdom-hearts-recollection" 
						} 
					}
				});
			}

			else
			{
				Variables.RichClient.SetPresence(new RichPresence()
				{
					Details = "On the Title Screen",
					State = null,
					
					Assets = new Assets()
					{
						LargeImageKey = "title",
						SmallImageKey = null,
						SmallImageText = null
					},
					
					Buttons = new DiscordRPC.Button[] 
					{ 
						new DiscordRPC.Button()
						{ 
							Label = "== Powered by Re:Fixed ==", 
							Url = "https://github.com/TopazTK/KH-ReFixed" 
						},
						new DiscordRPC.Button()
						{ 
							Label = "== Icons by Televo ==", 
							Url = "https://github.com/Televo/kingdom-hearts-recollection" 
						} 
					}
				});
			}
        }

		public static void RenameFinisher()
		{
			// Fetch the Status Menu pointer.
			var _statusPointer = Hypervisor.Read<ulong>(Variables.StatusAddress);
            var _commandPointer = Hypervisor.Read<ulong>(Variables.CommandAddress);

			// If the Status Menu is open, and it's pointer fetched:
			if (_statusPointer > 0 || _commandPointer > 0)
			{
				// Fetch the Finisher Menu pointer.
				var _finishPointer = _statusPointer > 0 ? Hypervisor.Read<ulong>(_statusPointer + 0xC8, true) : Hypervisor.Read<ulong>(_commandPointer + 0xF0, true); 

				// If the Finisher Menu is open, and it's pointer fetched:
				if (_finishPointer > 0)
				{
					// Read the input, and the finisher that is currently selected.
					// "Selected" means "Hovering", not "Active".
					var _inputRead = Hypervisor.Read<ushort>(Variables.InputAddress);

					var _selectedFinisher = _statusPointer > 0 ? Hypervisor.Read<byte>(_finishPointer + 0x8E, true) : Hypervisor.Read<ulong>(_finishPointer + 0xD8, true);
                    _selectedFinisher = _statusPointer > 0 ? _selectedFinisher : Hypervisor.Read<byte>(_selectedFinisher + 0x8E, true);

					// If the debounce is not active, and Triangle is pressed:
					if ((_inputRead & 0x1000) == 0x1000 && !Variables.Debounce)
					{ 
						// Activate debounce.
						Variables.Debounce = true;

						// Using the input form we made specifically for this:
						using(InputText _inForm = new InputText())
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
								Hypervisor.WriteArray(Variables.NameAddress + (ulong)(0x26 * _selectedFinisher), _fillerArray.ToArray());
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

		// Y'all I found the byte by accident, fml.

		public static void SeekReset()
        {
            var _inputRead = Hypervisor.Read<ushort>(Variables.InputAddress);

            if (_inputRead == 0x0C09 && !IsTitle())
            {
                Hypervisor.Write<byte>(Variables.LimiterAddress + 0x0C, 0x01);
                Hypervisor.Write<byte>(Variables.LimiterAddress + 0x0C, 0x00);
            }
        }

		/*
    		"Same sort of shit as KH2?"

			Same sort of shit as KH2, yes.
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
            var _basePointer = Hypervisor.Read<ulong>(Variables.SettingsPointer);
            var _secondaryPointer = Hypervisor.Read<ulong>(_basePointer + 0xA8, true);

            var _baseAddress = Hypervisor.Read<ulong>(_secondaryPointer + 0xAEE, true); 

            var _charRead = Hypervisor.Read<char>(_baseAddress, true);

            if (_charRead == 0x56)
            {
                for (int i = 0; i < Variables.SettingsOffsets.Length; i++)
                    Hypervisor.WriteArray(_baseAddress + Variables.SettingsOffsets[i], Encoding.ASCII.GetBytes(Variables.SettingsText[i]), true);
            }
        }

		public static void CreateAutosave()
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
            var _saveSlotRAM = Hypervisor.ReadArray(_saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot), 0x11, true);

            // Seek out the physical slot of the save to make.
            while (_saveSlotRAM[0] != 0x00 && !Encoding.Default.GetString(_saveSlotRAM).Contains("BBS-99"))
            {
                _saveSlot++;
                _saveSlotRAM = Hypervisor.ReadArray(_saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot), 0x11, true);
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
        }

        public static void HandleAutosave()
        {
            var _vibrationRead = Hypervisor.Read<byte>(Variables.VibrationAddress);

            if (_vibrationRead == 0x01)
            {
                var _battleRead = Hypervisor.Read<byte>(Variables.BattleAddress);
                var _loadRead = Hypervisor.Read<byte>(0x20D2AC);

                var _worldCheck = Hypervisor.Read<byte>(Variables.WorldAddress);
                var _roomCheck = Hypervisor.Read<byte>(Variables.WorldAddress + 0x01);

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
        }

		public static void Execute()
		{
            HandleAutosave();
            
			SeekReset();
			
			RenameFinisher();
			OverrideLimiter();
            OverrideText();

			ProcessRPC();
		}
	}
}
