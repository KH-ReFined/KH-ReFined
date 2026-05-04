#include "cmcustom.h"

char* _updateTopListOriginal = ResolveRelativeAddress<char*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x022F);

void (*Tz::CmCustom::CreateTopList)() = ResolveFunctionFromCall<void(*)()>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x0204);
char* (*Tz::CmCustom::GetFriendInfo)(int, int) = SignatureScan<char* (*)(int, int)>("\x48\x89\x5C\x24\x08\x45\x33\xDB\x8B\xDA\x45\x8B\xD3\x45\x8B\xCB", "xxxxxxxxxxxxxxxx");

int* Tz::CmCustom::m_pri = ResolveRelativeAddress<int*>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x0027);
char* Tz::CmCustom::LS_103_s_Seq = ResolveRelativeAddress<char*>(_updateTopListOriginal, 0x014D);

char* Tz::CmCustom::s_SelSeq = ResolveRelativeAddress<char*>("\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x68\xE8\x00\x00\x00\x00\x4C\x8B\xF8", "xxxxxxxxxxxxxxxxxx????xxx", 0x02FD);

char** Tz::CmCustom::LS_103_type = ResolveRelativeAddress<char**>(_updateTopListOriginal, 0x006F);
char** Tz::CmCustom::LS_62_type = Tz::CmCustom::LS_103_type - 0x01;
char** Tz::CmCustom::LS_45_type = Tz::CmCustom::LS_103_type + 0x01;
char** Tz::CmCustom::LS_52_type = Tz::CmCustom::LS_103_type + 0x02;

short* Tz::CmCustom::s_ButtonIcon = ResolveAbsoluteOffset<short*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x80\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x70\xE8\x00\x00\x00\x00\x48\x63\xD8\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x30\x48\x8B\xF0\x83\xFB\x19\x75\x0E", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxx????xxxx????xxxxxxxxxxxxx", 0x0172);
int* Tz::CmCustom::LS_57_s_MessTbl = ResolveAbsoluteOffset<int*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x80\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x70\xE8\x00\x00\x00\x00\x48\x63\xD8\xE8\x00\x00\x00\x00\x48\x89\x44\x24\x30\x48\x8B\xF0\x83\xFB\x19\x75\x0E", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxx????xxxx????xxxxxxxxxxxxx", 0x028C);

char** Tz::CmCustom::m_PartyInfo = ResolveRelativeAddress<char**>("\x40\x53\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\xE8", "xxxxxxxxxxxxxxx", 0x0063);
char** Tz::CmCustom::m_ListInfo = ResolveRelativeAddress<char**>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x90\x01\x00\x00\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x004D);

char* Tz::CmCustom::s_PlayerType = ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x90\x01\x00\x00\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x0046);
char* Tz::CmCustom::s_FriendType = ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x10\x48\x89\x6C\x24\x18\x48\x89\x74\x24\x20\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x90\x01\x00\x00\x48", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x003C);

char* Tz::CmCustom::LS_KH1F_Shortcuts = SignatureScan<char*>("\xBA\x02\xBD\x02\xC0\x02\xAB\x02\x00\x00\x7A\x44\x00\x00", "xxxxxxxxxxxxxx");

void Tz::CmCustom::SetupTop()
{
	auto _fetchBuffer = Tz::CmTop::GetListBuffer();
	Tz::SelHist::Create(0, -1);

	auto _fetchPriority = *Tz::CmCustom::m_pri;
	auto _processIdx = 0x00;
	*Tz::CmCustom::m_pri = 30017;

	for (int i = 0; i < 0x04; i++)
	{
		if (Tz::PartyInfo::GetSheet(*Tz::CmCustom::m_PartyInfo, i))
		{
			auto _fetchPlateZero = Tz::CmTop::GetTopPlateSeqTbl(0);
			auto _fetchPlateFirst = Tz::CmTop::GetTopPlateSeqTbl(1);

			Tz::Select::SetItem(*Tz::CmTop::m_MenuPtr, _processIdx, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, -1, reinterpret_cast<uint16_t*>(_fetchPlateZero), _fetchPlateFirst);
			Tz::Select::SetExOffset(*Tz::CmTop::m_MenuPtr, _processIdx, 0, -1);

			auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, _processIdx);

			*reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x10;
			*reinterpret_cast<char**>(_fetchItemPtr + 0x01F8) = Tz::PartyInfo::GetName(*Tz::CmCustom::m_PartyInfo, i);
			*reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x08;

			if (i == 0x00 && YS::ITEM::GetNumBackyard(0x0233))
			{
				auto _fetchPlate2Zero = Tz::CmTop::GetTopPlateSeqTbl(0) + 0x10;
				auto _fetchPlate2First = Tz::CmTop::GetTopPlateSeqTbl(1) + 0x10;

				Tz::Select::SetItem(*Tz::CmTop::m_MenuPtr, _processIdx + 1, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, -1, reinterpret_cast<uint16_t*>(_fetchPlate2Zero), _fetchPlate2First);
				Tz::Select::SetExOffset(*Tz::CmTop::m_MenuPtr, _processIdx + 1, 0, -1);

				auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, _processIdx + 1);

				*reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x10;
				*reinterpret_cast<char**>(_fetchItemPtr + 0x01F8) = YS::MESSAGE::GetData(0x4E80);
				*reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x010) &= ~0x08;

				_processIdx++;
			}

			_processIdx++;
		}
	}
	
	auto _fetchCurrPos = Tz::CmTop::GetCurPos(25);
	Tz::Select::SetCurrent(*Tz::CmTop::m_MenuPtr, _fetchCurrPos, true, 0x00, 0x02);
	Tz::MenuBase::SetSMode(0x00);
	*Tz::CmCustom::m_pri = _fetchPriority;
	Tz::CmCustom::CreateTopList();
	Tz::CmCustom::UpdateTopList();
}

