/*
==================================================
     KINGDOM HEARTS - RE:FINED COMMON FILE
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.IO;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Reflection.Emit;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;

namespace ReFined
{
    public static class Hypervisor
    {
        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool VirtualProtectEx(IntPtr hProcess, IntPtr lpAddress, int dwSize, uint flNewProtect, ref int lpflOldProtect);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, int dwSize, ref int lpNumberOfBytesWritten);

        [DllImport("kernel32.dll")]
        static extern bool ReadProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, int dwSize, ref int lpNumberOfBytesRead);

        public static Process Process;
        static IntPtr Handle;

        public static ulong BaseAddress;
        public static ulong PureAddress;

        public static ProcessModule DLLModule;
        public static ulong DLLAddress;


        /// <summary>
        /// Initialize the Hypervisor on a process.
        /// </summary>
        /// <param name="Input">The input process.</param>
        /// <param name="Offset">The offset which will be used with non-absolute writes.</param>
        public static void AttachProcess(Process Input, ulong Offset = 0)
        {
            Process = Input;
            Handle = Input.Handle;
            PureAddress = (ulong)Input.MainModule.BaseAddress;
            BaseAddress = PureAddress + Offset;

			foreach (ProcessModule _module in Input.Modules)
			{
				if (_module.ModuleName == "EOSSDK-Win64-Shipping.dll")
				{
					DLLModule = _module;
					DLLAddress = (ulong)_module.BaseAddress;
				}
			}
            
        }

        /// <summary>
        /// Reads a value with the type of T from an address.
        /// Unsafe, must be used with caution.
        /// </summary>
        /// <typeparam name="T">Type of the value to read. Must have a size.</typeparam>
        /// <param name="Address">The address of the value to read.</param>
        /// <param name="Absolute">Whether the address is an absolute address or not. Defaults to false.</param>
        /// <returns></returns>
        public static T Read<T>(ulong Address, bool Absolute = false) where T : struct
        {
            IntPtr _address = (IntPtr)(BaseAddress + Address);

            if (Absolute)
                _address = (IntPtr)(Address);

            var _dynoMethod = new DynamicMethod("SizeOfType", typeof(int), new Type[] { });
            ILGenerator _ilGen = _dynoMethod.GetILGenerator();

            _ilGen.Emit(OpCodes.Sizeof, typeof(T));
            _ilGen.Emit(OpCodes.Ret);

            var _outSize = (int)_dynoMethod.Invoke(null, null);

            var _outArray = new byte[_outSize];
            int _outRead = 0;

                ReadProcessMemory(Handle, _address, _outArray, _outSize, ref _outRead);

            var _gcHandle = GCHandle.Alloc(_outArray, GCHandleType.Pinned);
            var _retData = (T)Marshal.PtrToStructure(_gcHandle.AddrOfPinnedObject(), typeof(T));
            
            _gcHandle.Free();  
                    
            return _retData;
        }


        /// <summary>
        /// Writes a value with the type of T to an address.
        /// Unsafe, must be used with caution.
        /// </summary>
        /// <typeparam name="T">Type of the value to write. Must have a size.</typeparam>
        /// <param name="Address">The address which the value will be written to.</param>
        /// <param name="Absolute">Whether the address is an absolute address or not. Defaults to false.</param>
        public static void Write<T>(ulong Address, T Value, bool Absolute = false) where T : struct
        {
            IntPtr _address = (IntPtr)(BaseAddress + Address);

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

                WriteProcessMemory(Handle, _address, _inArray, _inArray.Length, ref _inWrite);
            }

            else
            {
                var _inArray = new byte[] { (byte)Convert.ChangeType(Value, typeof(byte)) };
                WriteProcessMemory(Handle, _address, _inArray, _inArray.Length, ref _inWrite);
            }
        }

        /// <summary>
        /// Reads a byte array from an address.
        /// </summary>
        /// <param name="Address">The address which the value will be read from.</param>
        /// <param name="Length">The length of the array to read.</param>
        /// <param name="Absolute">Whether the address is an absolute address or not. Defaults to false.</param>
        /// <returns></returns>
        public static byte[] ReadArray(ulong Address, int Length, bool Absolute = false)
        {
            IntPtr _address = (IntPtr)(BaseAddress + Address);

            if (Absolute)
                _address = (IntPtr)(Address);

            var _outArray = new byte[Length];
            int _outRead = 0;

            ReadProcessMemory(Handle, _address, _outArray, Length, ref _outRead);

            return _outArray;
        }

        /// <summary>
        /// Writes a byte array to an address.
        /// </summary>
        /// <param name="Address">The address which the value will be written to.</param>
        /// <param name="Value">The array to write.</param>
        /// <param name="Absolute">Whether the address is an absolute address or not. Defaults to false.</param>
        public static void WriteArray(ulong Address, byte[] Value, bool Absolute = false)
        {
            IntPtr _address = (IntPtr)(BaseAddress + Address);

            if (Absolute)
                _address = (IntPtr)(Address);

            int _inWrite = 0;

            WriteProcessMemory(Handle, _address, Value, Value.Length, ref _inWrite);
        }

        /// <summary>
        /// Reads a terminated string from address.
        /// </summary>
        /// <param name="Address">The address which the value will be read from.</param>
        /// <param name="Absolute">Whether the address is an absolute address or not. Defaults to false.</param>
        /// <returns></returns>
        public static string ReadTerminate(ulong Address, bool Absolute = false)
        {
            IntPtr _address = (IntPtr)(BaseAddress + Address);

            if (Absolute)
                _address = (IntPtr)(Address);

            var _length = 0;

            while (Read<byte>((ulong)(_address + _length), true) != 0x00)
                _length++;

            var _outArray = new byte[_length];
            int _outRead = 0;

            ReadProcessMemory(Handle, _address, _outArray, _length, ref _outRead);

            return Encoding.GetEncoding(437).GetString(_outArray);
        }

        /// <summary>
        /// Writes a string to an address.
        /// </summary>
        /// <param name="Address">The address which the value will be written to.</param>
        /// <param name="Value">The string to write.</param>
        /// <param name="Absolute">Whether the address is an absolute address or not. Defaults to false.</param>
        public static void WriteString(ulong Address, string Value, bool Absolute = false, bool Unicode = false)
        {
            IntPtr _address = (IntPtr)(BaseAddress + Address);

            if (Absolute)
                _address = (IntPtr)(Address);

            int _inWrite = 0;

            var _stringArray = Encoding.GetEncoding(437).GetBytes(Value);

            if (Unicode)
                _stringArray = Encoding.Unicode.GetBytes(Value);


            WriteProcessMemory(Handle, _address, _stringArray, _stringArray.Length, ref _inWrite);
        }

        /// <summary>
        /// Unlocks a particular block to be written.
        /// </summary>
        /// <param name="Address">The address of the subject block.</param>
        /// <param name="Absolute">Whether the address is an absolute address or not. Defaults to false.</param>
        public static void UnlockBlock(ulong Address, bool Absolute = false)
        {
            IntPtr _address = (IntPtr)(BaseAddress + Address);

            if (Absolute)
                _address = (IntPtr)(Address);

            int _oldProtect = 0;
            VirtualProtectEx(Handle, _address, 0x100000, 0x40, ref _oldProtect);
        }
    }
}