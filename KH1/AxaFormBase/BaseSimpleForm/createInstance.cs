/*
==================================================
      KINGDOM HEARTS - RE:FIXED FOR 1 FM!
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
				new BaseSimpleForm(_app, "KINGDOM HEARTS: FINAL MIX [Re:Fixed v1.25]");

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
					Rectangle _windRect = Screen.FromControl(BaseSimpleForm.theInstance).Bounds;

					if (_windRect.Bottom == theInstance.Height && _windRect.Right == theInstance.Width)
					{
						float _divisorValue = (float)theInstance.Width / (float)theInstance.Height;

						if (_divisorValue != ResolutionDiv)
						{
							ResolutionDiv = _divisorValue;
							Functions.OverrideAspect(_divisorValue);
						}
					}

					Functions.Execute();
				}

			}, MainToken);

			return theInstance;
		}
	}
}