void Tz::CmCustom::GetListInfo(int num)
{
	vector<short> _magicItemNum = { 0x0015, 0x0016, 0x0017, 0x0018, 0x0057, 0x0058 };

	auto _fetchCurrentType = Tz::CmCustom::s_PlayerType;
	auto _fetchSheetNum = num;

	if (num > 0)
	{
		if (num == 0x01 && YS::ITEM::GetNumBackyard(0x0233))
		{
			_fetchCurrentType = Tz::CmCustom::s_PlayerType - 0x04;
			_fetchSheetNum = 0x00;
		}

		else
		{
			_fetchCurrentType = Tz::CmCustom::s_FriendType;
			_fetchSheetNum = YS::ITEM::GetNumBackyard(0x0233) ? num - 1 : num;
		}
	}

	memset(*Tz::CmCustom::m_ListInfo, 0x00, 0x98);

	auto _fetchPartySheet = Tz::PartyInfo::GetSheet(*Tz::CmCustom::m_PartyInfo, _fetchSheetNum);
	auto _fetchPartyInventory = PC::CONVERTER::INT_TO_LONG_ADDRESS(*reinterpret_cast<uint32_t*>(_fetchPartySheet + 0x250));

	auto _processMagic = 0x00;
	auto _processEntry = 0x00;

	for (int i = 0; i < 0x03; i++)
	{
		if (_fetchCurrentType[i])
		{
			switch (_fetchCurrentType[i])
			{
			case 1:
				for (int i = 0; i < 0x06; i++)
				{
					auto _fetchMagicId = _magicItemNum[i];

					if (YS::ITEM::GetNumBackyard(_fetchMagicId))
					{
						*reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x08) = _fetchMagicId;
						*(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x0A) = YS::MAGIC::GetMpDriveStatus(i);

						_processMagic++;
						_processEntry++;
					}
				}

				**Tz::CmCustom::m_ListInfo = static_cast<char>(_processMagic);
				break;
			case 2:
			{
				set<uint32_t> _itemSet;
				auto _itemCount = *reinterpret_cast<char*>(_fetchPartyInventory + 0x12);

				for (int i = 0; i < _itemCount; i++)
				{
					auto _currItem = *reinterpret_cast<char*>(_fetchPartyInventory + 0x34 + 0x02 * i);

					if (!_currItem)
						continue;

					auto _autoReloadItem = *reinterpret_cast<char*>(_fetchPartyInventory + 0x44 + 0x02 * i);
					_itemSet.insert((_autoReloadItem == _currItem) ? _currItem | 0x80000000 : _currItem);
				}

				for (int i = 0; i < _itemSet.size(); i++)
				{
					auto _fetchElement = next(_itemSet.begin(), i);

					*reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x08) = *_fetchElement;
					*(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x0A) = *_fetchElement & 0x80000000 ? 0x01 : 0x00;

					if (_fetchCurrentType == Tz::CmCustom::s_FriendType)
					{
						auto _fetchPriority = reinterpret_cast<char*>(_fetchPartyInventory + kn::FriendSaveRam::item2index(*_fetchElement) + 0x0F5);
						*(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x0B) = *_fetchPriority;
					}

					_processEntry++;
				}

				*(*Tz::CmCustom::m_ListInfo + 0x01) = _itemSet.size();
			} break;
			case 3:
				*(*Tz::CmCustom::m_ListInfo + 0x03) = *reinterpret_cast<char*>(_fetchPartyInventory + 0x0F4);
				break;
			case 4:
			{
				vector<uint16_t> _abilityTable;

				for (int i = 0; i < 0x50; i++)
				{
					auto _fetchAbility = *reinterpret_cast<uint16_t*>(_fetchPartyInventory + 0x54 + 0x02 * i);
					auto _fetchAbilityAlone = _fetchAbility & 0x7FFF;
					
					if (_fetchAbilityAlone == 0x00)
						continue;

					auto _fetchItemType = *(YS::ITEM_TABLE::Get(_fetchAbilityAlone) + 0x06);

					if (_fetchItemType == 0x02 && _fetchAbility & 0x8000)
						_abilityTable.push_back(_fetchAbilityAlone);
				}

				sort(_abilityTable.begin(), _abilityTable.end());

				for (int i = 0; i < _abilityTable.size(); i++)
				{
					auto _fetchPriority = reinterpret_cast<char*>(_fetchPartyInventory + kn::FriendSaveRam::item2index(_abilityTable[i]) + 0x0F5);

					*reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x08) = _abilityTable[i];

					*(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x0A) = 1;
					*(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x0B) = *_fetchPriority;

					_processEntry++;
				}

				*(*Tz::CmCustom::m_ListInfo + 0x02) = _abilityTable.size();
			} break;
			default:
				continue;
			}
		}

		else 
		{
			for (int i = 0; i < 0x04; i++)
			{
				auto _isLimit = num == 0x01 && YS::ITEM::GetNumBackyard(0x0233);
				auto _fetchShortcut = _isLimit ? *reinterpret_cast<uint16_t*>(Tz::CmCustom::LS_KH1F_Shortcuts + 0x02 * i) : *reinterpret_cast<uint16_t*>(AREA::SaveData + 0x36F8 + 0x02 * i);

				if (_fetchShortcut)
				{
					auto _fetchItem = YS::ITEM_TABLE::Each(nullptr);

					while (_fetchItem)
					{
						auto _fetchType = *(_fetchItem + 0x02);
						auto _fetchId = *reinterpret_cast<uint16_t*>(_fetchItem);
						auto _fetchCommand = YS::ITEM::GetCommand(_fetchId);

						if (((_fetchType == 0x12 || _fetchType == 0x00) && _fetchCommand == _fetchShortcut) || _isLimit)
						{
							*reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x08) = _isLimit ? _fetchShortcut : _fetchId;

							if (_fetchType == 0x12)
							{
								auto _fetchMagicIdx = find(_magicItemNum.begin(), _magicItemNum.end(), *_fetchItem);
								auto _fetchDistance = distance(_magicItemNum.begin(), _fetchMagicIdx);

								*reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x0A) = YS::MAGIC::GetMpDriveStatus(_fetchDistance);
							}

							break;
						}

						_fetchItem = YS::ITEM_TABLE::Each(_fetchItem);
					}
				}

				else
					*reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _processEntry + 0x08) = 0x00;

				_processEntry++;
			}
		}
	}

	*(*Tz::CmCustom::m_ListInfo + 0x04) = _processEntry;
}

