using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ReFixed
{
    internal class Strings
    {
        public static string[] OriginText =
        {
            "Use",
            "Verwenden",
            "SÖÑ",
            "ActivÖíe",
            "SÖñ"
        };

        public static string[][] AutoSave = new string[][]
        {
            new string[]
            {
                "Auto-Save\x00",
                "ON\x00",
                "OFF\x00",
                "Enable auto-saving functionality.\x00",
                "Disable auto-saving functionality.\x00"
            },

            new string[]
            {
                "Auto-Speichern\x00",
                "An\x00",
                "Aus\x00",
                "Aktiviere die Auto-Speicherfunktion.\x00",
                "Deaktiviere die Auto-Speicherfunktion.\x00"
            },

            new string[]
            {
                "Auto-Guardado\x00",
                "Activado\x00",
                "Desactivado\x00",
                "Activa la función de auto-guardado.\x00",
                "Desactiva la función de auto-guardado.\x00"
            },

            new string[]
            {
                "Sauvegarde Auto\x00",
                "ActivÖíe\x00",
                "DesctivÖíe\x00",
                "Active la fonction de sauvegarde automatique.\x00",
                "Désactive la fonction de sauvegarde automatique.\x00"
            },

            new string[]
            {
                "Autosalvataggio\x00",
                "ON\x00",
                "OFF\x00",
                "Abilita funzione di salvataggio automatico.\x00",
                "Disabilita funzione di salvataggio automatico.\x00"
            }
        };

        public static string[][] DualAudio = new string[][]
        {
            new string[]
            {
                "Audio Language\x00",
                "English\x00",
                "Japanese\x00",
                "Switch between English and Japanese vocals.\x0A\u2219\x59(Trigger a significant load for the changes to take effect.)\x00",
            },

            new string[]
            {
                "Audio-Sprachausgabe\x00",
                "Englisch\x00",
                "Japanisch\x00",
                "Wechsle in die Englische Sprachausgabe.\x0A\u2219\x59(Aktiviere einen signifikanten Ladevorgang, damit\x000Adie Anderungen in Kraft treten.)\x00",
                "Wechsle in die Japanische Sprachausgabe.\x0A\u2219\x59(Aktiviere einen signifikanten Ladevorgang, damit\x000Adie Anderungen in Kraft treten.)\x00"
            },

            new string[]
            {
                "Idioma de las Voces\x00",
                "InglÖís\x00",
                "JaponÖís\x00",
                "Cambia las voces a InglÖís.\x0A\u2219\x59(Se necesitan cargar múltiples elementos para que surjan los cambios.)\x00",
                "Cambia las voces a JaponÖís.\x0A\u2219\x59(Se necesitan cargar múltiples elementos para que surjan los cambios.)\x00"
            },

            new string[]
            {
                "Langue de l'audio\x00",
                "Anglais\x00",
                "Japonais\x00",
                "Passer en mode audio Anglais.\x0A\u2219\x59(Notez un temps de chargement significatif avant\x000Al'application des modifications.)\x00",
                "Passer en mode audio Japonais.\x0A\u2219\x59(Notez un temps de chargement significatif avant\x000Al'application des modifications.)\x00"
            },

            new string[]
            {
                "Lingua dialoghi\x00",
                "Inglese\x00",
                "Giapponese\x00",
                "Cambia audio dialoghi in Inglese.\x0A\u2219\x59(Il caricamento potrebbe durare a lungo\x0000Aper effettuare i cambiamenti.) \x00",
                "Cambia audio dialoghi in Giapponese.\x0A\u2219\x59(Il caricamento potrebbe durare a lungo\x000Aper effettuare i cambiamenti.) \x00"
            }
        };
    }
}
