#include "hookintro.h"

bool INTRO_APPLIED = false;
bool ENFORCE_INTRO = false;

uint16_t FIRST_CONFIG = 0x0408;
uint16_t SECOND_CONFIG = 0x0000;

vector<char*> Tz::HookIntro::INTRO_OFFSETS = vector<char*>
{
	SignatureScan<char*>("\x48\x89\x5C\x24\x18\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x48\x8B\x05\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxx????"),
	SignatureScan<char*>("\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x4C\x8B\x3D\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxx????"),
	SignatureScan<char*>("\x40\x53\x48\x83\xEC\x20\x48\x8B\x0D\x00\x00\x00\x00\x48\x81\xC1\x40\x04\x00\x00\xE8\x00\x00\x00\x00", "xxxxxxxxx????xxxxxxxx????"),
	SignatureScan<char*>("\x48\x83\xEC\x38\x48\x8B\x0D\x00\x00\x00\x00\x48\x89\x5C\x24\x40\x48\x85\xC9\x74\x27\xE8\x00\x00\x00\x00", "xxxxxxx????xxxxxxxxxxx????"),
	SignatureScan<char*>("\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x40\x48\x8B\x05\x00\x00\x00\x00\x80\x78\x0C\x00\x74\x18\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxx????xxxxxxx????"),
	SignatureScan<char*>("\x48\x89\x5C\x24\x20\x56\x57\x41\x56\x48\x83\xEC\x30\xE8\x00\x00\x00\x00\x48\x8B\xC8\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxx????xxxx????"),
	SignatureScan<char*>("\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x8B\x15\x00\x00\x00\x00\x48\x8B\xC8\xE8\x00\x00\x00\x00", "xxxxx????xx????xxxx????")
};

#if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
vector<vector<uint32_t>> Tz::HookIntro::Entries = vector<vector<uint32_t>>
{
	vector<uint32_t> { 0x04, 0xC330, 0xC380, 0xC331, 0xC332, 0xC333, 0xCE33, 0xC334, 0xC335, 0xC336, 0xCE34, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF },
	vector<uint32_t> { 0x02, 0xC337, 0xC381, 0xC338, 0xC339, 0xC33A, 0xC33B, 0x0001, 0x0000 },
	vector<uint32_t> { 0x03, 0x5733, 0xFFFF, 0x5705, 0x5707, 0x5709, 0x5706, 0x5708, 0x570A, 0x0004, 0x0002, 0x0000 },
	vector<uint32_t> { 0x02, 0x5737, 0xFFFF, 0x5723, 0x5725, 0x5724, 0x5726, 0x2000, 0x0000  }
};
#elif BUILD_ARCHIPELAGO_LITE
vector<vector<uint32_t>> Tz::HookIntro::Entries = vector<vector<uint32_t>>
{
	vector<uint32_t> { 0x04, 0xC330, 0xC380, 0xC331, 0xC332, 0xC333, 0xCE33, 0xC334, 0xC335, 0xC336, 0xCE34, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF },
	vector<uint32_t> { 0x02, 0xC337, 0xC381, 0xC338, 0xC339, 0xC33A, 0xC33B, 0x0001, 0x0000 }
};
#else
vector<vector<uint32_t>> Tz::HookIntro::Entries = vector<vector<uint32_t>>
{
	vector<uint32_t> { 0x04, 0xC330, 0xC380, 0xC331, 0xC332, 0xC333, 0xCE33, 0xC334, 0xC335, 0xC336, 0xCE34, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF },
	vector<uint32_t> { 0x02, 0xC337, 0xC381, 0xC338, 0xC339, 0xC33A, 0xC33B, 0x0001, 0x0000 },
	vector<uint32_t> { 0x03, 0x5733, 0xFFFF, 0x5705, 0x5707, 0x5709, 0x5706, 0x5708, 0x570A, 0x0004, 0x0002, 0x0000 }
};
#endif

void Tz::HookIntro::Add(int Index, vector<uint32_t> Input)
{
	if (Index == UINT32_MAX)
		Entries.insert(Entries.end(), Input);

	else
		Entries.insert(Entries.begin() + Index, Input);

	Submit();
}

void Tz::HookIntro::Remove(int Index)
{
	Entries.erase(Entries.begin() + Index);
	Submit();
}

