using System;
using System.Linq;
using System.Threading;
using System.Windows.Forms;

using HidLibrary;
using DualShockAPI;
using DualSenseAPI;
using Nefarius.ViGEm.Client;
using Nefarius.ViGEm.Client.Exceptions;
using Nefarius.ViGEm.Client.Targets.Xbox360;

using BSharpConvention = Binarysharp.MSharp.Assembly.CallingConvention.CallingConventions;

namespace ReFined
{
    public class ControllerIO
    {
        public static float BIG_MOTOR;
        public static float SMALL_MOTOR;
        public static short TICK_COUNT;

        static float PAST_BIG;
        static float[] PAST_COLOR;

        static float PLAYER_HP;
        static float PLAYER_MP;
        static byte BATTLE_STATE;
        static byte FORM_STATE;

        static bool[] DEBOUNCE = new bool[0x20];

        /// <summary>
        /// Initializes the Controller Library.
        /// Supports: DualSense, DualShock 4
        /// </summary>
        public static void Initialize()
        {
            if (!Variables.CONTROLLER_FOUND && Variables.DUALSENSE_TOGGLE)
            {
                Helpers.Log("Enumerating DualSense controllers...", 0);
                var _senseList = DualSense.EnumerateControllers();

                if (_senseList.Count() != 0x00)
                {
                    Helpers.Log("Found a DualSense Controller! Initializing...", 0);
                    Variables.CONTROLLER_SENSE = _senseList.First();
                    Variables.CONTROLLER_SENSE.Acquire();

                    if (Variables.CONTROLLER_SENSE.IoMode == IoMode.Bluetooth)
                        Helpers.Log("DualSense is connected via Bluetooth.", 0);

                    else
                        Helpers.Log("DualSense is connected via USB.", 0);

                    Variables.CONTROLLER_FOUND = true;
                }

                else
                    Helpers.Log("DualSense is not detected! Looking for other types of controllers...", 1);
            }

            if (!Variables.CONTROLLER_FOUND && Variables.DUALSENSE_TOGGLE)
            {
                Helpers.Log("Enumerating Dualshock controllers...", 0);
                var _shockList = HidDevices.Enumerate(0x054C, 0x05C4, 0x09CC);

                if (_shockList.Count() != 0x00)
                {
                    var _hidDevice = _shockList.First();
                    _hidDevice.OpenDevice(false);

                    Helpers.Log("Found a DualShock 4 Controller! Initializing...", 0);
                    Variables.CONTROLLER_SHOCK = new DualShock(_hidDevice, 0);

                    if (Variables.CONTROLLER_SHOCK.Device.Capabilities.InputReportByteLength == 64)
                        Helpers.Log("DualShock 4 is connected via USB.", 0);

                    else
                        Helpers.Log("DualShock 4 is connected via Bluetooth.", 0);


                    Variables.CONTROLLER_FOUND = true;
                }

                else
                    Helpers.Log("DualShock 4 is not detected! Looking for other types of controllers...", 1);
            }

            if (Variables.DUALSENSE_TOGGLE)
            {
                Helpers.Log("Initializing ViGEm...", 0);

                try
                {
                    Variables.CONTROLLER_CLIENT = new ViGEmClient();
                    Variables.CONTROLLER_FAKE = Variables.CONTROLLER_CLIENT.CreateXbox360Controller();
                    Variables.CONTROLLER_FAKE.Connect();

                    Variables.CONTROLLER_FAKE.FeedbackReceived += delegate (object s, Xbox360FeedbackReceivedEventArgs a)
                    {
                        BIG_MOTOR = a.LargeMotor;
                        SMALL_MOTOR = a.SmallMotor;
                        TICK_COUNT = 0;
                    };
                }

                catch (VigemBusNotFoundException)
                {
                    Helpers.Log("ViGEmBUS Driver not installed! ERR504", 2);

                    var _messageResult = MessageBox.Show(
                        "The ViGEmBUS Driver is not installed! Please install this driver to play Re:Fined!\n\n" +
                        "Re:Fined will now be terminated.",
                        "Error #504: ViGEmBUS Driver not installed!", MessageBoxButtons.OK,
                        MessageBoxIcon.Error);

                    if (_messageResult == DialogResult.Cancel || _messageResult == DialogResult.OK)
                        Environment.Exit(-2);
                }
            }
        }

