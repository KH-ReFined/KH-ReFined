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
#include "next_form.h"
#include "panacea_alloc.h"

using namespace std;

namespace Tz
{
	class HookIntro
	{
	public:
		static vector<vector<uint32_t>> Entries;
		static vector<char*> INTRO_OFFSETS;

		static void Submit();
		static void Handle();

		static void Remove(int Index);
		static void Add(int Index, vector<uint32_t> Input);

		static uint8_t GetValue(uint32_t titleID);
	};
}