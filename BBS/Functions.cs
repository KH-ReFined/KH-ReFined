/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR BBS!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
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

            var _worldID = Hypervisor.Read<byte>(0x206D6C);
			var _roomID = Hypervisor.Read<byte>(0x206D6D);
            var _battleFlag = Hypervisor.Read<byte>(0x1098CF94);

			if (_worldID != 0xFF && _roomID != 0xFF)
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
			var _menuPointer = Hypervisor.Read<ulong>(Variables.FinisherAddress);

			// If the Status Menu is open, and it's pointer fetched:
			if (_menuPointer > 0)
			{
				// Fetch the Finisher Menu pointer.
				var _finishPointer = Hypervisor.Read<ulong>(_menuPointer + 0xC8, true);

				// If the Finisher Menu is open, and it's pointer fetched:
				if (_finishPointer > 0)
				{
					// Read the input, and the finisher that is currently selected.
					// "Selected" means "Hovering", not "Active".
					var _inputRead = Hypervisor.Read<ushort>(Variables.InputAddress);
					var _selectedFinisher = Hypervisor.Read<byte>(_finishPointer + 0x8E, true);

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

            if ((_inputRead & 0x0008) == 0x0008 && (_inputRead & 0x0001) == 0x0001)
                Hypervisor.Write<byte>(Variables.LimiterAddress + 0x0C, 0x01);
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

		public static void Execute()
		{
			SeekReset();
			
			RenameFinisher();
			OverrideLimiter();

			ProcessRPC();
		}
	}
}
