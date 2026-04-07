#include "sprite.h"

char* VTABLE_SPRITE = ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x41\x56\x41\x57\x48\x83\xEC\x20\xE8\x00\x00\x00\x00\x48\x8B", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx", 0x80);
char* VIEWPORT_ADDR = ResolveRelativeAddress<char*>("\x48\x8B\xC4\x57\x41\x56\x41\x57\x48\x81\xEC\x50\x01\x00\x00\x48\xC7\x44\x24\x20\xFE\xFF\xFF\xFF\x48\x89\x58\x10\x48\x89\x68\x18\x48\x89\x70\x20\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x40\x01\x00\x00\x48\x8B\xE9\x33\xD2\x41\xB8\x00\x01\x00\x00\x48\x8D\x4C\x24\x30\xE8\x00\x00\x00\x00\x45\x33\xFF", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxxxxxxxxxxxxxxxxxxxxxxxxxxx????xxx", 0x311);

dk::Sprite::setNum_t dk::Sprite::setNum = SignatureScan<dk::Sprite::setNum_t>("\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x89\x91\xD4\x01\x00\x00\x48", "xxxxxxxxxxxxxxxx");
dk::Sprite::setPos_t dk::Sprite::setPos = ResolveFunctionFromCall<dk::Sprite::setPos_t>("\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x57\x48\x83\xEC\x40\x48\x8B\xF9\x8B\xF2\x48\x83\xC1\x38\xE8\x00\x00\x00\x00\x84\xC0\x00\x00\x39\xB7\xAC\x0D\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxx????xx??xxxxxx", 0xBB);

void dk::Sprite::_Sprite(char* self)
{
	*reinterpret_cast<uint64_t*>(self) = reinterpret_cast<uint64_t>(VTABLE_SPRITE);
}

void dk::Sprite::initWork(char* Sprite)
{
	memset(Sprite + 0x1C0, 0x00, 0x10);
	*reinterpret_cast<float*>(Sprite + 0x1D0) = 1.0;

	*reinterpret_cast<uint64_t*>(Sprite + 0x1E0) = 0x00;
	*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;

	if (*reinterpret_cast<uint64_t*>(Sprite + 0x1E0))
	{
		*reinterpret_cast<uint64_t*>(Sprite + 0x1E8) = 0x00;
		*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;

		while (*reinterpret_cast<uint64_t*>(Sprite + 0x1E8))
		{
			*reinterpret_cast<uint64_t*>(Sprite + 0x1E0) = 0x00;
			*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;

			if (!*reinterpret_cast<uint64_t*>(Sprite + 0x1E0))
				break;

			*reinterpret_cast<uint64_t*>(Sprite + 0x1E8) = 0x00;
			*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;
		}
	}

	*reinterpret_cast<uint64_t*>(Sprite + 0x1E8) = 0x00;
	*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;

	if (*reinterpret_cast<uint64_t*>(Sprite + 0x1E8))
	{
		*reinterpret_cast<uint64_t*>(Sprite + 0x1E0) = 0x00;
		*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;

		if (*reinterpret_cast<uint64_t*>(Sprite + 0x1E0))
		{
			*reinterpret_cast<uint64_t*>(Sprite + 0x1E8) = 0x00;
			*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;

			while (*reinterpret_cast<uint64_t*>(Sprite + 0x1E8))
			{
				*reinterpret_cast<uint64_t*>(Sprite + 0x1E0) = 0x00;
				*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;

				if (!*reinterpret_cast<uint64_t*>(Sprite + 0x1E0))
					break;

				*reinterpret_cast<uint64_t*>(Sprite + 0x1E8) = 0x00;
				*reinterpret_cast<uint32_t*>(Sprite + 0x010) |= 0x0800;
			}
		}
	}
	
	*(Sprite + 0x1F1) = 0x00;
}

void dk::Sprite::create(char* Sprite, int priority, char* seqd, char* image, int num, int group, int offset16x9)
{
	dk::Obj2D::create(Sprite, priority, group);
	dk::Sprite::initWork(Sprite);

	YI::SEQUENCE::Init(Sprite + 0x20, seqd, image);

	*reinterpret_cast<uint32_t*>(Sprite + 0x1D4) = num;
	YI::SEQUENCE::SetNumberForce(Sprite + 0x20, num);

	*(Sprite + 0x1F0) = 0x00;
	*reinterpret_cast<uint32_t*>(Sprite + 0x1DC) = offset16x9;
	*reinterpret_cast<uint32_t*>(Sprite + 0x1D8) = UINT32_MAX;
}

void dk::Sprite::draw(char* Sprite)
{
	int _activeX = 0;
	int _activeY = 0;

	auto _fetchObject = reinterpret_cast<char*>(*reinterpret_cast<uint64_t*>(Sprite + 0x01E0));
	auto _fetchSequence = reinterpret_cast<char*>(*reinterpret_cast<uint64_t*>(Sprite + 0x01E8));

	auto _didObjectInit = *reinterpret_cast<uint32_t*>(Sprite + 0x0010) & 0x0800;
	auto _canObjectDraw = *reinterpret_cast<uint32_t*>(Sprite + 0x0010) & 0x1000;

	if (_didObjectInit != 0x0000)
	{
		if (_fetchObject)
			if (!dk::Obj2D::isExist(_fetchObject))
				return;

		if (_fetchSequence)
		{
			auto _canSequenceDraw = *reinterpret_cast<uint32_t*>(_fetchSequence + 0x0140) != 0x00;

			if (!_canSequenceDraw)
				return;
		}
	}

	if (_fetchObject)
	{
		_activeX = YI::SEQUENCE::GetActiveX(_fetchObject + 0x0020) + *reinterpret_cast<int*>(_fetchObject + 0x01C8) + *reinterpret_cast<int*>(_fetchObject + 0x01C0);
		_activeY = YI::SEQUENCE::GetActiveY(_fetchObject + 0x0020) + *reinterpret_cast<int*>(_fetchObject + 0x01CC) + *reinterpret_cast<int*>(_fetchObject + 0x01C4);

		*reinterpret_cast<int*>(Sprite + 0x01C0) = _activeX;
		*reinterpret_cast<int*>(Sprite + 0x01C4) = _activeY;
	}

	if (_fetchSequence)
	{
		_activeX = YI::SEQUENCE::GetActiveX(_fetchSequence) + *reinterpret_cast<int*>(_fetchSequence + 0x018C);
		_activeY = YI::SEQUENCE::GetActiveY(_fetchSequence) + *reinterpret_cast<int*>(_fetchSequence + 0x0190);

		*reinterpret_cast<int*>(Sprite + 0x01C0) = _activeX;
		*reinterpret_cast<int*>(Sprite + 0x01C4) = _activeY;
	}

	if ((_fetchObject || _fetchSequence) && (*reinterpret_cast<uint32_t*>(Sprite + 0x0010) & 0x0200) != 0x0000)
	{
		auto _activeRGBA = YI::SEQUENCE::GetActiveRGBA(_fetchObject ? _fetchObject + 0x0020: _fetchSequence);

		auto _factorA = static_cast<float>((_activeRGBA & 0xFF000000) >> 0x18) * 0.0078125;

		auto _factorR = *reinterpret_cast<float*>(Sprite + 0x017C);
		auto _factorG = *reinterpret_cast<float*>(Sprite + 0x0180);
		auto _factorB = *reinterpret_cast<float*>(Sprite + 0x0184);

		if ((*reinterpret_cast<uint32_t*>(Sprite + 0x0010) & 0x0400) == 0x0000)
		{
			_factorB = ((_activeRGBA & 0x00FF0000) >> 0x10) * 0.0078125;
			_factorG = ((_activeRGBA & 0x0000FF00) >> 0x08) * 0.0078125;
			_factorR = (_activeRGBA & 0x000000FF) * 0.0078125;
		}

		YI::SEQUENCE::SetColorRate(Sprite + 0x0020, _factorR, _factorG, _factorB, _factorA);
	}

	if (_canObjectDraw == 0x0000)
	{
		auto _fetchMemory = YS::PANACEA_ALLOC::Get("ASPECT_INFORMATION");
		auto _offsetValue = 85;

		if (_fetchMemory)
			memcpy(&_offsetValue, _fetchMemory, 0x04);

		auto _fetchAspect = *reinterpret_cast<int*>(Sprite + 0x1DC);
		auto _applyAspect = _fetchAspect == 0x00 ? 0x00 : (_fetchAspect > 0x00 ? _offsetValue : _offsetValue * -1);

		auto _objectStart = Sprite + 0x0020;

		auto _fetchCurrentX = *reinterpret_cast<int*>(Sprite + 0x01C8) + *reinterpret_cast<int*>(Sprite + 0x01C0);
		auto _fetchCurrentY = *reinterpret_cast<int*>(Sprite + 0x01CC) + *reinterpret_cast<int*>(Sprite + 0x01C4);

		*reinterpret_cast<int*>(_objectStart + 0x018C) = _fetchCurrentX + _applyAspect;
		*reinterpret_cast<int*>(_objectStart + 0x0190) = _fetchCurrentY;

		YI::SEQUENCE::Draw(Sprite + 0x0020);
	}
}

dk::Sprite::staticInitializer initialize;