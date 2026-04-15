#include "spritemessage.h"

uint32_t dk::SpriteMessage::drawMessage(char* Sprite)
{
	auto _fetchHudDraw = YS::PANACEA_ALLOC::Get("IS_HUDDRAW");
	auto _isHudDraw = true;

	if (_fetchHudDraw)
		memcpy(&_isHudDraw, _fetchHudDraw, 0x01);

	if (!_isHudDraw && !*YS::MENU::IsMenu)
		return NULL;

	char _messageDrawInst[0x100];

	int _activeX = 0;
	int _activeY = 0;

	auto _fetchObject = reinterpret_cast<char*>(*reinterpret_cast<uint64_t*>(Sprite + 0x01E0));
	auto _fetchSequence = reinterpret_cast<char*>(*reinterpret_cast<uint64_t*>(Sprite + 0x01E8));
	auto _fetchMessage = reinterpret_cast<char*>(*reinterpret_cast<uint64_t*>(Sprite + 0x01F8));

	auto _didObjectInit = *reinterpret_cast<uint32_t*>(Sprite + 0x0010) & 0x0800;
	auto _canObjectDraw = *reinterpret_cast<uint32_t*>(Sprite + 0x0010) & 0x1000;
	auto _hasParamXY = *reinterpret_cast<uint32_t*>(Sprite + 0x0010) & 0x0010;

	auto _isColored = *reinterpret_cast<uint32_t*>(Sprite + 0x0010) & 0x0004;
	auto _isColorSpecial = *reinterpret_cast<uint32_t*>(Sprite + 0x0010) & 0x0008;

	if (_didObjectInit != 0x0000)
	{
		if (_fetchObject)
			if (!dk::Obj2D::isExist(_fetchObject))
				return NULL;

		if (_fetchSequence)
		{
			auto _canSequenceDraw = *reinterpret_cast<uint32_t*>(_fetchSequence + 0x0140) - 1 <= 0x01;

			if (!_canSequenceDraw)
				return NULL;
		}
	}

	_activeX = YI::SEQUENCE::GetActiveX(Sprite + 0x0020) + *reinterpret_cast<int*>(Sprite + 0x01C8);
	_activeY = YI::SEQUENCE::GetActiveY(Sprite + 0x0020) + *reinterpret_cast<int*>(Sprite + 0x01CC);

	if (_hasParamXY != 0x0000)
	{
		_activeX += YI::SEQUENCE::GetParamX(Sprite + 0x0020);
		_activeY += YI::SEQUENCE::GetParamY(Sprite + 0x0020);
	}

	uint32_t _activeRGBA = *reinterpret_cast<int*>(Sprite + 0x0208);
	
	if (_isColored != 0x0000)
	{
		auto _fetchRGBA = YI::SEQUENCE::GetActiveRGBA(Sprite + 0x0020);

		if (_isColorSpecial != 0x0000)
			_fetchRGBA ^= (_activeRGBA ^ _fetchRGBA) & 0xFFFFFF;

		auto _valueA = static_cast<int>(((_fetchRGBA & 0xFF000000) >> 0x18) * *reinterpret_cast<float*>(Sprite + 0x0188));
		auto _valueB = static_cast<int>(((_fetchRGBA & 0x00FF0000) >> 0x10) * *reinterpret_cast<float*>(Sprite + 0x017C));
		auto _valueG = static_cast<int>(((_fetchRGBA & 0x0000FF00) >> 0x08) * *reinterpret_cast<float*>(Sprite + 0x0180));
		auto _valueR = static_cast<int>((_fetchRGBA & 0x000000FF) * *reinterpret_cast<float*>(Sprite + 0x0184));
		 
		_valueA = _valueA > 0xFF ? 0xFF : _valueA;
		_valueR = _valueR > 0xFF ? 0xFF : _valueR;
		_valueG = _valueG > 0xFF ? 0xFF : _valueG;
		_valueB = _valueB > 0xFF ? 0xFF : _valueB;

		_activeRGBA = _valueR | ((_valueG | ((_valueB | (_valueA << 8)) << 8)) << 8);
	}

	if (_fetchMessage)
	{
		auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
		auto _offsetValue = 85;

		if (_fetchMemory)
			memcpy(&_offsetValue, _fetchMemory, 0x04);

		auto _fetchAspect = *reinterpret_cast<int*>(Sprite + 0x0218);
		auto _applyAspect = _fetchAspect == 0x00 ? 0x00 : (_fetchAspect > 0x00 ? _offsetValue : _offsetValue * -1);

		auto _objectStart = Sprite + 0x0020;

		auto _fetchCurrentX = _activeX + *reinterpret_cast<int*>(Sprite + 0x01C0) + static_cast<int>(*reinterpret_cast<float*>(Sprite + 0x0200));
		auto _fetchCurrentY = _activeY + *reinterpret_cast<int*>(Sprite + 0x01C4) + static_cast<int>(*reinterpret_cast<float*>(Sprite + 0x0204));

		YS::MESSAGEDRAW::_MESSAGEDRAW(_messageDrawInst, *reinterpret_cast<int*>(Sprite + 0x0210), _fetchMessage, *reinterpret_cast<int*>(Sprite + 0x020C), *reinterpret_cast<int*>(Sprite + 0x0214));
		YS::MESSAGEDRAW::set_line_space(_messageDrawInst, YI::SEQUENCE::GetParamCr(Sprite + 0x0020));
		YS::MESSAGEDRAW::draw(_messageDrawInst, _fetchCurrentX + _applyAspect, _fetchCurrentY, _activeRGBA);
	}

	return _activeRGBA;
}

dk::SpriteMessage::staticInitializer initialize;