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
	// Token: 0x02000500 RID: 1280
	public partial class BaseSimpleForm : Form
	{
		// Token: 0x060001E3 RID: 483 RVA: 0x11D5A000 File Offset: 0x01482800
        public unsafe static BaseSimpleForm createInstance(AppInterface* _app, string title)
        {
            UpdateAgent.UpdateCheck();

            if (theInstance == null)
                new BaseSimpleForm(_app, "KINGDOM HEARTS: BIRTH BY SLEEP - FINAL MIX [Re:Fixed v2.75]");

            Cursor.Hide();
            theInstance.KeyDown += _keyEvent;

            CaptureStatus = true;
            _cursorHidden = true;

            if (!Variables.Initialized)
                Functions.Initialization();

            if (Variables.discordToggle)
                Variables.DiscordClient.Initialize();

            CancelSource = new CancellationTokenSource();
            MainToken = CancelSource.Token;

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

            return theInstance;
        }
	}
}