        /// <summary>
        /// Handles the input logic from external controllers.
        /// </summary>
        public static void ControllerInput()
        {
            try
            {
                if (Variables.CONTROLLER_SENSE != null)
                {
                    byte _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);
                    byte _menuRead = Hypervisor.Read<byte>(Variables.ADDR_MenuActive);

                    var _senseIn = Variables.CONTROLLER_SENSE.ReadOnce();

                    if (Variables.CONTROLLER_SENSE.IoMode == IoMode.Bluetooth)
                    {

                        if (_senseIn.BatteryStatus.Level <= 1 && !DEBOUNCE[8] && (_loadRead == 0x01 && _menuRead == 0x00 && !Operations.CheckTitle()))
                        {
                            Additions.ShowInformation(0x6012);
                            DEBOUNCE[8] = true;
                        }

                        else if (Operations.CheckTitle() && DEBOUNCE[8])
                            DEBOUNCE[8] = false;
                    }

                    // Fetch the buttons, fetch the analogs, and interpret them to the 360 false controller.

                    var _senseButtonArray = new bool[]
                    {
                        _senseIn.DPadUpButton, _senseIn.DPadDownButton, _senseIn.DPadLeftButton, _senseIn.DPadRightButton,
                        _senseIn.MenuButton, _senseIn.TouchpadButton,
                        _senseIn.L3Button, _senseIn.R3Button, _senseIn.L1Button, _senseIn.R1Button, false,
                        _senseIn.CrossButton, _senseIn.CircleButton, _senseIn.SquareButton, _senseIn.TriangleButton
                    };

                    var _senseAxisArray = new short[]
                    {
                        (short)(_senseIn.LeftAnalogStick.X * 32766F),
                        (short)(_senseIn.LeftAnalogStick.Y * 32766F),
                        (short)(_senseIn.RightAnalogStick.X * 32766F),
                        (short)(_senseIn.RightAnalogStick.Y * 32766F)
                    };

                    for (int i = 0; i < _senseButtonArray.Length; i++)
                        Variables.CONTROLLER_FAKE.SetButtonState(i, _senseButtonArray[i]);

                    for (int i = 0; i < _senseAxisArray.Length; i++)
                        Variables.CONTROLLER_FAKE.SetAxisValue(i, _senseAxisArray[i]);

                    Variables.CONTROLLER_FAKE.SetSliderValue(Xbox360Slider.LeftTrigger, (byte)(_senseIn.L2 * 255F));
                    Variables.CONTROLLER_FAKE.SetSliderValue(Xbox360Slider.RightTrigger, (byte)(_senseIn.R2 * 255F));
                }

                else if (Variables.CONTROLLER_SHOCK != null)
                {
                    Variables.CONTROLLER_SHOCK.retrieveData();

                    var _stateShock = Variables.CONTROLLER_SHOCK.currentState();

                    if (!Variables.CONTROLLER_SHOCK.IsUSB)
                    {
                        byte _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);
                        byte _menuRead = Hypervisor.Read<byte>(Variables.ADDR_MenuActive);

                        if (Variables.CONTROLLER_SHOCK.Charge <= 10 && !DEBOUNCE[8] && (_loadRead == 0x01 && _menuRead == 0x00 && !Operations.CheckTitle()))
                        {
                            Additions.ShowInformation(0x6022);
                            DEBOUNCE[8] = true;
                        }

                        else if (Operations.CheckTitle() && DEBOUNCE[8])
                            DEBOUNCE[8] = false;
                    }

                    var _shockButtonArray = new bool[]
                    {
                        _stateShock.DpadUp, _stateShock.DpadDown, _stateShock.DpadLeft, _stateShock.DpadRight,
                        _stateShock.Options, _stateShock.Share,
                        _stateShock.L3, _stateShock.R3, _stateShock.L1, _stateShock.R1, false,
                        _stateShock.Cross, _stateShock.Circle, _stateShock.Square, _stateShock.Triangle
                    };

                    var _shockAxisArray = new short[]
                    {
                        (short)(_stateShock.LX >= 128 ? ((128F - _stateShock.LX) * -255F) : (_stateShock.LX - 128F) * 255F),
                        (short)(_stateShock.LY >= 128 ? ((_stateShock.LY - 128F) * -255F) : (128F - _stateShock.LY) * 255F),
                        (short)(_stateShock.RX >= 128 ? ((128F - _stateShock.RX) * -255F) : (_stateShock.RX - 128F) * 255F),
                        (short)(_stateShock.RY >= 128 ? ((_stateShock.RY - 128F) * -255F) : (128F - _stateShock.RY) * 255F),
                    };

                    for (var i = 0; i < _shockButtonArray.Length; i++)
                        Variables.CONTROLLER_FAKE.SetButtonState(i, _shockButtonArray[i]);

                    for (var i = 0; i < _shockAxisArray.Length; i++)
                        Variables.CONTROLLER_FAKE.SetAxisValue(i, _shockAxisArray[i]);

                    Variables.CONTROLLER_FAKE.SetSliderValue(Xbox360Slider.LeftTrigger, _stateShock.L2);
                    Variables.CONTROLLER_FAKE.SetSliderValue(Xbox360Slider.RightTrigger, _stateShock.R2);

                    // Handle the controller disconnecting.

                    if (!Variables.CONTROLLER_SHOCK.Device.IsConnected || Variables.CONTROLLER_SHOCK.Device.IsTimedOut)
                    {
                        Variables.CONTROLLER_SHOCK = null;
                        Variables.CONTROLLER_FOUND = false;
                        Helpers.Log("DualShock 4 Controller has been disconnected.", 1);
                    }
                }
            }

