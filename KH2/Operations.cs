using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Collections.Generic;
using System.Text.RegularExpressions;
using System.Runtime.InteropServices;
using BSharpConvention = Binarysharp.MSharp.Assembly.CallingConvention.CallingConventions;

namespace ReFined
{
    public static class Operations
    {
        [DllImport("msvcrt.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern int memcmp(byte[] b1, byte[] b2, long count);

        /// <summary>
        /// Generates a CRC32 Table for Save Checksum Generation, courtesy of Xeeynamo.
        /// </summary>
        private static IEnumerable<uint> _fetchTable()
        {
            for (var x = 0; ; x++)
            {
                var r = x << 24;

                for (var j = 0; j < 0xff; j++)
                    r = r << 1 ^ (r < 0 ? 0x4C11DB7 : 0);

                yield return (uint)r;
            }
        }

        /// <summary>
        /// Check to see if the game is in the title screen.
        /// </summary>
        /// <returns>"True" if it's in the title, "False" otherwise.</returns>
        public static bool CheckTitle() =>
            Hypervisor.Read<uint>(Variables.ADDR_Area) == 0x00FFFFFF
         || Hypervisor.Read<uint>(Variables.ADDR_Area) == 0x00000101
         || Hypervisor.Read<uint>(Variables.ADDR_Title) == 0x00000001
         || Hypervisor.Read<uint>(Variables.ADDR_Reset) == 0x00000001;

        /// <summary>
        /// Generates the CRC32 Checksum for a Save File, courtesy of Xeeynamo.
        /// </summary>
        /// <param name="data">The save file itself.</param>
        /// <param name="offset">The offset of the true data.</param>
        /// <param name="checksum">The previous checksum, if any.</param>
        /// <returns>The CRC32 Checksum of the given data.</returns>
        public static uint SaveCRC32(byte[] data, int offset, uint checksum)
        {
            uint[] array = _fetchTable().Take(0x100).ToArray();

            for (var i = 0; i < offset; i++)
                checksum = array[(checksum >> 24) ^ data[i]] ^ (checksum << 8);

            return checksum ^ uint.MaxValue;
        }

        /// <summary>
        /// Fetches a string from a given MSG file.
        /// </summary>
        /// <param name="StartMSG">The address in which the MSG file starts. Must be a valid MSG file.</param>
        /// <param name="StringID">The ID of the String to fetch the pointer of.</param>
        /// <returns>The string requested in KHSCII.</returns>
        /// <exception cref="InvalidDataException"></exception>
        public static byte[] FetchStringMSG(ulong StartMSG, ushort StringID)
        {
            var _msnAbsolute = Hypervisor.Read<ulong>(StartMSG);

            var _checkFirst = Hypervisor.Read<int>(_msnAbsolute, true);
            var _checkSecond = Hypervisor.Read<int>(_msnAbsolute - 0x30, true);

            if (_checkFirst != 0x01 || _checkSecond != 0x01524142)
                return null;

            var _fetchCount = Hypervisor.Read<int>(_msnAbsolute + 0x04, true);
            var _fetchData = Hypervisor.ReadArray(_msnAbsolute + 0x08, _fetchCount * 0x08, true);

            var _offsetLocal = _fetchData.FindValue<int>(StringID);

            var _offsetString = Hypervisor.Read<int>(_msnAbsolute + _offsetLocal + 0x0C, true);

            int _readOffset = 0;
            List<byte> _returnList = new List<byte>();

            while (true)
            {
                var _byte = Hypervisor.Read<byte>(_msnAbsolute + (ulong)(_offsetString + _readOffset), true);

                _returnList.Add(_byte);

                if (_byte == 0x00)
                    break;

                else
                    _readOffset++;
            }

            return _returnList.ToArray();
        }

        /// <summary>
        /// Fetches the absolute pointer of a String stored inside a MSG file.
        /// Usually needed for functions such as "ShowInformation".
        /// </summary>
        /// <param name="StartMSG">The address in which the MSG file starts. Must be a valid MSG file.</param>
        /// <param name="StringID">The ID of the String to fetch the pointer of.</param>
        /// <returns>The absolute pointer of the given string.</returns>
        public static long FetchPointerMSG(ulong StartMSG, ushort StringID)
        {
            var _msnAbsolute = Hypervisor.Read<ulong>(StartMSG);

            var _checkFirst = Hypervisor.Read<int>(_msnAbsolute, true);
            var _checkSecond = Hypervisor.Read<int>(_msnAbsolute - 0x30, true);

            if (_checkFirst != 0x01 || _checkSecond != 0x01524142)
                return 0x00;

            var _fetchCount = Hypervisor.Read<int>(_msnAbsolute + 0x04, true);
            var _fetchData = Hypervisor.ReadArray(_msnAbsolute + 0x08, _fetchCount * 0x08, true);

            var _offsetLocal = _fetchData.FindValue<int>(StringID);

            var _offsetString = Hypervisor.Read<int>(_msnAbsolute + _offsetLocal + 0x0C, true);

            return (long)_msnAbsolute + _offsetString;
        }

        /// <summary>
        /// Fetches the offset of a String stored inside a MSG file.
        /// </summary>
        /// <param name="StartMSG">The address in which the MSG file starts. Must be a valid MSG file.</param>
        /// <param name="StringID">The ID of the String to fetch the pointer of.</param>
        /// <returns>The offset of the given string (Absolute).</returns>
        public static int FetchOffsetMSG(ulong StartMSG, ushort StringID)
        {
            var _msnAbsolute = Hypervisor.Read<ulong>(StartMSG);

            var _checkFirst = Hypervisor.Read<int>(_msnAbsolute, true);
            var _checkSecond = Hypervisor.Read<int>(_msnAbsolute - 0x30, true);

            if (_checkFirst != 0x01 || _checkSecond != 0x01524142)
                return 0x00;

            var _fetchCount = Hypervisor.Read<int>(_msnAbsolute + 0x04, true);
            var _fetchData = Hypervisor.ReadArray(_msnAbsolute + 0x08, _fetchCount * 0x08, true);

            var _offsetLocal = _fetchData.FindValue(StringID);

            var _offsetString = Hypervisor.Read<int>(_msnAbsolute + _offsetLocal + 0x0C, true);

            return _offsetString;
        }

        /// <summary>
        /// Finds the info of a String stored inside a MSG file.
        /// </summary>
        /// <param name="StartMSG">The address in which the MSG file starts. Must be a valid MSG file.</param>
        /// <param name="StringID">The ID of the String to fetch the pointer of.</param>
        /// <returns></returns>
        public static ulong FindInfoMSG(ulong StartMSG, ushort StringID)
        {
            var _msnAbsolute = Hypervisor.Read<ulong>(StartMSG);

            var _checkFirst = Hypervisor.Read<int>(_msnAbsolute, true);
            var _checkSecond = Hypervisor.Read<int>(_msnAbsolute - 0x30, true);

            if (_checkFirst != 0x01 || _checkSecond != 0x01524142)
                return 0x00;

            var _fetchCount = Hypervisor.Read<int>(_msnAbsolute + 0x04, true);
            var _fetchData = Hypervisor.ReadArray(_msnAbsolute + 0x08, _fetchCount * 0x08, true);

            var _offsetLocal = _fetchData.FindValue<int>(StringID);

            return _msnAbsolute + _offsetLocal + 0x08;
        }

        /// <summary>
        /// Converts any given string to a KH-Compliant string called "KHSCII".
        /// Supports the dynamic argument characters, characterized with {}.
        /// </summary>
        /// <param name="Input">The text to be converted, through an extension.</param>
        /// <returns>The given string in KHSCII.</returns>
        public static byte[] ToKHSCII(this string Input)
        {
            // A dictionary of all the special characters, which
            // are hard to convert through a mathematical formula.
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

            // Throughout the text, do:
            while (_charCount < Input.Length)
            {
                var _char = Input[_charCount];

                // Simple character conversion through mathematics.
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

                // If it hits a "{", we will know it's a command, not a character.
                else if (_char == '{')
                {
                    // A command is 6 characters long, in the format of "{0xTT}",
                    // with the "TT" being the 2-digit encode for that command.
                    var _command = Input.Substring(_charCount, 0x06);

                    if (Regex.IsMatch(_command, "^{0x[a-fA-F0-9][a-fA-F0-9]}$"))
                    {
                        var _value = _command.Substring(0x01, 0x04);
                        _outList.Add(Convert.ToByte(_value, 0x10));
                        _charCount += 6;
                    }
                }

                // Should it be anything we do not know, we look through
                // the special dictionary.
                else
                {
                    if (_specialDict.ContainsKey(_char))
                        _outList.Add(_specialDict[_char]);

                    else
                        _outList.Add(0x01);
                    _charCount++;
                }
            }

            // When the list ends, we add a terminator and return the string.
            _outList.Add(0x00);
            return _outList.ToArray();
        }

        /// <summary>
        /// Finds a file in the Buffer Cache.
        /// </summary>
        /// <param name="Input">The name of the file.</param>
        /// <returns>The absolute position of the file indicator in memory. "0" if not found.</returns>
        public static ulong FindFile(string Input)
        {
            var _memoryOffset = Hypervisor.PureAddress & 0x7FFF00000000;
            var _returnValue = Variables.SharpHook[(IntPtr)0x39A820].Execute<uint>(BSharpConvention.MicrosoftX64, Input, -1);
            return _returnValue == 0x00 ? 0x00 : _memoryOffset + _returnValue;
        }

        /// <summary>
        /// Gets the size of the file, used primarily to check if a file actually exists.
        /// </summary>
        /// <param name="Input">The name of the file.</param>
        /// <returns>A 32-bit integer containing the size in bytes, "0" if the file is not found.</returns>
        public static int GetFileSize(string Input) => Variables.SharpHook[(IntPtr)0x39E2F0].Execute<int>(Input);
        
        /// <summary>
        /// Generates and writes the current game state to both RAM and ROM Save Files.
        /// </summary>
        public static void GenerateSave()
        {
            // Prepare the pointers.
            var _pointerBase = Hypervisor.Read<ulong>(Variables.PINT_SaveInformation);
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
            var _saveData = Hypervisor.ReadArray(Variables.ADDR_SaveData, _saveDataLength);

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

            var _checkMagic = SaveCRC32(_magicArray, 8, uint.MaxValue);
            var _checkData = SaveCRC32(_dataArray, _dataArray.Length, _checkMagic ^ uint.MaxValue);

            #region RAM Save
            // Fetch the address for the save info.
            var _saveInfoAddrRAM = _saveInfoStartRAM + (ulong)(_saveInfoLength * _saveSlot);
            var _saveDataAddrRAM = _saveDataStartRAM + (ulong)(_saveDataLength * _saveSlot);

            // Write out the save information.
            Hypervisor.WriteArray(_saveInfoAddrRAM, Encoding.Default.GetBytes(_saveName), true);

            // Write the date in which the save was made.
            Hypervisor.Write(_saveInfoAddrRAM + 0x40, _writeDate, true);
            Hypervisor.Write(_saveInfoAddrRAM + 0x48, _writeDate, true);

            // Write the length of the save.
            Hypervisor.Write(_saveInfoAddrRAM + 0x50, _saveDataLength, true);

            // Write the header.
            Hypervisor.WriteArray(_saveDataAddrRAM, Encoding.ASCII.GetBytes("KH2J"), true);
            Hypervisor.Write<uint>(_saveDataAddrRAM + 0x04, 0x3A, true);

            // Write the checksum.
            Hypervisor.Write(_saveDataAddrRAM + 0x08, _checkData, true);

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
                _write.Write(Encoding.ASCII.GetBytes(_saveName));

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
                _write.Write(Encoding.ASCII.GetBytes("KH2J"));
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

            if (Variables.SAVE_MODE == 0x00)
            {
                Hypervisor.Write<byte>(Hypervisor.PureAddress + 0x18BA09, 0x28, true);
                Additions.ShowInformation(0x4D6C);
                Hypervisor.Write<byte>(Hypervisor.PureAddress + 0x18BA09, 0x22, true);
            }

        }
    }
}