void Tz::HookIntro::Submit()
{
	auto _introMemory = YS::PANACEA_ALLOC::Get("INTRO_MEMORY");

	if (!_introMemory)
	{
		YS::PANACEA_ALLOC::Allocate("INTRO_MEMORY", 0x300);
		_introMemory = YS::PANACEA_ALLOC::Get("INTRO_MEMORY");
	}

	for (int i = 0; i < Entries.size(); i++)
	{
		auto _currentEntry = Entries[i];

		memcpy(_introMemory + (0x2C * i), &_currentEntry[0], 0x04);
		memcpy(_introMemory + (0x2C * i) + 0x04, &_currentEntry[1], 0x04);
		memcpy(_introMemory + (0x2C * i) + 0x08, &_currentEntry[2], 0x04);

		for (int z = 0; z < (_currentEntry[0] == 0x01 ? 0x04 : _currentEntry[0]); z++)
			memcpy(_introMemory + (0x2C * i) + (0x04 * z) + 0x0C, &_currentEntry[0x03 + z], 0x04);

		for (int z = 0; z < (_currentEntry[0] == 0x01 ? 0x04 : _currentEntry[0]); z++)
			memcpy(_introMemory + (0x2C * i) + (0x04 * z) + 0x1C, &_currentEntry[0x03 + _currentEntry[0] + z], 0x04);
	}

	vector<uint8_t> _firstInit(0x10);

	uint8_t _childCount = Entries.size();
	uint8_t _lastIndex = Entries.size() - 0x01;

	fill(_firstInit.begin(), _firstInit.begin() + 0x10, 0x00);
	memcpy(_introMemory + 0x200, _firstInit.data(), 0x10);

	uint64_t _menuAddress = reinterpret_cast<uint64_t>(_introMemory);
	uint32_t _menuOffset = _introMemory - moduleInfo.startAddr;

	memcpy(INTRO_OFFSETS[0x03] + 0x097, &_childCount, 0x01);
	memcpy(INTRO_OFFSETS[0x03] + 0x1F5, &_childCount, 0x01);
	memcpy(INTRO_OFFSETS[0x03] + 0x531, &_childCount, 0x01);
	memcpy(INTRO_OFFSETS[0x04] + 0x1EF, &_childCount, 0x01);

	memcpy(INTRO_OFFSETS[0x00] + 0x3F7, &_lastIndex, 0x01);
	memcpy(INTRO_OFFSETS[0x01] + 0x3CB, &_lastIndex, 0x01);
	memcpy(INTRO_OFFSETS[0x02] + 0x031, &_lastIndex, 0x01);
	memcpy(INTRO_OFFSETS[0x03] + 0x08E, &_lastIndex, 0x01);

	memcpy(INTRO_OFFSETS[0x00] + 0x253, &_menuOffset, 0x04);
	_menuOffset += 0x04; memcpy(INTRO_OFFSETS[0x00] + 0x233, &_menuOffset, 0x04);
	_menuOffset += 0x08; memcpy(INTRO_OFFSETS[0x00] + 0x276, &_menuOffset, 0x04);
	_menuOffset += 0x10; memcpy(INTRO_OFFSETS[0x00] + 0x406, &_menuOffset, 0x04);

	RedirectLEA(INTRO_OFFSETS[0x01] + 0x1DA, _introMemory);
	RedirectLEA(INTRO_OFFSETS[0x01] + 0x3D7, _introMemory);
	RedirectLEA(INTRO_OFFSETS[0x01] + 0x0AF, _introMemory + 0x08);
	RedirectLEA(INTRO_OFFSETS[0x02] + 0x03D, _introMemory + 0x1C);

	_menuOffset += 0x1E4; memcpy(INTRO_OFFSETS[0x00] + 0x3B5, &_menuOffset, 0x04);

	RedirectLEA(INTRO_OFFSETS[0x01] + 0x0A8, _introMemory + 0x200);
	RedirectLEA(INTRO_OFFSETS[0x04] + 0x1F2, _introMemory + 0x200);
	RedirectMOV(INTRO_OFFSETS[0x05] + 0x2BF, _introMemory + 0x200);
	RedirectMOV(INTRO_OFFSETS[0x06] + 0x009, _introMemory + 0x200);
	RedirectCMP(INTRO_OFFSETS[0x06] + 0x017, _introMemory + 0x204);
}

void Tz::HookIntro::Handle()
{
	auto _introMemory = YS::PANACEA_ALLOC::Get("INTRO_MEMORY");

	uint16_t _checkBitwise = 0xCFF8;

	if (*YS::TITLE::IsTitle)
	{
		FIRST_CONFIG = 0x0408;
		SECOND_CONFIG = 0x0000;

		if (INTRO_APPLIED)
			INTRO_APPLIED = false;

		ENFORCE_INTRO = *YS::TITLE::IntroSelect == 0x00;

		for (int i = 1; i < Entries.size(); i++)
		{
			auto _optionCount = Entries[i][0x00];
			auto _fetchToggle = *(_introMemory + 0x200 + (i * 0x04));

			auto _getBitwise = Entries[i][0x03 + _optionCount * 2 + _fetchToggle];

			if (_getBitwise == 0xFFFF)
				continue;

			if (_checkBitwise & _getBitwise)
				SECOND_CONFIG |= _getBitwise;

			else
				FIRST_CONFIG |= _getBitwise;

			for (int z = 0; z < _optionCount; z++)
			{
				_getBitwise = Entries[i][0x03 + _optionCount * 2 + z];

				if (_checkBitwise & _getBitwise)
					break;

				_checkBitwise |= _getBitwise;
			}
		}
	}

	else if (!*YS::TITLE::IsTitle && !INTRO_APPLIED && ENFORCE_INTRO && YS::AREA::Current->World == 0x02 && (YS::AREA::Current->Room == 0x01 || YS::AREA::Current->Room == 0x20))
	{
		memcpy(YS::AREA::SaveData + 0x41A4, &FIRST_CONFIG, 0x02);
		memcpy(YS::AREA::SaveData + 0x41A6, &SECOND_CONFIG, 0x02);

		ENFORCE_INTRO = false;
		INTRO_APPLIED = true;
	}
}