            catch (System.AggregateException)
            {
                if (Variables.CONTROLLER_SENSE != null)
                {
                    Variables.CONTROLLER_SENSE.Release();
                    Variables.CONTROLLER_SENSE = null;
                    Helpers.Log("DualSense Controller has been disconnected.", 1);
                }

                Variables.CONTROLLER_FOUND = false;
            }
        }

        /// <summary>
        /// Handles the output logic to external controllers.
        /// </summary>
        public static void ControllerOutput()
        {
            try
            {
                float _hpCurrent = Hypervisor.Read<byte>(Variables.ADDR_PlayerHP);
                float _hpMaximum = Hypervisor.Read<byte>(Variables.ADDR_PlayerHP + 0x04);

                float _mpCurrent = Hypervisor.Read<byte>(Variables.ADDR_PlayerHP + 0x180);
                float _mpMaximum = Hypervisor.Read<byte>(Variables.ADDR_PlayerHP + 0x184);

                byte _loadRead = Hypervisor.Read<byte>(Variables.ADDR_LoadFlag);
                byte _formRead = Hypervisor.Read<byte>(Variables.ADDR_PlayerForm);
                byte _battleRead = Hypervisor.Read<byte>(Variables.ADDR_BattleFlag);


                if (!Operations.CheckTitle())
                {
                    switch (Variables.DUALSENSE_MODE)
                    {

                        case 0:
                            {
                                if (_hpCurrent != PLAYER_HP)
                                {
                                    PLAYER_HP = _hpCurrent;

                                    float _healthPercent = _hpCurrent / _hpMaximum;
                                    Variables.SENSE_COLOR = new float[] { 1.0F - _healthPercent, _healthPercent, 0.0F };
                                }

                                break;
                            }

                        case 1:
                            {
                                if (_mpCurrent != PLAYER_MP && Variables.DUALSENSE_MODE == 0x01)
                                {
                                    PLAYER_MP = _mpCurrent;

                                    float _magicPercent = _mpCurrent / _mpMaximum;
                                    float _magicDeduct = 0.8F * _magicPercent;

                                    Variables.SENSE_COLOR = new float[] { 0.8F - _magicDeduct, 0.0F, 0.8F };
                                }

                                break;
                            }

                        case 2:
                            {
                                if (_battleRead != BATTLE_STATE)
                                {
                                    BATTLE_STATE = _battleRead;

                                    switch (_battleRead)
                                    {
                                        case 0:
                                            Variables.SENSE_COLOR = new float[] { 0.0F, 0.0F, 1.0F };
                                            break;
                                        case 1:
                                            Variables.SENSE_COLOR = new float[] { 1.0F, 0.8F, 0.0F };
                                            break;
                                        case 2:
                                            Variables.SENSE_COLOR = new float[] { 0.9F, 0.0F, 0.0F };
                                            break;
                                    }
                                }

                                break;
                            }

                        case 3:
                            {
                                if (_formRead != FORM_STATE)
                                {
                                    switch (_formRead)
                                    {
                                        case 0:
                                            Variables.SENSE_COLOR = new float[] { 0.5F, 0.5F, 0.5F };
                                            break;
                                        case 1:
                                            Variables.SENSE_COLOR = new float[] { 1.0F, 0.0F, 0.0F };
                                            break;
                                        case 2:
                                            Variables.SENSE_COLOR = new float[] { 0.0F, 1.0F, 0.0F };
                                            break;
                                        case 3:
                                            Variables.SENSE_COLOR = new float[] { 1.0F, 0.5F, 0.0F };
                                            break;
                                        case 4:
                                            Variables.SENSE_COLOR = new float[] { 1.0F, 1.0F, 0.0F };
                                            break;
                                        case 5:
                                            Variables.SENSE_COLOR = new float[] { 1.0F, 1.0F, 1.0F };
                                            break;
                                        case 6:
                                            Variables.SENSE_COLOR = new float[] { 0.0F, 0.0F, 0.0F };
                                            break;
                                    }
                                }

                                break;
                            }
                    }
                }

                else
                    Variables.SENSE_COLOR = new float[] { 0.00F, 0.75F, 1.00F };

                // Controller Handling for the DualSense protocol. Latency -> BT : 0.5ms, USB: 250ms
                // For some reason, laggy as shit on USB. I was not able to determine why yet.

                if (Variables.CONTROLLER_SENSE != null)
                {
                    var _outState = Variables.CONTROLLER_SENSE.OutputState;

                    _outState.LightbarBehavior = LightbarBehavior.CustomColor;
                    _outState.LightbarColor = new LightbarColor(Variables.SENSE_COLOR[0], Variables.SENSE_COLOR[1], Variables.SENSE_COLOR[2]);

                    // Both rumbles will play the highest of both rumble inputs.
                    // The KH port normally only plays the BIG_MOTOR, cancelling about half of the rumble.

                    _outState.RightRumble = Math.Max(BIG_MOTOR, SMALL_MOTOR) / 255F;
                    _outState.LeftRumble = Math.Max(BIG_MOTOR, SMALL_MOTOR) / 255F;

                    // This section is highly experimental and is subject to change.

                    if (Variables.DUALSENSE_TRIGGERS)
                    {
                        _outState.L2Effect = new TriggerEffect.Section(0.3F, 0.4F);
                        _outState.R2Effect = new TriggerEffect.Section(0.3F, 0.4F);
                    }

                    // This section may no longer be necessary.
                    // I am not taking my chances yet.

                    PAST_BIG = BIG_MOTOR;
                    TICK_COUNT += 1;

                    if (TICK_COUNT >= 400 && PAST_BIG == BIG_MOTOR)
                    {
                        _outState.RightRumble = 0F;
                        _outState.LeftRumble = 0F;
                    }

                    Variables.CONTROLLER_SENSE.OutputState = _outState;
                    Variables.CONTROLLER_SENSE.WriteOnce();
                }

                // Controller Handling for the DualSense protocol. Latency -> 10ms on both.

                else if (Variables.CONTROLLER_SHOCK != null)
                {
                    Variables.CONTROLLER_SHOCK.setLedColor((byte)(Variables.SENSE_COLOR[0] * 0xFF), (byte)(Variables.SENSE_COLOR[1] * 0xFF), (byte)(Variables.SENSE_COLOR[2] * 0xFF));

                    Variables.CONTROLLER_SHOCK.BigRumble = (byte)Math.Max(BIG_MOTOR, SMALL_MOTOR);
                    Variables.CONTROLLER_SHOCK.SmallRumble = (byte)Math.Max(BIG_MOTOR, SMALL_MOTOR);

                    PAST_BIG = BIG_MOTOR;
                    TICK_COUNT += 1;

                    if (TICK_COUNT >= 400 && PAST_BIG == BIG_MOTOR)
                    {
                        Variables.CONTROLLER_SHOCK.BigRumble = 0x00;
                        Variables.CONTROLLER_SHOCK.SmallRumble = 0x00;
                    }

                    Variables.CONTROLLER_SHOCK.sendOutputReport();
                }
            }

            catch (System.AggregateException)
            { }

        }

        /// <summary>
        /// Handles the reconnect logic of external controllers.
        /// </summary>
        public static void ControllerReconnect()
        {
            if (!Variables.CONTROLLER_FOUND && Variables.DUALSENSE_TOGGLE)
            {
                if (Variables.CONTROLLER_SENSE == null)
                {
                    var _devices = DualSense.EnumerateControllers();

                    if (_devices.Count() != 0x00)
                    {
                        Variables.CONTROLLER_SENSE = _devices.First();
                        Variables.CONTROLLER_SENSE.Acquire();

                        Helpers.Log("DualSense Controller has been reconnected.", 0);

                        if (Variables.CONTROLLER_SENSE.IoMode == IoMode.Bluetooth)
                            Helpers.Log("DualSense is connected via Bluetooth", 0);

                        else
                            Helpers.Log("DualSense is connected via USB.", 0);

                        Variables.CONTROLLER_FOUND = true;
                    }
                }

                if (Variables.CONTROLLER_SHOCK == null)
                {
                    var _shockList = HidDevices.Enumerate(0x054C, 0x05C4);

                    if (_shockList.Count() != 0x00)
                    {
                        var _hidDevice = _shockList.First();
                        _hidDevice.OpenDevice(false);

                        Helpers.Log("Found a DualShock 4 Controller! Initializing...", 0);
                        Variables.CONTROLLER_SHOCK = new DualShock(_hidDevice, 0);

                        if (Variables.CONTROLLER_SHOCK.Device.Capabilities.InputReportByteLength == 64)
                            Helpers.Log("DualShock 4 is connected via USB.", 0);

                        else
                            Helpers.Log("DualShock 4 is connected via Bluetooth.", 0);

                        Variables.CONTROLLER_FOUND = true;
                    }
                }
            }
        }

        /// <summary>
        /// Handles the disconnect logic of external controllers.
        /// </summary>
        public static void ControllerDisconnect()
        {
            if (Operations.CheckTitle() && !DEBOUNCE[10])
                DEBOUNCE[10] = true;

            if (!Operations.CheckTitle())
            {
                if (DEBOUNCE[10])
                {
                    DEBOUNCE[10] = false;
                    DEBOUNCE[9] = true;
                }

                else
                {
                    var _menuRead = Hypervisor.Read<int>(Variables.ADDR_MenuType);
                    var _loadRead = Hypervisor.Read<int>(Variables.ADDR_LoadFlag);

                    var _checkMenu = Hypervisor.Read<int>(Variables.ADDR_MenuActive);

                    if (_checkMenu == 0 && _menuRead != 0x03 && _loadRead == 0x01 && !DEBOUNCE[9])
                    {
                        Variables.SharpHook[(IntPtr)0x2E23A0].Execute(BSharpConvention.MicrosoftX64, 1, 2);

                        Thread.Sleep(500);
                        Additions.ShowSLWarning(0x6010);

                        DEBOUNCE[9] = true;
                    }
                }
            }
        }

        /// <summary>
        /// Execute the Input Logic.
        /// </summary>
        public static void ExecuteInput()
        {
            if (Variables.CONTROLLER_FOUND)
            {
                ControllerInput();

                if (DEBOUNCE[9])
                    DEBOUNCE[9] = false;
            }

            else
            {
                ControllerReconnect();
                ControllerDisconnect();
                Thread.Sleep(2000);
            }
        }

        /// <summary>
        /// Execute the Output Logic.
        /// </summary>
        public static void ExecuteOutput()
        {
            if (Variables.CONTROLLER_FOUND)
                ControllerOutput();

            else
                Thread.Sleep(2000);
        }
    }
}