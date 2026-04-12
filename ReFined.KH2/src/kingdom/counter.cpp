#include "counter.h"

// Trust me, this is easier than rewriting this function.

YS::LIMIT::destroy_t YS::LIMIT::destroy = SignatureScan<YS::LIMIT::destroy_t>("\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xF9\x8B\x09\xE8\x00\x00\x00\x00\x48\x8B\xC8\xE8", "xxxxxxxxxxxxxxxx????xxxx");
YS::MISSION_COUNT::destroy_t YS::MISSION_COUNT::destroy = ResolveRelativeAddress<YS::MISSION_COUNT::destroy_t>(reinterpret_cast<char*>(YS::LIMIT::destroy), 0xF9);
dk::INFORMATION::forceLeaveCounter_t dk::INFORMATION::forceLeaveCounter = ResolveRelativeAddress<dk::INFORMATION::forceLeaveCounter_t>(reinterpret_cast<char*>(YS::MISSION_COUNT::destroy), 0x13);
dk::COUNTER::forceLeave_t dk::COUNTER::forceLeave = ResolveRelativeAddress<dk::COUNTER::forceLeave_t>(reinterpret_cast<char*>(dk::INFORMATION::forceLeaveCounter), 0x1B);

void dk::COUNTER::update(char* counter)
{
	dk::INFO_BASE::update(counter);

	if (*reinterpret_cast<uint32_t*>(counter + 0x1CC0) == 0x01)
	{
		if (!dk::Obj2D::isExist(counter + 0x0300))
			dk::COUNTER::forceLeave(counter);
	}

	else
	{
		dk::Sprite::setPos(counter + 0x0300, *reinterpret_cast<int*>(counter + 0x0028), *reinterpret_cast<int*>(counter + 0x002C));

		auto _comboCurrent = **reinterpret_cast<int**>(counter + 0x1C98);
		auto _maxComboCurrent = **reinterpret_cast<int**>(counter + 0x1CA8);
		*reinterpret_cast<int*>(counter + 0x1CA0) = _comboCurrent;
		*reinterpret_cast<int*>(counter + 0x1CB0) = _maxComboCurrent;

		auto _comboDisplay = *reinterpret_cast<int*>(counter + 0x1CA4);

		if (_comboDisplay != _comboCurrent)
		{
			char _numSeqDictionary[0x0A] = { 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15 };
			char _arraySequenceNum[0x05] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

			vector<char> _arrayFinalSequence;

			int _sequenceNumOffsetX = 0x28;

			sprintf(_arraySequenceNum, "%d", _comboCurrent);

			auto _articleCount = *reinterpret_cast<int*>(counter + 0x1CB8) - 1;
			auto _indexFetcher = 0;

			for (int i = 0; i < 5; i++)
			{
				if (!_arraySequenceNum[i])
					_arrayFinalSequence.push_back(0x0C);

				else
					_arrayFinalSequence.insert(_arrayFinalSequence.begin(), _numSeqDictionary[_arraySequenceNum[i] - 0x30]);
			}

			for (int i = _articleCount; i >= 0; i--)
			{
				*reinterpret_cast<uint32_t*>(counter + 0x04D4 + 0x01F8 * (i + 2)) = _arrayFinalSequence[i];
				YI::SEQUENCE::SetNumberForce(counter + 0x0320 + 0x01F8 * (i + 2), _arrayFinalSequence[i]);

				*(counter + 0x04F0 + 0x01F8 * (i + 2)) = 0x00;

				*reinterpret_cast<uint32_t*>(counter + 0x0310 + 0x01F8 * (i + 2)) &= ~0x40;
			}

			if (_articleCount >= 1)
			{
				for (int i = _articleCount; i >= 1; i--)
				{
					if (_arrayFinalSequence[i] != 0x0C)
						break;

					if (*reinterpret_cast<int*>(counter + 0x1CBC) == 0x02)
						*reinterpret_cast<uint32_t*>(counter + 0x0310 + 0x01F8 * (i + 2)) |= 0x40;

					else
					{
						*reinterpret_cast<uint32_t*>(counter + 0x04D4 + 0x01F8 * (i + 2)) = 0x16;
						YI::SEQUENCE::SetNumberForce(counter + 0x0320 + 0x01F8 * (i + 2), 0x16);

						*(counter + 0x04F0 + 0x01F8 * (i + 2)) = 0x00;
					}
				}
			}
		}

		*reinterpret_cast<int*>(counter + 0x1CA4) = *reinterpret_cast<int*>(counter + 0x1CA0);
		*reinterpret_cast<int*>(counter + 0x1CB4) = *reinterpret_cast<int*>(counter + 0x1CB0);
	}
}

dk::COUNTER::staticInitializer initialize;