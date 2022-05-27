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
        public static CancellationTokenSource CancelSource;
        public static CancellationToken MainToken;
        public static Task MainTask;

        public unsafe static BaseSimpleForm createInstance(AppInterface* _app, string title)
        {
            if (BaseSimpleForm.theInstance == null)
                new BaseSimpleForm(_app, "KINGDOM HEARTS II - FINAL MIX [Re:Fixed v2.75]");

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
                        Thread.Sleep(5);
                    }
                },
                MainToken
            );

            return BaseSimpleForm.theInstance;
        }
    }
}
