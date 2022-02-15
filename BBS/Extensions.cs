/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR BBS!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace ReFixed
{
    public static class Extensions
    {
		public static uint CalculateCRC32(Stream stream)
        {
            uint checksum = 0;
            
            using (BinaryReader reader = new BinaryReader(stream))
            {
                reader.BaseStream.Seek(0x10, SeekOrigin.Begin);
                for (int i = 0; i < reader.BaseStream.Length / 4 - 4; i++)
                    checksum += reader.ReadUInt32();
            }

            return checksum;
        }
    }
}