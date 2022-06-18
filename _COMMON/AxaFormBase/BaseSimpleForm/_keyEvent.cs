using System;
using System.ComponentModel;
using System.Drawing;
using System.Runtime.ExceptionServices;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using Axa;

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
                <Module>.Axa.setMouseActive(true);
			}

			else if (!CaptureStatus && _cursorHidden)
			{
				Cursor.Show();
                _cursorHidden = false;
                <Module>.Axa.setMouseActive(false);
			}
		}
	}
}
