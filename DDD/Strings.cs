/*
==================================================
      KINGDOM HEARTS - RE:FINED FOR DDD!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ReFined
{
    internal class Strings
    {
        public static byte[] JPOffset = new byte[]
        {
            0x0A
        };

        public static string[][] DropString =
        {
            new string[]
            {
                "Auto-Drop Timer\u0000",
                "Enable/Disable the Auto-Drop Timer.\u0000"
            },
                        
            new string[]
            {
                "Auto-Sturz Timer",
                "Aktiviere/Deaktiviere den Auto-Sturz Timer.",
            },

            new string[]
            {
                "Indicador de Sopor",
                "Activa/Desactiva el indicador de Sopor.",
            },

            new string[]
            {
                "Auto-Drop Timer",
                "Enable/Disable the Auto-Drop Timer",
            },

            new string[]
            {
                "Timer dell'Auto-Caduta",
                "Abilita/Disabilità il Timer dell'Auto-Caduta.",
            }
        };

        public static string[][] DualAudio = new string[][]
        {
            new string[]
            {
                "Audio Language\u0000",
                "English\u0000",
                "Japanese\u0000",
                "Shift the Audio Language between English and Japanese.\x0A",
                "(A Drop is necessary for the changes to take effect.)\u0000"
            }
        };
    }
}
