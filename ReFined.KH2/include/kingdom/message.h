#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include <cstdint>
#include <map>
#include <sstream>
#include <vector>

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

			static GetData_t GetData;
			static GetSize_t GetSize;

			static char* SizeTable;

			static string DecodeKHSCII(const char* Input);
			static vector<char> EncodeKHSCII(string Input);
		};
	}
}