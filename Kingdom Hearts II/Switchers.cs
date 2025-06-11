using KH2FML;
using System.Linq;

namespace ReFined
{
    /// <summary>
    /// This class is responsible for the switching functions.
    /// </summary>
    public static class Switchers
    {
        public static string US_SUFFIX;
        public static string FM_SUFFIX;

        static bool MUSIC_PAST;
        static bool ENEMY_PAST;

        static bool ATLANTICA_SWITCH;

        public static void AudioSwitch()
        {
            var _audioRead = Hypervisor.Read<byte>(Variables.ADDR_Config + 0x02);
            var _paxCheck = Hypervisor.ReadString(Variables.DATA_PAXPath + 0x10);

            var _stringANM = "anm/{0}/";
            var _stringPAX = "obj/%s.a.{0}";
            var _stringEVT = "voice/{0}/event/";
            var _stringBTL = "voice/{0}/battle/";
            var _stringGMI = "voice/{0}/gumibattle/gumi.win32.scd";

            var _audioSuffix = "us";
            var _audioFormat = String.Format(_stringPAX, _audioSuffix);

            var _langArray = Locals.LOADED_LANGS.Where(x => x.Value != 0x00);

            US_SUFFIX = "us";
            FM_SUFFIX = "fm";

            try
            {
                if (Locals.AUDIO_MODE == 0x01)
                {
                    _audioSuffix = _langArray.ElementAt(0).Key.ToLower();
                    _audioFormat = String.Format(_stringPAX, _audioSuffix);

                    if (Locals.AUDIO_SUB_ONLY)
                    {
                        _audioSuffix = _langArray.ElementAt(_audioRead).Key.ToLower();
                        _audioFormat = String.Format(_stringPAX, _audioSuffix);
                    }
                }

                else if (Locals.AUDIO_MODE == 0x02)
                {
                    _audioSuffix = _langArray.ElementAt(_audioRead + 0x01).Key.ToLower();
                    _audioFormat = String.Format(_stringPAX, _audioSuffix);
                }
            }

            catch (ArgumentOutOfRangeException)
            {
                Terminal.Log("Caught an exception within Multi Audio... Switching to English Audio.", 1);
                Locals.AUDIO_MODE = 0x00;

                _audioSuffix = "us";
                _audioFormat = String.Format(_stringPAX, _audioSuffix);

                US_SUFFIX = "us";
                FM_SUFFIX = "fm";

            }

            if (_paxCheck != _audioFormat)
            {
                Terminal.Log("Switching to " + _audioSuffix.ToUpper() + " Audio...", 0);

                if (Locals.AUDIO_MODE != 0x00)
                {
                    US_SUFFIX = _audioSuffix;
                    FM_SUFFIX = _audioSuffix;
                }

                Hypervisor.Write(Variables.DATA_PAXPath, String.Format(_stringPAX, FM_SUFFIX));
                Hypervisor.Write(Variables.DATA_PAXPath + 0x10, String.Format(_stringPAX, US_SUFFIX));

                Hypervisor.Write(Variables.DATA_ANBPath, String.Format(_stringANM, US_SUFFIX != "jp" ? "us" : US_SUFFIX));
                Hypervisor.Write(Variables.DATA_ANBPath + 0x08, String.Format(_stringANM, US_SUFFIX != "jp" ? "fm" : FM_SUFFIX));

                Hypervisor.Write(Variables.DATA_BTLPath, String.Format(_stringBTL, US_SUFFIX));
                Hypervisor.Write(Variables.DATA_EVTPath, String.Format(_stringEVT, US_SUFFIX));

                Hypervisor.Write(Variables.DATA_GMIPath, String.Format(_stringGMI, US_SUFFIX));
                Hypervisor.Write(Variables.DATA_GMIPath + 0x28, String.Format(_stringGMI, US_SUFFIX));
            }
        }

        public static void MusicSwitch()
        {
            var _worldRead = Hypervisor.Read<byte>(Variables.ADDR_Area);
            var _roomRead = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x01);
            var _eventRead = Hypervisor.Read<byte>(Variables.ADDR_Area + 0x04);

            var _musicalCheck = _worldRead == 0x0B && !Locals.MIRAGE_ARENA && ((_roomRead == 0x04 && _eventRead == 0x42) ||
                                                                               (_roomRead == 0x01 && _eventRead == 0x43) ||
                                                                               (_roomRead == 0x03 && _eventRead == 0x44) ||
                                                                               (_roomRead == 0x09 && _eventRead == 0x45) ||
                                                                               (_roomRead == 0x04 && _eventRead == 0x46));

            if (Locals.MUSIC_MODE != MUSIC_PAST || (!_musicalCheck && ATLANTICA_SWITCH))
            {
                if (ATLANTICA_SWITCH)
                    AxaInterface.Suspend(false);

                Terminal.Log(String.Format("Switching Music to {0}...", Locals.MUSIC_MODE ? "Vanilla" : "Remastered"), 0);
                Hypervisor.Write<byte>(Variables.DATA_BGMPath, Locals.MUSIC_MODE ? [0x70, 0x73, 0x32, 0x6D, 0x64] : [0x6D, 0x75, 0x73, 0x69, 0x63]);

                if (ATLANTICA_SWITCH)
                {
                    AxaInterface.Resume(false);
                    ATLANTICA_SWITCH = false;
                }

                MUSIC_PAST = Locals.MUSIC_MODE;
            }

            else if (_musicalCheck && !ATLANTICA_SWITCH)
            {
                AxaInterface.Suspend(false);
                Terminal.Log("Atlantica Detected! Switching to the according soundtrack...", 0);
                Hypervisor.Write<byte>(Variables.DATA_BGMPath, [0x6D, 0x75, 0x73, 0x69, 0x63]);
                AxaInterface.Resume(false);
                
                ATLANTICA_SWITCH = true;
            }
        }

        public static void EnemySwitch()
        {
            if (!Variables.IS_TITLE && Locals.ENEMY_MODE != ENEMY_PAST)
            {
                Locals.ENEMY_LOADING = true;

                Terminal.Log(String.Format("Switching Enemies to the {0} Palette...", Locals.ENEMY_MODE ? "Classic" : "Special"), 0);


                foreach (var _id in Locals.OBJENTRY_BOSS)
                {
                    var _fetchObjentry = Shisutemu.FetchObject(_id);
                    var _readName = Hypervisor.ReadString(_fetchObjentry + 0x08, true);

                    
                    Hypervisor.Write(_fetchObjentry + 0x08, Locals.ENEMY_MODE ? "V_" + _readName.Substring(2) : "B_" + _readName.Substring(2), true);
                }

                foreach (var _id in Locals.OBJENTRY_ENEMY)
                {
                    var _fetchObjentry = Shisutemu.FetchObject(_id);
                    var _readName = Hypervisor.ReadString(_fetchObjentry + 0x08, true);

                    Hypervisor.Write(_fetchObjentry + 0x08, Locals.ENEMY_MODE ? "V_" + _readName.Substring(2) : "M_" + _readName.Substring(2), true);
                }

                Locals.ENEMY_LOADING = false;
                ENEMY_PAST = Locals.ENEMY_MODE;
            }
        }
    }
}
