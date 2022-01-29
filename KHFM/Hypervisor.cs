/*
=================================================
      KINGDOM HEARTS - RE:FIXED FOR 1 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER MIT. GIVE CREDIT WHERE IT'S DUE! 
=================================================
*/

using System;
using System.IO;
using System.Diagnostics;
using System.Reflection.Emit;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;

namespace ReFixed
{
	public static class Hypervisor
	{
        [DllImport("kernel32.dll")]
        public static extern bool ReadProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, int dwSize, ref int lpNumberOfBytesRead);

        [DllImport("kernel32.dll", SetLastError = true)]
        static extern bool WriteProcessMemory(IntPtr hProcess, IntPtr lpBaseAddress, byte[] lpBuffer, int dwSize, ref int lpNumberOfBytesWritten);
        
        public static T Read<T>(long Address) where T : struct
        {
            var _dynoMethod = new DynamicMethod("SizeOfType", typeof(int), new Type[] { });
            ILGenerator _ilGen = _dynoMethod.GetILGenerator();

            _ilGen.Emit(OpCodes.Sizeof, typeof(T));
            _ilGen.Emit(OpCodes.Ret);

            var _outSize = (int)_dynoMethod.Invoke(null, null);

            var _outArray = new byte[_outSize];
            int _outRead = 0;

            ReadProcessMemory(Variables.GameHandle, (IntPtr)(Variables.GameAddress + Address), _outArray, _outSize, ref _outRead);

            var _gcHandle = GCHandle.Alloc(_outArray, GCHandleType.Pinned);
            var _retData = (T)Marshal.PtrToStructure(_gcHandle.AddrOfPinnedObject(), typeof(T));
            
            _gcHandle.Free();  
                    
            return _retData;
        }

        public static void Write<T>(long Address, T Value) where T : struct
        {
			var _inArray = (byte[])typeof(BitConverter).GetMethod("GetBytes", new[] { typeof(T) }) .Invoke(null, new object[] { Value });
            int _inWrite = 0;

            WriteProcessMemory(Variables.GameHandle, (IntPtr)(Variables.GameAddress + Address), _inArray, _inArray.Length, ref _inWrite);
        }

        public static byte[] ReadArray(long Address, int Length)
        {
            var _outArray = new byte[Length];
            int _outRead = 0;

            ReadProcessMemory(Variables.GameHandle, (IntPtr)(Variables.GameAddress + Address), _outArray, Length, ref _outRead);

            return _outArray;
        }

        public static void WriteArray(long Address, byte[] Value)
        {
            int _inWrite = 0;

            WriteProcessMemory(Variables.GameHandle, (IntPtr)(Variables.GameAddress + Address), Value, Value.Length, ref _inWrite);
        }
    }
}