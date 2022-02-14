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
using System.Runtime.InteropServices;

using DiscordRPC;

namespace ReFixed
{
	public static class Functions
	{
        public static void ProcessRPC()
        {
            var _healthValue = Hypervisor.Read<byte>(0x029B8CC6);
            var _magicValue = Hypervisor.Read<byte>(0x029B8CCE);
            var _levelValue = Hypervisor.Read<byte>(Variables.LevelAddress);
			var _diffValue = Hypervisor.Read<byte>(Variables.DifficultyAddress);

            var _stringState = string.Format("Level {0} | {1} Mode", _levelValue, Variables.ModeText.ElementAtOrDefault(_diffValue));
            var _stringDetail = string.Format("HP: {0} | MP: {1}", _healthValue, _magicValue);

            var _worldID = Hypervisor.Read<byte>(Variables.WorldAddress);
			var _gummiCheck = Hypervisor.Read<byte>(0x00163C17);
            var _battleFlag = Hypervisor.Read<byte>(0x024C3352);

            var _timeValue = Math.Floor(Hypervisor.Read<int>(Variables.TimeAddress) / 60F);
            var _timeMinutes = Math.Floor((_timeValue % 3600F) / 60F);
            var _timeHours = Math.Floor(_timeValue / 3600F);

            var _timeText = string.Format("In-Game Time: {0}", string.Format("{0}:{1}", _timeHours.ToString("00"), _timeMinutes.ToString("00")));

			if (_worldID != 0xFF)
			{
				Variables.RichClient.SetPresence(new RichPresence()
				{
					Details = _stringDetail,
					State = _stringState,
					Assets = new Assets()
					{
						LargeImageKey = _gummiCheck == 0 ? Variables.WorldImages.ElementAtOrDefault(_worldID) : "wm",
						LargeImageText = _timeText,
						SmallImageKey = _battleFlag % 2 == 0 ? "safe" : "battle",
						SmallImageText = _battleFlag % 2 == 0 ? "Safe" : "In Battle"
					},
					
					Buttons = new Button[] 
					{ 
						new Button()
						{ 
							Label = "== Powered by Re:Fixed ==", 
							Url = "https://github.com/TopazTK/KH-ReFixed" 
						},
						new Button()
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
							Label = "Powered by Re:Fixed", 
							Url = "https://github.com/TopazTK/KH-ReFixed" 
						},
						new DiscordRPC.Button()
						{ 
							Label = "Icons by Televo", 
							Url = "https://github.com/Televo" 
						} 
					}
				});
			}
        }

		public static void OverrideText()
		{
		    if (Hypervisor.Read<byte>(Variables.FovTextAddresses[1]) != 0x30)
		    {
				for (uint i = 0; i < Variables.FovTextOffsets.Length; i++)
					Hypervisor.Write<ushort>(Variables.FovTextAddresses[0] + (0x02 * i), Variables.FovTextOffsets[i]);

				for (uint i = 0; i < Variables.CamTextOffsets.Length; i++)
					Hypervisor.Write<ushort>(Variables.CamTextAddresses[0] + (0x02 * i), Variables.CamTextOffsets[i]);

				Hypervisor.WriteArray(Variables.FovTextAddresses[1], Variables.FovTextString.ToKHSCII());
				Hypervisor.WriteArray(Variables.CamTextAddresses[1], Variables.CamTextString.ToKHSCII());
		    }
		}

		public static void OverrideFov()
		{
		    var _fovToggle = Hypervisor.Read<int>(Variables.FovSwitchAddress);

		    switch(_fovToggle)
		    {
				case 0:
				{
					var _fovFirst = Hypervisor.Read<float>(Variables.FovAddresses[0]);

					if (_fovFirst != 400F)
					{
					for (uint i = 0; i < Variables.FovAddresses.Length; i++)
						Hypervisor.Write<float>(Variables.FovAddresses[i], Variables.FovClassic[i]);
					}

					break;
				}

				case 1:
				{
					var _fovFirst = Hypervisor.Read<float>(Variables.FovAddresses[0]);

					if (_fovFirst != 600F)
					{
					for (int i = 0; i < Variables.FovAddresses.Length; i++)
						Hypervisor.Write<float>(Variables.FovAddresses[i], Variables.FovEnhanced[i]);
					}

					break;
				}
		    }
		}

		public static void OverrideAspect(float InputValue)
		{
			float _floatValue = 9F;

			switch (InputValue)
			{
				case 3.5F:
					_floatValue = 4.5F;
					break;
				case 2.3F:
					_floatValue = 6.75F;
					break;
				case 1.7F:
					_floatValue = 9F;
					break;
				case 1.6F:
					_floatValue = 10F;
					break;
				case 1.3F:
					_floatValue = 12F;
					break;
			}

			Hypervisor.UnlockBlock(0x10F2E);
			Hypervisor.Write<float>(0x10F2E, _floatValue);
		}

