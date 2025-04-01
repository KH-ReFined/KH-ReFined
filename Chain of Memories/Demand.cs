using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ReFined.COM
{
    public static class Demand
    {
        public static void TriggerReset()
        {
            if (!Variables.IS_TITLE && Variables.IS_LOADED)
            {
                // Because we don't want to see the copyright notices on every soft reset.

                if (!Variables.QUICK_BOOT)
                {
                    Hypervisor.Write<byte>(Boot.FUNC_CHECKCOPYRIGHT + 0x029, 0x73);
                    Variables.QUICK_BOOT = true;
                }

                var _inputPINT = Hypervisor.GetPointer64(Variables.PINT_Input, [0x0C]);
                var _readInput = Hypervisor.Read<Variables.BUTTON>(_inputPINT, true);

                if (_readInput == Variables.RESET_COMBO)
                    Hypervisor.Write(Variables.ADDR_Reset, 0x04);
            }
        }
    }
}