void Tz::CmCustom::UpdateTopList()
{
	int _itemNumArray[0x06];

	auto _fetchBuffer = Tz::CmTop::GetListBuffer();
	auto _fetchCurrent = **Tz::CmTop::m_MenuPtr;

	Tz::CmCustom::GetListInfo(_fetchCurrent);
	Tz::CmCustom::MakeListInfo2ItemMess();	

	*Tz::CmCustom::LS_103_type = Tz::CmCustom::s_PlayerType;

	if (_fetchCurrent > 0)
	{
		if (_fetchCurrent == 0x01 && YS::ITEM::GetNumBackyard(0x0233))
			*Tz::CmCustom::LS_103_type = Tz::CmCustom::s_PlayerType - 0x04;

		else
			*Tz::CmCustom::LS_103_type = Tz::CmCustom::s_FriendType;
	}

	auto _fetchFont = Tz::CmTop::GetFontColorSeqNum(0);

	auto _firstBuffer = reinterpret_cast<char*>(_fetchBuffer) + 0x1258;
	auto _secondBuffer = reinterpret_cast<char*>(_fetchBuffer) + 0x7660;

	for (int i = 0; i < 18; i++)
	{
		dk::Sprite::setNum(_secondBuffer - 0x3300, *Tz::CmTop::m_DummySeq);
		dk::Sprite::setNum(_secondBuffer, _fetchFont);

		*_firstBuffer = 0;
		*reinterpret_cast<char**>(_firstBuffer + 0x6600) = nullptr;

		_secondBuffer += 0x220;
		_firstBuffer += 0x220;
	}

	auto _fetchListInfo = *Tz::CmCustom::m_ListInfo;
	auto _calcItemOffset = 0x00;

	for (int i = 0; i < 3; i++)
	{
		auto _fetchTypeByte = *(*Tz::CmCustom::LS_103_type + i);
		_itemNumArray[i] = Tz::CmCustom::GetCustomItemNum(_fetchTypeByte);

		if (*Tz::CmCustom::LS_103_type == Tz::CmCustom::s_PlayerType - 0x04)
			break;
	}

	for (int i = 0; i < 0x03; i++)
	{
		auto _itemIndex = *(*Tz::CmCustom::LS_103_type + i);
		auto _fetchSeq = *reinterpret_cast<uint16_t*>(reinterpret_cast<uint64_t>(Tz::CmCustom::LS_103_s_Seq) + 0x02 * _itemIndex);

		if (_itemNumArray[i])
		{
			for (int z = 0; z < _itemNumArray[i]; z++)
			{
				auto _fetchBufferIndex = reinterpret_cast<char*>(_fetchBuffer) + 0x220 * _calcItemOffset + 0x1258 + 0x220 * z;
				auto _currOffset = _calcItemOffset - i;

				auto _calcSeq = reinterpret_cast<char*>(_fetchBuffer) + 0x220 * (_calcItemOffset + z) + 0x4360;
				dk::Sprite::setNum(_calcSeq, _fetchSeq);

				auto _fetchFontColor = 0x00;

				if (_itemIndex != 0x03 && *Tz::CmCustom::LS_103_type != Tz::CmCustom::s_PlayerType - 0x04)
					_fetchFontColor = Tz::CmTop::GetItemFontColor(*reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * z + 0x08), 0x01, _itemIndex == 0x01);

				auto _fetchFontColorSqd = Tz::CmTop::GetFontColorSeqNum(_fetchFontColor);
				auto _calcFontSeq = reinterpret_cast<char*>(_fetchBuffer) + 0x220 * (_calcItemOffset + z) + 0x7660;

				dk::Sprite::setNum(_calcFontSeq, _fetchFontColorSqd);
				*reinterpret_cast<char**>(_fetchBufferIndex + 0x6600) = reinterpret_cast<char*>(_fetchBuffer) + 32 * (_calcItemOffset + z + 0x6E7);

				if (*Tz::CmCustom::LS_103_type == Tz::CmCustom::s_FriendType && _itemIndex != 0x03)
					*reinterpret_cast<char**>(_fetchBufferIndex) = reinterpret_cast<char*>(_fetchBuffer) + 0x04 * (_calcItemOffset + z) + 0xDFE0;
			}
		
			_calcItemOffset = _calcItemOffset + _itemNumArray[i];
		}
	}
}

int Tz::CmCustom::GetCustomItemNum(int type)
{
	auto _fetchListInfo = *Tz::CmCustom::m_ListInfo;

	if (type)
	{
		switch (type)
		{
		case 1:
			return *_fetchListInfo;
		case 2:
			return _fetchListInfo[1];
		case 3:
			return 1;
		case 4:
			return _fetchListInfo[2];
		}
	}

	else
		return 0x04;
}

int Tz::CmCustom::CurPos2CustomType(int pos)
{
	*Tz::CmCustom::LS_52_type = Tz::CmCustom::s_PlayerType;

	if (Tz::CmTop::GetCurPos(25))
	{
		if (Tz::CmTop::GetCurPos(25) == 0x01 && YS::ITEM::GetNumBackyard(0x0233))
			*Tz::CmCustom::LS_52_type = Tz::CmCustom::s_PlayerType - 0x04;

		else
			*Tz::CmCustom::LS_52_type = Tz::CmCustom::s_FriendType;
	}

	auto _fetchItemCalc = 0x00;

	for (int i = 0; i < 3; i++)
	{
		auto _fetchItemType = *(*Tz::CmCustom::LS_52_type + i);
		auto _fetchItemNum = Tz::CmCustom::GetCustomItemNum(_fetchItemType);

		_fetchItemCalc += _fetchItemNum;

		if (pos < _fetchItemCalc)
			return _fetchItemType;
	}

	return -1;
}

