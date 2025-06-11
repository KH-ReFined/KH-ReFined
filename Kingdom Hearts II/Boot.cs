using KH2FML;
using System.Reflection;
using System.Diagnostics;
using System.IO.Compression;

using Octokit;
using System.Net;
using Microsoft.Win32;
using System.Net.Http;
using System.Security.Principal;
using System.Runtime.InteropServices;

using DiscordClient = Discord.Discord;
using System.Globalization;

namespace ReFined
{
    public static class Boot
    {
        public static double VERSION = 4.00;

        public static int CRASH_ITERATOR;
        public static DateTime CRASH_TIMER = DateTime.MinValue;

        [DllImport("kernel32")]
        static extern bool AllocConsole();

        public static bool ENABLE_IDLEMOVIE;
        public static bool ENABLE_QUICKBOOT;
        public static ulong TITLEINIT_OFFSET;

        static string[] _defaultTOML =
        [
            "[General]",
            "liteMode = false",
            "discordRPC = true",
            "resetCombo = [\"L2\", \"R2\"]",
            "isFlashbang = true",
            "quickBoot = false",
            "saveSlot = 99",
            "",
            "[Accessibility]",
            "shakeType = \"partial\"",
            "autoAttack = false",
            "",
            "[\"Chain of Memories\"]",
            "zeroEXP = false",
            "alternativeControls = false",
            "",
            "[\"Kingdom Hearts II\"]",
            "idleMovie = false",
            "driveShortcuts = true",
            "resetPrompt = true",
            "deathPrompt = \"retry\"",
            "limitShortcuts = [\"sonic\", \"arcanum\", \"raid\", \"ragnarok\"]",
        ];

        static string[] _namesDLL =
        [
            "keystone.dll",
            "discord_game_sdk.dll",
            "System.Runtime.CompilerServices.Unsafe.dll"
        ];

        static Dictionary<string, string> _localeSystem = new Dictionary<string, string>()
        {
            { "de", "KH2-GermanVO" },
            { "fr", "KH2-FrenchVO" },
            { "es", "KH2-SpanishVO" },
        };

