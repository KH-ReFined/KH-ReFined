#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "message.h"
#include "sprite.h"
#include "select.h"
#include "menuutil.h"
#include "sequence.h"
#include "hookintro.h"
#include "memorymgr.h"
#include "panacea_alloc.h"

extern "C"
{

    namespace Title
    {
        class DLL_EXPORT NewGame
        {
        public:
            static char* SetupResult()
            {
                auto _sqdMain = *m_SeqTbl + 0x440;
                Tz::MenuUtil::CreateMess(*m_SeqTbl + 0x440, *m_pri, 0, 0, *m_Lay, 28, 29, UINT32_MAX, 0, 0);

                *reinterpret_cast<uint32_t*>(_sqdMain + 0x010) &= ~0x10;
                *reinterpret_cast<uint32_t*>(_sqdMain + 0x010) &= ~0x08;

                *reinterpret_cast<uint32_t*>(_sqdMain + 0x214) = 2;

                *reinterpret_cast<float*>(_sqdMain + 0x200) = YI::SEQUENCE::GetParamX(_sqdMain + 0x20);
                *reinterpret_cast<float*>(_sqdMain + 0x204) = YI::SEQUENCE::GetParamY(_sqdMain + 0x20);

                int _procElements = 0x00;
                vector<Tz::HookIntro::Entry> _procEntries;
                vector<int> _procSelect;

                for (int i = 0x00; i < Tz::HookIntro::Entries.size(); i++)
                {
                    auto _selectPtr = YS::PANACEA_ALLOC::Get("INTRO_MEMORY") + 0x400 + (0x04 * i);
                    if (Tz::HookIntro::Entries[i].Flair != UINT16_MAX)
                    {
                        if (!Tz::HookIntro::Entries[i].SubEntry)
                        {
                            _procEntries.push_back(Tz::HookIntro::Entries[i]);
                            _procSelect.push_back(*_selectPtr);
                        }

                        else if (find_if(Tz::HookIntro::Entries.begin(), Tz::HookIntro::Entries.end(), [i](const Tz::HookIntro::Entry _fetchEntry) { return _fetchEntry.Title == Tz::HookIntro::Entries[i].SubEntry->Title; }) == Tz::HookIntro::Entries.end())
                        {
                            _procEntries.push_back(Tz::HookIntro::Entries[i]);
                            _procSelect.push_back(*_selectPtr);
                        }
                    }
                }

                // Create and render option selections.
                for (int i = 0x00; i < _procEntries.size(); i++)
                {
                    auto _fetchIntro = YS::PANACEA_ALLOC::Get("INTRO_MEMORY");

                    auto _rectSqd = *m_SeqTbl + 0x660 + (0x220 * _procElements);
                    Tz::MenuUtil::CreateMess(_rectSqd, *m_pri, 0, 0, *m_Lay, 26, UINT32_MAX, UINT32_MAX, 0, 0);

                    *reinterpret_cast<uint32_t*>(_rectSqd + 0x010) &= ~0x10;
                    *reinterpret_cast<uint32_t*>(_rectSqd + 0x010) &= ~0x08;
                    *reinterpret_cast<uint32_t*>(_rectSqd + 0x010) &= ~0x08;

                    *reinterpret_cast<char**>(_rectSqd + 0x01F8) = YS::MESSAGE::GetData(_procEntries[i].Flair);
                    Tz::MenuUtil::SetSprtParent(_rectSqd, _sqdMain);

                    auto _textSqd = *m_SeqTbl + (0x660 + (0x220 * _procEntries.size()) + 0x220 * _procElements);
                    Tz::MenuUtil::CreateMess(_textSqd, *m_pri, 0, 0, *m_Lay, 13, UINT32_MAX, UINT32_MAX, 0, 0);

                    *reinterpret_cast<uint32_t*>(_textSqd + 0x010) &= ~0x10;
                    *reinterpret_cast<char**>(_textSqd + 0x1F8) = YS::MESSAGE::GetData(_procEntries[i].Buttons[_procSelect[i]]);

                    Tz::MenuUtil::SetSprtParent(_textSqd, *m_SeqTbl + 0x660 + 0x220 * _procElements);

                    *reinterpret_cast<int*>(_textSqd + 0x1C8) = YI::SEQUENCE::GetParamX(_rectSqd + 0x20);
                    *reinterpret_cast<int*>(_textSqd + 0x1CC) = YI::SEQUENCE::GetParamY(_rectSqd + 0x20);

                    _procElements++;
                }

                // Set offsets of option selections.
                for (int i = 0x00; i < _procElements; i++)
                {
                    auto _spaceParam = _procElements <= 0x04 ? 30 : (_procElements <= 0x06 ? 25 : 21);

                    auto _offsetSqd = 0x660 + (0x220 * i);
                    auto _fetchSeqd = *m_SeqTbl + _offsetSqd;

                    *reinterpret_cast<int*>(_fetchSeqd + 0x1C8) = 0x00;
                    *reinterpret_cast<int*>(_fetchSeqd + 0x1CC) = -3 + (_spaceParam * (_procElements * -0.5)) + _spaceParam * i;
                }

                // Create and render the buttons.
                for (int i = 0x00; i < 0x02; i++)
                {
                    Tz::Select::SetItem(*m_SelPtr, i, 0, 0, *m_Lay, UINT32_MAX, reinterpret_cast<uint16_t*>(s_Plate2Seq), reinterpret_cast<uint16_t*>(s_Plate2Seq + 0x08));

                    auto _fetchItemPtr = *reinterpret_cast<char**>(*reinterpret_cast<char**>(*m_SelPtr + 0x28) + i * 0x08);

                    *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x10;
                    *reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x08;

                    *reinterpret_cast<int*>(_fetchItemPtr + 0x214) = 0x02;

                    *reinterpret_cast<int*>(_fetchItemPtr + 0x1C8) = i * YI::SEQUENCE::GetParamCr(_fetchItemPtr + 0x20);
                    *reinterpret_cast<int*>(_fetchItemPtr + 0x1CC) = 0x00;

                    Tz::MenuUtil::SetSprtParent(_fetchItemPtr, _sqdMain);

                    *reinterpret_cast<char**>(_fetchItemPtr + 0x1F8) = YS::MESSAGE::GetData(0xC384 - i);
                    *reinterpret_cast<float*>(_fetchItemPtr + 0x200) = (float)YI::SEQUENCE::GetParamEx(_fetchItemPtr + 0x20);
                }

                Tz::Select::SetCurrent(*m_SelPtr, 0x01, true, 0x00, 0x02);

                auto _menuPtr16 = reinterpret_cast<uint16_t*>(*m_MenuPtr);

                auto _selectResult = _menuPtr16[0x0B];
                auto _selectCalc = static_cast<uint32_t>(_menuPtr16[0x00] + _menuPtr16[0x09]);

                auto _selectPos = _selectCalc;

                if (_selectResult)
                    _selectPos = _selectCalc % _selectResult;

                auto _mInfo = reinterpret_cast<char**>(reinterpret_cast<char*>(m_SeqTbl) - 0x08);
                auto _fetchText = 0xC32F;

                if (**_mInfo <= 1)
                    _fetchText = *reinterpret_cast<int*>(YS::PANACEA_ALLOC::Get("INTRO_MEMORY") + 0x200 + 0x0B * **_mInfo + _selectPos * 0x04 + 0x1C);

                *reinterpret_cast<char**>(*m_SeqTbl + 0x638) = YS::MESSAGE::GetData(_fetchText);
                *reinterpret_cast<int*>(*_mInfo + 0x04) = 0x00;

                return *_mInfo;
            }

            static char* SetupConfig()
            {
                auto _sqdMain = *m_SeqTbl + 0x440;
                auto _mInfo = reinterpret_cast<char**>(reinterpret_cast<char*>(m_SeqTbl) - 0x08);

                return *_mInfo;
            }

            static inline char** m_SeqTbl = FetchRelativePointer<char**>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D", "xxxxxxxxxxxxxxxxxxxxxxx", 0x17);
            static inline int* m_pri = FetchRelativePointer<int*>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D", "xxxxxxxxxxxxxxxxxxxxxxx", 0x2D);
            static inline char** m_Lay = FetchRelativePointer<char**>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D", "xxxxxxxxxxxxxxxxxxxxxxx", 0x20);
            static inline char** m_SelPtr  = FetchRelativePointer<char**>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D", "xxxxxxxxxxxxxxxxxxxxxxx", 0x273);
            static inline char** m_MenuPtr = reinterpret_cast<char**>(FetchRelativePointer<char*>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D", "xxxxxxxxxxxxxxxxxxxxxxx", 0x17) - 0x28);
            static inline char* s_Plate2Seq = FetchRelativePointer<char*>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D", "xxxxxxxxxxxxxxxxxxxxxxx", 0x25D);

            private:
                static bool _init()
                {
                    RedirectFunction("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D", "xxxxxxxxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(SetupResult), 0x41E);
                    return true;
                }

            #if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
            static inline bool _doInit = _init();
            #endif
        };
    }
}
