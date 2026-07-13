#pragma once

#include "menu.h"
#include "area.h"
#include "gauge.h"
#include "title.h"
#include "steam.h"
#include "cmdata.h"
#include "party.h"
#include "binarc.h"
#include "message.h"
#include "cmconfig.h"
#include "sequence.h"
#include "softreset.h"
#include "memorymgr.h"
#include "information.h"

#include "panacea_alloc.h"

using namespace std;

namespace Tz
{
	class HookConfig
	{
	protected:
		static inline vector<char*> CONFIG_OFFSETS = vector<char*>
		{
			FindSignature<char*>("\x40\x53\x48\x83\xEC\x20\x0F\xB6\xD9\x48\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x4C\x8B\x1D\x00\x00\x00\x00", "xxxxxxxxxxxx????x????xxx????"),
			FindSignature<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x80\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x70\xE8\x00\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxx????xxx????"),
			FindSignature<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\x48\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xF8\xE8\x00\x00\x00\x00\x8B\xF0\x83\xF8\x02\x0F\x85\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xD8\xE8\x00\x00\x00\x00\x84\xC0\x75\x59\x48\x8B\x0D\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxx????x????xxx????xxxxxxx????x????xxx????xxxxxxx????"),
			FindSignature<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xFA\x8B\xD9\xE8\x00\x00\x00\x00\x8D\x0C\x3B\x44\x8D\x04\x9D\x00\x00\x00\x00", "xxxxxxxxxxxxxxx????xxxxxxxxxxx"),
			FindSignature<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\xE8\x00\x00\x00\x00\x45\x33\xC0\x33\xC9\x41\x8D\x50\xFF\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxx????xxxxxxxxxx????"),
			FindSignature<char*>("\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x58\xE8\x00\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00\x4C\x8B\xF8\xE8\x00\x00\x00\x00\x41\xBD\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxx????xxx????xxxx????xx????"),
			FindSignature<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x33\xDB\x48\x8D\x35\x00\x00\x00\x00\x33\xFF\x66\x0F\x1F\x44\x00\x00\x83\xFB\x06\x0F\x87\xB5\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxx")
		};
		
		static inline char* COMMAND_MENU_ADDR = FetchRelativePointer<char*>("\x40\x56\x48\x83\xEC\x30\x8B\x35\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x48\x89\x5C\x24\x40\x48\x89\x7C\x24\x48", "xxxxxxxx????x????xxxx????x????xxxx????xxxxxxxxxx", 0x50);
		static inline bool* COMMAND_FLAG_ADDR = FetchRelativePointer<bool*>("\x40\x56\x48\x83\xEC\x30\x8B\x35\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\x00\x00\x00\x00\x48\x89\x5C\x24\x40\x48\x89\x7C\x24\x48", "xxxxxxxx????x????xxxx????x????xxxx????xxxxxxxxxx", 0x5D) + 0x04;
		static inline char* COMMAND_TYPE_ADDR = FetchRelativePointer<char*>("\x48\x83\xEC\x28\x48\x8D\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\x48\x83\xC4\x28\xE9\x00\x00\x00\x00\xCC\xCC\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\xC3", "xxxxxxx????x????xxx????xxx????xxx????xxxxx????xxxxx????xxx????x", 0x1A);

		static inline char* CAMP_OFFSET = FindSignature<char*>("\x48\x8B\xC4\x48\x81\xEC\x88\x00\x00\x00\x48\x89\x58\x18\xBA\x02\x00\x00\x00\x48\x89\x68\xF8\x48\x89\x70\xF0", "xxxxxxxxxxxxxxxxxxxxxxxxxxx");
		static inline char* CAMPINIT_OFFSET = FindSignature<char*>("\x66\x44\x89\x35\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x44\x88\x35\x00\x00\x00\x00\x0F\x95\x05\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x4C\x8D\x05\x00\x00\x00\x00\xC7\x44\x24\x30\x8C\x00\x00\x00", "xxxx????x????xxxxx????xxx????x????xxx????xxxxxxxx");

		static inline char* MENUSELECT_OFFSET = FindSignature<char*>("\x40\x55\x53\x48\x8D\x6C\x24\xB1\x48\x81\xEC\x98\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxx????");

		static inline char* (*MENUSEQ_POINTER)() = FetchRelativePointer<char* (*)()>(reinterpret_cast<char*>(Tz::CmConfig::UpdateList), 0x12);
		