		public static void OverrideMP()
		{
			var _catchMagic = Hypervisor.Read<byte>(Variables.SoraMagicAddress);

			switch (_catchMagic)
			{
				case 0:
					Hypervisor.Write<byte>(Variables.SoraMPAddress, 0x00);
					break;
				default:
					if (Hypervisor.Read<byte>(Variables.SoraMPAddress) == 0x00)
						Hypervisor.Write<byte>(Variables.SoraMPAddress, 0x01);
					break;
			}
		}

		public static void SeekReset()
		{
			var _inputRead = Hypervisor.Read<ushort>(Variables.InputAddress);

			var _selectRead = Hypervisor.Read<byte>(Variables.SaveMenuSelect);
			var _amountRead = Hypervisor.Read<byte>(Variables.SaveMenuSelect + 0x044);

			var _buttonRead = (_inputRead & 0x01) == 0x01 && (_inputRead & 0x08) == 0x08;
			var _saveMenuRead = (_selectRead == _amountRead - 0x01) && (_inputRead & 0x4000) == 0x4000;
			
			if (_buttonRead || _saveMenuRead)
			{
				Hypervisor.Write<byte>(Variables.ResetAddresses[0], 0x01);
				Hypervisor.Write<byte>(Variables.ResetAddresses[1], 0x01);
			}
		}