        /// <summary>
        /// Initializes everything to do with Re:Fined.
        /// </summary>
        public static void Initialization()
        {
            if (!Directory.Exists("Logs"))
                Directory.CreateDirectory("Logs");

            Terminal.Initialize("Logs/ReFined-" + DateTime.Now.ToString("dd-MM-yyyy") + ".log");

            // Try to see if we are on Linux. Wine will NOT have "winlogon.exe" but every version of Windows since 95 should.
            var _checkLinux = Process.GetProcessesByName("winlogon").Length == 0x00;

            // Assemble the current information.
            var _currentEXE = Assembly.GetExecutingAssembly();
            var _currentProcess = Process.GetCurrentProcess();
            var _checkDevEnv = !_currentEXE.FullName.Contains("KINGDOM HEARTS II FINAL MIX");
            var _checkDLLs = File.Exists(_namesDLL[0]) && File.Exists(_namesDLL[1]) && File.Exists(_namesDLL[2]);

            // Construct the app start information in-case we need to reboot or otherwise ensure Re:Fined.
            var _startInfo = new ProcessStartInfo();

            _startInfo.UseShellExecute = true;
            _startInfo.WorkingDirectory = Environment.CurrentDirectory;
            _startInfo.FileName = Assembly.GetEntryAssembly().Location;

            _startInfo.Verb = "runas";

            // Sets the HTTP Security Protocol because Octokit will refuse to work and throw exceptions otherwise.
            ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls12;

            // Check if the config file exists, and if it does, handle it.
            if (File.Exists("reFined.cfg"))
            {
                using (var _tomlFile = File.OpenText("reFined.cfg"))
                {
                    // Try. The TOML parser will throw and exception if the config file is not valid.
                    try
                    {
                        var _tomlTable = TOML.Parse(_tomlFile);

                        var _generalTable = _tomlTable["General"];

                        Locals.IS_LITE = _generalTable["liteMode"].AsBoolean;
                        Locals.RPC_ENABLED = _generalTable["discordRPC"].AsBoolean;
                        Locals.IS_FLASHBANG = _generalTable["isFlashbang"].AsBoolean;

                        Locals.SAVE_SLOT = _generalTable["saveSlot"].AsInteger - 0x01;
                        Locals.RESET_COMBO = Variables.BUTTON.NONE;

                        ENABLE_QUICKBOOT = _generalTable["quickBoot"].AsBoolean;

                        foreach (TomlString _button in _generalTable["resetCombo"].AsArray)
                        {
                            Variables.BUTTON _buttonOut;
                            Enum.TryParse(_button, out _buttonOut);
                            Locals.RESET_COMBO |= _buttonOut;
                        }

                        var _accessTable = _tomlTable["Accessibility"];

                        var _shakeFetch = _accessTable["shakeType"].AsString;

                        if (_shakeFetch == "none")
                            Locals.SHAKE_ENABLED = false;

                        if (_shakeFetch == "all")
                            Locals.SHAKE_BLOOM = true;

                        Locals.AUTOATTACK = _accessTable["autoAttack"].AsBoolean;

                        var _gameTable = _tomlTable["Kingdom Hearts II"];

                        ENABLE_IDLEMOVIE = _gameTable["idleMovie"].AsBoolean;

                        Locals.FORM_SHORTCUT = _gameTable["driveShortcuts"].AsBoolean;
                        Locals.RESET_PROMPT = _gameTable["resetPrompt"].AsBoolean;
                        Locals.RETRY_FIRST = _gameTable["deathPrompt"].AsString == "retry" ? true : false;

                        var _limitTable = _gameTable["limitShortcuts"].AsArray;
                        Continuous.LIMIT_SHORT = new short[4];

                        for (int i = 0; i < _limitTable.ChildrenCount; i++)
                            Continuous.LIMIT_SHORT[i] = Locals.DICTIONARY_LMT[_limitTable[i].AsString];

                        Locals.NAME_LABEL = (Locals.IS_LITE ? "Re:Freshed" : "Re:Fined");
                    }

                    // Upon catching: Just make a new file.
                    catch (Exception)
                    { 
                        File.WriteAllLines("reFined.cfg", _defaultTOML);

                        Continuous.LIMIT_SHORT = new short[4];

                        Continuous.LIMIT_SHORT[0] = Locals.DICTIONARY_LMT["sonic"];
                        Continuous.LIMIT_SHORT[1] = Locals.DICTIONARY_LMT["arcanum"];
                        Continuous.LIMIT_SHORT[2] = Locals.DICTIONARY_LMT["raid"];
                        Continuous.LIMIT_SHORT[3] = Locals.DICTIONARY_LMT["ragnarok"];
                    }
                }
            }

            // Otherwise, create a new one with default settings.
            else
                File.WriteAllLines("reFined.cfg", _defaultTOML);

            // Fetch the location of the EXE and see if Panacea exists.
            var _panaceaExists = File.Exists("panacea_settings.txt") && ((!_checkLinux && File.Exists("DBGHELP.dll")) || (_checkLinux && File.Exists("version.dll")));
            var _currentPath = Path.GetDirectoryName(_currentEXE.Location);

            // If it does not:
            if (!_panaceaExists)
            {
                // Don't check for admin rights in Linux.
                if (Variables.PLATFORM == "STEAM" && !_checkLinux && !_checkDevEnv)
                {
                    var _windowsID = WindowsIdentity.GetCurrent();
                    var _windowsPrincipal = new WindowsPrincipal(_windowsID);

                    var _checkAdmin = _windowsPrincipal.IsInRole(WindowsBuiltInRole.Administrator);

                    if (!_checkAdmin)
                    {
                        Process.Start(_startInfo);
                        _currentProcess.Kill();
                    }
                }

                if (Variables.PLATFORM == "STEAM" && !_checkDevEnv)
                {
                    // Notify the user of Panacea's mispresence and ask if it should be fixed.
                    var _messagePanacea = MessageBox.Show("It seems that you do not currently have Panacea installed!\n" +
                                                           Locals.NAME_LABEL + " requires Panacea to function properly. Would you like to install it automatically?\n" +
                                                          "(It will also install the main mod package. Internet connection is required.)", "ERROR #400 - Panacea Not Found!", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

                    // If the answer is "YES":
                    if (_messagePanacea == DialogResult.Yes)
                    {
                        // Show the console regardless of Panacea's settings. We need this for a feedback loop to the recipient.
                        AllocConsole();

                        Terminal.Log("Initiating Auto-Install. The process will take a while.", 0);

                        // Initialize the web client.
                        using var _webClient = new HttpClient();
                        _webClient.Timeout = Timeout.InfiniteTimeSpan;

                        // Check if we can access the internet.
                        // We use the GStatic service for this because if Google is offline the world is ending anyways.

                        try { using var _responseOnline = _webClient.GetAsync("http://www.gstatic.com/generate_204").Result; }


                        catch (HttpRequestException)
                        {
                            var _messageNoConnection = MessageBox.Show("Connection to the Internet cannot be established.\n" +
                                                                        "Auto-Install cannot continue. " + Locals.NAME_LABEL + " will now terminate.\n",
                                                                        "ERROR #220 - No Internet!", MessageBoxButtons.OK, MessageBoxIcon.Error);

                            if (_messageNoConnection != DialogResult.None)
                            {
                                Terminal.Log("No Internet Connection detected! Auto-Install cannot continue! CODE: 220", 2);
                                _currentProcess.Kill();
                            }
                        }

                        Terminal.Log("Fetching all OpenKH releases from GitHub...", 1);

                        // Initialize the GitClient. Regularly, Re:Fined uses the token of a burner account I made with a burner e-mail and a burner password.
                        // However, I ain't sharing the token for that. If you are going to compile Re:Fined for yourself, either remove this code, or get your own token.

                        var _gitClient = new GitHubClient(new ProductHeaderValue("REFINED_FETCH_PANACEA"));

                        try
                        {
                            // OpenKH does not use stable releases, thus, this is needed.
                            // This also spams like 50 API requests a minute so that's why we need to initialize the client with authorization.

                            var _latestPanacea = _gitClient.Repository.Release.GetLatest("OpenKH", "OpenKh").Result;

                            // Calculate all the paths we will use moving foward.

                            var _downPath = Path.GetTempPath() + "openKH.zip";
                            var _modPath = Path.Combine(_currentPath, "MOD_DATA");
                            var _panaceaPath = Path.Combine(_currentPath, "DBGHELP.dll");
                            var _dependencyPath = Path.Combine(_currentPath, "dependencies");

                            // If on Linux, Panacea has to be "version.dll".
                            if (_checkLinux)
                                _panaceaPath = Path.Combine(_currentPath, "version.dll");

                            var _actualModsPath = Path.Combine(_modPath, "kh2");

                            // Fetch the URL of the latest release. OpenKH only has a single ZIP file, so the index of the asset we need is always zero.

                            var _latestDLL = _latestPanacea.Assets[0].BrowserDownloadUrl;

                            Terminal.Log("Downloading OpenKH from GitHub...", 1);

                            // Initialize the response and streams for OpenKH's ZIP.
                            // We do this because:
                            // - It's better to load in RAM than to initialize TEMP files.
                            // - WebClient is deprecated.

                            using var _responsePanacea = _webClient.GetAsync(_latestDLL).Result;
                            using var _streamPanacea = _responsePanacea.Content.ReadAsStreamAsync().Result;

                            // Open the Stream as a ZIP.
                            // We do this through System.IO's ZIP standard, as DotNetZip is extremely insecure.

                            using (var _downZIP = new ZipArchive(_streamPanacea))
                            {
                                Terminal.Log("Trying to find OpenKH.Panacea...", 1);

                                // Fetch specifically "OpenKH.Panacea.dll".
                                var _fetchPanacea = _downZIP.Entries.FirstOrDefault(x => x.Name == "OpenKH.Panacea.dll");

                                // Fetch all of Panacea's dependencies.
                                var _fetchDependencies = new List<ZipArchiveEntry>()
                                {
                                    _downZIP.Entries.FirstOrDefault(x => x.Name == "bass.dll"),
                                    _downZIP.Entries.FirstOrDefault(x => x.Name == "libatrac9.dll"),
                                    _downZIP.Entries.FirstOrDefault(x => x.Name == "libcelt-0061.dll"),
                                    _downZIP.Entries.FirstOrDefault(x => x.Name == "libcelt-0110.dll"),
                                    _downZIP.Entries.FirstOrDefault(x => x.Name == "libg719_decode.dll"),
                                    _downZIP.Entries.FirstOrDefault(x => x.Name == "libmpg123-0.dll"),
                                    _downZIP.Entries.FirstOrDefault(x => x.Name == "libspeex-1.dll"),
                                    _downZIP.Entries.FirstOrDefault(x => x.Name == "libvorbis.dll"),
                                };

                                _fetchDependencies.AddRange(_downZIP.Entries.Where(x => x.Name.Contains("vgmstream")));

                                // If we can't find it, ask me to bug Vlad about finding out WHY the FUCK?

                                if (_fetchPanacea == null)
                                {
                                    var _messageSadge = MessageBox.Show("Panacea is not found in OpenKH Archive! Tell Topaz to complain to Vlad about this!\n" +
                                                                        "Auto-Install cannot continue. " + Locals.NAME_LABEL + " will now terminate.\n",
                                                                        "ERROR #420 - Where Panacea?!", MessageBoxButtons.OK, MessageBoxIcon.Error);

                                    if (_messageSadge != DialogResult.None)
                                    {
                                        Terminal.Log("OpenKH Archive Anomaly when extracting! Tell Vlad! CODE: 420", 2);
                                        _currentProcess.Kill();
                                    }
                                }

                                // Else, extract it and adjust the settings file with the modpath.
                                Terminal.Log("File found! Installing Panacea...", 1);
                                _fetchPanacea.ExtractToFile(_panaceaPath, true);

                                Terminal.Log("Adjusting Panacea Settings...", 1);
                                File.WriteAllText("panacea_settings.txt", "mod_path=" + _modPath);

                                Terminal.Log("Extracting all of Panacea's dependencies...", 1);

                                // Create the directory for all dependencies.
                                Directory.CreateDirectory(_dependencyPath);

                                // Extract said dependencies.
                                foreach (var _entry in _fetchDependencies)
                                    _entry.ExtractToFile(Path.Combine(_dependencyPath, _entry.Name), true);
                            }

                            Terminal.Log("Fetching all \"KH-ReFined/KH2-MAIN\" releases from GitHub...", 1);

                            // I refuse to use stable releases for this purpose. But I have an actual fuckin' reason for it.
                            // This ALSO spams 50 API requests a minute.
                            var _fetchMod = _gitClient.Repository.Release.GetAll("KH-ReFined", "KH2-MAIN").Result[0];
                            var _latestMOD = _fetchMod.Assets[0].BrowserDownloadUrl;

                            Terminal.Log("Downloading \"KH-ReFined/KH2-MAIN\" from GitHub...", 1);

                            // Initialize the response and streams for Re:Fined's ZIP.
                            using var _repsonseReFined = _webClient.GetAsync(_latestMOD).Result;
                            using var _streamReFined = _repsonseReFined.Content.ReadAsStreamAsync().Result;

                            // Open the ZIP and extract it to where it needs to be.
                            using (var _downZIP = new ZipArchive(_streamReFined))
                            {
                                Terminal.Log("Installing \"KH-ReFined/KH2-MAIN\" to Panacea's Mod folder...", 1);
                                _downZIP.ExtractToDirectory(_actualModsPath);
                            }

                            var _messageAddonsLite = MessageBox.Show("Would you like to install the add-ons for Original Soundtrack and Vanilla Heartless toggles? " +
                                                                     "This process will take a bit.", "Install Add-on?", MessageBoxButtons.YesNo, MessageBoxIcon.Question);

                            if (_messageAddonsLite == DialogResult.Yes)
                            {
                                Terminal.Log("Fetching all \"KH-ReFined/KH2-VanillaOST\" releases from GitHub...", 1);

                                // I refuse to use stable releases for this purpose.
                                var _fetchOST = _gitClient.Repository.Release.GetAll("KH-ReFined", "KH2-VanillaOST").Result[0];
                                var _latestOST = _fetchOST.Assets[0].BrowserDownloadUrl;

                                Terminal.Log("Downloading \"KH-ReFined/KH2-VanillaOST\" from GitHub...", 1);

                                // Initialize the response and streams for Re:Fined's ZIP.
                                using var _repsonseOST = _webClient.GetAsync(_latestOST).Result;
                                using var _streamOST = _repsonseOST.Content.ReadAsStreamAsync().Result;

                                // Open the ZIP and extract it to where it needs to be.
                                using (var _downZIP = new ZipArchive(_streamOST))
                                {
                                    Terminal.Log("Installing \"KH-ReFined/KH2-VanillaOST\" to Panacea's Mod folder...", 1);
                                    _downZIP.ExtractToDirectory(_actualModsPath);
                                }

                                Terminal.Log("Fetching all \"KH-ReFined/KH2-VanillaEnemy\" releases from GitHub...", 1);

                                // I refuse to use stable releases for this purpose.
                                var _fetchENEMY = _gitClient.Repository.Release.GetAll("KH-ReFined", "KH2-VanillaEnemy").Result[0];
                                var _latestENEMY = _fetchENEMY.Assets[0].BrowserDownloadUrl;

                                Terminal.Log("Downloading \"KH-ReFined/KH2-VanillaEnemy\" from GitHub...", 1);

                                // Initialize the response and streams for Re:Fined's ZIP.
                                using var _repsonseENEMY = _webClient.GetAsync(_latestENEMY).Result;
                                using var _streamENEMY = _repsonseENEMY.Content.ReadAsStreamAsync().Result;

                                // Open the ZIP and extract it to where it needs to be.
                                using (var _downZIP = new ZipArchive(_streamENEMY))
                                {
                                    Terminal.Log("Installing \"KH-ReFined/KH2-VanillaEnemy\" to Panacea's Mod folder...", 1);
                                    _downZIP.ExtractToDirectory(_actualModsPath);
                                }
                            }

                            var _messageAddonJapanese = MessageBox.Show("Would you like to install the add-on for the Japanese Voices toggle?\n" +
                                                                         "This process will take a bit, as the patch is around 2GB in size.", "Install Add-on?", MessageBoxButtons.YesNo, MessageBoxIcon.Question);

                            if (_messageAddonJapanese == DialogResult.Yes)
                            {
                                Terminal.Log("Fetching all \"KH-ReFined/KH2-JapaneseVO\" releases from GitHub...", 1);

                                // I refuse to use stable releases for this purpose.
                                var _fetchJP = _gitClient.Repository.Release.GetAll("KH-ReFined", "KH2-JapaneseVO").Result[0];
                                var _latestJP = _fetchJP.Assets[0].BrowserDownloadUrl;

                                Terminal.Log("Downloading \"KH-ReFined/KH2-JapaneseVO\" from GitHub...", 1);

                                // Initialize the response and streams for Re:Fined's ZIP.
                                using var _repsonseJP = _webClient.GetAsync(_latestJP).Result;
                                using var _streamJP = _repsonseJP.Content.ReadAsStreamAsync().Result;

                                // Open the ZIP and extract it to where it needs to be.
                                using (var _downZIP = new ZipArchive(_streamJP))
                                {
                                    Terminal.Log("Installing \"KH-ReFined/KH2-JapaneseVO\" to Panacea's Mod folder...", 1);
                                    _downZIP.ExtractToDirectory(_actualModsPath);
                                }
                            }

                            var _fetchLocale = CultureInfo.InstalledUICulture.Name.Substring(0, 2);

                            if (_localeSystem.ContainsKey(_fetchLocale))
                            {
                                var _messageAddonsLocale = MessageBox.Show("Would you like to install the add-on for the " + _localeSystem[_fetchLocale].Replace("KH2-", "").Replace("VO", "") + " Voices toggle?\n" +
                                                                           "This process will take a bit, as the patch is around 1-2GB in size.", "Install Add-on?", MessageBoxButtons.YesNo, MessageBoxIcon.Question);

                                if (_messageAddonsLocale == DialogResult.Yes)
                                {
                                    var _githubID = "KH-ReFined/" + _localeSystem[_fetchLocale];
                                    Terminal.Log("Fetching all \"" + _githubID + "\" releases from GitHub...", 1);

                                    // I refuse to use stable releases for this purpose.
                                    var _fetchVO = _gitClient.Repository.Release.GetAll("KH-ReFined", _localeSystem[_fetchLocale]).Result[0];
                                    var _latestVO = _fetchVO.Assets[0].BrowserDownloadUrl;

                                    Terminal.Log("Downloading \"" + _githubID + "\" from GitHub...", 1);

                                    // Initialize the response and streams for Re:Fined's ZIP.
                                    using var _repsonseVO = _webClient.GetAsync(_latestVO).Result;
                                    using var _streamVO = _repsonseVO.Content.ReadAsStreamAsync().Result;

                                    // Open the ZIP and extract it to where it needs to be.
                                    using (var _downZIP = new ZipArchive(_streamVO))
                                    {
                                        Terminal.Log("Installing \"" + _githubID + "\" to Panacea's Mod folder...", 1);
                                        _downZIP.ExtractToDirectory(_actualModsPath);
                                    }
                                }
                            }

                            // We done.
                            var _messageComplete = MessageBox.Show("Panacea and the Base Mod for " + Locals.NAME_LABEL + " has been installed successfully! " +
                                                                     Locals.NAME_LABEL + " will now restart.",
                                                                    "Panacea Setup Completed!", MessageBoxButtons.OK, MessageBoxIcon.Information);

                            // Restart the process.
                            if (_messageComplete != DialogResult.None)
                            {
                                Process.Start(_startInfo);

                                Thread.Sleep(1000);
                                _currentProcess.Kill();
                            }
                        }

                        // We have exceeded the given rate limit. GitHub sucks.
                        catch (RateLimitExceededException)
                        {
                            var _messageRate = MessageBox.Show("Rate Limit for GitHub operations has been exceeded! Please run " + Locals.NAME_LABEL + " again after 1 hour.\n" +
                                                                Locals.NAME_LABEL + " will now terminate.",
                                                               "ERROR #600 - GitHub Rate Limit Exceeded!", MessageBoxButtons.OK, MessageBoxIcon.Error);

                            if (_messageRate != DialogResult.None)
                            {
                                Terminal.Log("GitHub Rate Limit Exceeded! CODE: 600", 2);
                                _currentProcess.Kill();
                            }
                        }
                    }

                    // If the answer is NO, just close the program.
                    else
                    {
                        Terminal.Log("User rejected the auto-install procedure. CODE: 210", 2);
                        _currentProcess.Kill();
                    }
                }

                else if (!_checkDevEnv)
                {
                    var _messagePanacea = MessageBox.Show("It seems that you do not currently have Panacea installed!\n" +
                                                           Locals.NAME_LABEL + " requires Panacea to function properly.\n" +
                                                           Locals.NAME_LABEL + " will now terminate.", "ERROR #400 - Panacea Not Found!", MessageBoxButtons.OK, MessageBoxIcon.Error);

                    if (_messagePanacea != DialogResult.None)
                        _currentProcess.Kill();
                }
            }

            var _welcomeText = "Welcome to {0} {1}!";
            Terminal.Log(String.Format(_welcomeText, Locals.NAME_LABEL, VERSION.ToString("v0.00")), 0);

            // If any one of the necessary DLLs are missing, extract them from resources.
            if (!_checkDLLs && !_checkDevEnv)
            {
                // Don't check for admin rights in Linux.
                if (Variables.PLATFORM == "STEAM" && !_checkLinux && !_checkDevEnv)
                {
                    var _windowsID = WindowsIdentity.GetCurrent();
                    var _windowsPrincipal = new WindowsPrincipal(_windowsID);

                    var _checkAdmin = _windowsPrincipal.IsInRole(WindowsBuiltInRole.Administrator);

                    if (!_checkAdmin)
                    {
                        Process.Start(_startInfo);
                        _currentProcess.Kill();
                    }
                }

                foreach (var _fileDLL in _namesDLL)
                {
                    var _exeResources = _currentEXE.GetManifestResourceNames();
                    var _resourceFile = Array.Empty<byte>();

                    using var _fileStream = _currentEXE.GetManifestResourceStream(_exeResources.First(x => x.Contains(_fileDLL)));
                    using var _streamReader = new BinaryReader(_fileStream);

                    _resourceFile = _streamReader.ReadBytes((int)_fileStream.Length);
                    File.WriteAllBytes(Path.Combine(_currentPath, _fileDLL), _resourceFile);
                }

                // If we are somehow on Linux;
                if (_checkLinux && !_checkDevEnv)
                {
                    // Fetch the Wine DLL Override registry.
                    var _mainKey = Registry.CurrentUser.OpenSubKey(@"Software\Wine\DllOverrides", true);

                    // Set values for the DLLs we want to override.
                    _mainKey.SetValue("version", "native,builtin");
                    _mainKey.SetValue("keystone", "native,builtin");
                    _mainKey.SetValue("discord_game_sdk", "native,builtin");

                    var _messageOverride = MessageBox.Show("Wine detected! All the necessary DLL Overrides were applied!\n" +
                                                            "For the overrides to apply, " + Locals.NAME_LABEL + "will now restart.",
                                                            "INFO #550 - DLL Overrides added!", MessageBoxButtons.OK, MessageBoxIcon.Information);

                    if (_messageOverride != DialogResult.None)
                    {
                        // Restart the process, since we need the game to be booted AFTER the overrides.
                        Process.Start(_startInfo);
                        _currentProcess.Kill();
                    }
                }
            }

            // Seek out the process and attach to it.
            var _mainProcess = Process.GetProcessesByName("KINGDOM HEARTS II FINAL MIX")[0];
            Hypervisor.AttachProcess(_mainProcess);

            // Initialize KH2-FML.
            Terminal.Log("Initializing Kingdom Hearts II - Flexible Modding Library...", 1);
            Entry.Initialize(Hypervisor.Process);

            Terminal.Log("Initializing Discord GameSDK...", 1);

            // If the RPC is requested;
            if (Locals.RPC_ENABLED)
            {
                // Try to initialize the RPC. If it fails, disable it.
                try { Locals.DiscordClient = new DiscordClient(833511404274974740, (ulong)Discord.CreateFlags.NoRequireDiscord); }
                catch
                {
                    Terminal.Log("Discord was not found! Disabling RPC...", 1);
                    Locals.RPC_ENABLED = false;
                }
            }

            // This code block is about sigscanning functions. I would not advise touching this.

            Terminal.Log("Trying to find all the hot-fix functions...", 1);

            Demand.FUNC_UPDATESHORTCUTS = Hypervisor.FindSignature<IntPtr>("48 83 EC 28 E8 97 F9 FF FF 48 83 C4 28 E9 DE 02 00 00");
            Demand.FADESET_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 08 57 48 83 EC 20 E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? F3 48 0F 2C F8");

            Continuous.SAVE_OFFSET = Hypervisor.FindSignature<ulong>("40 55 53 48 8D 6C 24 B1 48 81 EC C8 00 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 3F 48 8B D9 E8 ?? ?? ?? ??");
            Continuous.PROMPT_OFFSET = Hypervisor.FindSignature<ulong>("C7 05 ?? ?? ?? ?? 01 00 00 00 E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ??");
            Continuous.LIMITER_OFFSET = Hypervisor.FindSignature<ulong>("89 1D ?? ?? ?? ?? 0F 57 C0 89 05 ?? ?? ?? ?? 0F 57 D2 83 FF 08");

            Critical.LIST_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 18 57 48 83 EC 20 33 DB 48 89 6C 24 30 41 8B F8 48 8B E9");
            Critical.ICON_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 08 57 48 83 EC 20 48 8B FA 41 0F B6 D9 41 0F B6 D0 E8 ?? ?? ?? ??");
            Critical.EQUIP_OFFSET = Hypervisor.FindSignature<ulong>("48 83 EC 28 E8 ?? ?? ?? ?? 0F B6 48 02 84 C9 74 19");
            Critical.CATEGORY_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 41 54 41 55 41 56 41 57 48 81 EC 90 01 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 80 01 00 00 33 F6 89 4C 24 28 85 C9 48 8D 05 ?? ?? ?? ??");
            Critical.ASPECT_OFFSET = Hypervisor.FindSignature<ulong>("48 83 EC 28 0F 10 41 48 4C 8B C9 4C 8B D2 F3 0F 10 25 ?? ?? ?? ?? 0F 57 ED 0F 11 02");
            Critical.AREAINIT_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 10 56 48 83 EC 30 33 F6 48 89 7C 24 40 F6 05 ?? ?? ?? ?? 01 48 8B F9 74 28 8B 1D ?? ?? ?? ?? 8B CB");
            Critical.MAGICCLEAR_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 18 48 89 6C 24 20 57 48 83 EC 40 48 8B 05 ?? ?? ?? ?? 48 89 74 24 50 48 8B D8 4C 89 74 24 58 48 85 C0 0F 84 ?? ?? ?? ?? 0F 29 74 24 30 F3 0F 10 35 ?? ?? ?? ?? 0F 29 7C 24 20 0F 57 FF 48 85 DB 75 08");

            Critical.CONTROL_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 18 55 41 56 41 57 48 83 EC 20 4C 8B 41 08 48 8B D9");
            Critical.MENUSELECT_OFFSET = Hypervisor.FindSignature<ulong>("40 55 53 48 8D 6C 24 B1 48 81 EC 98 00 00 00 48 8B 05 ?? ?? ?? ??");
            Critical.INFORMATION_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 18 57 48 81 EC D0 00 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 C0 00 00 00 48 8B DA 48 8B F9");

            Critical.FUNC_MAGICGETTABLE = Hypervisor.FindSignature<IntPtr>("40 53 48 83 EC 20 4C 8B 15 ?? ?? ?? ?? 45 33 C9 41 8B D1");
            Critical.FUNC_ITEMSELECTUPDATE = Hypervisor.FindSignature<IntPtr>("48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 54 41 55 41 56 41 57 48 83 EC 40 45 32 E4 E8 ?? ?? ?? ??");
            Critical.FUNC_CONFIGUPDATEACTIVE = Hypervisor.FindSignature<IntPtr>("48 83 EC 28 48 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 63 D0 48 8B 05 ?? ?? ?? ?? 48 0F BE 0C 02");
            Critical.FUNC_CONFIGUPDATELIST = Hypervisor.FindSignature<IntPtr>("40 53 55 56 57 41 54 41 55 41 56 41 57 48 83 EC 58 E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 4C 8B F8 E8 ?? ?? ?? ??");

            Critical.ADJUSTGLOW_OFFSET = Hypervisor.FindSignature<ulong>("4C 8B DC 49 89 5B 20 55 56 57 41 54 41 56 49 8D AB 18 F2 FF FF 48 81 EC C0 0E 00 00 48 8B 05 ?? ?? ?? ??");
            Critical.INITVIEWPORT_OFFSET = Hypervisor.FindSignature<ulong>("48 83 EC 38 E8 ?? ?? ?? ?? 48 C7 44 24 20 00 00 00 00 0F 10 54 24 20 F3 0F 10 48 10 F3 0F 10 40 14 0F C6 D2 D2 F3 0F 10 D1 0F C6 D2 27 F3 0F 10 D0 0F C6 D2 39 0F 11 90 5C 01 00 00 48 83 C4 38 C3");
            Critical.ADJUSTVIEWPORT_OFFSET = Hypervisor.FindSignature<ulong>("48 83 EC 78 0F 29 74 24 60 0F 28 F1 0F 29 7C 24 50 0F 28 FA 44 0F 29 44 24 40 44 0F 28 C3 44 0F 29 4C 24 30 44 0F 28 C8 E8 ?? ?? ?? ?? 45 0F C6 C9 E1 F3 44 0F 10 CE 0F 28 74 24 60 45 0F C6 C9 C6 F3 44 0F 10 CF 0F 28 7C 24 50 45 0F C6 C9 27 F3 45 0F 10 C8 44 0F 28 44 24 40 45 0F C6 C9 39 44 0F 11 88 5C 01 00 00 44 0F 28 4C 24 30 48 83 C4 78 C3");

            TITLEINIT_OFFSET = Hypervisor.FindSignature<ulong>("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 20 48 8B F1 33 C9 E8 ?? ?? ?? ?? 33 C9 48 8B F8 E8 ?? ?? ?? ??");

            if (!ENABLE_IDLEMOVIE)
                Hypervisor.Write(TITLEINIT_OFFSET + 0x4A, new byte[] { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });

            if (ENABLE_QUICKBOOT)
                Hypervisor.Write(Variables.DATA_TTLPath, "title_fast.2ld");

            var _hotfixSound = Hypervisor.FindSignature<ulong>("40 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 E0 48 81 EC 20 01 00 00 48 C7 44 24 60 FE FF FF FF 48 89 9C 24 60 01 00 00 48 8B 05 ?? ?? ?? ??");
            Hypervisor.Write<byte>(_hotfixSound + 0x162, [0x31, 0xC0, 0x90, 0x90, 0x90]);

            Hypervisor.DeleteInstruction(Critical.MAGICCLEAR_OFFSET + 0x18A, 0x05);

            Terminal.Log("All functions found successfully!", 0);

            // Check for the base mod's existence before continuing. If it does not, error out and kill.

            if (IO.GetFileSize("reFined-v4.bin") == 0)
            {
                var _messageNoMods = MessageBox.Show("The main patch required for " + Locals.NAME_LABEL + "'s operation was not found!\n" +
                                                     "Please ensure that you have installed " + Locals.NAME_LABEL + "'s main patch \"KH-ReFined/KH2-MAIN\" into the Mod Loader and built accordingly.\n" +
                                                      Locals.NAME_LABEL + " will now terminate.",
                                                     "ERROR #404 - Base Patch not found!", MessageBoxButtons.OK, MessageBoxIcon.Error);

                if (_messageNoMods != DialogResult.None)
                    _currentProcess.Kill();
            }

            // Check for Kite's Mirage Arena mod, and adjust the RPC accordingly.

            if (IO.GetFileSize("mirageArena.bin") != 0x00)
            {
                Terminal.Log("Mirage Arena Detected! Adjusting Discord RPC accordingly...", 0);
                Locals.MIRAGE_ARENA = true;
            }

            // Initialize the various menus we are going to use.

            Terminal.Log("Initializing all the menus...", 1);

            Locals.MAIN_INTRO = new Intro();
            Locals.MAIN_CONFIG = new Config();
            Locals.MAIN_CONTINUE = new Continue();

            Terminal.Log("Menus initialized successfully!", 0);

            // Add options for "Autosave Mode" and "Button Prompts" to both Config and Intro menus.

            Terminal.Log("Adding base options to all menus...", 1);

            var _saveConfig = new Config.Entry(3, 0x5704, [0x5705, 0x5707, 0x5709], [0x5706, 0x5708, 0x570A]);
            var _promptConfig = new Config.Entry(2, 0x5722, [0x5723, 0x5725], [0x5724, 0x5726]);

            var _saveIntro = new Intro.Entry(3, 0x5733, 0xFFFF, [0x5705, 0x5707, 0x5709], [0x5706, 0x5708, 0x570A]);
            var _promptIntro = new Intro.Entry(2, 0x5737, 0xFFFF, [0x5723, 0x5725], [0x5724, 0x5726]);

            Locals.MAIN_CONFIG.Children.Insert(0x06, _promptConfig);
            Locals.MAIN_CONFIG.Children.Insert(0x06, _saveConfig);

            Locals.MAIN_INTRO.Children.Add(_saveIntro);
            Locals.MAIN_INTRO.Children.Add(_promptIntro);

            Terminal.Log("Options added successfully!", 0);

            // If Lite Mode is not enabled;

            if (!Locals.IS_LITE)
            {
                // Check for the precense of these mods in order:
                //
                // -> Battle Start/Stop Lines
                // -> Garden of Assemblage Randomizer
                // -> Ticket to Retribution
                // -> Ticket to Absolution
                //
                // and adjust functions accordingly.

                if (IO.GetFileSize("se/activateCombat.win32.scd") != 0x00)
                {
                    Terminal.Log("Battle Start/Stop Lines Detected! Enabling functions...", 0);
                    Locals.COMBAT_MODE = true;
                }

                if (IO.GetFileSize("obj/H_ZZ020_DC.mdlx") != 0x00)
                {
                    Terminal.Log("Garden of Assemblage Randomizer Detected! Adjusting some functionality accordingly...", 0);
                    Locals.RANDOMIZER = true;
                }

                if (IO.GetFileSize("obj/W_EX010_RF.mdlx") != 0x00)
                {
                    Terminal.Log("Ticket to Retribution detected! Enabling necessary flags...", 0);
                    Locals.DETECTED_RETRIBUTION = true;
                }

                if (IO.GetFileSize("obj/W_EX010_RX.mdlx") != 0x00)
                {
                    Terminal.Log("Ticket to Absolution detected! Enabling necessary flags...", 0);
                    Locals.DETECTED_ABSOLUTION = true;
                }

                // Add the option to use the Quadratum Menu.

                Terminal.Log("Adding in the option for the Quadratum Menu...", 1);

                var _configCount = Locals.MAIN_CONFIG.Children.Count;
                var _commandConfig = new Config.Entry(3, 0xB71C, [0xB734, 0xB735, 0x572C], [0xB736, 0xB737, 0x572D]);

                Locals.MAIN_CONFIG.Children.RemoveAt(_configCount - 0x02);
                Locals.MAIN_CONFIG.Children.Insert(_configCount - 0x02, _commandConfig);

                Terminal.Log("Trying to detect language packs...", 1);

                // Detect the presence of all language packs.
                Locals.LOADED_LANGS = new Dictionary<string, int>()
                {
                    { "JP", IO.GetFileSize("voice/jp/battle/tt0_sora.win32.scd") },
                    { "ES", IO.GetFileSize("voice/es/battle/tt0_sora.win32.scd") },
                    { "DE", IO.GetFileSize("voice/de/battle/tt0_sora.win32.scd") },
                    { "BG", IO.GetFileSize("voice/bg/battle/tt0_sora.win32.scd") }
                };

                // Construct the button list for all detected packs other than Japanese.
                var _configButtons = new List<ushort>
                {
                    (ushort) (Locals.LOADED_LANGS["ES"] != 0x00 ? 0x5710 : 0x0000),
                    (ushort) (Locals.LOADED_LANGS["DE"] != 0x00 ? 0x5712 : 0x0000),
                    (ushort) (Locals.LOADED_LANGS["BG"] != 0x00 ? 0x5714 : 0x0000),
                };

                // Construct the description list for all detected packs other than Japanese.
                var _configDescriptions = new List<ushort>
                {
                    (ushort) (Locals.LOADED_LANGS["ES"] != 0x00 ? 0x5711 : 0x0000),
                    (ushort) (Locals.LOADED_LANGS["DE"] != 0x00 ? 0x5713 : 0x0000),
                    (ushort) (Locals.LOADED_LANGS["BG"] != 0x00 ? 0x5715 : 0x0000),
                };

                // Remove all the invalid listings from said lists.

                _configButtons.RemoveAll(x => x == 0x0000);
                _configDescriptions.RemoveAll(x => x == 0x0000);

                if (_configButtons.Count > 0)
                    Terminal.Log("Detected MULTI5 Languages! Adjusting the menus accordingly...", 1);

                // Construct the configs for main and sub audio switchers.

                var _audioSubConfig = new Config.Entry(0, 0x572B, [], []);
                var _audioMainConfig = new Config.Entry(1, 0x570B, [0x570C], [0x570D]);

                var _audioSubIntro = new Intro.Entry(0, 0x5734, 0xFFFF, [], []);
                var _audioMainIntro = new Intro.Entry(1, 0x5734, 0xFFFF, [0x570C], [0x570D]);

                // Add the buttons of detected languages to the configs.
                foreach (var _button in _configButtons)
                {
                    _audioSubIntro.Buttons.Add(_button);
                    _audioSubConfig.Buttons.Add(_button);
                }

                // Add the descriptions of detected languages to the configs.
                foreach (var _description in _configDescriptions)
                {
                    _audioSubIntro.Descriptions.Add(_description);
                    _audioSubConfig.Descriptions.Add(_description);
                }

                // If Japanese is loaded, asjust the menus in this way:
                // -> English / Japanese / [Eur01/European]
                // -> Eur01 / Eur02 / Eur03

                if (Locals.LOADED_LANGS["JP"] != 0x00)
                {
                    Terminal.Log("Detected Japanese Langauge Pack! Adjusting the menus accordingly...", 1);

                    // Add the Japanese Option.

                    _audioMainConfig.Buttons.Add(0x570E);
                    _audioMainConfig.Descriptions.Add(0x570F);

                    _audioMainIntro.Buttons.Add(0x570E);
                    _audioMainIntro.Descriptions.Add(0x570F);

                    // If there is an European language detected;

                    if (_configButtons.Count >= 1)
                    {
                        // If only one is detected, construct and add that. Otherwise, add the "European" option.

                        var _buttonAdd = (ushort)(_configButtons.Count == 0x01 ? _configButtons[0] : 0x5716);
                        var _descriptionAdd = (ushort)(_configDescriptions.Count == 0x01 ? _configDescriptions[0] : 0x5717);

                        _audioMainConfig.Buttons.Add(_buttonAdd);
                        _audioMainConfig.Descriptions.Add(_descriptionAdd);

                        _audioMainIntro.Buttons.Add(_buttonAdd);
                        _audioMainIntro.Descriptions.Add(_descriptionAdd);
                    }
                }

                // If Japanese is not detected, but at least one EU language is:

                else if (_configButtons.Count >= 1)
                {
                    // If it's less than 3 entries, both will only be on the main selection.
                    if (_configButtons.Count < 0x03)
                        Locals.AUDIO_SUB_ONLY = true;

                    // If less than 3 languages detected, configure them. Otherwise, add the "European" option.
                    ushort[] _buttonAdd = Locals.AUDIO_SUB_ONLY ? [_configButtons[0], (ushort)(_configButtons.Count == 0x01 ? 0xFFFF : _configButtons[1])] : [0x5716];
                    ushort[] _descriptionAdd = Locals.AUDIO_SUB_ONLY ? [_configDescriptions[0], (ushort)(_configDescriptions.Count == 0x01 ? 0xFFFF : _configDescriptions[1])] : [0x5717];

                    foreach (var _button in _buttonAdd)
                    {
                        if (_button != 0xFFFF)
                        {
                            _audioMainIntro.Buttons.Add(_button);
                            _audioMainConfig.Buttons.Add(_button);
                        }
                    }

                    foreach (var _description in _descriptionAdd)
                    {
                        if (_description != 0xFFFF)
                        {
                            _audioMainIntro.Descriptions.Add(_description);
                            _audioMainConfig.Descriptions.Add(_description);
                        }
                    }
                }

                // If any other language packs are detected, activate the options for them.

                if (_audioMainConfig.Count > 1)
                {
                    Locals.MAIN_INTRO.Children.Add(_audioMainIntro);
                    Locals.MAIN_CONFIG.Children.Insert(Locals.MAIN_CONFIG.Children.Count - 0x03, _audioMainConfig);

                    Critical.AUDIO_ACTIVE = true;
                }

                // If the main audio selection includes "European", activate the audio sub-menu.

                if (_audioMainConfig.Buttons.Contains(0x5716))
                {
                    Locals.AUDIO_SUB_INTRO = _audioSubIntro;
                    Locals.AUDIO_SUB_CONFIG = _audioSubConfig;
                }

                Terminal.Log("Trying to detect addons...", 1);

                // Detect and activate Vanilla OST and Vanilla Heartless addons respectively.

                if (IO.GetFileSize("bgm/ps2md050.win32.scd") != 0x00)
                {
                    Terminal.Log("A Music Pack was located! Adding the options for it...", 1);

                    var _musicConfig = new Config.Entry(2, 0x5718, [0x5719, 0x571B], [0x571A, 0x571C]);
                    var _musicIntro = new Intro.Entry(2, 0x5735, 0x0000, [0x5719, 0x571B], [0x571A, 0x571C]);

                    Locals.MAIN_INTRO.Children.Add(_musicIntro);
                    Locals.MAIN_CONFIG.Children.Insert(Locals.MAIN_CONFIG.Children.Count - 0x03, _musicConfig);

                    Critical.MUSIC_ACTIVE = true;
                }

                if (IO.GetFileSize("obj/V_BB100.mdlx") != 0x00)
                {
                    Terminal.Log("An Enemy Palette Pack was located! Adding the options for it...", 1);

                    var _enemyConfig = new Config.Entry(2, 0x571D, [0x571E, 0x5720], [0x571F, 0x5721]);
                    var _enemyIntro = new Intro.Entry(2, 0x5736, 0x0000, [0x571E, 0x5720], [0x571F, 0x5721]);

                    Locals.MAIN_INTRO.Children.Add(_enemyIntro);
                    Locals.MAIN_CONFIG.Children.Insert(Locals.MAIN_CONFIG.Children.Count - 0x03, _enemyConfig);

                    Critical.ENEMY_ACTIVE = true;
                }

                // If not on the Randomizer, enable and add in Roxas Skip.

                if (!Locals.RANDOMIZER)
                {
                    Terminal.Log("Adding in the option to skip the prologue...", 1);

                    var _skipIntro = new Intro.Entry(2, 0x5730, 0xFFFF, [0x5738, 0x5739], [0x5731, 0x5732]);
                    Locals.MAIN_INTRO.Children.Add(_skipIntro);
                }

                // If on the randomizer, and we have one of the Special Keyblades: Add all Rando-Specific entries to their roulette.

                if (Locals.RANDOMIZER && (Locals.DETECTED_RETRIBUTION || Locals.DETECTED_ABSOLUTION))
                {
                    Terminal.Log("Randomizer with Retribution/Absolution detected! Adding support for Randomizer Keyblades...", 1);

                    Locals.KEY_DICTIONARY.Add(0x0053, 0x002C);
                    Locals.KEY_DICTIONARY.Add(0x0054, 0x002D);
                    Locals.KEY_DICTIONARY.Add(0x0055, 0x0047);
                    Locals.KEY_DICTIONARY.Add(0x0074, 0x0051);
                }

                Terminal.Log("All adjustments have been done successfully!", 0);
            }

            Terminal.Log("Initialization completed! Enjoy!", 0);
            Locals.INITIALIZED = true;
        }

        /// <summary>
        /// Handles the execution.
        /// </summary>
        public static void Execute()
        {
            Task.Run(() =>
            {
                while (!Locals.GAME_LOADED)
                {
                    var _readMessage = Hypervisor.Read<ulong>(Variables.PINT_SystemMSG);
                    if (_readMessage != 0x00)
                    {
                        Terminal.Log("Main Message file loaded! Executing the engine!", 1);
                        Locals.GAME_LOADED = true;
                        break;
                    }
                }
            });

            Task.Run(() =>
            {
                while (true)
                {
                    if (Locals.INITIALIZED && Locals.GAME_LOADED)
                    {
                        Task.Run(() =>
                        {
                            while (true)
                            {
                            SWITCHER_RESET:
                                try
                                {
                                    if (!Locals.IS_LITE)
                                    {
                                        Demand.CombatMode();

                                        if (Critical.MUSIC_ACTIVE)
                                            Switchers.MusicSwitch();

                                        Switchers.AudioSwitch();

                                        if (Critical.ENEMY_ACTIVE)
                                            Switchers.EnemySwitch();

                                        Thread.Sleep(10);
                                    }
                                }

                                catch (Exception SWITCHER_EX)
                                {
                                    var _tempTimer = DateTime.Now;

                                    if (CRASH_TIMER == DateTime.MinValue)
                                        CRASH_TIMER = DateTime.Now;

                                    DateTimeOffset _timeDifference = _tempTimer = CRASH_TIMER;
                                    if (_timeDifference.Second <= 30)
                                    {
                                        CRASH_ITERATOR++;

                                        if (CRASH_ITERATOR >= 10)
                                        {
                                            var _messageTerminate = MessageBox.Show(Locals.NAME_LABEL + " has had a thread crash more than 10 times in 30 seconds!\n" +
                                                                                    "Please send your Log file to TopazTK for investigation!\n" +
                                                                                    Locals.NAME_LABEL + " will now terminate!", "ERROR #950 - Thread Crash Overload!", MessageBoxButtons.OK, MessageBoxIcon.Error);

                                            if (_messageTerminate != DialogResult.None)
                                            {
                                                Terminal.Log("One or more threads has crashed more than 5 times in the span of 30 seconds! Terminating " + Locals.NAME_LABEL + "...", 2);
                                                Process.GetCurrentProcess().Kill();
                                            }
                                        }
                                    }

                                    else
                                    {
                                        CRASH_ITERATOR = 0;
                                        CRASH_TIMER = DateTime.MinValue;
                                    }

                                    Terminal.Log("The SWITCHER Thread crashed due an exception! Attempting to restart...", 2);
                                    Terminal.Log(SWITCHER_EX);
                                    goto SWITCHER_RESET;
                                }
                            }
                        });

                        Task.Run(() =>
                        {

                            while (true)
                            {
                            CRITICAL_RESET:
                                try
                                {
                                    if (!Locals.IS_LITE)
                                        Critical.ApplyCrowns();

                                    Critical.HandleAutosave();
                                    Critical.AspectCorrection();

                                    Thread.Sleep(10);
                                }

                                catch (Exception CRITICAL_EX)
                                {
                                    var _tempTimer = DateTime.Now;

                                    if (CRASH_TIMER == DateTime.MinValue)
                                        CRASH_TIMER = DateTime.Now;

                                    DateTimeOffset _timeDifference = _tempTimer = CRASH_TIMER;
                                    if (_timeDifference.Second <= 30)
                                    {
                                        CRASH_ITERATOR++;

                                        if (CRASH_ITERATOR >= 10)
                                        {
                                            var _messageTerminate = MessageBox.Show(Locals.NAME_LABEL + " has had a thread crash more than 10 times in 30 seconds!\n" +
                                                                                    "Please send your Log file to TopazTK for investigation!\n" +
                                                                                    Locals.NAME_LABEL + " will now terminate!", "ERROR #950 - Thread Crash Overload!", MessageBoxButtons.OK, MessageBoxIcon.Error);

                                            if (_messageTerminate != DialogResult.None)
                                            {
                                                Terminal.Log("One or more threads has crashed more than 5 times in the span of 30 seconds! Terminating " + Locals.NAME_LABEL + "...", 2);
                                                Process.GetCurrentProcess().Kill();
                                            }
                                        }
                                    }

                                    else
                                    {
                                        CRASH_ITERATOR = 0;
                                        CRASH_TIMER = DateTime.MinValue;
                                    }

                                    Terminal.Log("The CRITICAL Thread crashed due an exception! Attempting to restart...", 2);
                                    Terminal.Log(CRITICAL_EX);
                                    goto CRITICAL_RESET;
                                }
                            }
                        });

                        Task.Run(() =>
                        {
                            while (true)
                            {
                            BIG_LOAD_RESET:
                                try
                                {
                                    Continuous.FixSavePoint();
                                    Continuous.EnforcePrompts();
                                    Continuous.CorrectTutorials();
                                    Continuous.ModifyLimitShorts();
                                    Continuous.HandleFramelimiter();

                                    Demand.TriggerReset();
                                    Demand.HandleShortcuts();

                                    Critical.HandleIntro();
                                    Critical.HandleConfiguration();

                                    if (Locals.RANDOMIZER)
                                        Critical.EnablersAP();

                                    if (!Locals.IS_LITE)
                                    {
                                        Continuous.ActivateWarpGOA();

                                        Demand.HandleEncounter();
                                        Demand.HandleAutoattack();

                                        Critical.HandleShake();
                                        Critical.SortMagic();

                                        if (Locals.RANDOMIZER)
                                            Critical.RegisterMagic();

                                        Critical.RetryBattles();
                                        Critical.PrologueSkip();
                                        Critical.RetributionLogic();
                                        Critical.AllowFormShorcuts();
                                    }

                                    Thread.Sleep(10);
                                }

                                catch (Exception BIG_LOAD_EX)
                                {
                                    var _tempTimer = DateTime.Now;

                                    if (CRASH_TIMER == DateTime.MinValue)
                                        CRASH_TIMER = DateTime.Now;

                                    DateTimeOffset _timeDifference = _tempTimer = CRASH_TIMER;
                                    if (_timeDifference.Second <= 30)
                                    {
                                        CRASH_ITERATOR++;

                                        if (CRASH_ITERATOR >= 10)
                                        {
                                            var _messageTerminate = MessageBox.Show(Locals.NAME_LABEL + " has had a thread crash more than 10 times in 30 seconds!\n" +
                                                                                    "Please send your Log file to TopazTK for investigation!\n" +
                                                                                    Locals.NAME_LABEL + " will now terminate!", "ERROR #950 - Thread Crash Overload!", MessageBoxButtons.OK, MessageBoxIcon.Error);

                                            if (_messageTerminate != DialogResult.None)
                                            {
                                                Terminal.Log("One or more threads has crashed more than 5 times in the span of 30 seconds! Terminating " + Locals.NAME_LABEL + "...", 2);
                                                Process.GetCurrentProcess().Kill();
                                            }
                                        }
                                    }

                                    else
                                    {
                                        CRASH_ITERATOR = 0;
                                        CRASH_TIMER = DateTime.MinValue;
                                    }

                                    Terminal.Log("The BIG_LOAD Thread crashed due an exception! Attempting to restart...", 2);
                                    Terminal.Log(BIG_LOAD_EX);
                                    goto BIG_LOAD_RESET;
                                }
                            }
                        });

                        Task.Run(() =>
                        {
                            while (true)
                            {
                            EXTERNAL_RESET:
                                try
                                {
                                    if (Locals.RPC_ENABLED)
                                    {
                                        External.DiscordRPC();
                                        Thread.Sleep(500);
                                    }
                                }


                                catch (Exception EXTERNAL_EX)
                                {
                                    var _tempTimer = DateTime.Now;

                                    if (CRASH_TIMER == DateTime.MinValue)
                                        CRASH_TIMER = DateTime.Now;

                                    DateTimeOffset _timeDifference = _tempTimer = CRASH_TIMER;
                                    if (_timeDifference.Second <= 30)
                                    {
                                        CRASH_ITERATOR++;

                                        if (CRASH_ITERATOR >= 10)
                                        {
                                            var _messageTerminate = MessageBox.Show(Locals.NAME_LABEL + " has had a thread crash more than 5 times in 30 seconds!\n" +
                                                                                    "Please send your Log file to TopazTK for investigation!\n" +
                                                                                    Locals.NAME_LABEL + " will now terminate!", "ERROR #950 - Thread Crash Overload!", MessageBoxButtons.OK, MessageBoxIcon.Error);

                                            if (_messageTerminate != DialogResult.None)
                                            {
                                                Terminal.Log("One or more threads has crashed more than 10 times in the span of 30 seconds! Terminating " + Locals.NAME_LABEL + "...", 2);
                                                Process.GetCurrentProcess().Kill();
                                            }
                                        }
                                    }

                                    else
                                    {
                                        CRASH_ITERATOR = 0;
                                        CRASH_TIMER = DateTime.MinValue;
                                    }

                                    Terminal.Log("The EXTERNAL Thread crashed due an exception! Attempting to restart...", 2);
                                    Terminal.Log(EXTERNAL_EX);
                                    goto EXTERNAL_RESET;
                                }
                            }
                        });

                        break;
                    }
                }
            });
        }
    }
}
