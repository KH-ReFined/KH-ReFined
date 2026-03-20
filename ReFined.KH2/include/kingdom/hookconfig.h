#pragma once

#include <cstdint>
#include <vector>

#include "information.h"
#include "area.h"
#include "gauge.h"
#include "title.h"
#include "steam.h"
#include "binarc.h"
#include "party.h"
#include "menu.h"
#include "message.h"
#include "cmconfig.h"
#include "softreset.h"
#include "sequence.h"
#include "panacea_alloc.h"
#include "memorymgr.h"

using namespace std;

namespace Tz
{
	class HookConfig
	{
	public:
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

		static vector<char*> CONFIG_OFFSETS;
		static vector<Entry> Entries;

		static void Submit();
		static void Handle();

		static void Remove(int Index);
		static void Add(int Index, Entry Input);

		static uint8_t GetValue(uint16_t titleID);
	};
}