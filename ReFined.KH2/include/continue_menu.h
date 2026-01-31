#pragma once

#include <cstdint>
#include <vector>

#include "SigScan.h"

using namespace std;

namespace ReFined
{
	class Continue
	{

	public:
		static struct Entry
		{
		public:
			uint16_t Opcode;
			uint16_t Label;

			Entry(uint16_t opcode, uint16_t label);
		};

		static vector<char*> CONFIG_OFFSETS;

		static vector<Entry> Children;

		static void Submit();

		static void Add(int Index, Entry Input);
		static void Remove(int Index);
	};
}