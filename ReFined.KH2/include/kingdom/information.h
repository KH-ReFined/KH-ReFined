#pragma once

#define DLL_EXPORT __declspec(dllexport)
extern "C"
{
	namespace dk {

		class DLL_EXPORT INFORMATION {
		public:
			using openInformationWindow_t = void(*)(const char* message);
			static openInformationWindow_t openInformationWindow;

			using openGetMunny_t = void(*)(int munny);
			static openGetMunny_t openGetMunny;
		};
	}
}