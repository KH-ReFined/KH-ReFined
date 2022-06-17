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
using System.Windows;
using System.Runtime;
using System.Reflection;
using System.Diagnostics;
using System.Windows.Forms;
using System.Security.Principal;
using System.Collections.Generic;
using System.Text.RegularExpressions;

using NAudio.Wave;

namespace ReFixed
{
    public static class Helpers
    {
		private static void LaunchAdmin()
		{
			if (!CheckAdmin())
			{
				ProcessStartInfo _process = new ProcessStartInfo();
				_process.UseShellExecute = true;
				_process.WorkingDirectory = Environment.CurrentDirectory;
				_process.FileName = Assembly.GetEntryAssembly().CodeBase;

				_process.Verb = "runas";

				try
				{
					Process.Start(_process);
					Environment.Exit(0);
				}
				
				catch(Exception ex)
				{
					var _boxMessage = "Re:Fixed must be launched with Admin Previliges\n" +
									  "the first time it's installed. Please make sure you\n" +
									  "run it as an Administrator.";  

                    var _boxTitle = "Error #403 - Forbidden";  
                    var _boxButtons = MessageBoxButtons.OK;  

                    var _boxResult = MessageBox.Show(_boxMessage, _boxTitle, _boxButtons, MessageBoxIcon.Error);  

                    if (_boxResult != null)
						Environment.Exit(-1);
				}
			}
		}

		private static bool CheckAdmin()
		{
			var _identity = WindowsIdentity.GetCurrent();
			var _principal = new WindowsPrincipal(_identity);

			return _principal.IsInRole(WindowsBuiltInRole.Administrator);
		}

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
			if (!File.Exists("reFixed.ini"))
			{
				var _outIni = new string[]
				{
					"[General]",
					"autoSave = true",
					"discordRPC = true",
					"saveIndicator = true",
					"",
					"[Kingdom Hearts]",
					"battleChests = true",
					"",
					"[Kingdom Hearts II]",
					"festivityEngine = true"
				};

				File.WriteAllLines("reFixed.ini", _outIni);
			}

			else
			{
				var _configIni = new TinyIni("reFixed.ini");	

                Variables.saveToggle = Convert.ToBoolean(_configIni.Read("autoSave", "General"));
				Variables.rpcToggle = Convert.ToBoolean(_configIni.Read("discordRPC", "General"));
                Variables.sfxToggle = Convert.ToBoolean(_configIni.Read("saveIndicator", "General"));

				if (_configIni.KeyExists("debugMode", "General"))
					Variables.devMode = Convert.ToBoolean(_configIni.Read("debugMode", "General"));
			}
		}
    }
}
