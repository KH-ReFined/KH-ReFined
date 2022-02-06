/*
=================================================
      KINGDOM HEARTS - RE:FIXED FOR BBS!
       COPYRIGHT TOPAZ WHITELOCK - 2022
 LICENSED UNDER MIT. GIVE CREDIT WHERE IT'S DUE! 
=================================================
*/

using System;
using System.Text;
using System.Linq;
using System.Diagnostics;
using System.Runtime.InteropServices;

using System.Windows.Forms;

using AxaFormBase;
using ReFixed.Forms;

namespace ReFixed
{
	public static class Functions
	{
		public static void RenameFinisher()
		{
			var _menuPointer = Hypervisor.Read<ulong>(Variables.FinisherAddress);

			if (_menuPointer > 0)
			{
				var _finishPointer = Hypervisor.Read<ulong>(_menuPointer + 0xC8, true);

				if (_finishPointer > 0)
				{
					var _inputRead = Hypervisor.Read<ushort>(Variables.InputAddress);
					var _selectedFinisher = Hypervisor.Read<byte>(_finishPointer + 0x8E, true);

					if ((_inputRead & 0x1000) == 0x1000 && !Variables.Debounce)
					{ 
						Variables.Debounce = true;

						using(InputText _inForm = new InputText())
						{
							BaseSimpleForm.theInstance.suspend();

							var _result = _inForm.ShowDialog();

							if (_result == DialogResult.OK)
							{
								var _textArray = Encoding.ASCII.GetBytes(_inForm.FinisherName);

								var _fillerArray = new List<byte>();

								_fillerArray.AddRange(_textArray);
								_fillerArray.AddRange(new byte[0x10 - _textArray.Length]);

								Hypervisor.WriteArray(Variables.NameAddress + (ulong)(0x26 * _selectedFinisher), _fillerArray.ToArray());
							}

							_inputRead = 0x00;
							BaseSimpleForm.theInstance.resume();
						}
					}

					else if ((_inputRead & 0x1000) != 0x1000 && Variables.Debounce)
						Variables.Debounce = false;
				}
			}
		}

		public static void Execute()
		{
			RenameFinisher();
		}
	}
}
