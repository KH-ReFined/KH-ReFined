/*
==================================================
    KINGDOM HEARTS - reFined COMMON FILE!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.IO;
using System.Linq;
using System.Windows;
using System.Runtime;
using System.Reflection;
using System.Diagnostics;
using System.Windows.Forms;
using System.Security.Principal;
using System.Collections.Generic;
using System.Text.RegularExpressions;

using NAudio.Wave;

namespace ReFined
{
    public static class Helpers
    {
	    static string _logFileName = "";

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

		public static void InitConfig()
		{			
			if (!File.Exists("reFinedLegacy.ini"))
			{
				var _outIni = new string[]
				{
					"[General]",
					"autoSave = false",
					"discordRPC = true",
					"autoAttack = false",
					"saveIndicator = true",
					"",
					"# Options: vanilla, remastered",
					"musicMode = remastered",
					"",
					"# Options: true = Controller, false = Keyboard, auto = Autodetect",
					"controllerPrompt = auto",
					"",
					"[Kingdom Hearts]",
					"battleChests = true",
					"",
					"[Kingdom Hearts II]",
					"festivityEngine = true",
					"driveShortcuts = true",
					"",
					"# Options: retry, continue",
					"defaultPrompt = retry",
					"",
					"# Options: sonic, arcanum, raid, ragnarok",
					"# Order: [CONFIRM], TRI, SQU, [JUMP]",
					"# Duplicates are allowed. All 4 slots must be filled.",
					"limitShortcuts = [sonic, arcanum, raid, ragnarok]"
				};

				File.WriteAllLines("reFinedLegacy.ini", _outIni);
			}

			else
			{
				var _fileRead = File.ReadAllText("reFinedLegacy.ini");

				if (!_fileRead.Contains("musicMode"))
				{
					File.Delete("reFinedLegacy.ini");
					InitConfig();
				}
				
				else
				{
					var _configIni = new TinyIni("reFinedLegacy.ini");	

					Variables.saveToggle = Convert.ToBoolean(_configIni.Read("autoSave", "General"));
					Variables.rpcToggle = Convert.ToBoolean(_configIni.Read("discordRPC", "General"));
					Variables.attackToggle = Convert.ToBoolean(_configIni.Read("autoAttack", "General"));
					Variables.sfxToggle = Convert.ToBoolean(_configIni.Read("saveIndicator", "General"));

					Variables.vanillaMusic = _configIni.Read("musicMode", "General") == "vanilla" ? true : false;

					var _contValue = _configIni.Read("controllerPrompt", "General");

					if (_contValue.ToLower() == "auto")
						Variables.autoController = true;

					else
					{
						Variables.autoController = false;
						Variables.contToggle = Convert.ToBoolean(_contValue);
					}

					if (_configIni.KeyExists("debugMode", "General"))
						Variables.devMode = Convert.ToBoolean(_configIni.Read("debugMode", "General"));
				}
			}
		}

		public static void Log(string Input, byte Type)
		{
			try
			{
				var _formatStr = "[{0}] {1}: {2}";

				var _dateStr = DateTime.Now.ToString("dd-MM-yyyy");
				var _timeStr = DateTime.Now.ToString("hh:mm:ss");

				var _session = 1;
				var _typeStr = "";

				if (_logFileName == "")
				{
					_logFileName = "ReFined-" + _dateStr + ".txt";

					FILE_CHECK:
					if (File.Exists(_logFileName))
					{
						_logFileName = "ReFined-" + _dateStr + "_SESSION_" + _session + ".txt";
						_session++;

						goto FILE_CHECK;
					}
				}

				switch(Type)
				{
					case 0:
						_typeStr = "MESSAGE";
						break;

					case 1:
						_typeStr = "WARNING";
						break;

					case 2:
						_typeStr = "ERROR";
						break;
				}

				using (StreamWriter _write = File.AppendText(_logFileName))
					_write.WriteLine(String.Format(_formatStr, _timeStr, _typeStr, Input));

				if (Variables.devMode)
					Console.WriteLine(String.Format(_formatStr, _timeStr, _typeStr, Input));
			}

			catch (Exception) {}
		}

		public static void LogException(Exception Input)
		{
			try
			{
				var _formatStr = "[{0}] {1}";

				var _dateStr = DateTime.Now.ToString("dd-MM-yyyy");
				var _timeStr = DateTime.Now.ToString("hh:mm:ss");

				var _session = 1;

				if (_logFileName == "")
				{
					_logFileName = "ReFined-" + _dateStr + ".txt";

					FILE_CHECK:
					if (File.Exists(_logFileName))
					{
						_logFileName = "ReFined-" + _dateStr + "SESSION_" + _session + ".txt";
						_session++;

						goto FILE_CHECK;
					}
				}

				var _exString = Input.ToString().Replace("   ", "").Replace(System.Environment.NewLine, " ");

				using (StreamWriter _write = File.AppendText(_logFileName))
					_write.WriteLine(String.Format(_formatStr, _timeStr, _exString));

				if (Variables.devMode)
					Console.WriteLine(String.Format(_formatStr, _timeStr, _exString));
			}

			catch (Exception) {}
		}
    }
}