void Tz::CmCustom::MakeListInfo2ItemMess()
{
	int _itemNumArray[0x06];

	auto _fetchMenuMode = Tz::MenuBase::GetMode();
	auto _fetchBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());

	auto _getCurPos = Tz::CmTop::GetCurPos(0x19);

	if (_fetchMenuMode == 25)
		_getCurPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

	auto _isLimit = _getCurPos == 0x01 && YS::ITEM::GetNumBackyard(0x0233);

	*Tz::CmCustom::LS_62_type = Tz::CmCustom::s_PlayerType;
	
	if (_getCurPos > 0)
	{
		if (_isLimit)
			*Tz::CmCustom::LS_62_type = Tz::CmCustom::s_PlayerType - 0x04;

		else
			*Tz::CmCustom::LS_62_type = Tz::CmCustom::s_FriendType;
	}

	for (int i = 0; i < 3; i++)
	{
		auto _fetchTypeByte = *(*Tz::CmCustom::LS_62_type + i);
		_itemNumArray[i] = Tz::CmCustom::GetCustomItemNum(_fetchTypeByte);

		if (*Tz::CmCustom::LS_62_type == Tz::CmCustom::s_PlayerType - 0x04)
			break;
	}

	memset(_fetchBuffer + 0xDCE0, 0x00, 0x0360);

	auto _calcItemOffset = 0;
	auto _didProcessAI = false;

	for (int i = 0; i < 3; i++)
	{
		auto _fetchCurrType = *(*Tz::CmCustom::LS_62_type + i);

		for (int z = 0; z < _itemNumArray[i]; z++)
		{
			auto _isSpecial = false;
			auto _processCommand = false;
			char* _itemInfoStart = nullptr;
			char* _fetchMainItem = nullptr;

			int _fetchMessage = UINT32_MAX;
			char* _messageAddr = nullptr;

			uint16_t _fetchButtonIcon = UINT16_MAX;

			if (_fetchCurrType)
				_processCommand = _fetchCurrType == 0x01;

			else
			{
				_processCommand = true;

				if (_fetchMenuMode == 25)
					_fetchButtonIcon = Tz::CmCustom::s_ButtonIcon[_calcItemOffset + z];
			}

			auto _fetchListInfo = *Tz::CmCustom::m_ListInfo;

			if (_fetchCurrType == 0x03)
			{
				_messageAddr = _fetchBuffer + (_calcItemOffset + z + 0x06E7) * 0x20;
				_fetchMessage = Tz::CmCustom::LS_57_s_MessTbl[*(*Tz::CmCustom::m_ListInfo + 0x03)];
				_didProcessAI = true;
			}

			else
			{
				auto _fetchEntry = *reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * (_calcItemOffset + z - _didProcessAI) + 0x08);
				auto _fetchItem = YS::ITEM_TABLE::Get(_fetchEntry);
				auto _fetchItemType = _fetchItem ? *(_fetchItem + 0x02) : -1;

				if (_fetchEntry && (_fetchItemType == 0x12))
				{
					_isSpecial = *(*Tz::CmCustom::m_ListInfo + 0x08 * (_calcItemOffset + z - _didProcessAI) + 0x0A) == 0x00;
					_itemInfoStart = *Tz::CmCustom::m_ListInfo + 0x08 * (_calcItemOffset + z - _didProcessAI);
				}

				else
				{
					_isSpecial = *(_fetchListInfo + 0x08 * (_calcItemOffset + z - _didProcessAI) + 0x0A) == 0x01;
					_itemInfoStart = _fetchListInfo + 0x08 * (_calcItemOffset + z - _didProcessAI);
				}

				auto _fetchItemId = *reinterpret_cast<uint16_t*>(_itemInfoStart + 0x08);

				_messageAddr = _fetchBuffer + (_calcItemOffset + z + 0x06E7) * 0x20;
				_fetchMessage = 0x8448;

				_fetchMainItem = _fetchItemId ? YS::ITEM_TABLE::Get(_fetchItemId) : nullptr;

				if (_fetchButtonIcon != UINT16_MAX)
					Tz::CmComm::FontIcon(_fetchButtonIcon, _messageAddr, true);

				else if (_fetchMainItem)
					Tz::CmComm::ItemIcon(*(_fetchMainItem + 2), _messageAddr, _isSpecial, true);

				if (_fetchItemId)
				{
					if (_fetchMainItem)
					{
						if (_processCommand)
						{
							auto _fetchCommand = YS::ITEM::GetCommand(*reinterpret_cast<uint16_t*>(_fetchMainItem));
							_fetchMessage = *reinterpret_cast<uint16_t*>(YS::COMMAND_ELEM::Get(_fetchCommand) + 0x08);
						}

						else
							_fetchMessage = *reinterpret_cast<uint16_t*>(_fetchMainItem + 0x08);
					}

					else
					{
						_fetchMessage = *reinterpret_cast<uint16_t*>(YS::COMMAND_ELEM::Get(_fetchItemId) + 0x08);

						if (_fetchButtonIcon == UINT16_MAX)
						{
							Tz::CmComm::FontIcon(0x04, _messageAddr, true);
							_messageAddr += 0x02;
						}
					}
				}

				if (_fetchButtonIcon != UINT16_MAX || _fetchMainItem)
					_messageAddr += 0x02;
			}

			auto _fetchData = YS::MESSAGE::GetData(_fetchMessage);
			memcpy(_messageAddr, _fetchData, YS::MESSAGE::GetSize(_fetchData));

			if (!_isLimit && _getCurPos)
			{
				if (_fetchCurrType == 0x02 || _fetchCurrType == 0x04)
				{
					auto _fetchFriend = Tz::CmCustom::GetFriendInfo(_fetchCurrType, *(_fetchListInfo + 0x08 * (_calcItemOffset + z) + 0x03));
					Tz::CmComm::FontIcon(_fetchFriend[0x04], reinterpret_cast<char*>(_fetchBuffer) + 0x04 * (_calcItemOffset + z) + 0xDFE0, true);
				}
			}
		}
	
		_calcItemOffset += _itemNumArray[i];
	}
}

