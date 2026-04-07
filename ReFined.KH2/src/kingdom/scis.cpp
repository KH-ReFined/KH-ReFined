#include "scis.h"

void dk::Scis::setScis(char* scissor, short x, short y, short width, short height)
{
	auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
	auto _offsetValue = 85;

	if (_fetchMemory)
		memcpy(&_offsetValue, _fetchMemory, 0x04);

	auto _applyAspect = (_offsetValue * -1) + 85;

	long long _calcScis = 0x00;

	_calcScis |= (static_cast<long long>(x + _applyAspect) & 0x000000000000FFFF);
	_calcScis |= (static_cast<long long>(width + (x + _applyAspect) - 1) & 0x000000000000FFFF) << 16;
	_calcScis |= (static_cast<long long>(y) & 0x000000000000FFFF) << 32;
	_calcScis |= (static_cast<long long>(height + y - 1) & 0x000000000000FFFF) << 48;

	*reinterpret_cast<long long*>(scissor + 0x10) = _calcScis;
}

dk::Scis::staticInitializer initialize;