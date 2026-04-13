#include "mission_gauge.h"

dk::MISSION_GAUGE::create_t dk::MISSION_GAUGE::create = SignatureScan<dk::MISSION_GAUGE::create_t>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x50\x48\x8B\x84\x24", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

void dk::MISSION_GAUGE::updateGauge(char* missionGauge)
{
	auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
	auto _offsetValue = 85;

	if (_fetchMemory)
		memcpy(&_offsetValue, _fetchMemory, 0x04);

	auto _fetchMission = YS::CACHE_BUFF::SearchByName("msn/us/EH14_MS103.bar", -1) || YS::CACHE_BUFF::SearchByName("msn/us/EH26_MS108.bar", -1);

	if (_fetchMission && _offsetValue > 85)
		_offsetValue = 85;

	auto _applyAspect = 0x00;

	auto _factorFloat = *reinterpret_cast<float*>(missionGauge + 0x0CD8);
	auto _fetchParamEx = YI::SEQUENCE::GetParamEx(missionGauge + 0x0908);
	 
	YI::SEQUENCE::SetScaleX(missionGauge + 0x0518, *reinterpret_cast<float*>(missionGauge + 0x0CE8) / _factorFloat);
	YI::SEQUENCE::SetScaleX(missionGauge + 0x0710, *reinterpret_cast<float*>(missionGauge + 0x0CE8) / _factorFloat);

	*reinterpret_cast<int*>(missionGauge + 0x08E8 + 0x01C8) = static_cast<int>((*reinterpret_cast<float*>(missionGauge + 0x0CE8) / _factorFloat) * static_cast<float>(_fetchParamEx));
	*reinterpret_cast<int*>(missionGauge + 0x08E8 + 0x01CC) = 0x00;

	auto _fetchActiveX = YI::SEQUENCE::GetActiveX(missionGauge + 0x160);
	auto _fetchNumber = *reinterpret_cast<uint32_t*>(missionGauge + 0x160 + 0x158);

	switch (_fetchNumber)
	{
		case 0x10:
			_applyAspect = _offsetValue * -1;
			break;
		case 0x11:
			_applyAspect = _offsetValue;
			break;
		default:
			_applyAspect = 0x00;
			break;
	}

	if (YI::SEQUENCE::GetParamSize(missionGauge + 0x160) == -1)
		_applyAspect = -85;

	*reinterpret_cast<int*>(missionGauge + 0x0028) = _applyAspect + _fetchActiveX;
}

dk::MISSION_GAUGE::staticInitializer initialize;