void Tz::CmCustom::UpdateHelpMess()
{
	map<uint16_t, uint16_t> _limitShortcutMap =
	{
		{ 0x02BA, 0x0239 },
		{ 0x02BD, 0x023A },
		{ 0x02C0, 0x023B },
		{ 0x02AB, 0x023C }
	};

	short _magicHelpText[] = { 0x9DFB, 0x9DFA };
	short _itemHelpText[] = { 0x9DFF, 0x9DFE };
	short _itemFreqHelpText[] = { 0x9E5E, 0x9E5F };

	auto _fetchMenuMode = Tz::MenuBase::GetMode();
	auto _fetchBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());
	auto _fetchSelectPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

	auto _isLimit = ((_fetchMenuMode == 25 && _fetchSelectPos == 0x01) || (_fetchMenuMode != 25 && Tz::CmTop::GetCurPos(25) == 0x01)) && YS::ITEM::GetNumBackyard(0x0233);

	uint16_t _messageId = UINT16_MAX;
	uint16_t _sideMessageId = UINT16_MAX;

	switch (_fetchMenuMode)
	{
		case 25: 
		{
			if (_fetchSelectPos == 0x00)
				_messageId = 0x3B89;

			else
			{
				if (_isLimit)
					_messageId = 0x575C;

				else
					_messageId = 0x3B8A;
			}
		} break;

		case 26:
		case 27:
		case 28:
		{
			auto _fetchCustomType = Tz::CmCustom::CurPos2CustomType(_fetchSelectPos);

			if (_fetchCustomType == 0x03)
				_messageId = *reinterpret_cast<uint16_t*>(Tz::CmCustom::GetFriendInfo(0x03, *(*Tz::CmCustom::m_ListInfo + 3)) + 0x02);

			else
			{
				auto _fetchMainPos = Tz::CmTop::GetCurPos(25);

				if (_fetchMainPos && !_isLimit)
					_fetchSelectPos -= 1;

				auto _fetchItemId = *reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectPos + 0x08);

				if (_fetchItemId)
				{
					auto _fetchItemEntry = YS::ITEM_TABLE::Get(_fetchItemId);

					if (_fetchItemEntry)
						_messageId = *reinterpret_cast<uint16_t*>(_fetchItemEntry + 0x0A);

					else if (_isLimit)
					{
						auto _fetchCommandItem = YS::ITEM_TABLE::Get(_limitShortcutMap[_fetchItemId]);
						_messageId = *reinterpret_cast<uint16_t*>(_fetchCommandItem + 0x0A);
					}

					else
						_messageId = *reinterpret_cast<uint16_t*>(YS::COMMAND_ELEM::Get(_fetchItemId) + 0x08);
				}

				if (_fetchMenuMode == 27 && !_isLimit)
					_sideMessageId = *reinterpret_cast<uint16_t*>(Tz::CmCustom::GetFriendInfo(_fetchCustomType, *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectPos + 0x0B)) + 0x02);
			}
		} break;
		case 29:
		{
			if (_fetchSelectPos > 0x00)
			{
				auto _itemInfo = reinterpret_cast<char*>(*Tz::CmTop::m_ItemInfo + 0x04);
				auto _fetchItemId = *reinterpret_cast<uint16_t*>(_itemInfo + 0x06 * (_fetchSelectPos - 0x01));
				auto _fetchItemEntry = YS::ITEM_TABLE::Get(_fetchItemId);

				_messageId = *reinterpret_cast<uint16_t*>(_fetchItemEntry + 0x0A);
			}

			else
				_messageId = 0x1DF7;
		} break;

		case 30:
			_messageId = _magicHelpText[_fetchSelectPos];
			break;

		case 31:
		{
			auto _fetchMainPos = Tz::CmTop::GetCurPos(25);

			_messageId = _itemHelpText[_fetchSelectPos % 0x02];

			if (_fetchMainPos && !_isLimit)
				_sideMessageId = _itemFreqHelpText[_fetchSelectPos / 0x02];
		} break;
	}

	if (_messageId != UINT16_MAX)
	{
		auto _fetchData = YS::MESSAGE::GetData(_messageId);
		memcpy(_fetchBuffer + 0xE0A0, _fetchData, YS::MESSAGE::GetSize(_fetchData) + 0x01);

		if (_sideMessageId != UINT16_MAX)
		{
			auto _fetchSideData = YS::MESSAGE::GetData(_sideMessageId);
			memcpy(_fetchBuffer + 0xE0A0 + YS::MESSAGE::GetSize(_fetchData), _fetchSideData, YS::MESSAGE::GetSize(_fetchSideData) + 0x01);
		}
	}
	 
	Tz::CMenuHelp::Create(_messageId != UINT16_MAX ? _fetchBuffer + 0xE0A0 : nullptr, true);
}

