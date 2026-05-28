#pragma once

#define DLL_EXPORT __declspec(dllexport)

#include "sound.h"
#include "obj2d.h"
#include "sprite.h"
#include "sequence.h" 
#include "info_base.h"
#include "memorymgr.h"
#include "information.h"

extern "C"
{
    namespace dk
    {
        class DLL_EXPORT TIMER
        {
		private:
			static bool _init()
			{
				RedirectFunction("\x40\x55\x53\x56\x57\x41\x56\x48\x8B\xEC\x48\x83\xEC\x60\x48\x8B\xD9\xE8", "xxxxxxxxxxxxxxxxxx", reinterpret_cast<uint64_t>(update), 0x44B);
				return true;
			}

			#if !defined(BUILD_ARCHIPELAGO) && !defined(BUILD_ARCHIPELAGO_LITE)
			static inline bool _doInit = _init();
			#endif

        public:
            static inline uint32_t(*getTime)() = FetchFunctionFromCall<uint32_t(*)()>("\x40\x55\x53\x56\x57\x41\x56\x48\x8B\xEC\x48\x83\xEC\x60\x48\x8B\xD9\xE8", "xxxxxxxxxxxxxxxxxx", 0x10A);
            static inline bool(*isDown)() = FetchFunctionFromCall<bool(*)()>("\x40\x55\x53\x56\x57\x41\x56\x48\x8B\xEC\x48\x83\xEC\x60\x48\x8B\xD9\xE8", "xxxxxxxxxxxxxxxxxx", 0x27F);

            static void update(char* timer)
			{
				dk::INFO_BASE::update(timer);

				auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
				auto _offsetValue = 85;

				if (_fetchMemory)
					memcpy(&_offsetValue, _fetchMemory, 0x04);

				auto _applyAspect = 0x00;

				auto _fetchActiveX = YI::SEQUENCE::GetActiveX(timer + 0x1CF0);
				auto _fetchNumber = *reinterpret_cast<uint32_t*>(timer + 0x1CF0 + 0x158);

				switch (_fetchNumber)
				{
				case 0x1A:
					_applyAspect = _offsetValue * -1;
					break;
				case 0x1B:
					_applyAspect = _offsetValue;
					break;
				default:
					_applyAspect = 0x00;
					break;
				}

				*reinterpret_cast<int*>(timer + 0x0028) = _applyAspect + _fetchActiveX;

				if (*reinterpret_cast<uint32_t*>(timer + 0x1EA8) == 0x01)
				{
					if (!dk::Obj2D::isExist(timer + 0x0160))
					{

					}

					return;
				}

				else
				{
					dk::Sprite::setPos(timer + 0x0160, *reinterpret_cast<int*>(timer + 0x0028), *reinterpret_cast<int*>(timer + 0x002C));

					auto _timeCurrent = *reinterpret_cast<uint32_t*>(timer + 0x1EA0);
					auto _timeDisplay = *reinterpret_cast<uint32_t*>(timer + 0x1EA4);

					if (_timeCurrent == UINT32_MAX)
						_timeCurrent = dk::TIMER::getTime();

					auto _currMinute = (_timeCurrent % 216000) / 3600;
					auto _currSecond = ((_timeCurrent % 216000) % 3600) / 60;
					auto _currSalise = ((_timeCurrent % 216000) % 3600) % 60 * 1.6666666;

					if (_timeDisplay != _timeCurrent)
					{
						auto _paramEx = YI::SEQUENCE::GetParamEx(timer + 0x0378);

						if (_paramEx == 1 && _timeCurrent > 0)
						{
							_timeCurrent += 60;

							_currMinute = (_timeCurrent % 216000) / 3600;
							_currSecond = ((_timeCurrent % 216000) % 3600) / 60;
							_currSalise = ((_timeCurrent % 216000) % 3600) % 60 * 1.6666666;
						}

						char _numSeqDictionary[0x0A] = { 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 };

						char _arrayMinuteNum[0x03];
						char _arraySecondNum[0x03];
						char _arraySaliseNum[0x03];

						sprintf_s(_arrayMinuteNum, "%d", _currMinute);
						sprintf_s(_arraySecondNum, "%d", _currSecond);
						sprintf_s(_arraySaliseNum, "%d", static_cast<int>(_currSalise));

						vector<char> _sequenceMinute;
						vector<char> _sequenceSecond;
						vector<char> _sequenceSalise;

						for (int i = 0; i < 2; i++)
						{
							if (!_arrayMinuteNum[i])
								_sequenceMinute.push_back(0x0C);

							else
								_sequenceMinute.insert(_sequenceMinute.begin(), _numSeqDictionary[_arrayMinuteNum[i] - 0x30]);

							if (!_arraySecondNum[i])
								_sequenceSecond.push_back(0x0C);

							else
								_sequenceSecond.insert(_sequenceSecond.begin(), _numSeqDictionary[_arraySecondNum[i] - 0x30]);

							if (!_arraySaliseNum[i])
								_sequenceSalise.push_back(0x0C);

							else
								_sequenceSalise.insert(_sequenceSalise.begin(), _numSeqDictionary[_arraySaliseNum[i] - 0x30]);
						}

						dk::Sprite::setNum(timer + 0x0550, _sequenceMinute[1]);
						dk::Sprite::setNum(timer + 0x0748, _sequenceMinute[0]);

						dk::Sprite::setNum(timer + 0x0940, _sequenceSecond[1]);
						dk::Sprite::setNum(timer + 0x0B38, _sequenceSecond[0]);

						dk::Sprite::setNum(timer + 0x0D30, _sequenceSalise[1]);
						dk::Sprite::setNum(timer + 0x0F28, _sequenceSalise[0]);

						dk::Sprite::setNum(timer + 0x1120, 0x17);
						dk::Sprite::setNum(timer + 0x1318, 0x18);

						if (_paramEx == 1 && _timeCurrent > 0)
						{
							_timeCurrent -= 60;

							_currMinute = (_timeCurrent % 216000) / 3600;
							_currSecond = ((_timeCurrent % 216000) % 3600) / 60;
							_currSalise = ((_timeCurrent % 216000) % 3600) % 60 * 1.6666666;
						}
					}

					if (*reinterpret_cast<uint32_t*>(timer + 0x1EA0) != UINT32_MAX || (!dk::TIMER::isDown() || _currMinute > 0))
					{
						*reinterpret_cast<uint32_t*>(timer + 0x1EA4) = _timeCurrent;
						return;
					}

					if (_currSecond <= 2 && _currSecond >= 0 && _timeCurrent != 0x00)
					{
						auto _bitwiseCheck = _currSecond == 2 ? 1 : (_currSecond == 1 ? 2 : 4);

						if (!dk::Obj2D::isExist(timer + 0x1900 - (0x01F8 * _currSecond)) && (*reinterpret_cast<uint32_t*>(timer + 0x1E90) & _bitwiseCheck) == 0x00)
						{
							dk::Sprite::create(timer + 0x1900 - (0x01F8 * _currSecond), 3600, *reinterpret_cast<char**>(timer + 0x1E98), timer + 0x0060, 0x1D + _currSecond, 0x07, 0x00);
							SOUND::PlaySFX(0x2B);

							*reinterpret_cast<uint32_t*>(timer + 0x1E90) |= _bitwiseCheck;
						}
					}

					else if (_timeCurrent == 0x00)
					{
						if (!dk::Obj2D::isExist(timer + 0x1AF8) && (*reinterpret_cast<uint32_t*>(timer + 0x1E90) & 0x08) == 0x00)
						{
							dk::Sprite::create(timer + 0x1AF8, 3600, *reinterpret_cast<char**>(timer + 0x1E98), timer + 0x0060, 0x1C, 0x07, 0x00);
							*reinterpret_cast<uint32_t*>(timer + 0x1E90) |= 0x08;
						}
					}

					*reinterpret_cast<uint32_t*>(timer + 0x1EA4) = _timeCurrent;
					return;
				}
			}
        };
    }
}