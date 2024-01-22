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
using ReFined;

using DiscordRPC;

namespace AxaFormBase
{
	public partial class BaseSimpleForm : Form
	{
		public static void _keyEvent(object s, KeyEventArgs e)
		{
			if ((e.Control && e.Alt) || (e.Control && e.KeyCode == Keys.Escape))
				CaptureStatus = !CaptureStatus;

			if (CaptureStatus && !_cursorHidden)
			{
				Cursor.Hide();
                _cursorHidden = true;
			}

			else if (!CaptureStatus && _cursorHidden)
			{
				Cursor.Show();
                _cursorHidden = false;
			}
		}

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

                if (Variables.DEV_MODE)
                AllocConsole();

                Helpers.Log("Launching Re:Fined...", 0);
                
                if (BaseSimpleForm.theInstance == null)
                    new BaseSimpleForm(_app, "KINGDOM HEARTS II - FINAL MIX [Re:Fined v5.00]");

                Cursor.Hide();
                theInstance.KeyDown += _keyEvent;

                CaptureStatus = true;
                _cursorHidden = true;

                if (Variables.DISCORD_TOGGLE)
                    Variables.DiscordClient.Initialize();
                    
                CancelSource = new CancellationTokenSource();
                MainToken = BaseSimpleForm.CancelSource.Token;

                Hypervisor.AttachProcess(Process.GetCurrentProcess(), 0x56454E);

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

                            Thread.Sleep(1);
                        }
                    },
                    MainToken
                );

                Helpers.Log("Re:Fined launched with no errors!", 0);

                return BaseSimpleForm.theInstance;
            }

            catch (Exception _caughtEx)
            {
                Helpers.LogException(_caughtEx);
                Helpers.Log("Re:Fined failed to launch because of an exception!", 1);
                Environment.Exit(-1);
                return null;
            }
        }
	}
}
