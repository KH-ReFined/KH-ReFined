/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 2 FM!
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
        public static bool CaptureStatus;

        public static CancellationTokenSource CancelSource;
        public static CancellationToken MainToken;
        public static Task MainTask;

        [DllImport("kernel32")]
		static extern bool AllocConsole();

        public unsafe static BaseSimpleForm createInstance(AppInterface* _app, string title)
        {
            try
            {
                Helpers.InitConfig();

                if (Variables.devMode)
                AllocConsole();

                Helpers.Log("Launching Re:Fixed...", 0);
                
                # UpdateAgent.UpdateCheck();

                if (BaseSimpleForm.theInstance == null)
                    new BaseSimpleForm(_app, "KINGDOM HEARTS - RE: CHAIN OF MEMORIES [Re:Fixed v3.00]");

                Cursor.Hide();
                theInstance.KeyDown += _keyEvent;

                CaptureStatus = true;
                _cursorHidden = true;

                if (Variables.rpcToggle)
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

                            Thread.Sleep(5);
                        }
                    },
                    MainToken
                );

                Helpers.Log("Re:Fixed launched with no errors!", 0);

                return BaseSimpleForm.theInstance;
            }

            catch (Exception _caughtEx)
            {
                Helpers.LogException(_caughtEx);
                Helpers.Log("Re:Fixed failed to launch because of an exception!", 1);
                Environment.Exit(-1);
                return null;
            }
        }
    }
}
