
using System.Collections.ObjectModel;
using System.Collections.Specialized;

using ReFined.Common;
using ReFined.KH2.Information;

namespace ReFined.KH2.Menus
{
    public class Intro
    {
        public class Entry
        {
            public uint Count;
            public uint Flair;
            public uint Title;
            public List<uint> Buttons;
            public List<uint> Descriptions;

            public uint[] Export()
            {
                var _returnList = new List<uint>()
                {
                    Count,
                    Flair,
                    Title 
                };

                for (int i = 0; i < 4; i++)
                {
                    if (i < Count)
                        _returnList.Add(Buttons[i]);

                    else
                        _returnList.Add(0xFFFFFFFF);
                }

                for (int i = 0; i < 4; i++)
                {
                    if (i < Count)
                        _returnList.Add(Descriptions[i]);

                    else
                        _returnList.Add(0xFFFFFFFF);
                }

                return _returnList.ToArray();
            }
        }

        public static ObservableCollection<Entry> Children;

        public Intro()
        {
            Terminal.Log("Initializing Menu: Intro, with Default Parameters...", 0);

            var _entAutosave = new Entry()
            {
                Count = 3,
                Flair = 0xE005,
                Title = 0xFFFFFFFF,

                Buttons = new List<uint>()
                {
                    0x81C1,
                    0x81C2,
                    0x81C3
                },

                Descriptions = new List<uint>()
                {
                    0x81CD,
                    0x81CE,
                    0x81CF
                }
            };
            var _entVibration = new Entry()
            {
                Count = 2,
                Flair = 0xC337,
                Title = 0xC381,

                Buttons = new List<uint>()
                {
                    0xC338,
                    0xC339
                },

                Descriptions = new List<uint>()
                {
                    0xC33A,
                    0xC33B
                }
            };
            var _entDifficulty = new Entry()
            {
                Count = 4,
                Flair = 0xC330,
                Title = 0xC380,

                Buttons = new List<uint>()
                {
                    0xC331,
                    0xC332,
                    0xC333,
                    0xCE33
                },

                Descriptions = new List<uint>()
                {
                    0xC334,
                    0xC335,
                    0xC336,
                    0xCE34
                }
            };
            var _entController = new Entry()
            {
                Count = 3,
                Flair = 0xE009,
                Title = 0xFFFFFFFF,

                Buttons = new List<uint>()
                {
                    0x81CA,
                    0x81CB,
                    0x81CC,
                },

                Descriptions = new List<uint>()
                {
                    0x81D6,
                    0x81D7,
                    0x81D8
                }
            };

            Children = new ObservableCollection<Entry>()
            {
                _entDifficulty,
                _entVibration,
                _entAutosave,
                _entController
            };

            Children.CollectionChanged += Submit;

            Terminal.Log("Menu initialized!", 0);

            Submit();
        }

        public void Submit(object? sender = null, NotifyCollectionChangedEventArgs e = null)
        {
            if (sender != null)
                Terminal.Log("Inserting New Entry to Intro...", 0);

            else
                Terminal.Log("Submitting Menu: Intro - " + Children.Count + " Entries detected!", 0);

            for (int i = 0; i < Children.Count; i++)
            {
                var _childExport = Children[i].Export();
                var _childWrite = _childExport.SelectMany(BitConverter.GetBytes).ToArray();

                Hypervisor.WriteArray(Variables.ADDR_NewGameMenu + (ulong)(i * 0x2C), _childWrite);
            }

            byte _lastIndex = (byte)(Children.Count - 1);

            Hypervisor.Write(0x2B79D7, (byte)Children.Count);
            Hypervisor.Write(0x2B7B35, (byte)Children.Count);
            Hypervisor.Write(0x2B7E71, (byte)Children.Count);
            Hypervisor.Write(0x2B802F, (byte)Children.Count);

            Hypervisor.Write(0x2B79CE, _lastIndex);
            Hypervisor.Write(0x2B8D07, _lastIndex);
            Hypervisor.Write(0x2B912B, _lastIndex);
            Hypervisor.Write(0x2B91B1, _lastIndex);

            if (sender == null)
            Terminal.Log("Menu submitted successfully!", 0);
        }
    }
}