#include "information_window.h"

void dk::INFORMATION_WINDOW::init(char* infoWindow, char* message)
{
	auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
	auto _offsetValue = 85;

	if (_fetchMemory)
		memcpy(&_offsetValue, _fetchMemory, 0x04);

	auto _applyAspect = (_offsetValue * -1) + 85;

	char _messageDrawInst[0x100];

	SOUND::PlaySFX(0x22);
	*reinterpret_cast<int*>(infoWindow + 0x049C) &= ~2;
	*reinterpret_cast<int*>(infoWindow + 0x02B0) &= ~0x0040;
	YS::MESSAGEDRAW::_MESSAGEDRAW(_messageDrawInst, 0x00, message, 0, 0x00);

	int _paramX = YI::SEQUENCE::GetParamX(infoWindow + 0x0060);
	auto _messageEndPoint = *reinterpret_cast<int*>(_messageDrawInst + 0x70);

	*reinterpret_cast<float*>(infoWindow + 0x0240) = static_cast<float>(_paramX);

	*reinterpret_cast<int*>(infoWindow + 0x0498) = _paramX - _messageEndPoint;
	*reinterpret_cast<float*>(infoWindow + 0x04A0) = 180.0;
}

dk::INFORMATION_WINDOW::staticInitializer initialize;