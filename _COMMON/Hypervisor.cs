/*
==================================================
     KINGDOM HEARTS - RE:FIXED COMMON FILE
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.IO;
using System.Text;
using System.Diagnostics;
using System.Reflection.Emit;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;

namespace ReFixed
{
	public static class Hypervisor
	{
        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern bool VirtualProtectEx(IntPtr hProcess, IntPtr lpAddress, int dwSize, uint flNewProtect, ref int lpflOldProtect);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, int dwSize, ref int lpNumberOfBytesWritten);

        [DllImport("kernel32.dll")]
        public static extern bool ReadProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, int dwSize, ref int lpNumberOfBytesRead);

        public static T Read<T>(ulong Address, bool Absolute = false) where T : struct
        {
            IntPtr _address = (IntPtr)(Variables.GameAddress + Address);

            if (Absolute)
                _address = (IntPtr)(Address);

            var _dynoMethod = new DynamicMethod("SizeOfType", typeof(int), new Type[] { });
            ILGenerator _ilGen = _dynoMethod.GetILGenerator();

            _ilGen.Emit(OpCodes.Sizeof, typeof(T));
            _ilGen.Emit(OpCodes.Ret);

            var _outSize = (int)_dynoMethod.Invoke(null, null);

            var _outArray = new byte[_outSize];
            int _outRead = 0;

                ReadProcessMemory(Variables.GameHandle, _address, _outArray, _outSize, ref _outRead);

            var _gcHandle = GCHandle.Alloc(_outArray, GCHandleType.Pinned);
            var _retData = (T)Marshal.PtrToStructure(_gcHandle.AddrOfPinnedObject(), typeof(T));
            
            _gcHandle.Free();  
                    
            return _retData;
        }

        public static void Write<T>(ulong Address, T Value, bool Absolute = false) where T : struct
        {
            IntPtr _address = (IntPtr)(Variables.GameAddress + Address);

            if (Absolute)
                _address = (IntPtr)(Address);

            var _dynoMethod = new DynamicMethod("SizeOfType", typeof(int), new Type[] { });
            ILGenerator _ilGen = _dynoMethod.GetILGenerator();

            _ilGen.Emit(OpCodes.Sizeof, typeof(T));
            _ilGen.Emit(OpCodes.Ret);

            var _inSize = (int)_dynoMethod.Invoke(null, null);
            int _inWrite = 0;

            if (_inSize > 1)
            {
                var _inArray = (byte[])typeof(BitConverter).GetMethod("GetBytes", new[] { typeof(T) }) .Invoke(null, new object[] { Value });

                WriteProcessMemory(Variables.GameHandle, _address, _inArray, _inArray.Length, ref _inWrite);
            }

            else
            {
                var _inArray = { (byte)Convert.ChangeType(Value, typeof(byte)) };
                WriteProcessMemory(Variables.GameHandle, _address, _inArray, _inArray.Length, ref _inWrite);

            }
        }

        public static byte[] ReadArray(ulong Address, int Length, bool Absolute = false)
        {
            IntPtr _address = (IntPtr)(Variables.GameAddress + Address);

            if (Absolute)
                _address = (IntPtr)(Address);

            var _outArray = new byte[Length];
            int _outRead = 0;

            ReadProcessMemory(Variables.GameHandle, _address, _outArray, Length, ref _outRead);

            return _outArray;
        }

        public static void WriteArray(ulong Address, byte[] Value, bool Absolute = false)
        {
            IntPtr _address = (IntPtr)(Variables.GameAddress + Address);

            if (Absolute)
                _address = (IntPtr)(Address);

            int _inWrite = 0;

            WriteProcessMemory(Variables.GameHandle, _address, Value, Value.Length, ref _inWrite);
        }

        public static string ReadTerminate(ulong Address, bool Absolute = false)
        {
            IntPtr _address = (IntPtr)(Variables.GameAddress + Address);

            if (Absolute)
                _address = (IntPtr)(Address);

            var _length = 0;

            while (Read<byte>((ulong)(_address + _length), Absolute) != 0x00)
                _length++;

            var _outArray = new byte[_length];
            int _outRead = 0;

            ReadProcessMemory(Variables.GameHandle, _address, _outArray, _length, ref _outRead);

            return Encoding.Default.GetString(_outArray);
        }

        public static void UnlockBlock(ulong Address, bool Absolute = false)
        {
			IntPtr _address = (IntPtr)(Variables.GameAddress + Address);

            if (Absolute)
                _address = (IntPtr)(Address);

            int _oldProtect = 0;
            VirtualProtectEx(Variables.GameHandle, _address, 0x100, 0x40, ref _oldProtect);
        }
    }
}