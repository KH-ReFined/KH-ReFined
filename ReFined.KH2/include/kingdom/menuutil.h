#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include "memorymgr.h"

extern "C"
{
	namespace Tz
	{
		class DLL_EXPORT MenuUtil
		{
		public:
			using CreateSprt_t = char* (*)(char* spriteMessage, int priority, uint32_t s_unit, uint32_t i_unit, char* layout, int fin, uint32_t loop, int group, int offset16x9);
			static CreateSprt_t CreateSprt;

			using CreateMess_t = char* (*)(char* spriteMessage, int priority, uint32_t s_unit, uint32_t i_unit, char* layout, int fin, uint32_t loop, int messageNo, int group, int offset16x9);
			static CreateMess_t CreateMess;

			using SetSprtParent_t = void(*)(char* target, char* parent);
			static SetSprtParent_t SetSprtParent;
		};
	}
}