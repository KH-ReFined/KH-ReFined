/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 1 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
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
            var _levelValue = Hypervisor.Read<byte>(0x02A453CE);
			var _diffValue = Hypervisor.Read<byte>(0x02A5B7F6);

            var _stringState = string.Format("Level {0} | {1} Mode", _levelValue, Variables.ModeText.ElementAtOrDefault(_diffValue));
            var _stringDetail = string.Format("HP: {0} | MP: {1}", _healthValue, _magicValue);

            var _worldID = Hypervisor.Read<byte>(0x01F9C4D6);
			var _gummiCheck = Hypervisor.Read<byte>(0x00163C17);
            var _battleFlag = Hypervisor.Read<byte>(0x024C3352);

            var _timeValue = Math.Floor(Hypervisor.Read<int>(0x01F9BC4E) / 60F);
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
				Hypervisor.Write<byte>(Variables.ResetAddresses[0], 0x02);
				Hypervisor.Write<byte>(Variables.ResetAddresses[1], 0x01);
				Hypervisor.Write<byte>(Variables.ResetAddresses[2], 0x01);
			}
		}

		public static void Execute()
		{
			SeekReset();
			OverrideFov();
			
		    OverrideText();
		    OverrideMP();

			ProcessRPC();
		}
	}
}