void Tz::CmCustom::UpdateCustomList()
{
	int _itemNumArray[0x06];

	uint16_t _listSelSeq[] = { 0x0108, 0x0107, 0x0106, 0x00FA, 0x00F8, 0x0000, 0x0000, 0x0000 };
	uint16_t _listSubSeq[] = { 0x011A, 0x0119, 0x0118, 0x0115, 0x0114, 0x0000 };
	uint16_t _listSubMsg[] = { 0x847D, 0x8481, 0x8482, 0x8483, 0x8484, 0x0000 };
	uint16_t _listShortcutSubMsg[] = { 0x847D, 0x847E, 0x847F, 0x8480 };

	auto _fetchBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());
	auto _fetchSelectTop = *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x12);

	auto _fetchCurPos = Tz::CmTop::GetCurPos(25);
	auto _isLimit = _fetchCurPos == 0x01 && YS::ITEM::GetNumBackyard(0x0233);

	if (_isLimit && *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x16) != 0x04)
	{
		*reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x16) = 0x04;

		*reinterpret_cast<uint32_t*>(*Tz::CmTop::m_SclBar + 0x28) = 0x01;
		*reinterpret_cast<uint32_t*>(*Tz::CmTop::m_SclBar + 0x2C) = 0x01;
	}

	Tz::ScrollBar::SetPos(*Tz::CmTop::m_SclBar, _fetchSelectTop);

	*Tz::CmCustom::LS_45_type = Tz::CmCustom::s_PlayerType;

	if (_fetchCurPos)
	{
		if (_isLimit)
		{
			*Tz::CmCustom::LS_45_type = Tz::CmCustom::s_PlayerType - 0x04;
			*reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x16) = 0x04;
		}

		else
			*Tz::CmCustom::LS_45_type = Tz::CmCustom::s_FriendType;
	}

	auto _fetchItemMax = *reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x04);
	auto _fetchFontColorSeq = Tz::CmTop::GetFontColorSeqNum(0);

	for (int i = 0; i < _fetchItemMax; i++)
	{
		auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, i);
		dk::Sprite::setNum(_fetchItemPtr, *Tz::CmTop::m_DummySeq);
	}

	for (int i = 0; i < 0x09; i++)
	{
		dk::Sprite::setNum(_fetchBuffer + 0x1060 + 0x0220 * i + 0x6600, _fetchFontColorSeq);
		*reinterpret_cast<char**>(_fetchBuffer + 0x1258 + 0x0220 * i + 0x6600) = nullptr;

		dk::Sprite::setNum(_fetchBuffer + 0x1060 + 0x0220 * i, *Tz::CmTop::m_DummySeq);
		*reinterpret_cast<char**>(_fetchBuffer + 0x1258 + 0x0220 * i) = nullptr;

		dk::Sprite::setNum(_fetchBuffer + 0xA960 + 0x01F8 * i, *Tz::CmTop::m_DummySeq);
	}

	for (int i = 0; i < 3; i++)
	{
		auto _fetchTypeByte = *(*Tz::CmCustom::LS_45_type + i);
		_itemNumArray[i] = Tz::CmCustom::GetCustomItemNum(_fetchTypeByte);

		if (*Tz::CmCustom::LS_45_type == Tz::CmCustom::s_PlayerType - 0x04)
			break;
	}

	auto _fetchMaxLoop = *(*Tz::CmCustom::m_ListInfo + 0x04);
	auto _itemNumberCalc = 0x00;

	auto _processEntry = 0x00;

	if (_fetchCurPos && !_isLimit)
		_fetchMaxLoop++;

	if (_fetchMaxLoop > 0x09)
		_fetchMaxLoop = 0x09;

	for (int i = 0; i < 3; i++)
	{
		auto _fetchTypeByte = *(*Tz::CmCustom::LS_45_type + i);

		if (_processEntry >= _fetchMaxLoop)
			break;

		if (_itemNumArray[i])
		{
			for (int z = 0; z < _itemNumArray[i]; z++)
			{
				auto _fetchMessagePtr = _fetchBuffer + (0x220 * _processEntry) + 0x7858;
				auto _fetchMessage = (_fetchBuffer + 0x20 * (z + 0x6E7)) + (0x20 * _itemNumberCalc);

				if (_processEntry >= _fetchMaxLoop)
					break;

				if (z + _processEntry >= _fetchSelectTop)
				{
					if (z == 0x00 || _fetchTypeByte == 0x00)
					{
						dk::Sprite::setNum(_fetchBuffer + 0x220 * _processEntry + 0x1060, _listSubSeq[_fetchTypeByte]);
						*reinterpret_cast<char**>(_fetchMessagePtr - 0x6600) = YS::MESSAGE::GetData(_fetchTypeByte ? _listSubMsg[_fetchTypeByte] : _listShortcutSubMsg[z]);
					}

					auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, _processEntry);
					dk::Sprite::setNum(_fetchItemPtr, _listSelSeq[_fetchTypeByte]);

					auto _fetchFontColor = 0x00;

					if (_fetchTypeByte != 0x03 && !_isLimit)
						_fetchFontColor = Tz::CmTop::GetItemFontColor(*reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * i + 0x08), 0x01, _fetchTypeByte == 0x01);
					
					_fetchFontColorSeq = Tz::CmTop::GetFontColorSeqNum(_fetchFontColor);
					dk::Sprite::setNum(_fetchBuffer + 0x220 * _processEntry + 0x7660, _fetchFontColorSeq);
					*reinterpret_cast<char**>(_fetchMessagePtr) = _fetchMessage;

					_processEntry++;
					_fetchMessagePtr += 0x220;
				}			
			}
		}
		
		_itemNumberCalc += _itemNumArray[i];
	}
}

