/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 2 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.Linq;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace ReFixed
{
    public static class Extensions
    {
        public static byte[] ToKHSCII(this string inText)
		{
			var _specialDict = new Dictionary<char, byte>
			{
				{ ' ', 0x01 },
				{ '\n', 0x02 },
				{ '-', 0x2C },
				{ '!', 0x48 },
				{ '?', 0x49 },
				{ '%', 0x4A },
				{ '/', 0x4B },
				{ '.', 0x4F },
				{ ',', 0x50 },
				{ ';', 0x51 },
				{ ':', 0x52 },
				{ '\'', 0x57 },
				{ '(', 0x5A },
				{ ')', 0x5B },
				{ '[', 0x62 },
				{ ']', 0x63 }
			};

			var _outList = new List<byte>();
			var _charCount = 0;

			while (_charCount < inText.Length)
			{
				var _char = inText[_charCount];

				if (_char >= 'a' && _char <= 'z')
				{
					_outList.Add((byte)(_char + 0x39));
					_charCount++;
				}

				else if (_char >= 'A' && _char <= 'Z')
				{
					_outList.Add((byte)(_char - 0x13));
					_charCount++;
				}

				else if (_char >= '0' && _char <= '9')
				{
					_outList.Add((byte)(_char + 0x60));
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

		private static uint CalculateCRC32(byte[] data, int offset, uint checksum)
		{
			uint[] array = GetCRC32Table(0x4C11DB7).Take(0x100).ToArray();
			
			for (var i = 0; i < offset; i++)
                checksum = crc_table[(checksum >> 24) ^ data[i]] ^ (checksum << 8);

            return checksum ^ uint.MaxValue;
		}

		private static IEnumerable<uint> GetCRC32Table(int polynomial)
        {
            for (var x = 0; ; x++)
            {
                var r = x << 24;

                for (var j = 0; j < 0xff; j++)
                    r = r << 1 ^ (r < 0 ? polynomial : 0);

                yield return (uint)r;
            }
        }
    }
}