		static inline bool FORCE_UPDATE;

	public:
		static inline bool CONFIG_INIT;
		static inline bool CONFIG_FETCH;
		static inline bool CONFIG_FIRST_INIT;
		static inline uint16_t SETTING_MEMORY;

		struct Entry
		{
			public:
				uint16_t Count;
				uint16_t Title;
				vector<uint16_t> Buttons;
				vector<uint16_t> Descriptions;
				vector<uint16_t> Toggles;
				uint16_t SubToggle;
				Entry* SubEntry;
		};

		static inline vector<Entry> Entries = vector<Tz::HookConfig::Entry>
		{
			Tz::HookConfig::Entry { 0x02, 0xB717, vector<uint16_t> { 0xB71E, 0xB71F },		   vector<uint16_t> { 0xB720, 0xB721 },			vector<uint16_t> { 0x0000, 0x0010 },		 0x0000, nullptr },
			Tz::HookConfig::Entry { 0x02, 0xB718, vector<uint16_t> { 0xB722, 0xB723 },		   vector<uint16_t> { 0xB724, 0xB725 },			vector<uint16_t> { 0x0000, 0x0020 },		 0x0000, nullptr },
			Tz::HookConfig::Entry { 0x02, 0xC2F5, vector<uint16_t> { 0xC2F8, 0xC2F9 },		   vector<uint16_t> { 0xC2FA, 0xC2FB },			vector<uint16_t> { 0x0000, 0x0100 },	 	 0x0000, nullptr },
			Tz::HookConfig::Entry { 0x02, 0xC2F6, vector<uint16_t> { 0xC2FC, 0xC2FD },		   vector<uint16_t> { 0xC2FE, 0xC2FF },			vector<uint16_t> { 0x0000, 0x0080 },		 0x0000, nullptr },
			Tz::HookConfig::Entry { 0x03, 0xC2F7, vector<uint16_t> { 0xC302, 0xC300, 0xC301 }, vector<uint16_t> { 0xC305, 0xC303, 0xC304 }, vector<uint16_t> { 0x0200, 0x0400, 0x0000},  0x0000, nullptr },
			Tz::HookConfig::Entry { 0x02, 0xB719, vector<uint16_t> { 0xB726, 0xB727 },		   vector<uint16_t> { 0xB728, 0xB729 },			vector<uint16_t> { 0x0008, 0x0000 },		 0x0000, nullptr },
			#ifndef BUILD_ARCHIPELAGO_LITE
			Tz::HookConfig::Entry { 0x03, 0x5704, vector<uint16_t> { 0x5705, 0x5707, 0x5709 }, vector<uint16_t> { 0x5706, 0x5708, 0x570A }, vector<uint16_t> { 0x0004, 0x0002, 0x0000 }, 0x0000, nullptr },
			#endif
			#if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
			Tz::HookConfig::Entry { 0x02, 0x5722, vector<uint16_t> { 0x5723, 0x5725 },		   vector<uint16_t> { 0x5724, 0x5726 },			vector<uint16_t> { 0x2000, 0x0000 },		 0x0000, nullptr },
			Tz::HookConfig::Entry { 0x03, 0x5754, vector<uint16_t> { 0x5755, 0x5756, 0x5757 }, vector<uint16_t> { 0x5758, 0x5759, 0x575A }, vector<uint16_t> { 0x4000, 0x8000, 0x0000 }, 0x0000, nullptr },
			Tz::HookConfig::Entry { 0x02, 0xB71A, vector<uint16_t> { 0xB72A, 0xB752 },		   vector<uint16_t> { 0xB72C, 0xB72D },			vector<uint16_t> { 0x0001, 0x0000 },	     0x0000, nullptr },
			Tz::HookConfig::Entry { 0x03, 0xB71C, vector<uint16_t> { 0xB734, 0x572C, 0xB735 }, vector<uint16_t> { 0xB736, 0x572D, 0xB737 }, vector<uint16_t> { 0x0000, 0x0800, 0x0040 }, 0x0000, nullptr },
			#else
			Tz::HookConfig::Entry { 0x02, 0xB71A, vector<uint16_t> { 0xB72A, 0xB752 },		   vector<uint16_t> { 0xB72C, 0xB72D },			vector<uint16_t> { 0x0001, 0x0000 },	     0x0000, nullptr },
			Tz::HookConfig::Entry { 0x02, 0xB71C, vector<uint16_t> { 0xB734, 0xB735 }, vector<uint16_t> { 0xB736, 0xB737 }, vector<uint16_t> { 0x0000, 0x0040 }, 0x0000, nullptr },
			#endif


			Tz::HookConfig::Entry { 0x01, 0xB71D, vector<uint16_t> { 0xB738, 0xB739, 0xB73A, 0xCE30 }, vector<uint16_t> { 0xB73B, 0xB73C, 0xB73D, 0xCE31 }, vector<uint16_t> { 0x0000, 0x0000, 0x0000, 0x0000 }, 0x0000, nullptr }
		};

