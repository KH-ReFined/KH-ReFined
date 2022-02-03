/*
=================================================
      KINGDOM HEARTS - RE:FIXED FOR 2 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER MIT. GIVE CREDIT WHERE IT'S DUE! 
=================================================
*/

using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace ReFixed
{
	public static class Functions
	{
        public static bool IsTitle()
        {
            var _roomCheck = Hypervisor.Read<uint>(Variables.RoomAddress);
            var _titleCheck = Hypervisor.Read<uint>(Variables.TitleFlagAddress);

            return _roomCheck == 0x00FFFFFF || _roomCheck == 0x00000101 || _titleCheck == 0x01;
        }

        public static void HandleMagicSort()
        {
            // Fetch the input and read the pointer to the second CM.
            var _inputRead = Hypervisor.Read<ushort>(Variables.InputAddress);
            var _menuPointer = Hypervisor.Read<ulong>(Variables.MagicAddresses[0]);

            if (_menuPointer != 0)
            {
                // Read the secondary CM's type.
                var _menuRead = Hypervisor.Read<byte>(_menuPointer, true);

                // If the Magic Menu is the second CM to be shown (ID: 0x01):
                if (_menuRead == 0x01)
                {
                    // Fetch the current magic index and the max index.
                    var _magicIndex = Hypervisor.Read<byte>(Variables.MagicAddresses[2]);
                    var _magicMax = Hypervisor.Read<byte>(_menuPointer + 0x10) - 0x01;

                    // Set to 0x01 if it's going up, set to 0x02 if it's going down.
                    // TODO: Change from R2/L2 to R2+DirectionPad
                    var _inputCheck = (_inputRead & 0x1001) == 0x1001 ? 0x01 : (_inputRead & 0x4001) == 0x4001 ? 0x02 : 0x00;

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

            var _vibRead = Hypervisor.Read<byte>(Variables.VibrationAddress);
            var _diffRead = Hypervisor.Read<byte>(Variables.DifficultyAddress);

            if (_skipBool && !IsTitle())
            {
                switch (_vibRead)
                {
                    case 0x01:
                        Variables.SkipComplete = true;
                        Hypervisor.Write<byte>(Variables.VibrationAddress, 0);
                        break;
                    case 0x00:
                        Variables.SkipRoxas = true;
                        Variables.SkipComplete = false;
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

                    Hypervisor.Write<uint>(0x4447F2, 0x1FF00001);
                    Hypervisor.Write<uint>(0x4447F6, 0x00000000);
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
                        Hypervisor.Write<byte>(0x445017, 0x18);
                        Hypervisor.Write<byte>(0x445018, 0x18);
                        Hypervisor.WriteArray(0x445066, new byte[] { 0x89, 0x01, 0x88, 0x01, 0xA5, 0x01, 0x94, 0x01, 0x97, 0x01, 0x97, 0x01, 0x95, 0x01, 0x52, 0x00, 0x8A, 0x00, 0x9E, 0x00 });
                    }

                    else
                    {
                        Hypervisor.Write<byte>(0x445017, 0x1E);
                        Hypervisor.Write<byte>(0x445018, 0x1E);
                        Hypervisor.WriteArray(0x445066, new byte[] { 0x52, 0x00, 0x8A, 0x00, 0x9E, 0x00 });
                    }
                    
                    Hypervisor.Write<byte>(0x446222, 0x04);
                    
                    Hypervisor.Write<byte>(0x44622A, 0x06);
                    Hypervisor.Write<byte>(0x44622C, 0x40);
                    Hypervisor.Write<byte>(0x44622F, 0x02);

                    Variables.SkipRoxas = false;
                    Variables.SkipComplete = true;
                }
            }
        }

        public static void SeekReset()
        {
            var _inputRead = Hypervisor.Read<ushort>(Variables.InputAddress);

            if ((_inputRead & 0x0800) == 0x0800 && (_inputRead & 0x0100) == 0x0100)
                Hypervisor.Write<byte>(Variables.TitleBackAddress, 0x01);
        }

        public static void OverrideText()
        {
            var _textCheck = Hypervisor.Read<byte>(Variables.TitleTextAddresses[1]);

            if (_textCheck != 0x46)
            {
                var _buttOffset = Hypervisor.Read<uint>(Variables.TitleButtonAddress);
                Hypervisor.Write<uint>(Variables.TitleButtonAddress, _buttOffset + 0x01);

                for (int i = 0; i < Variables.TitleStrings.Count; i++)
                    Hypervisor.WriteArray(Variables.TitleTextAddresses[i], Variables.TitleStrings[i].ToKHSCII());
            }
        }

        public static void Execute()
        {
            SeekReset();
            OverrideText();
            HandleMagicSort();
            HandleTutorialSkip();
        }
    }
}