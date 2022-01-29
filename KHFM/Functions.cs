/*
=================================================
      KINGDOM HEARTS - RE:FIXED FOR 1 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER MIT. GIVE CREDIT WHERE IT'S DUE! 
=================================================
*/

using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace ReFixed
{
	public static class Functions
	{
        public static void OverrideText()
        {
            // Config Text

            if (Hypervisor.Read<byte>(Variables.FovTextAddresses[1]) != 0x30)
            {
                for (int i = 0; i < Variables.FovTextOffsets.Length; i++)
                    Hypervisor.Write<ushort>(Variables.FovTextAddresses[0] + (0x02 * i), Variables.FovTextOffsets[i]);
                
                for (int i = 0; i < Variables.CamTextOffsets.Length; i++)
                    Hypervisor.Write<ushort>(Variables.CamTextAddresses[0] + (0x02 * i), Variables.CamTextOffsets[i]);

                Hypervisor.WriteArray(Variables.FovTextAddresses[1], Variables.FovTextArray);
                Hypervisor.WriteArray(Variables.CamTextAddresses[1], Variables.CamTextArray);
            }
        }

        public static void OverrideFov()
        {
            var _fovToggle = Hypervisor.Read<int>(Variables.FovSwitchAddress);

            switch(_fovToggle)
            {
                case 0:
                {
                    var _fovFirst = Hypervisor.Read<float>(Variables.FovAddresses[0]);

                    if (_fovFirst != 400F)
                    {
                        for (int i = 0; i < Variables.FovAddresses.Length; i++)
                            Hypervisor.Write<float>(Variables.FovAddresses[i], Variables.FovClassic[i]);
                    }

                    break;
                }

                case 1:
                {
                    var _fovFirst = Hypervisor.Read<float>(Variables.FovAddresses[0]);

                    if (_fovFirst != 600F)
                    {
                        for (int i = 0; i < Variables.FovAddresses.Length; i++)
                            Hypervisor.Write<float>(Variables.FovAddresses[i], Variables.FovEnhanced[i]);
                    }

                    break;
                }
            }
        }

        public static void Execute()
        {
            OverrideText();
            OverrideFov();
        }
    }
}