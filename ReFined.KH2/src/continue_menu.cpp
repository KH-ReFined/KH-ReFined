#include "continue_menu.h"
														  
char* CONTINUE_OPTION_PTR = ResolveRelativeAddress<char*>("\x0F\xB6\x01\x48\x8D\x15\x00\x00\x00\x00\x48\x8B\x14\xC2\x0F\xB6\x41\x01\x3B\x02\x7C\x0C\x48\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xC3\x48\x8D\x0C\x40\x48\x8D\x42\x04\x48\x8D\x04\xC8\x48\x89\x05\x00\x00\x00\x00\xC3", "xxxxxx????xxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxx????x", 0x19) - 0x04;

vector<ReFined::Continue::Entry> ReFined::Continue::Children = vector<ReFined::Continue::Entry>
{
	ReFined::Continue::Entry(0x02, 0x8AB0),
	ReFined::Continue::Entry(0x01, 0x8AAF),
};

ReFined::Continue::Entry::Entry(uint16_t opcode, uint16_t label)
{
	this->Opcode = opcode;
	this->Label = label;
};

void ReFined::Continue::Submit()
{
	auto _childSize = Children.size();
	auto _optionsPtr = *reinterpret_cast<const uint64_t*>(CONTINUE_OPTION_PTR);
	
	if (_optionsPtr == 0x00 || _childSize > 0x04)
		return;

	memcpy(reinterpret_cast<char*>(_optionsPtr + 0x34A), &_childSize, 0x02);

	for (int i = 0x00; i < 0x04; i++)
	{
		if (i < _childSize)
		{
			auto _currChild = Children.at(i);
			memcpy(reinterpret_cast<char*>(_optionsPtr + 0x34C + (0x04 * i)), &_currChild, 0x04);
		}

		else
		{
			uint32_t _zeroChild = 0x00;
			memcpy(reinterpret_cast<char*>(_optionsPtr + 0x34C + (0x04 * i)), &_zeroChild, 0x04);
		}
	}
}

void ReFined::Continue::Add(int Index, Entry Input)
{
	Children.insert(Children.begin() + Index, Input);
	Submit();
}

void ReFined::Continue::Remove(int Index)
{
	Children.erase(Children.begin() + Index);
	Submit();
}