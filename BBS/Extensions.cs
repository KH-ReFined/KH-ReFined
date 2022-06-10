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

using NAudio.Wave;
using NAudio.Wave.SampleProviders;

namespace ReFixed
{
    public static class Extensions
    {
        public static void PlaySFX(string Input)
		{
			var _output = new DirectSoundOut();
			var _wavRead = new AudioFileReader(Input);

			var _volumeSFX = Hypervisor.Read<byte>(0x10ADCEB4);
            var _volumeMaster = Hypervisor.Read<byte>(0x10ADCEB0);
            
			var _sfxValue = Hypervisor.Read<float>(0x7B5D4 + (ulong)(0x04 * _volumeSFX));
			var _masterValue = Hypervisor.Read<float>(0x7B5D4 + (ulong)(0x04 * _volumeMaster));

			_wavRead.Volume = _sfxValue * _masterValue;

			_output.Init(_wavRead);
			_output.Play();
		}

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
