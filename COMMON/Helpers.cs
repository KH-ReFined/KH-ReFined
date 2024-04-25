/*
==================================================
    KINGDOM HEARTS - RE:FINED COMMON FILE!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.Diagnostics;
using System.IO;
using System.Linq;

namespace ReFined
{
    public static class Helpers
    {
	    static string _logFileName = "";

		public static void InitConfig()
		{			
			if (!File.Exists("reFined.ini"))
			{
				var _outIni = new string[]
				{
					"[General]",
					"discordRPC = true",
					"cloudSave = true",
                    "resetCombo = 0x0003",
                    "dualSenseIntegration = true",
                    "dualSenseNotifications = false",
                    "",
                    "# Options: 0 = HP Tracking, 1 = MP Tracking, 2 = Battle State Tracking, 3 = Form Tracking",
                    "dualSenseMode = 0",
                    "",
					"[Kingdom Hearts II]",
					"adjustRatio = false",
					"driveShortcuts = true",
					"",
					"# Options: retry, continue",
					"deathPrompt = retry",
					"resetPrompt = true",
					"",
					"# Options: sonic, arcanum, raid, ragnarok",
					"# Order: [CONFIRM], TRI, SQU, [JUMP]",
					"# Duplicates are allowed. All 4 slots must be filled.",
					"limitShortcuts = [sonic, arcanum, raid, ragnarok]",
					"",
                    "[Kingdom Hearts]",
                    "battleChests = true",
                };

				File.WriteAllLines("reFined.ini", _outIni);
			}

			else
			{
				var _fileRead = File.ReadAllText("reFined.ini");

				if (!_fileRead.Contains("cloudSave"))
				{
					File.Delete("reFined.ini");
					InitConfig();
				}
				
				else
				{
					var _configIni = new TinyIni("reFined.ini");

					var _isWindows = Process.GetProcessesByName("winlogon").Count() > 0x00;

					if (_isWindows)
					{
						Variables.DUALSENSE_TOGGLE = Convert.ToBoolean(_configIni.Read("dualSenseIntegration", "General"));
						Variables.DUALSENSE_MODE = Convert.ToByte(_configIni.Read("dualSenseMode", "General"));
						Variables.DUALSENSE_NOTIFICATIONS = Convert.ToBoolean(_configIni.Read("dualSenseNotifications", "General"));
					}

					else
					{
						Variables.DUALSENSE_TOGGLE = false;
                        Variables.DUALSENSE_NOTIFICATIONS = false;
                    }

                    Variables.DISCORD_TOGGLE = Convert.ToBoolean(_configIni.Read("discordRPC", "General"));
                    Variables.RESET_COMBO = Convert.ToUInt16(_configIni.Read("resetCombo", "General"), 16);

                    if (_configIni.KeyExists("debugMode", "General"))
						Variables.DEV_MODE = Convert.ToBoolean(_configIni.Read("debugMode", "General"));
				}
			}
		}

		public static void Log(string Input, byte Type)
		{
			try
			{
                var _documentsPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
                var _logDir = Path.Combine(_documentsPath, "Kingdom Hearts/Logs");

                var _formatStr = "{0}{1}: {2}";

				var _dateStr = DateTime.Now.ToString("dd-MM-yyyy");
				var _timeStr = "[" + DateTime.Now.ToString("hh:mm:ss") + "] ";

				var _session = 1;

				var _typeStr = "";

				if (_logFileName == "")
				{
					_logFileName = "ReFined-" + _dateStr + ".txt";

					FILE_CHECK:
					if (File.Exists(Path.Combine(_logDir, _logFileName)))
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

				using (StreamWriter _write = File.AppendText(Path.Combine(_logDir, _logFileName)))
					_write.WriteLine(String.Format(_formatStr, _timeStr, _typeStr, Input));

				if (Variables.DEV_MODE)
				{
					Console.Write(_timeStr);
					Console.ForegroundColor = Type == 0x00 ? ConsoleColor.Green : (Type == 0x01 ? ConsoleColor.Yellow : ConsoleColor.Red);
					Console.Write(_typeStr + ": ");
					Console.ForegroundColor = ConsoleColor.Gray;
					Console.WriteLine(Input);
				}
			}

			catch (Exception) {}
		}

		public static void LogException(Exception Input)
		{
			try
			{
                var _documentsPath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
                var _logDir = Path.Combine(_documentsPath, "Kingdom Hearts/Logs");

                var _formatStr = "[{0}] {1}";

				var _dateStr = DateTime.Now.ToString("dd-MM-yyyy");
				var _timeStr = DateTime.Now.ToString("hh:mm:ss");

				var _session = 1;

				if (_logFileName == "")
				{
					_logFileName = "ReFined-" + _dateStr + ".txt";

					FILE_CHECK:
					if (File.Exists(Path.Combine(_logDir, _logFileName)))
					{
						_logFileName = "ReFined-" + _dateStr + "_SESSION_" + _session + ".txt";
						_session++;

						goto FILE_CHECK;
					}
				}

				var _exString = Input.ToString().Replace("   ", "").Replace(System.Environment.NewLine, " ");

                using (StreamWriter _write = File.AppendText(Path.Combine(_logDir, _logFileName)))
                    _write.WriteLine(String.Format(_formatStr, _timeStr, _exString));

                if (Variables.DEV_MODE)
                {
                    Console.Write("[" + _timeStr + "] ");
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.Write("EXCEPTION: ");
                    Console.ForegroundColor = ConsoleColor.Gray;
                    Console.WriteLine(_exString);
                }
            }

			catch (Exception) {}
		}
    }
}
