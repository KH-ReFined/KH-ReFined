/*
==================================================
    KINGDOM HEARTS - RE:FIXED COMMON FILE!
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

namespace ReFixed
{
    public static class Extensions
    {
        public static void PlaySFX(string Input)
		{
			var _output = new DirectSoundOut();
			var _wavRead = new AudioFileReader(Input);

            var _volumeMaster = Hypervisor.Read<byte>(Variables.ADDR_MasterVolume);
            var _volumeSFX = Hypervisor.Read<byte>(Variables.ADDR_MasterVolume + 0x04);

			var _sfxValue = Hypervisor.Read<float>(Variables.ADDR_VolumeTable + (ulong)(0x04 * _volumeSFX));
			var _masterValue = Hypervisor.Read<float>(Variables.ADDR_VolumeTable + (ulong)(0x04 * _volumeMaster));

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

        public static ulong FindValue(this byte[] Source, uint Value)
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
    }
}
