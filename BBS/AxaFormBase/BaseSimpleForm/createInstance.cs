/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR BBS!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
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

		public static float ResolutionDiv; 

		public unsafe static BaseSimpleForm createInstance(AppInterface* _app, string title)
		{
			if (theInstance == null)
				new BaseSimpleForm(_app, "KINGDOM HEARTS: BIRTH BY SLEEP - FINAL MIX [Re:Fixed v1.25]");

			Variables.RichClient.Initialize();

			CancelSource = new CancellationTokenSource();
			MainToken = BaseSimpleForm.CancelSource.Token;

			Variables.GameProcess = Process.GetCurrentProcess();
			Variables.GameHandle = Variables.GameProcess.Handle;
			Variables.ExeAddress = (ulong)Variables.GameProcess.MainModule.BaseAddress.ToInt64();
			Variables.GameAddress = Variables.ExeAddress + Variables.BaseAddress;

			MainTask = Task.Factory.StartNew(delegate()
			{
				while (!MainToken.IsCancellationRequested)
				{
					Functions.Execute();
				}

			}, MainToken);

			return theInstance;
		}
	}
}
