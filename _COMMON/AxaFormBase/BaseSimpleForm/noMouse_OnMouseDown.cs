/*
==================================================
      KINGDOM HEARTS - RE:FIXED COMMON FILE!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER DBAD. GIVE CREDIT WHERE IT'S DUE! 
==================================================
*/

using System;
using System.Drawing;
using System.Threading;
using System.Windows.Forms;
using System.ComponentModel;
using System.Threading.Tasks;
using System.Runtime.ExceptionServices;
using System.Runtime.InteropServices;

using Axa;

namespace AxaFormBase
{
	public partial class BaseSimpleForm : Form
	{
		private void noMouse_OnMouseDown(object sender, MouseEventArgs e)
		{
			if (!_captureStatus && !_cursorHidden)
			{
				Cursor.Hide();

			    _cursorHidden = true;		
                _captureStatus = true;
            }
		}
	}
}
