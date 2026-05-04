#pragma once

#include <cstdint>
#include <vector>

#include "information.h"
#include "area.h"
#include "gauge.h"
#include "title.h"
#include "steam.h"
#include "party.h"
#include "menu.h"
#include "message.h"
#include "cmconfig.h"
#include "softreset.h"
#include "sequence.h"
#include "cmcustom.h"
#include "panacea_alloc.h"
#include "memorymgr.h"

using namespace std;

namespace Tz
{
	class HookIntro
	{
	public:
		struct Entry
		{
		public:
			uint32_t Count;
			uint32_t Title;
			uint32_t Flair;
			vector<uint32_t> Buttons;
			vector<uint32_t> Descriptions;
			vector<uint32_t> Toggles;
			uint32_t SubToggle;
			Entry* SubEntry;
		};

		static map<uint32_t**, Tz::HookIntro::Entry> IntroSeeks;
		static vector<Entry> Entries;
		static vector<char*> INTRO_OFFSETS;

		static int ActiveCount;

		static void Submit();
		static void Handle();

		static void Remove(int Index);
		static void Add(int Index, Entry Input);

		static uint8_t GetValue(uint32_t titleID);
	};
}