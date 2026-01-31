#include "config_menu.h"
#include "SigScan.h"

vector<ReFined::ConfigMenu::Entry> ReFined::ConfigMenu::Children = vector<ReFined::ConfigMenu::Entry>
{
	ReFined::ConfigMenu::Entry(0x02, 0xB717, vector<uint16_t>{ 0xB71E, 0xB71F }, vector<uint16_t>{ 0xB720, 0xB721 }),
	ReFined::ConfigMenu::Entry(0x02, 0xB718, vector<uint16_t>{ 0xB722, 0xB723 }, vector<uint16_t>{ 0xB724, 0xB725 }),
	ReFined::ConfigMenu::Entry(0x02, 0xC2F5, vector<uint16_t>{ 0xC2F8, 0xC2F9 }, vector<uint16_t>{ 0xC2FA, 0xC2FB }),
	ReFined::ConfigMenu::Entry(0x02, 0xC2F6, vector<uint16_t>{ 0xC2FC, 0xC2FD }, vector<uint16_t>{ 0xC2FE, 0xC2FF }),
	ReFined::ConfigMenu::Entry(0x03, 0xC2F7, vector<uint16_t>{ 0xC302, 0xC300, 0xC301 }, vector<uint16_t>{ 0xC305, 0xC303, 0xC304 }),
	ReFined::ConfigMenu::Entry(0x02, 0xB719, vector<uint16_t>{ 0xB726, 0xB727 }, vector<uint16_t>{ 0xB728, 0xB729 }),
	ReFined::ConfigMenu::Entry(0x03, 0x5704, vector<uint16_t>{ 0x5705, 0x5707, 0x5709 }, vector<uint16_t>{ 0x5706, 0x5708, 0x570A }),
	ReFined::ConfigMenu::Entry(0x02, 0x5722, vector<uint16_t>{ 0x5723, 0x5725 }, vector<uint16_t>{ 0x5724, 0x5726 }),
	ReFined::ConfigMenu::Entry(0x02, 0xB71A, vector<uint16_t>{ 0xB72A, 0xB752 }, vector<uint16_t>{ 0xB72C, 0xB72D }),
	ReFined::ConfigMenu::Entry(0x03, 0xB71C, vector<uint16_t>{ 0xB734, 0xB735, 0x572C}, vector<uint16_t>{ 0xB736, 0xB737, 0x572D }),
	ReFined::ConfigMenu::Entry(0x01, 0xB71D, vector<uint16_t>{ 0xB738, 0xB739, 0xB73A, 0xCE30 }, vector<uint16_t>{ 0xB73B, 0xB73C, 0xB73D, 0xCE31 })
};

vector<char*> ReFined::ConfigMenu::CONFIG_OFFSETS = vector<char*>
{
	SignatureScan<char*>("\x40\x53\x48\x83\xEC\x20\x0F\xB6\xD9\x48\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x4C\x8B\x1D\x00\x00\x00\x00", "xxxxxxxxxxxx????x????xxx????"),
	SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x80\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x70\xE8\x00\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxx????xxx????"),
	SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\x48\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xF8\xE8\x00\x00\x00\x00\x8B\xF0\x83\xF8\x02\x0F\x85\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x8B\xD8\xE8\x00\x00\x00\x00\x84\xC0\x75\x59\x48\x8B\x0D\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxx????x????xxx????xxxxxxx????x????xxx????xxxxxxx????"),
	SignatureScan<char*>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x8B\xFA\x8B\xD9\xE8\x00\x00\x00\x00\x8D\x0C\x3B\x44\x8D\x04\x9D\x00\x00\x00\x00", "xxxxxxxxxxxxxxx????xxxxxxxxxxx"),
	SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x30\xE8\x00\x00\x00\x00\x45\x33\xC0\x33\xC9\x41\x8D\x50\xFF\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxxx????xxxxxxxxxx????"),
	SignatureScan<char*>("\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x58\xE8\x00\x00\x00\x00\x48\x8B\x0D\x00\x00\x00\x00\x4C\x8B\xF8\xE8\x00\x00\x00\x00\x41\xBD\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxx????xxx????xxxx????xx????"),
	SignatureScan<char*>("\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x33\xDB\x48\x8D\x35\x00\x00\x00\x00\x33\xFF\x66\x0F\x1F\x44\x00\x00\x83\xFB\x06\x0F\x87\xB5\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxx")
};

