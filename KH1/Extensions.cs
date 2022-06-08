/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 1 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.Linq;
using System.Collections.Generic;
using System.Text.RegularExpressions;

using NAudio.Wave;

namespace ReFixed
{
    public static class Extensions
    {
        public static void PlaySFX(string Input)
		{
			var _output = new DirectSoundOut();
			var _wavRead = new AudioFileReader(Input);

			var _volumeSFX = Hypervisor.Read<byte>(0x1F36672);
            var _volumeMaster = Hypervisor.Read<byte>(0x1F3666E);
            
			var _sfxValue = Hypervisor.Read<float>(0x364B2 + (ulong)(0x04 * _volumeSFX));
			var _masterValue = Hypervisor.Read<float>(0x364B2 + (ulong)(0x04 * _volumeMaster));

			_wavRead.Volume = _sfxValue * _masterValue;

			_output.Init(_wavRead);
			_output.Play();
		}

        public static ulong FindValue(this byte[] Source, byte[] Value)
        {
            ulong _charSlot = (ulong)(Source.Length - Value.Length + 1);

            for (ulong i = 0; i < _charSlot; i++)
            {
                if (Source[i] != Value[0])
                    continue;

                for (ulong j = (ulong)Value.Length - 1; j >= 1; j--)
                {
                    if (Source[i + j] != Value[j])
                        break;

                    if (j == 1)
                        return i;
                }
            }
            return 0xFFFFFFFFFFFFFFFF;
        }

        public static ulong FindValue(this byte[] Source, ushort Value)
        {
            var _pattern = BitConverter.GetBytes(Value);
            ulong _charSlot = (ulong)(Source.Length - _pattern.Length + 1);

            for (ulong i = 0; i < _charSlot; i++)
            {
                if (Source[i] != _pattern[0])
                    continue;

                for (ulong j = (ulong)_pattern.Length - 1; j >= 1; j--)
                {
                    if (Source[i + j] != _pattern[j])
                        break;

                    if (j == 1)
                        return i;
                }
            }
            return 0xFFFFFFFFFFFFFFFF;
        }

        public static byte[] ToKHSCII(this string inText)
        {
            var _specialDict = new Dictionary<char, byte>
            {
                { ' ', 0x01 },
                { '\n', 0x02 },
                { '-', 0x6E },
                { '!', 0x5F },
                { '?', 0x60 },
                { '%', 0x62 },
                { '/', 0x66 },
                { '.', 0x68 },
                { ',', 0x69 },
                { ';', 0x6C },
                { ':', 0x6B },
                { '\'', 0x71 },
                { '(', 0x74 },
                { ')', 0x75 },
                { '[', 0x76 },
                { ']', 0x77 }
            };

            var _outList = new List<byte>();
            var _charCount = 0;

            while (_charCount < inText.Length)
            {
                var _char = inText[_charCount];

                if (_char >= 'a' && _char <= 'z')
                {
                    _outList.Add((byte)(_char - 0x1C));
                    _charCount++;
                }
				
                else if (_char >= 'A' && _char <= 'Z')
                {
                    _outList.Add((byte)(_char - 0x16));
                    _charCount++;
                }
				
                else if (_char >= '0' && _char <= '9')
                {
                    _outList.Add((byte)(_char - 0x0F));
                    _charCount++;
                }

                else if (_char == '{')
                {
                    var _command = inText.Substring(_charCount, 0x06);

                    if (Regex.IsMatch(_command, "^{0x[a-fA-F0-9][a-fA-F0-9]}$"))
                    {
                        var _value = _command.Substring(0x01, 0x04);
                        _outList.Add(Convert.ToByte(_value, 0x10));
                        _charCount += 6;
                    }
                }

                else
                {
                    if (_specialDict.ContainsKey(_char))
                        _outList.Add(_specialDict[_char]);
                    else
                        _outList.Add(0x01);
                    _charCount++;
                }
            }

            _outList.Add(0x00);
            return _outList.ToArray();
        }
    }
}
