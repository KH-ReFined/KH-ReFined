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
using System.Threading;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

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
			Hypervisor.UnlockBlock(0x10F2E);

            Variables.Initialized = true;
        }

        /*
            CheckTitle:

            Checks certain points in RAM to see if the player is in the Title Screen.
            Returns **true** if so, returns **false** otherwise. 
        */
		public static bool CheckTitle() => Hypervisor.Read<byte>(Variables.WorldAddress) == 0xFF || 
										   Hypervisor.Read<byte>(Variables.WorldAddress + 0x68) == 0xFF || 
										   Hypervisor.Read<byte>(Variables.WorldAddress) == 0x00 || 
										   Hypervisor.Read<byte>(Variables.LevelAddress) == 0;

		/*
            ResetGame:

            Triggers a soft-reset if the proper input is given.
            The input is sought in Execute().

            INPUT: L1 + R1 + START + SELECT.
        */
        public static void ResetGame()
		{
			var _inputRead = Hypervisor.Read<ushort>(Variables.InputAddress);

			var _selectRead = Hypervisor.Read<byte>(Variables.SaveMenuSelect);
			var _amountRead = Hypervisor.Read<byte>(Variables.SaveMenuSelect + 0x044);

			var _buttonRead = _inputRead == 0x0C09;
			var _saveMenuRead = (_selectRead == _amountRead - 0x01) && (_inputRead & 0x4000) == 0x4000;
			
			if (_buttonRead || _saveMenuRead)
			{
				Hypervisor.Write<byte>(Variables.ResetAddresses[0], 0x01);
				Hypervisor.Write<byte>(Variables.ResetAddresses[1], 0x01);
			}
		}
		
		/*
            AspectCorrection:

            Automatically detect the aspect ratio in Full Screen
			and accommodate for it.
        */
		public static void AspectCorrection(float InputValue)
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

			Hypervisor.Write<float>(0x10F2E, _floatValue);
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

        /*
            AutosaveEngine:

            As the name suggests, handle the logic behind Autosave functionality.
        */
		public static void AutosaveEngine()
        {		    
			var _fadeCheck = Hypervisor.Read<byte>(0x138DB2);
			var _battleRead = Hypervisor.Read<byte>(0x024C3352);
			var _titleCheck = Hypervisor.Read<byte>(Variables.ResetAddresses[1]);

			var _worldCheck = Hypervisor.Read<byte>(Variables.WorldAddress);
			var _roomCheck = Hypervisor.Read<byte>(Variables.WorldAddress + 0x68);

			if (_titleCheck > 0x02 && _battleRead == 0x00 && _fadeCheck == 0x80)
			{
				if (Variables.SaveWorld != _worldCheck)
				{ 
					GenerateSave();
					Variables.SaveIterator = 0;
				}

				else if (Variables.SaveRoom != _roomCheck && _worldCheck >= 1)
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

		/*
			FieldOfView:

			Toggle between the original and a KH2-esque Field of View.
			This is detemined by the **Vibration** option at the Camp Menu.
		*/
		public static void FieldOfView()
		{
		    switch(Hypervisor.Read<int>(Variables.VibrationAddress))
		    {
				case 0:
				{
					if (Hypervisor.Read<float>(Variables.FovAddresses[0]) != 400F)
						for (uint i = 0; i < Variables.FovAddresses.Length; i++)
							Hypervisor.Write<float>(Variables.FovAddresses[i], Variables.FovClassic[i]);

					break;
				}

				case 1:
				{
					if (Hypervisor.Read<float>(Variables.FovAddresses[0]) != 600F)
						for (int i = 0; i < Variables.FovAddresses.Length; i++)
							Hypervisor.Write<float>(Variables.FovAddresses[i], Variables.FovEnhanced[i]);

					break;
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

		/*
            MagicHide:

            Hides the MP Bar until Sora learns a spell.
        */
        public static void MagicHide()
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

		/*
            DiscordEngine:

            Handle the Discord Rich Presence of Re:Fixed.
            To be executed on a separate thread.
        */
        public static void DiscordEngine()
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
				Variables.DiscordClient.SetPresence(new RichPresence
				{
					Details = _stringDetail,
					State = _stringState,
					Assets = new Assets
					{
						LargeImageKey = _gummiCheck == 0 ? Variables.WorldImages.ElementAtOrDefault(_worldID) : "wm",
						LargeImageText = _timeText,
						SmallImageKey = _battleFlag % 2 == 0 ? "safe" : "battle",
						SmallImageText = _battleFlag % 2 == 0 ? "Safe" : "In Battle"
					},
					
					Buttons = _rpcButtons
				});
			}

			else
			{				
				Variables.DiscordClient.SetPresence(new RichPresence
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


		/*
            Execute:

            Executes the main logic within Re:Fixed.
        */
        public static void Execute()
        {
            #region High Priority
                if (!Variables.Initialized)
					Initialization();

				ResetGame();
                AutosaveEngine();
            #endregion

            #region Mid Priority
				MagicHide();
                FieldOfView();
            #endregion

            #region Low Priority
                TextAdjust();
            #endregion

            #region Discord
                if (Variables.DiscordTask == null)
                {
                    Variables.CancelSource = new CancellationTokenSource();
                    Variables.DiscordToken = Variables.CancelSource.Token;

                    Variables.DiscordTask = Task.Factory.StartNew(delegate()
                    {
                        while (!Variables.DiscordToken.IsCancellationRequested)
                        {
                            DiscordEngine();
                            Thread.Sleep(5);
                        }
                    }, Variables.DiscordToken);
                }
            #endregion
        }
	}
}