ReFined::ConfigMenu::Entry::Entry(uint16_t count, uint16_t titleID, vector<uint16_t> buttonIDs, vector<uint16_t> descriptionIDs)
{
	this->Count = count;
	this->TitleID = titleID;
	this->ButtonIDs = buttonIDs;
	this->DescriptionIDs = descriptionIDs;
};

void ReFined::ConfigMenu::Submit()
{
	for (int i = 0; i < Children.size(); i++)
	{
		auto _currentEntry = Children[i];

		memcpy(ReFined::MemoryManager::Fetch("CONFIG_MEMORY") + (0x14 * i), &_currentEntry.Count, 0x02);
		memcpy(ReFined::MemoryManager::Fetch("CONFIG_MEMORY") + (0x14 * i) + 0x02, &_currentEntry.TitleID, 0x02);

		for (int z = 0; z < _currentEntry.ButtonIDs.size(); z++)
			memcpy(ReFined::MemoryManager::Fetch("CONFIG_MEMORY") + (0x14 * i) + (0x02 * z) + 0x04, &_currentEntry.ButtonIDs[z], 0x02);

		for (int z = 0; z < _currentEntry.DescriptionIDs.size(); z++)
			memcpy(ReFined::MemoryManager::Fetch("CONFIG_MEMORY") + (0x14 * i) + (0x02 * z) + 0x0C, &_currentEntry.DescriptionIDs[z], 0x02);
	}

	uint8_t _childCount = Children.size();
	uint8_t _lastIndex = Children.size() - 0x01;
	uint8_t _commandIndex = Children.size() - 0x02;

	uint8_t _pageFlag = 0x20;
	uint8_t _countFlag = _childCount;

	if (_childCount >= 0x09)
	{
		_pageFlag = 0x24;
		_countFlag = 0x09;
	}

	char* _difficultyPtr = ReFined::MemoryManager::Fetch("CONFIG_MEMORY") + 0x04 + 0x14U * _lastIndex;
	vector<uint8_t> _preventUpdateInst{ 0xE9, 0xDC, 0x00, 0x00, 0x00, 0x90 };

	RedirectLEA(CONFIG_OFFSETS[0] + 0x031, ReFined::MemoryManager::Fetch("CONFIG_MEMORY"));
	RedirectLEA(CONFIG_OFFSETS[0] + 0x047, ReFined::MemoryManager::Fetch("CONFIG_MEMORY"));
	RedirectLEA(CONFIG_OFFSETS[2] + 0x27B, ReFined::MemoryManager::Fetch("CONFIG_MEMORY"));
	RedirectLEA(CONFIG_OFFSETS[2] + 0x291, ReFined::MemoryManager::Fetch("CONFIG_MEMORY"));
	RedirectLEA(CONFIG_OFFSETS[4] + 0x2C3, ReFined::MemoryManager::Fetch("CONFIG_MEMORY"));
	RedirectLEA(CONFIG_OFFSETS[5] + 0x078, ReFined::MemoryManager::Fetch("CONFIG_MEMORY"));
	RedirectLEA(CONFIG_OFFSETS[5] + 0x0BC, ReFined::MemoryManager::Fetch("CONFIG_MEMORY"));
	RedirectLEA(CONFIG_OFFSETS[5] + 0x1DA, ReFined::MemoryManager::Fetch("CONFIG_MEMORY"));

	RedirectLEA(CONFIG_OFFSETS[2] + 0x323, ReFined::MemoryManager::Fetch("CONFIG_MEMORY") + 0x0C);
	RedirectLEA(CONFIG_OFFSETS[4] + 0x487, ReFined::MemoryManager::Fetch("CONFIG_MEMORY") + 0x0C);

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

	CONFIG_OFFSETS[6][0xE5] = 0x00;
}

void ReFined::ConfigMenu::Add(int Index, Entry Input)
{
	Children.insert(Children.begin() + Index, Input);
	Submit();
}

void ReFined::ConfigMenu::Remove(int Index)
{
	Children.erase(Children.begin() + Index);
	Submit();
}