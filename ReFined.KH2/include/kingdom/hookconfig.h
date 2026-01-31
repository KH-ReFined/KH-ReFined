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
	class HookConfig
	{
	public:
		static vector<char*> CONFIG_OFFSETS;
		static vector<vector<uint16_t>> Entries;

		static void Submit();
		static void Handle();

		static void Remove(int Index);
		static void Add(int Index, vector<uint16_t> Input);

		static uint8_t GetValue(uint16_t titleID);
	};
}