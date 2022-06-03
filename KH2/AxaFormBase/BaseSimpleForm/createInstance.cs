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
        static bool _captureStatus;

        public static CancellationTokenSource CancelSource;
        public static CancellationToken MainToken;
        public static Task MainTask;

        public static void keyEvent(object s, KeyEventArgs e)
		{
			if ((e.Control && e.Alt) || (e.Control && e.KeyCode == Keys.Escape))
				_captureStatus = !_captureStatus;

			if (_captureStatus && !_cursorHidden)
			{
				Cursor.Hide();
                _cursorHidden = true;
			}

			else if (!_captureStatus && _cursorHidden)
			{
				Cursor.Show();
                _cursorHidden = false;
			}
		}

        public unsafe static BaseSimpleForm createInstance(AppInterface* _app, string title)
        {
            if (BaseSimpleForm.theInstance == null)
                new BaseSimpleForm(_app, "KINGDOM HEARTS II - FINAL MIX [Re:Fixed v2.75]");

            Cursor.Hide();
            theInstance.KeyDown += _keyEvent;

            captureStatus = true;
            cursorHidden = true;

            Variables.DiscordClient.Initialize();

            CancelSource = new CancellationTokenSource();
            MainToken = BaseSimpleForm.CancelSource.Token;

            Hypervisor.AttachProcess(Process.GetCurrentProcess(), Variables.BASE_OFFSET);

            MainTask = Task.Factory.StartNew(
                delegate()
                {
                    while (!MainToken.IsCancellationRequested)
                    {
                        Functions.Execute();

                        if (Form.ActiveForm != null && _captureStatus)
                        {
                            var _scrPoint = theInstance.PointToScreen(new Point(0, 0));
                            Cursor.Position = new Point(_scrPoint.X + theInstance.Width / 2, _scrPoint.Y + theInstance.Height / 2);
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