		static void Submit()
		{
			auto _configMemory = YS::PANACEA_ALLOC::Get("CONFIG_MEMORY");

			if (!_configMemory)
			{
				YS::PANACEA_ALLOC::Allocate("CONFIG_MEMORY", 0x200);
				_configMemory = YS::PANACEA_ALLOC::Get("CONFIG_MEMORY");
			}

			for (int i = 0; i < Entries.size(); i++)
			{
				auto _currentEntry = Entries[i];

				memcpy(_configMemory + (0x14 * i), &_currentEntry.Count, 0x02);
				memcpy(_configMemory + (0x14 * i) + 0x02, &_currentEntry.Title, 0x02);

				for (int z = 0; z < (_currentEntry.Count == 0x01 ? 0x04 : _currentEntry.Count); z++)
				{
					memcpy(_configMemory + (0x14 * i) + (0x02 * z) + 0x04, &_currentEntry.Buttons[z], 0x02);
					memcpy(_configMemory + (0x14 * i) + (0x02 * z) + 0x0C, &_currentEntry.Descriptions[z], 0x02);
				}
			}

			uint8_t _childCount = Entries.size();
			uint8_t _lastIndex = Entries.size() - 0x01;
			uint8_t _commandIndex = Entries.size() - 0x02;

			uint8_t _pageFlag = 0x20;
			uint8_t _countFlag = _childCount;

			if (_childCount >= 0x09)
			{
				_pageFlag = 0x24;
				_countFlag = 0x09;
			}

			char* _difficultyPtr = _configMemory + 0x04 + 0x14U * _lastIndex;
			vector<uint8_t> _preventUpdateInst{ 0xE9, 0xDC, 0x00, 0x00, 0x00, 0x90 };

			RedirectLEA(CONFIG_OFFSETS[0] + 0x031, _configMemory);
			RedirectLEA(CONFIG_OFFSETS[0] + 0x047, _configMemory);
			RedirectLEA(CONFIG_OFFSETS[2] + 0x27B, _configMemory);
			RedirectLEA(CONFIG_OFFSETS[2] + 0x291, _configMemory);
			RedirectLEA(CONFIG_OFFSETS[4] + 0x2C3, _configMemory);
			RedirectLEA(CONFIG_OFFSETS[5] + 0x078, _configMemory);
			RedirectLEA(CONFIG_OFFSETS[5] + 0x0BC, _configMemory);
			RedirectLEA(CONFIG_OFFSETS[5] + 0x1DA, _configMemory);

			RedirectLEA(CONFIG_OFFSETS[2] + 0x323, _configMemory + 0x0C);
			RedirectLEA(CONFIG_OFFSETS[4] + 0x487, _configMemory + 0x0C);

			RedirectLEA(CONFIG_OFFSETS[5] + 0x0F5, _difficultyPtr);
			RedirectLEA(CONFIG_OFFSETS[5] + 0x1BB, _difficultyPtr);
			RedirectLEA(CONFIG_OFFSETS[5] + 0x1FC, _difficultyPtr);
			RedirectLEA(CONFIG_OFFSETS[5] + 0x244, _difficultyPtr);

			memcpy(CONFIG_OFFSETS[1] + 0x127 + 0x02, &_countFlag, 0x01);
			memcpy(CONFIG_OFFSETS[1] + 0x24B + 0x03, &_countFlag, 0x01);
			memcpy(CONFIG_OFFSETS[4] + 0x18E + 0x01, &_countFlag, 0x01);
			memcpy(CONFIG_OFFSETS[5] + 0x025 + 0x02, &_countFlag, 0x01);

			memcpy(CONFIG_OFFSETS[3] + 0x069 + 0x01, &_childCount, 0x01);
			memcpy(CONFIG_OFFSETS[4] + 0x16F + 0x01, &_childCount, 0x01);

			memcpy(CONFIG_OFFSETS[1] + 0x150 + 0x03, &_lastIndex, 0x01);
			memcpy(CONFIG_OFFSETS[1] + 0x2E1 + 0x02, &_lastIndex, 0x01);
			memcpy(CONFIG_OFFSETS[2] + 0x34F + 0x02, &_lastIndex, 0x01);
			memcpy(CONFIG_OFFSETS[3] + 0x01E + 0x02, &_lastIndex, 0x01);
			memcpy(CONFIG_OFFSETS[4] + 0x1FA + 0x02, &_lastIndex, 0x01);
			memcpy(CONFIG_OFFSETS[5] + 0x1D5 + 0x04, &_lastIndex, 0x01);
			memcpy(CONFIG_OFFSETS[5] + 0x2AB + 0x02, &_lastIndex, 0x01);

			memcpy(CONFIG_OFFSETS[0] + 0x05E + 0x03, &_commandIndex, 0x01);
			memcpy(CONFIG_OFFSETS[2] + 0x2A8 + 0x03, &_commandIndex, 0x01);

			memcpy(CONFIG_OFFSETS[1] + 0x325 + 0x02, &_pageFlag, 0x01);
			memcpy(CONFIG_OFFSETS[5] + 0x2EF + 0x02, &_pageFlag, 0x01);

			memcpy(CONFIG_OFFSETS[4] + 0x33, _preventUpdateInst.data(), _preventUpdateInst.size());

			if (*reinterpret_cast<const int*>(MENU::MenuType + 0x5C) == 0xAF20)
			{
				char _nopArray[0x04];
				char _menuSelectInst[0x04];

				fill(_nopArray, _nopArray + 0x04, 0x90);

				memcpy(_menuSelectInst, MENUSELECT_OFFSET + 0x46, 0x04);
				memcpy(MENUSELECT_OFFSET + 0x46, _nopArray, 0x04);

				auto _pointPage = *MENU::SubOptionSel + 0x12;
				auto _pointCurrent = *MENU::SubOptionSel;
				auto _pointMaximum = *MENU::SubOptionSel + 0x16;

				uint8_t _configSize = Entries.size();
				memcpy(const_cast<char*>(_pointMaximum), &_configSize, 0x01);

				uint8_t _pageCurrent = *_pointPage;
				uint8_t _indexCurrent = *_pointCurrent;
				uint8_t _pageAmount = _configSize - 0x09;

				if (_pageCurrent >= _pageAmount)
				{
					uint8_t _indexCalcuation = _indexCurrent + (_pageCurrent - _pageAmount);

					memcpy(const_cast<char*>(_pointPage), &_pageAmount, 0x01);
					memcpy(const_cast<char*>(_pointCurrent), &_indexCalcuation, 0x01);
				}

				Tz::CmConfig::UpdateActive();

				memcpy(MENUSELECT_OFFSET + 0x46, _menuSelectInst, 0x04);
			}

			CONFIG_OFFSETS[6][0xE5] = 0x00;
		}

