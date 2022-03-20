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
                { '-', 0x54 },
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
                { ']', 0x63 },
                { 'à', 0xB7 },
                { 'á', 0xB8 },
                { 'â', 0xB9 },
                { 'ä', 0xBA },
                { 'è', 0xBB },
                { 'é', 0xBC },
                { 'ê', 0xBD },
                { 'ë', 0xBE },
                { 'ì', 0xBF },
                { 'í', 0xC0 },
                { 'î', 0xC1 },
                { 'ï', 0xC2 },
                { 'ñ', 0xC3 },
                { 'ò', 0xC4 },
                { 'ó', 0xC5 },
                { 'ô', 0xC6 },
                { 'ö', 0xC7 },
                { 'ù', 0xC8 },
                { 'ú', 0xC9 },
                { 'û', 0xCA },
                { 'ü', 0xCB },
                { 'ç', 0xE8 },
                { 'À', 0xD0 },
                { 'Á', 0xD1 },
                { 'Â', 0xD2 },
                { 'Ä', 0xD3 },
                { 'È', 0xD4 },
                { 'É', 0xD5 },
                { 'Ê', 0xD6 },
                { 'Ë', 0xD7 },
                { 'Ì', 0xD8 },
                { 'Í', 0xD9 },
                { 'Î', 0xDA },
                { 'Ï', 0xDB },
                { 'Ñ', 0xDC },
                { 'Ò', 0xDD },
                { 'Ó', 0xDE },
                { 'Ô', 0xDF },
                { 'Ö', 0xE0 },
                { 'Ù', 0xE1 },
                { 'Ú', 0xE2 },
                { 'Û', 0xE3 },
                { 'Ü', 0xE4 },
                { '¡', 0xE5 },
                { '¿', 0xE6 },
                { 'Ç', 0xE7 }
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

        public static uint CalculateCRC32(byte[] data, int offset, uint checksum)
        {
            uint[] array = GetCRC32Table(0x4C11DB7).Take(0x100).ToArray();

            for (var i = 0; i < offset; i++)
                checksum = array[(checksum >> 24) ^ data[i]] ^ (checksum << 8);

            return checksum ^ uint.MaxValue;
        }

        public static IEnumerable<uint> GetCRC32Table(int polynomial)
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
