/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 1 FM!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.IO;
using System.Drawing;
using System.Threading;
using System.Diagnostics;
using System.Windows.Forms;
using System.ComponentModel;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Runtime.ExceptionServices;

using Axa;
using ReFixed;

using DiscordRPC;

namespace AxaFormBase
{
    public partial class BaseSimpleForm : Form
    {
        static bool _cursorHidden;
        static bool CaptureStatus;

        public static CancellationTokenSource CancelSource;
        public static CancellationToken MainToken;
        public static Task MainTask;

        public static float _resDiv;

        public unsafe static BaseSimpleForm createInstance(AppInterface* _app, string title)
        {
            UpdateAgent.UpdateCheck();

            if (BaseSimpleForm.theInstance == null)
                new BaseSimpleForm(_app, "KINGDOM HEARTS - FINAL MIX [Re:Fixed v2.90]");

            Cursor.Hide();
            theInstance.KeyDown += _keyEvent;

            CaptureStatus = true;
            _cursorHidden = true;

            if (File.Exists("reFixed.ini"))
            {
                var _configIni = new TinyIni("reFixed.ini");

                Variables.saveToggle = Convert.ToBoolean(_configIni.Read("autoSave", "ReFixed"));
                Variables.sfxToggle = Convert.ToBoolean(_configIni.Read("saveIndicator", "ReFixed"));
                Variables.discordToggle = Convert.ToBoolean(_configIni.Read("discordRPC", "ReFixed"));
            }

            else
                File.WriteAllText("reFixed.ini", "[ReFixed]\n" + "autoSave = true\n" + "discordRPC = true\n" + "saveIndicator = true");

            if (Variables.discordToggle)
                Variables.DiscordClient.Initialize();
                
            CancelSource = new CancellationTokenSource();
            MainToken = BaseSimpleForm.CancelSource.Token;

            Hypervisor.AttachProcess(Process.GetCurrentProcess(), Variables.BASE_ADDRESS);

            theInstance.timer1.Start();

            MainTask = Task.Factory.StartNew(
                delegate()
                {
                    while (!MainToken.IsCancellationRequested)
                    {
                        Functions.Execute();

                        if (Form.ActiveForm != null && CaptureStatus)
                        {
                            var _scrPoint = theInstance.PointToScreen(new Point(0, 0));
                            Cursor.Position = new Point(_scrPoint.X + theInstance.Width / 2, _scrPoint.Y + theInstance.Height / 2);
                        }

                        else
                            CaptureStatus = false;

                        Rectangle _windRect = Screen.FromControl(BaseSimpleForm.theInstance).Bounds;

                        if (_windRect.Bottom == theInstance.Height && _windRect.Right == theInstance.Width)
                        {
                            float _divisorValue =
                                (float)theInstance.Width / (float)theInstance.Height;

                            if (_divisorValue != _resDiv)
                            {
                                _resDiv = _divisorValue;
                                Functions.AspectCorrection(_divisorValue);
                            }
                        }
                        
                        Thread.Sleep(5);
                    }
                },
                MainToken
            );

            return BaseSimpleForm.theInstance;
        }
    }
}