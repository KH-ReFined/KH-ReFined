#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <map>
#include <sstream>
#include <vector>
#include <Windows.h>
#include <regex>
#include <iomanip> 

#include "memorymgr.h"

using namespace std;
extern "C"
{
	namespace YS
	{
		class DLL_EXPORT MESSAGE
		{
		public:
			using GetData_t = char* (*)(int id);
			using GetSize_t = const size_t(*)(const char* message);
			using Sprintf_t = char*(*)(char* buff, const char* string, ...);

			static GetData_t GetData;
			static GetSize_t GetSize;
			static Sprintf_t Sprintf;

			static char* SizeTable;

			static string DecodeKHSCII(const char* Input);
			static vector<char> EncodeKHSCII(string Input);
		};
	}
}