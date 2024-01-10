/*
==================================================
     KINGDOM HEARTS - RE:FIXED COMMON FILE
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.IO;
using System.Net;
using System.Linq;
using System.Text;
using System.Reflection;
using System.Diagnostics;
using System.Windows.Forms;
using System.Globalization;
using System.Reflection.Emit;
using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters.Binary;

using Octokit;
using Ionic.Zip;
using ReFined.Forms;

namespace ReFined
{
	public class UpdateAgent
	{
        protected static readonly double _version = 3.00;
        protected static FormDL _downForm = new FormDL();
        
        public static void UpdateCheck()
        {
            try
            {
                var _gitClient = new GitHubClient(new ProductHeaderValue("ReFined-Updater"));
                var _latestInfo = _gitClient.Repository.Release.GetLatest("TopazTK", "KH-ReFined").Result;

                var _latestNumber = Convert.ToDouble(_latestInfo.TagName.Substring(1), CultureInfo.InvariantCulture);
                var _latestFile = _latestInfo.Assets[0].BrowserDownloadUrl;

                var _downPath = Path.GetTempPath() + "reFinedUpdate.zip";
                var _exePath = Assembly.GetExecutingAssembly().Location;

                var _nameVersion = "[v{0}].exe";

                var _exeAssembly = Assembly.GetExecutingAssembly();
                var _verInformation = FileVersionInfo.GetVersionInfo(_exeAssembly.Location);
                var _strVersion = _verInformation.FileVersion;

                var _formatVersion = String.Format(_nameVersion, _strVersion);

                if (_latestNumber > _version)
                {
                    var _boxMessage = "A new version of Re:Fined has been detected!\n" +
                                    "[Current: v{0}, Latest: v{1}]\n\n" +
                                    "Do you wish to update the game?";  

                    var _boxTitle = "Re:Fined Updater";  
                    var _boxButtons = MessageBoxButtons.YesNo;  

                    var _boxFormat = String.Format(_boxMessage, _version.ToString("0.00"), _latestNumber.ToString("0.00"));

                    var _boxResult = MessageBox.Show(_boxFormat, _boxTitle, _boxButtons, MessageBoxIcon.Information);  

                    if (_boxResult == DialogResult.Yes)
                    {
                        using (var _client = new WebClient())
                        {
                            _client.DownloadProgressChanged += onDownloadProgress;

                            _client.DownloadFileAsync(new System.Uri(_latestFile), _downPath);

                            var _downResult = _downForm.ShowDialog();

                            if (_downResult != DialogResult.OK)
                                _client.CancelAsync();

                            else
                            {
                                using (var _zipArch = ZipFile.Read(_downPath))
                                {
                                    var _entryList = _zipArch.Entries;
                                    var _fileList = _entryList.Where(x => x.FileName.Contains(_formatVersion)).ToList();

                                    foreach (var _file in _fileList)
                                    {
                                        var _exeBase = AppDomain.CurrentDomain.BaseDirectory;

                                        var _templateFM = _exeBase + "KINGDOM HEARTS {0} FINAL MIX.exe";
                                        var _templateVN = _exeBase + "KINGDOM HEARTS {0}.exe";

                                        var _extractName = _exeBase + "KINGDOM HEARTS FINAL MIX.exe";

                                        if (_file.FileName.Contains("Birth by Sleep"))
                                            _extractName = String.Format(_templateFM, "Birth by Sleep");

                                        if (_file.FileName.Contains("Chain of Memories"))
                                            _extractName = String.Format(_templateVN, "Re_Chain of Memories");

                                        if (_file.FileName.Contains("Dream Drop Distance"))
                                            _extractName = String.Format(_templateVN, "Dream Drop Distance");

                                        if (_file.FileName.Contains("II"))
                                            _extractName = String.Format(_templateFM, "II");

                                        if (!Directory.Exists(_exeBase + "BACKUP_EXE/"))
                                            Directory.CreateDirectory(_exeBase + "BACKUP_EXE/");
                                        var _dateStr = DateTime.Now.ToString("dd_MM_yyyy-hh_mm_ss");

                                        File.Move(_extractName, _extractName.Replace(_exeBase, _exeBase + "BACKUP_EXE/").Replace(".exe", "_" + _dateStr + ".exe"));

                                        using (FileStream _exeStream = new FileStream(_extractName, System.IO.FileMode.OpenOrCreate))
                                            _file.Extract(_exeStream);
                                    }
                                } 

                                File.Delete(_downPath);
                                Process.Start(_exePath);
                                System.Environment.Exit(0);
                            }
                        }
                    }
                }
            }

            catch (Exception)
            {
                var _boxMessage = "Re:Fined was not able to check for updates.\n" +
                                  "Initializing the game normally...";  

                var _boxTitle = "Re:Fined Updater";  
                var _boxButtons = MessageBoxButtons.OK;  

                MessageBox.Show(_boxMessage, _boxTitle, _boxButtons, MessageBoxIcon.Error);  
            }
        }

        static void onDownloadProgress(object sender, DownloadProgressChangedEventArgs e)
        {
            _downForm.dlProgress.Value = e.ProgressPercentage;

            if (e.ProgressPercentage == 100)
                _downForm.CompleteCall();
        }
    }
}