		public static void CreateAutosave()
        {
            // Prepare the pointers.
            var _pointerBase = Hypervisor.Read<ulong>(Variables.InformationPointer);
            var _pointerSecond = Hypervisor.Read<ulong>(_pointerBase + 0x10, true);

            // Prepare the strings.
			var _descName = "-99/system.bin";
            var _saveName = "BISLPS-25198-99";
            var _savePath = Hypervisor.ReadTerminate(_pointerBase + 0x40, true) + "\\KHFM.png";

            // Calculate the Unix Date.
            var _currDate = DateTime.Now;
            var _unix = new DateTime(1970, 1, 1);
            var _writeDate = Convert.ToUInt64((_currDate - _unix).TotalSeconds);

            // Prepare the variables for Save Info.
            var _saveSlot = 0;
            var _saveInfoLength = 0x158;
            var _saveDataLength = 0x16C40;

            var _saveInfoStartRAM = _pointerSecond + 0x10;
            var _saveDataStartRAM = _pointerSecond + 0x10CD0;

            var _saveInfoStartFILE = 0x320;
            var _saveDataStartFILE = 0x10D30;

            // Read the save from RAM.
            var _saveData = Hypervisor.ReadArray(Variables.SaveAddress, _saveDataLength);

            // Read the save slot.
            var _saveSlotRAM = Hypervisor.ReadArray(_saveInfoStartRAM + (ulong)((_saveInfoLength * 2) * _saveSlot), 0x11, true);

            // Seek out the physical slot of the save to make.
            while (_saveSlotRAM[0] != 0x00 && !Encoding.Default.GetString(_saveSlotRAM).Contains("25198-99"))
            {
                _saveSlot++;
                _saveSlotRAM = Hypervisor.ReadArray(_saveInfoStartRAM + (ulong)((_saveInfoLength * 2) * _saveSlot), 0x11, true);
            }

			// Read the necessary values.
			var _timeRead = Hypervisor.Read<uint>(Variables.TimeAddress);
			var _levelRead = Hypervisor.Read<byte>(Variables.LevelAddress);
			var _munnyRead = Hypervisor.Read<uint>(Variables.MunnyAddress);
			var _worldRead = Hypervisor.Read<byte>(Variables.WorldAddress);
			var _diffRead = Hypervisor.Read<byte>(Variables.DifficultyAddress);
			var _roomRead = Hypervisor.Read<byte>(Variables.WorldAddress + 0x68);

            #region RAM Save
                // Fetch the address for the save info.
                var _saveInfoAddrRAM = _saveInfoStartRAM + (ulong)((_saveInfoLength * 2) * _saveSlot);
                var _saveDataAddrRAM = _saveDataStartRAM + (ulong)((_saveDataLength * 2) * _saveSlot);

                // Write out the save information.
                Hypervisor.WriteArray(_saveInfoAddrRAM, Encoding.Default.GetBytes(_saveName), true);

                // Write the date in which the save was made.
                Hypervisor.Write<ulong>(_saveInfoAddrRAM + 0x40, _writeDate, true);
                Hypervisor.Write<ulong>(_saveInfoAddrRAM + 0x48, _writeDate, true);

                // Write the length of the save.
                Hypervisor.Write<int>(_saveInfoAddrRAM + 0x50, _saveDataLength - 0x40, true);

				// Write out the descriptor information.
                Hypervisor.WriteArray(_saveInfoAddrRAM + (ulong)_saveInfoLength, Encoding.Default.GetBytes(_descName), true);

                // Write the date in which the descriptor was written.
                Hypervisor.Write<ulong>(_saveInfoAddrRAM + (ulong)(_saveInfoLength + 0x40), _writeDate, true);
                Hypervisor.Write<ulong>(_saveInfoAddrRAM + (ulong)(_saveInfoLength + 0x48), _writeDate, true);

                // Write the length of the descriptor.
				// This shoudl be 0x40, but Team Osaka is STUPID so it's 0x400 instead.
				// No, nothing after the 0x40 bit is utilized.
                Hypervisor.Write<int>(_saveInfoAddrRAM + (ulong)(_saveInfoLength + 0x50), 0x400, true);

                // Write, the save.
                Hypervisor.WriteArray(_saveDataAddrRAM, _saveData, true);

				// Write the Magic Values for the descriptor.
                Hypervisor.WriteArray(_saveDataAddrRAM + (ulong)_saveDataLength, Encoding.Default.GetBytes("KHSQ"), true);
				Hypervisor.Write<uint>(_saveDataAddrRAM + (ulong)(_saveDataLength + 0x04), 0x1317F14, true);

				// Write the actual descriptor data.
				Hypervisor.Write<byte>(_saveDataAddrRAM + (ulong)(_saveDataLength + 0x08), _levelRead, true);
				Hypervisor.Write<uint>(_saveDataAddrRAM + (ulong)(_saveDataLength + 0x0C), _munnyRead, true);
				Hypervisor.Write<uint>(_saveDataAddrRAM + (ulong)(_saveDataLength + 0x10), _timeRead, true);

				Hypervisor.Write<byte>(_saveDataAddrRAM + (ulong)(_saveDataLength + 0x38), _diffRead, true);	
				Hypervisor.Write<byte>(_saveDataAddrRAM + (ulong)(_saveDataLength + 0x3C), _worldRead, true);
				Hypervisor.Write<byte>(_saveDataAddrRAM + (ulong)(_saveDataLength + 0x3E), _roomRead, true);
            #endregion
            
            #region File Save

                // Fetch the address for the save info.
                var _saveInfoAddr = _saveInfoStartFILE + (_saveInfoLength * 2) * (_saveSlot - 0x01);
                var _saveDataAddr = _saveDataStartFILE + (_saveDataLength * 2) * _saveSlot;
                
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
                    _write.Write(_saveDataLength - 0x40);

					// Write out the descriptor information.
                    _stream.Position = _saveInfoAddr + _saveInfoLength;
                    _write.Write(Encoding.Default.GetBytes(_descName));

                    // The date in which the descriptor was written.
                    _stream.Position = _saveInfoAddr + _saveInfoLength + 0x40;
                    _write.Write(_writeDate);
                    _stream.Position = _saveInfoAddr + _saveInfoLength + 0x48;
                    _write.Write(_writeDate);

                    // The length of the descriptor.
                    _stream.Position = _saveInfoAddr + _saveInfoLength + 0x50;
                    _write.Write(0x400);
                    
                    // Write, the save.
                    _stream.Position = _saveDataAddr;
                    _write.Write(_saveData);

					// Write the Magic Values for the descriptor.
                    _stream.Position = _saveDataAddr + _saveDataLength;
                    _write.Write(Encoding.Default.GetBytes("KHSQ"));
					_stream.Position = _saveDataAddr + _saveDataLength + 0x04;
                    _write.Write(0x1317F14);

					// Write the actual descriptor data.
					_stream.Position = _saveDataAddr + _saveDataLength + 0x08;
                    _write.Write(_levelRead);
					_stream.Position = _saveDataAddr + _saveDataLength + 0x0C;
                    _write.Write(_munnyRead);
					_stream.Position = _saveDataAddr + _saveDataLength + 0x10;
                    _write.Write(_timeRead);

					_stream.Position = _saveDataAddr + _saveDataLength + 0x38;
                    _write.Write(_diffRead);
					_stream.Position = _saveDataAddr + _saveDataLength + 0x3C;
                    _write.Write(_worldRead);
					_stream.Position = _saveDataAddr + _saveDataLength + 0x3E;
                    _write.Write(_roomRead);
                }
            #endregion
        }

		public static void HandleAutosave()
        {
			var _fadeCheck = Hypervisor.Read<byte>(0x138DB2);
            var _battleRead = Hypervisor.Read<byte>(0x024C3352);
			var _titleCheck = Hypervisor.Read<byte>(Variables.ResetAddresses[1]);

            var _worldCheck = Hypervisor.Read<byte>(Variables.WorldAddress);
            var _roomCheck = Hypervisor.Read<byte>(Variables.WorldAddress + 0x68);

            // If not in the title screen, nor in a battle, and the room is loaded:
            if (_titleCheck > 0x02 && _battleRead == 0x00 && _fadeCheck == 0x80)
            {
                // If the past WorldID is not equal to the current WorldID:
                if (Variables.SaveWorld != _worldCheck)
                { 
                    CreateAutosave();
                    Variables.SaveIterator = 0;
                }

                else if (Variables.SaveRoom != _roomCheck && _worldCheck >= 1)
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

		public static void Execute()
		{
			HandleAutosave();

			SeekReset();
			OverrideFov();
			
		    OverrideText();
		    OverrideMP();

			ProcessRPC();
		}
	}
}