void Tz::CmCustom::SetupCustom()
{ 
	uint16_t IndiCustomSelSeq[] = { 0x00CA, 0x00C1, 0x00B8, 0x00A6, 0x009D };
	uint16_t IndiCustomTitleSeq[] = { 0x00CE, 0x00C5, 0x00BC, 0x00AA, 0x00A1, 0x0000 };
	uint16_t IndiCustomBaseSeq[] = { 0x00C6, 0x00C8, 0x00C7, 0x00BD, 0x00BF, 0x00BE, 0x00B4, 0x00B6, 0x00B5, 0x00A2, 0x00A4, 0x00A3, 0x0099, 0x009B, 0x009A, 0x0000 };

	int _fetchInfoMax = -1;

	auto _fetchBuffer = reinterpret_cast<char*>(Tz::CmTop::GetListBuffer());
	auto _fetchMode = Tz::MenuBase::GetMode();
	auto _fetchSelectPos = Tz::CmTop::GetSelectPos(26);
	auto _fetchCustomType = Tz::CmCustom::CurPos2CustomType(_fetchSelectPos);

	auto _currSeqdPtr = Tz::CmCustom::s_SelSeq + 0x04;

	while (_currSeqdPtr < reinterpret_cast<char*>(Tz::CmCustom::LS_62_type) + 0x04)
	{
		*reinterpret_cast<uint16_t*>(_currSeqdPtr - 0x04) = IndiCustomSelSeq[_fetchCustomType];
		*reinterpret_cast<uint16_t*>(_currSeqdPtr - 0x02) = IndiCustomSelSeq[_fetchCustomType];
		*reinterpret_cast<uint16_t*>(_currSeqdPtr) = IndiCustomSelSeq[_fetchCustomType];
		*reinterpret_cast<uint16_t*>(_currSeqdPtr + 0x02) = IndiCustomSelSeq[_fetchCustomType];

		_currSeqdPtr += 0x04;
	}

	auto _isLimit = Tz::CmTop::GetSelectPos(25) == 0x01 && YS::ITEM::GetNumBackyard(0x0233);

	if (_fetchCustomType)
	{
		switch (_fetchCustomType)
		{
			case 0x01:
				_fetchInfoMax = 0x02;
				break;
			case 0x02:
			{
				_fetchInfoMax = 0x02;

				if (Tz::CmTop::GetSelectPos(25))
					_fetchInfoMax = 0x04;
			} break;

			case 0x03:
				_fetchInfoMax = 0x06;
				break;
			default:
				_fetchInfoMax = 0x03;
				break;
		}
	}

	else
	{
		memset(*Tz::CmTop::m_ItemInfo, 0x00, 0x30);

		if (!_isLimit)
			Tz::ItemInfo::SetItemInfo(*Tz::CmTop::m_ItemInfo, 0x00, 0x0A);

		else
		{
			memcpy(*Tz::CmTop::m_ItemInfo, "\x04", 0x01);

			memcpy(*Tz::CmTop::m_ItemInfo + 0x04, "\x39\x02", 0x02);
			memcpy(*Tz::CmTop::m_ItemInfo + 0x0A, "\x3A\x02", 0x02);
			memcpy(*Tz::CmTop::m_ItemInfo + 0x10, "\x3B\x02", 0x02);
			memcpy(*Tz::CmTop::m_ItemInfo + 0x16, "\x3C\x02", 0x02);
		}

		if (!_isLimit)
			Tz::ItemInfo::AddKnowItemInfo(*Tz::CmTop::m_ItemInfo);

		Tz::ItemInfo::MakeEmptyMsg(*Tz::CmTop::m_ItemInfo, -1);
		Tz::ItemInfo::SortItemInfo(*Tz::CmTop::m_ItemInfo);

		_fetchInfoMax = *reinterpret_cast<int*>(*Tz::CmTop::m_ItemInfo);
		Tz::ItemInfo::MakeCommandMsg(*Tz::CmTop::m_ItemInfo, 0, _fetchInfoMax);

		if (_isLimit)
			for (int i = 0; i < 0x04; i++)
				*reinterpret_cast<uint16_t*>(*Tz::CmTop::m_ItemInfo + 0x0784 + 0x10 + (0x50 * i)) = 0x0409;

		_fetchInfoMax += 0x01;
	}

	Tz::SelHist::Create(0x02, -1);
	auto _fetchPriority = *Tz::CmCustom::m_pri;

	Tz::MenuUtil::CreateSprt(_fetchBuffer, _fetchPriority, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, IndiCustomBaseSeq[3 * _fetchCustomType], IndiCustomBaseSeq[3 * _fetchCustomType + 0x02], 0, 0);
	Tz::MenuUtil::CreateMess(_fetchBuffer + 0x07E0, _fetchPriority + 0x01, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, IndiCustomTitleSeq[_fetchCustomType], -1, -1, 0, 0);
	
	*reinterpret_cast<uint32_t*>(_fetchBuffer + 0x07F0) &= ~0x10;
	Tz::MenuUtil::SetSprtParent(_fetchBuffer + 0x07E0, _fetchBuffer);

	auto _fetchFontColor = !_isLimit ? Tz::CmTop::GetItemFontColor(*reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectPos + 0x08), 1, _fetchCustomType == 1) : 0x00;
	auto _fetchFontColorSeq = Tz::CmTop::GetFontColorSeqNum(_fetchFontColor);

	Tz::MenuUtil::CreateMess(_fetchBuffer + 0x7660, _fetchPriority + 0x02, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, _fetchFontColorSeq, -1, -1, 0, 0);

	*reinterpret_cast<char**>(_fetchBuffer + 0x7858) = _fetchBuffer + (_fetchSelectPos + 0x6E7) * 0x20;
	Tz::MenuUtil::SetSprtParent(_fetchBuffer + 0x7660, _fetchBuffer + 0x07E0);

	*reinterpret_cast<uint32_t*>(_fetchBuffer + 0x7670) &= ~0x10;
	*reinterpret_cast<uint32_t*>(_fetchBuffer + 0x7670) &= ~0x08;

	auto _fetchSizePage = _fetchInfoMax > 0x08 ? 0x08 : _fetchInfoMax;

	_fetchFontColorSeq = Tz::CmTop::GetFontColorSeqNum(0x00);

	for (int i = 0; i < _fetchSizePage; i++)
	{
		auto _sideObject = _fetchBuffer + 0x7880 + 0x220 * i;

		Tz::Select::SetItem(*Tz::CmTop::m_MenuPtr, i, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, -1, reinterpret_cast<uint16_t*>(Tz::CmCustom::s_SelSeq), Tz::CmCustom::s_SelSeq + 0x08);
		
		auto _fetchItemPtr = Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, i);
		auto _paramCr = YI::SEQUENCE::GetParamCr(_fetchItemPtr + 0x20);

		Tz::Select::SetExOffset(*Tz::CmTop::m_MenuPtr, i, 0, _paramCr);

		*reinterpret_cast<uint32_t*>(_fetchItemPtr + 0x10) &= ~0x10;

		if (_fetchMode == 31 && Tz::CmTop::GetSelectPos(25) && !_isLimit)
			*reinterpret_cast<char**>(Tz::Select::GetItemPtr(*Tz::CmTop::m_MenuPtr, i) + 0x220) = const_cast<char*>(moduleInfo.startAddr + 0x35AF10);

		Tz::MenuUtil::SetSprtParent(_fetchItemPtr, _fetchBuffer);
		Tz::MenuUtil::CreateMess(_sideObject, _fetchPriority + 0x02, *Tz::CmTop::m_SeqUnit, *Tz::CmTop::m_ImgUnit, *Tz::CmTop::m_Lay, _fetchFontColorSeq, -1, -1, 0, 0);
		Tz::MenuUtil::SetSprtParent(_sideObject, _fetchItemPtr);

		*reinterpret_cast<uint32_t*>(_sideObject + 0x10) &= ~0x10;
		*reinterpret_cast<uint32_t*>(_sideObject + 0x10) &= ~0x08;
	}

	auto _fetchLayout = *Tz::CmTop::m_Lay + 0x50;
	auto _fetchSeqUnit = YI::LAYOUT::GetSequenceUnit(_fetchLayout, *Tz::CmTop::m_SeqUnit);
	auto _fetchImage = YI::IMAGE_FREEZE::GetImage(*reinterpret_cast<char**>(_fetchLayout + 0x48), *Tz::CmTop::m_ImgUnit);
	Tz::ScrollBar::Create(*Tz::CmTop::m_SclBar, _fetchPriority + 0x01, _fetchSeqUnit, _fetchImage, Tz::CmTop::GetScrBarSeqTbl(), _fetchInfoMax, _fetchSizePage, _fetchBuffer);

	Tz::Select::SetPageMode(*Tz::CmTop::m_MenuPtr, 0x01);
	*reinterpret_cast<uint16_t*>(*Tz::CmTop::m_MenuPtr + 0x1C) = _fetchSizePage;

	if (_fetchInfoMax > 0x08)
		Tz::Select::SetSelectMax(*Tz::CmTop::m_MenuPtr, _fetchInfoMax);

	reinterpret_cast<void(*)()>(moduleInfo.startAddr + 0x35C2A0)();

	Tz::MenuBase::SetSMode(0);

	reinterpret_cast<void(*)()>(moduleInfo.startAddr + 0x35DDE0)();
}

