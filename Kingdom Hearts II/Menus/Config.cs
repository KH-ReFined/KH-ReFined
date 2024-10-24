﻿
using System.Collections.ObjectModel;
using System.Collections.Specialized;

using ReFined.Common;
using ReFined.KH2.Information;
using ReFined.Libraries;

namespace ReFined.KH2.Menus
{
    public class Config
    {
        public class Entry
        {
            public ushort Count;
            public ushort Title;
            public List<ushort> Buttons;
            public List<ushort> Descriptions;

            public ushort[] Export()
            {
                var _returnList = new List<ushort>()
                {
                    Count,
                    Title
                };

                for (int i = 0; i < 4; i++)
                {
                    if (i < Buttons.Count)
                        _returnList.Add(Buttons[i]);

                    else
                        _returnList.Add(0x0000);
                }

                for (int i = 0; i < 4; i++)
                {
                    if (i < Buttons.Count)
                        _returnList.Add(Descriptions[i]);

                    else
                        _returnList.Add(0x0000);
                }

                return _returnList.ToArray();
            }
        }

        public ObservableCollection<Entry> Children;

        public Config()
        {
            Terminal.Log("Initializing Menu: Config, with Default Parameters...", 0);

            var _entFieldCam = new Entry()
            {
                Count = 2,
                Title = 0xB717,
                
                Buttons = new List<ushort>()
                {
                    0xB71E,
                    0xB71F
                },

                Descriptions = new List<ushort>()
                {
                    0xB720,
                    0xB721
                }
            };
            var _entRightStick = new Entry()
            {
                Count = 2,
                Title = 0xB718,

                Buttons = new List<ushort>()
                {
                    0xB722,
                    0xB723
                },

                Descriptions = new List<ushort>()
                {
                    0xB724,
                    0xB725
                }
            };
            var _entCameraV = new Entry()
            {
                Count = 2,
                Title = 0xC2F5,
                
                Buttons = new List<ushort>()
                {
                    0xC2F8,
                    0xC2F9
                },

                Descriptions = new List<ushort>()
                {
                    0xC2FA, 
                    0xC2FB
                }
            };
            var _entCameraH = new Entry()
            {
                Count = 2,
                Title = 0xC2F6,
                
                Buttons = new List<ushort>()
                {
                    0xC2FC,
                    0xC2FD
                },

                Descriptions = new List<ushort>()
                {
                    0xC2FE, 
                    0xC2FF
                }
            };
            var _entSummonFX = new Entry()
            {
                Count = 3,
                Title = 0xC2F7,
                

                Buttons = new List<ushort>()
                {
                    0xC302,
                    0xC300,
                    0xC301
                },

                Descriptions = new List<ushort>()
                {
                    0xC305, 
                    0xC303, 
                    0xC304
                }
            };
            var _entNavigation = new Entry()
            {
                Count = 2,
                Title = 0xB719,
                
                Buttons = new List<ushort>()
                {
                    0xB726,
                    0xB727
                },

                Descriptions = new List<ushort>()
                {
                    0xB728, 
                    0xB729
                }
            };
            var _entAutosave = new Entry()
            {
                Count = 3,
                Title = 0x01BC,
                
                Buttons = new List<ushort>()
                {
                    0x01C1,
                    0x01C2,
                    0x01C3
                },

                Descriptions = new List<ushort>()
                {
                    0x01CD, 
                    0x01CE, 
                    0x01CF
                }
            };
            var _entController = new Entry()
            {
                Count = 2,
                Title = 0x01C0,
                
                Buttons = new List<ushort>()
                {
                    0x01CA,
                    0x01CC
                },

                Descriptions = new List<ushort>()
                {
                    0x01D6, 
                    0x01D8
                }
            };
            var _entVibration = new Entry()
            {
                Count = 2,
                Title = 0xB71A,
                
                Buttons = new List<ushort>()
                {
                    0xB72A,
                    0xB752
                },

                Descriptions = new List<ushort>()
                {
                    0xB72C, 
                    0xB72D
                }
            };
            var _entCommandKH2 = new Entry()
            {
                Count = 3,
                Title = 0xB71C,

                Buttons = new List<ushort>()
                {
                    0xB734,
                    0xB735,
                    0x01CB
                },

                Descriptions = new List<ushort>()
                {
                    0xB736, 
                    0xB737,
                    0x01D7
                }
            };

            var _entDifficulty = new Entry()
            {
                Count = 1,
                Title = 0xB71D,

                Buttons = new List<ushort>()
                {
                    0xB738,
                    0xB739,
                    0xB73A,
                    0xCE30
                },

                Descriptions = new List<ushort>()
                {
                    0xB73B, 
                    0xB73C, 
                    0xB73D, 
                    0xCE31
                }
            };
           
            Children = new ObservableCollection<Entry>()
            {
                _entFieldCam,
                _entRightStick,
                _entCameraV,
                _entCameraH,
                _entSummonFX,
                _entNavigation,
                _entAutosave,
                _entController,
                _entVibration,
                _entCommandKH2,
                _entDifficulty
            };  
                
            Children.CollectionChanged += Submit;
                
            Terminal.Log("Menu initialized!", 0);
                
            Submit();
        }       
        public void Submit(object? sender = null, NotifyCollectionChangedEventArgs e = null)
        {
            if (sender != null)
                Terminal.Log("Inserting New Entry to Config...", 0);

            else
                Terminal.Log("Submitting Menu: Config - " + Children.Count + " Entries detected!", 0);

            for (int i = 0; i < Children.Count; i++)
            {
                var _childExport = Children[i].Export();
                var _childWrite = _childExport.SelectMany(BitConverter.GetBytes).ToArray();
                Hypervisor.Write(Variables.ADDR_ConfigMenu + (ulong)(i * 0x14), _childWrite);
            }

            byte _lastIndex = (byte)(Children.Count - 1);

            byte _pageFlag = 0x20;
            byte _countFlag = (byte)Children.Count;

            if (Children.Count >= 9)
            {
                _pageFlag = 0x24;
                _countFlag = 0x09;
            }

            var _diffPointer = (0x820004U + 0x14U * _lastIndex);

            // List Redirectors.
            Hypervisor.RedirectInstruction(Variables.HFIX_ConfigOffsets[0] + 0x031, 0x820000);
            Hypervisor.RedirectInstruction(Variables.HFIX_ConfigOffsets[0] + 0x047, 0x820000);
            Hypervisor.RedirectInstruction(Variables.HFIX_ConfigOffsets[2] + 0x27B, 0x820000);
            Hypervisor.RedirectInstruction(Variables.HFIX_ConfigOffsets[2] + 0x291, 0x820000);
            Hypervisor.RedirectInstruction(Variables.HFIX_ConfigOffsets[4] + 0x2C3, 0x820000);
            Hypervisor.RedirectInstruction(Variables.HFIX_ConfigOffsets[5] + 0x078, 0x820000);
            Hypervisor.RedirectInstruction(Variables.HFIX_ConfigOffsets[5] + 0x0BC, 0x820000);
            Hypervisor.RedirectInstruction(Variables.HFIX_ConfigOffsets[5] + 0x1DA, 0x820000);

            // Description Redirectors.
            Hypervisor.RedirectInstruction(Variables.HFIX_ConfigOffsets[2] + 0x323, 0x82000C);
            Hypervisor.RedirectInstruction(Variables.HFIX_ConfigOffsets[4] + 0x487, 0x82000C);

            // Write the count flag.
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[1] + 0x127 + 0x02, _countFlag);
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[1] + 0x24B + 0x03, _countFlag);
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[4] + 0x18E + 0x01, _countFlag);
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[5] + 0x025 + 0x02, _countFlag);

            // Write the count of the config menu.
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[3] + 0x069 + 0x01, (byte)Children.Count);
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[4] + 0x16F + 0x01, (byte)Children.Count);

            // Write the last index of the menu.
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[1] + 0x150 + 0x03, _lastIndex);
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[1] + 0x2E1 + 0x02, _lastIndex);
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[2] + 0x34F + 0x02, _lastIndex);
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[3] + 0x01E + 0x02, _lastIndex);
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[4] + 0x1FA + 0x02, _lastIndex);
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[5] + 0x1D5 + 0x04, _lastIndex);
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[5] + 0x2AB + 0x02, _lastIndex);

            // Redirect Difficulty Option selectors.
            Hypervisor.RedirectInstruction(Variables.HFIX_ConfigOffsets[5] + 0x0F5, _diffPointer);
            Hypervisor.RedirectInstruction(Variables.HFIX_ConfigOffsets[5] + 0x1BB, _diffPointer);
            Hypervisor.RedirectInstruction(Variables.HFIX_ConfigOffsets[5] + 0x1FC, _diffPointer); 
            Hypervisor.RedirectInstruction(Variables.HFIX_ConfigOffsets[5] + 0x244, _diffPointer);

            // Must always point to Command Menu Selection.
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[0] + 0x05E + 0x03, (byte)(_lastIndex - 1));
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[2] + 0x2A8 + 0x03, (byte)(_lastIndex - 1));

            // Write the page flags.
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[1] + 0x325 + 0x02, _pageFlag);
            Hypervisor.Write(Variables.HFIX_ConfigOffsets[5] + 0x2EF + 0x02, _pageFlag);

            Hypervisor.Write<byte>(0x365545, 0x00);

            if (sender == null)
                Terminal.Log("Menu has been submitted successfully!", 0);
        }
    }
}