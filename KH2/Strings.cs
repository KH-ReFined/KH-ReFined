using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ReFixed
{
    internal class Strings
    {
        public static uint[] RoxasIDs =
        {
            0x4337,
            0x4338,
            0x4339,
            0x433A,
            0x433B,
            0x4381,
            0x372E,
            0x4383,
            0x0A7C
        };

        public static uint[] SettingIDs =
        {
            0x371A,
            0x372A,
            0x3752,
            0x372C,
            0x372D,
            0x433D,
            0x3733,
            0x371C,
            0X3723
        };

        public static uint[] LimitIDs =
        {
            0x4E8D,
            0x4E8F,
            0x4E93,

            0x4EFE,
            0x4F01,
            0x4EF6,

            0x4EFF,
            0x4F00,
            0x4F03,

            0x4E8E
        };

        public static uint OnID = 0x3726;
        public static uint OffID = 0x3727;

        public static uint FullID = 0x4300;
        public static uint NoneID = 0x4301;

        public static uint ContinueID = 0x0AB0;
        public static uint OpenKHID = 0x433C;

        public static string[] LimitText =
        {
            "Sonic Blade",
            "Ars Arcanum",
            "Ragnarok",
            "Rave"
        };

        public static string[][] RoxasSkip =
        {
            new string[]
            {
                "Play Roxas' Story?",
                "YES",
                "NO",
                "Play through Roxas' Story normally.",
                "                Skip Roxas' Story entirely.{0x02}{0x07}{0xFF}{0xFF}{0x00}{0x80}(You will miss important story elements if you do!)",
                "Roxas' Story"
            },
                        
            new string[]
            {
                "Möchtest du Roxas' Geschichte spielen?",
                "Ja",
                "Nein",
                "Spiele ganz normal durch Roxas' Geschichte.",
                "          Überspringe Roxas' Geschichte komplett.{0x02}{0x07}{0xFF}{0xFF}{0x00}{0x80}(Dadurch werden wichtige Story-Elemente übersprungen!)",
                "Roxas' Geschichte"
            },

            new string[]
            {
                "¿Quieres jugar la historia de Roxas?",
                "Sí",
                "No",
                "Juega la historia completa.",
                "       Saltar historia de Roxas.{0x02}{0x07}{0xFF}{0xFF}{0x00}{0x80}(¡Te perderás elementos importantes\n      de la historia si lo haces!)",
                "Historia de Roxas"
            },

            new string[]
            {
                "Lancer l'arc de Roxas ?",
                "Oui",
                "Non",
                "Jouer l'arc de Roxas normalement.",
                "                       Passer l'arc de Roxas.{0x02}{0x07}{0xFF}{0xFF}{0x00}{0x80}(Vous passerez à coté d'éléments importants de l'histoire!)",
                "L'arc de Roxas"
            },

            new string[]
            {
                "Gioca la storia di Roxas?",
                "Sì",
                "No",
                "Gioca la storia di Roxas normalmente.",
                "        Salta la storia di Roxas completamente.{0x02}{0x07}{0xFF}{0xFF}{0x00}{0x80}(Perderai elementi della storia se decidi di saltarla!)",
                "La storia di Roxas"
            }
        };

        public static string[][] AutoSave = new string[][]
        {
            new string[]
            {
                "Auto-Save",
                "ON",
                "OFF",
                "Enable auto-saving functionality.",
                "Disable auto-saving functionality."
            },

            new string[]
            {
                "Auto-Speichern",
                "An",
                "Aus",
                "Aktiviere die Auto-Speicherfunktion.",
                "Deaktiviere die Auto-Speicherfunktion."
            },

            new string[]
            {
                "Auto-Guardado",
                "Activado",
                "Desactivado",
                "Activa la función de auto-guardado.",
                "Desactiva la función de auto-guardado."
            },

            new string[]
            {
                "Sauvegarde Automatique",
                "Activé",
                "Desactivé",
                "Active la fonction de sauvegarde automatique.",
                "Désactive la fonction de sauvegarde automatique."
            },

            new string[]
            {
                "Salvataggio Automatico",
                "ON",
                "OFF",
                "Abilita funzione di salvataggio automatico.",
                "Disabilita funzione di salvataggio automatico."
            }
        };

        public static string[][] DualAudio = new string[][]
        {
            new string[]
            {
                "Audio Language",
                "English",
                "Japanese",
                "Switch between {0} and Japanese speech.\n{{0x07}}{{0xFF}}{{0xFF}}{{0x00}}{{0x80}}(The world must be reloaded for the changes\nto take effect!){{0x03}}"
            },

            new string[]
            {
                "Audio-Sprachausgabe",
                "Englisch",
                "Japanisch",
                "Wechsle zwischen der {0} und Japanischen\nSprachausgabe. {{0x07}}{{0xFF}}{{0xFF}}{{0x00}}{{0x80}}(Die Welt muss neu geladen werden,\nbevor die Effekte in Kraft treten!){{0x03}}"
            },

            new string[]
            {
                "Idioma de las Voces",
                "Inglés",
                "Japonés",
                "Cambia entre las voces en {0} y Japonés.\n{{0x07}}{{0xFF}}{{0xFF}}{{0x00}}{{0x80}}(¡El mundo necesita volver a cargarse\npara que surjan los cambios!){{0x03}}"
            },

            new string[]
            {
                "Langue de l'audio",
                "Anglais",
                "Japonais",
                "Passer de l'audio {0} à Japonais, vice-versa.\n{{0x07}}{{0xFF}}{{0xFF}}{{0x00}}{{0x80}}(Le monde dans lequel vous vous trouvez doit être \nchargé à nouveau pour que les changements prennnent effet!){{0x03}}"
            },

            new string[]
            {
                "Voci Dialoghi",
                "Inglese",
                "Giapponese",
                "Scegli tra doppiaggio {0} o Giapponese.\n{{0x07}}{{0xFF}}{{0xFF}}{{0x00}}{{0x80}}(Il mondo deve essere ricaricato prima\nche i cambiamenti prendano effetto!){{0x03}}"
            }
        };

        public static string[][] VoicePatch = new string[][]
        {
            new string[]
            {
                "German",
                "Spanish",
                "French"
            },

            new string[]
            {
                "Deutsch",
                "Französisch",
                "Spanisch"
            },

            new string[]
            {
                "Alemán",
                "Francés",
                "Español"
            },

            new string[]
            {
                "Allemand",
                "Français",
                "Espagnol"
            },

            new string[]
            {
                "Tedesco",
                "Francese",
                "Tedesco"
            }
        };

        public static string[][] FixText = new string[][]
        {
            new string[]
            {
                "ON",
                "OFF"
            },

            new string[]
            {
                "An",
                "Aus"
            },

            null,
            null,

            new string[]
            {
                "ON",
                "OFF"
            }
        };

        public static string[][] RetryPrompt = new string[][]
        {
            new string[]
            {
                "Continue",
                "Retry"
            },

            new string[]
            {
                "Fortsetzen",
                "Neuer Versuch"
            },

            new string[]
            {
                "Continuar",
                "Reintentar"
            },

            new string[]
            {
                "Continuer",
                "Réessayer"
            },

            new string[]
            {
                "Continua",
                "Riprova"
            }
        };
    }
}