		static void Handle()
		{
			auto _configMemory = *MENU::Config;

			auto _primaryBitwise = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A4);
			auto _secondaryBitwise = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);

			auto _menuOK = *MENU::MenuType == 0x08 && (*MENU::SubMenuType == 0x24 || *(MENU::SubMenuType + 0x04) == 0x24);

			if (!*YS::TITLE::IsTitle)
			{
				if (!CONFIG_FIRST_INIT)
				{
					bool _isQuadratum = (_primaryBitwise & 0x0800) == 0x0800;
					const char* _fileName = _isQuadratum ? "field2d/%s/qd0command.2dd" : "field2d/%s/zz0command.2dd";

					memcpy(COMMAND_MENU_ADDR, _fileName, 0x19);
					memcpy(COMMAND_MENU_ADDR + 0x20, _fileName + 0x0B, 0x0E);

					CONFIG_FIRST_INIT = true;
				}

				if (_menuOK)
				{
					if (!CONFIG_INIT)
					{
						if (!CONFIG_FETCH)
						{
							SETTING_MEMORY = _primaryBitwise;
							CONFIG_FETCH = true;
						}

						bool _isQuadratum = *(AREA::SaveData + 0x41A7);

						vector<uint8_t> _configArray;
						uint8_t _configSize = Entries.size();

						_configArray.resize(_configSize - 1);

						uint16_t _checkBitwise = 0x0000;

						for (int i = 0; i < _configArray.size(); i++)
						{
							auto _offIndex = 0xFF;

							auto _encodedName = YS::MESSAGE::GetData(Entries[i].Title);
							auto _decodedName = YS::MESSAGE::DecodeKHSCII(_encodedName);

							for (int z = 0; z < Entries[i].Count; z++)
							{
								auto _fetchBitwise = Entries[i].Toggles[z];

								if (_fetchBitwise == 0x0000)
								{
									_configArray[i] = z;
									continue;
								}

								auto _seekBitwise = (_checkBitwise & _fetchBitwise) == 0x00 ? _primaryBitwise : _secondaryBitwise;

								if (_fetchBitwise != 0x0000 && (_seekBitwise & _fetchBitwise) == _fetchBitwise)
								{
									_configArray[i] = z;
									printf("%s is set: %d\n", _decodedName.c_str(), z);
									break;
								}
							}

							for (int z = 0; z < Entries[i].Count; z++)
							{
								auto _fetchBitwise = Entries[i].Toggles[z];

								if ((_checkBitwise & _fetchBitwise) == 0x0000)
									_checkBitwise |= _fetchBitwise;
							}

							printf("%s is set: %d\n", _decodedName.c_str(), _configArray[i]);
						}

						_configArray.push_back(*(AREA::SaveData + 0x2498));

						if (_configMemory == 0x00)
							return;

						memcpy(reinterpret_cast<char*>(_configMemory), _configArray.data(), _configArray.size());

						if (*reinterpret_cast<const int*>(MENU::MenuType + 0x5C) == 0xAF20)
						{
							char _nopArray[0x04];
							char _menuSelectInst[0x04];

							fill(_nopArray, _nopArray + 0x04, 0x90);

							memcpy(_menuSelectInst, MENUSELECT_OFFSET + 0x46, 0x04);
							memcpy(MENUSELECT_OFFSET + 0x46, _nopArray, 0x04);

							auto _pointPage = *MENU::SubOptionSel + 0x12;
							auto _pointCurrent = *MENU::SubOptionSel;
							auto _pointMaximum = *MENU::SubOptionSel + 0x16;

							memcpy(const_cast<char*>(_pointMaximum), &_configSize, 0x01);

							uint8_t _pageCurrent = *_pointPage;
							uint8_t _indexCurrent = *_pointCurrent;
							uint8_t _pageAmount = _configSize - 0x09;

							if (_pageCurrent >= _pageAmount)
							{
								uint8_t _indexCalcuation = _indexCurrent + (_pageCurrent - _pageAmount);

								memcpy(const_cast<char*>(_pointPage), &_pageAmount, 0x01);
								memcpy(const_cast<char*>(_pointCurrent), &_indexCalcuation, 0x01);
							}

							auto _calcSeqPoint = MENUSEQ_POINTER() + 0x448B;
							auto _fetchSeqPoint = *reinterpret_cast<char**>(_calcSeqPoint);

							if (_fetchSeqPoint != nullptr && _fetchSeqPoint > moduleInfo.moduleStart && _fetchSeqPoint < moduleInfo.moduleEnd)
								Tz::CmConfig::UpdateList();

							Tz::CmConfig::UpdateActive();

							memcpy(MENUSELECT_OFFSET + 0x46, _menuSelectInst, 0x04);
						}

						CONFIG_INIT = true;
					}

					if (*reinterpret_cast<const int*>(MENU::MenuType + 0x5C) == 0xAF20)
					{
						vector<uint8_t> _fetchConfig(Entries.size());
						memcpy(_fetchConfig.data(), reinterpret_cast<char*>(_configMemory), Entries.size());

						uint16_t _constructPrimary = 0x0000;
						uint16_t _constructSecondary = 0x0000;

						uint16_t _checkBitwise = 0x0000;

						for (int i = 0; i < Entries.size() - 1; i++)
						{
							auto _encodedName = YS::MESSAGE::GetData(Entries[i].Title);
							auto _decodedName = YS::MESSAGE::DecodeKHSCII(_encodedName);

							if (_fetchConfig[i] > 0x02)
								break;

							auto _fetchBitwise = Entries[i].Toggles[_fetchConfig[i]];

							if ((_checkBitwise & _fetchBitwise) == _fetchBitwise)
								_constructSecondary |= _fetchBitwise;

							else
								_constructPrimary |= _fetchBitwise;

							for (int z = 0; z < Entries[i].Count; z++)
							{
								auto _getBitwise = Entries[i].Toggles[z];

								if ((_checkBitwise & _getBitwise) == 0x0000)
									_checkBitwise |= _getBitwise;
							}
						}

						bool _switchFlag = true;
						bool _isQuadratum = false;

						if ((_constructPrimary & 0x0800) == 0x0800)
						{
							_isQuadratum = true;
							_constructPrimary |= 0x0040;
						}

						const char* _fileName = _isQuadratum ? "field2d/%s/qd0command.2dd" : "field2d/%s/zz0command.2dd";

						memcpy(COMMAND_MENU_ADDR, _fileName, 0x19);
						memcpy(COMMAND_MENU_ADDR + 0x20, _fileName + 0x0B, 0x0E);

						memcpy(AREA::SaveData + 0x41A4, &_constructPrimary, 0x02);
						memcpy(AREA::SaveData + 0x41A6, &_constructSecondary, 0x02);

						memcpy(COMMAND_FLAG_ADDR, &_switchFlag, 0x01);


						for (int i = 0; i < Entries.size(); i++)
						{
							if (Entries[i].SubEntry)
							{
								auto _canFind = find_if(Entries.begin(), Entries.end(),
									[i](const Tz::HookConfig::Entry _fetchEntry) {
										return _fetchEntry.Title == Entries[i].SubEntry->Title;
									});

								auto _fetchBitwise = Entries[i].Toggles[_fetchConfig[i]];

								if (_fetchBitwise == Entries[i].SubToggle && _canFind == Entries.end())
								{
									Tz::HookConfig::Add(i + 1, *Entries[i].SubEntry);

									CONFIG_INIT = false;
									CONFIG_FETCH = false;

									return;
								}

								else if (_fetchBitwise != Entries[i].SubToggle && _canFind != Entries.end())
								{
									Tz::HookConfig::Remove(_canFind - Entries.begin());

									CONFIG_INIT = false;
									CONFIG_FETCH = false;

									return;
								}
							}
						}
					}

					auto _pointCurrent = *MENU::SubOptionSel + 0x12;
					auto _fetchCampBinarc = *reinterpret_cast<char**>(Tz::CmData::FileInfo + 0x40);

					if (*MENU::SubOptionSel && _fetchCampBinarc)
					{
						auto _intptrSQD = *reinterpret_cast<uint32_t*>(YS::BINARC::get_info_by_tag(_fetchCampBinarc, 0x1C, 0x706D6163, 0) + 0x08);
						auto _addressSQD = PC::CONVERTER::INTPTR_TO_POINTER(_intptrSQD);

						uint8_t _pageCount = Entries.size() - 0x09;
						uint8_t _pageFactor = 0x18 * _pageCount;

						auto _pageOffset = (_pageFactor / _pageCount) * *_pointCurrent;

						auto _offsetCalc = 0x64 + _pageOffset;
						auto _floatCalc = (0xC0 - _pageFactor) * 0.01F;
						auto _factorCalc = 0x64 - (_pageFactor + 0x01) + _pageOffset;

						memcpy(_addressSQD + 0x21498, &_offsetCalc, 0x04);
						memcpy(_addressSQD + 0x2149C, &_offsetCalc, 0x04);
						memcpy(_addressSQD + 0x21528, &_offsetCalc, 0x04);
						memcpy(_addressSQD + 0x2152C, &_offsetCalc, 0x04);

						memcpy(_addressSQD + 0x21568, &_floatCalc, 0x04);
						memcpy(_addressSQD + 0x2156C, &_floatCalc, 0x04);

						memcpy(_addressSQD + 0x215B8, &_factorCalc, 0x04);
						memcpy(_addressSQD + 0x215BC, &_factorCalc, 0x04);
					}
				}

				else if (!_menuOK && CONFIG_INIT)
					CONFIG_INIT = false;
			}

			else if (*YS::TITLE::IsTitle && CONFIG_FIRST_INIT)
				CONFIG_FIRST_INIT = false;
		}

		static void Remove(int Index)
		{
			Entries.erase(Entries.begin() + Index);
			Submit();
		}

		static void Add(int Index, Tz::HookConfig::Entry Input)
		{
			if (Index == UINT32_MAX)
				Entries.insert(Entries.end() - 1, Input);
			else
				Entries.insert(Entries.begin() + Index, Input);

			Submit();
		}

		static uint8_t GetValue(uint16_t titleID);
	};
}