void Tz::CmCustom::ChangeCustomInfo()
{
	map<uint16_t, uint16_t> _limitShortcutMap =
	{
		{ 0x0239, 0x02BA },
		{ 0x023A, 0x02BD },
		{ 0x023B, 0x02C0 },
		{ 0x023C, 0x02AB }
	};

	auto _fetchSelectPos = Tz::CmTop::GetSelectPos(26);
	auto _fetchCustomType = Tz::CmCustom::CurPos2CustomType(_fetchSelectPos);
	auto _fetchMenuSelectPos = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);

	auto _isLimit = Tz::CmTop::GetCurPos(25) == 0x01 && YS::ITEM::GetNumBackyard(0x0233);

	auto _fetchCommand = 0x00;

	switch (_fetchCustomType)
	{
		case 4:
			Tz::CmCustom::ChageAbility(_fetchMenuSelectPos);
			break;
		case 3:
			Tz::CmCustom::ChangePartyBehavior(_fetchMenuSelectPos);
			break;
		case 2:
		{
			if (Tz::CmTop::GetSelectPos(25))
				_fetchSelectPos--;

			auto _fetchReplenishFlag = *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectPos + 0x0A) != 0x00;
			auto _fetchBehaviorFlag = *(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectPos + 0x0B) == 0x02;
			auto _fetchOffset = _fetchReplenishFlag;

			if (_fetchBehaviorFlag && Tz::CmTop::GetSelectPos(25))
				_fetchOffset += 2;

			if (_fetchOffset != _fetchMenuSelectPos);
				// Tz::CmCustom::ChangeAutoReplenishment();
		} break;
		case 1:
		{
			auto _fetchMpFlag = Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectPos + 0x0A != 0x00;

			if (_fetchMpFlag != _fetchMenuSelectPos);
				// Tz::CmCustom::ChageMpDrive();
		}
		default:
		{
			if (_fetchMenuSelectPos && !_isLimit)
			{
				auto _fetchInfo = *Tz::CmTop::m_ItemInfo + 0x04;
				_fetchCommand = YS::ITEM::GetCommand(*reinterpret_cast<uint16_t*>(_fetchInfo + 0x06 * (_fetchMenuSelectPos - 0x01)));
			}

			else if (_fetchMenuSelectPos)
			{
				auto _fetchInfo = *Tz::CmTop::m_ItemInfo + 0x04;
				_fetchCommand = _limitShortcutMap[*reinterpret_cast<uint16_t*>(_fetchInfo + 0x06 * (_fetchMenuSelectPos - 0x01))];
			}

			if (!_isLimit)
				*reinterpret_cast<uint16_t*>(AREA::SaveData + (_fetchSelectPos * 0x02) + 0x36F8) = _fetchCommand;

			else
				*reinterpret_cast<uint16_t*>(Tz::CmCustom::LS_KH1F_Shortcuts + (_fetchSelectPos * 0x02)) = _fetchCommand;

		} break;
	}

	Tz::CmCustom::GetListInfo(Tz::CmTop::GetSelectPos(25));
}

void Tz::CmCustom::ChageAbility(int pos)
{
	auto _fetchMode = Tz::MenuBase::GetMode();
	auto _fetchFriendInfo = Tz::CmCustom::GetFriendInfo(4, -1);
	auto _fetchSelectPos = Tz::CmTop::GetSelectPos(25);
	auto _fetchSheet = *reinterpret_cast<uint32_t*>(Tz::PartyInfo::GetSheet(*Tz::CmCustom::m_PartyInfo, YS::ITEM::GetNumBackyard(0x0233) ? _fetchSelectPos - 1 : _fetchSelectPos) + 0x250);

	auto _fetchSelectAbility = Tz::CmTop::GetSelectPos(26);

	if (_fetchMode == 26 || _fetchMode == 27)
		_fetchSelectAbility = Tz::Select::GetSelectPos(*Tz::CmTop::m_MenuPtr);		

	auto _fetchIndex = kn::FriendSaveRam::item2index(*reinterpret_cast<uint16_t*>(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectAbility));
	auto _friendIterator = 0x00;

	if (pos < 0x00)
	{
		while (*(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectAbility + 0x03) != *(_fetchFriendInfo + 0x08 * _friendIterator + 0x05))
		{
			_friendIterator++;

			if (_friendIterator >= 0x03)
				break;
		}

		if (_friendIterator < 0x03)
			pos = _friendIterator;

		if (++pos >= 0x03)
			pos = 0x00;
	}

	auto _fetchAbility = *(_fetchFriendInfo + 0x08 * pos + 0x05);
	auto _convertSheet = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(_fetchSheet));

	*(_convertSheet + _fetchIndex + 0x00F5) = _fetchAbility;
	*(*Tz::CmCustom::m_ListInfo + 0x08 * _fetchSelectAbility + 0x03) = _fetchAbility;
}

void Tz::CmCustom::ChangePartyBehavior(int pos)
{
	auto _fetchSelectPos = Tz::CmTop::GetSelectPos(25);
	auto _fetchSheet = *reinterpret_cast<uint32_t*>(Tz::PartyInfo::GetSheet(*Tz::CmCustom::m_PartyInfo, YS::ITEM::GetNumBackyard(0x0233) ? _fetchSelectPos - 1 : _fetchSelectPos) + 0x250);
	auto _convertSheet = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(_fetchSheet));

	if (pos >= 0)
	{
		*(_convertSheet + 0x00F4) = pos;
		*(*Tz::CmCustom::m_ListInfo + 0x03) = pos;
	}

	else
	{
		auto _behaviorPtr = _convertSheet + 0x00F4;
		*_behaviorPtr += 1;

		if (*_behaviorPtr >= 0x06)
			*_behaviorPtr = 0;

		*(*Tz::CmCustom::m_ListInfo + 0x03) = *_behaviorPtr;
	}
}