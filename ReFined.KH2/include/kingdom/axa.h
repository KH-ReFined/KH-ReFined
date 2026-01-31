#pragma once

#define DLL_EXPORT __declspec(dllexport)

extern "C"
{
	namespace PC
	{
		class DLL_EXPORT AXA
		{
		public:
			using DebugPrint_t = void(*)(const char* string);
			static DebugPrint_t DebugPrint;
		};
	}
}