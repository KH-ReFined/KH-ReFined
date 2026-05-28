#pragma once

#include "memorymgr.h"

using namespace std;

namespace ReFined
{
	class Continue
	{
	protected:
		static inline char* CONTINUE_OPTION_PTR = FetchRelativePointer<char*>("\x0F\xB6\x01\x48\x8D\x15\x00\x00\x00\x00\x48\x8B\x14\xC2\x0F\xB6\x41\x01\x3B\x02\x7C\x0C\x48\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xC3\x48\x8D\x0C\x40\x48\x8D\x42\x04\x48\x8D\x04\xC8\x48\x89\x05\x00\x00\x00\x00\xC3", "xxxxxx????xxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxx????x", 0x19) - 0x04;

	public:
		static struct Entry
		{
		public:
			uint16_t Opcode;
			uint16_t Label;

			Entry(uint16_t opcode, uint16_t label)
			{
				this->Opcode = opcode;
				this->Label = label;
			};
		};

		static vector<char*> CONFIG_OFFSETS;

		static inline vector<Entry> Children = vector<ReFined::Continue::Entry>
		{
			ReFined::Continue::Entry(0x02, 0x8AB0),
			ReFined::Continue::Entry(0x01, 0x8AAF),
		};

		static void Submit()
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

		static void Add(int Index, Entry Input)
		{
			Children.insert(Children.begin() + Index, Input);
			Submit();
		}

		static void Remove(int Index)
		{
			Children.erase(Children.begin() + Index);
			Submit();
		}
	};
}