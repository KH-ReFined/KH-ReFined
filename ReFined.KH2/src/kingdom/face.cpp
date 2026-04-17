#include "face.h"

dk::FACE::getFaceSed_t dk::FACE::getFaceSed = SignatureScan<dk::FACE::getFaceSed_t>("\x40\x55\x48\x83\xEC\x20\xC6\x81\x04\x03\x00\x00\x00\x49\x8B\xC0", "xxxxxxxxxxxxxxxx");

void dk::FACE::create(char* face, int priority, int type, char* object, int status, int group, int offset16x9)
{
	*reinterpret_cast<uint32_t*>(face + 0x300) = type;
	char* _fetchFaceSqd = nullptr;

	if (dk::FACE::getFaceSed(face, &_fetchFaceSqd, object) == 1)
	{
		auto _statusCheck = 0x00;

		if (!status || status != 0x01 && (_statusCheck = 0x02, status != 0x02))
			_statusCheck = 0x03;

		auto _fetchObjectBinarc = *reinterpret_cast<char**>(object + 0x0928);

		dk::Sprite::create(face, priority, _fetchFaceSqd, face + 0x200, _statusCheck, group, offset16x9);

		if (YS::BINARC::get_info_by_tag(_fetchObjectBinarc, 0x18, 0x6E777263, 0) != 0x00)
		{
			auto _allocCrown = (char*)malloc(0x300);
			memset(_allocCrown, 0x00, 0x300);

			auto _addressCrownIMD = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(*reinterpret_cast<uint32_t*>(YS::BINARC::get_info_by_tag(_fetchObjectBinarc, 0x18, 0x6E777263, 0) + 0x08)));
			auto _addressCrownSQD = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(*reinterpret_cast<uint32_t*>(YS::BINARC::get_info_by_tag(_fetchObjectBinarc, 0x19, 0x6E777263, 0) + 0x08)));

			dk::Sprite::_Sprite(_allocCrown);

			YI::IMAGE::_IMAGE(_allocCrown + 0x200);
			YI::IMAGE::Init(_allocCrown + 0x200, _addressCrownIMD);

			*reinterpret_cast<uint32_t*>(_allocCrown + 0x210) = *reinterpret_cast<uint32_t*>(face + 0x210);
			*reinterpret_cast<uint32_t*>(_allocCrown + 0x22C) = *reinterpret_cast<uint32_t*>(face + 0x22C);

			reinterpret_cast<void(*)(char*)>(moduleInfo.startAddr + 0x191010)(_allocCrown + 0x200);

			YI::IMAGE::InitLoadImage(_allocCrown + 0x200);

			dk::Sprite::create(_allocCrown, priority, _addressCrownSQD, _allocCrown + 0x200, status, group, offset16x9);

			*reinterpret_cast<char**>(face + 0x0308) = _allocCrown;
		}
	}

	else
	{
		dk::Obj2D::create(face, priority, group);

		*reinterpret_cast<uint32_t*>(face + 0x1DC) = offset16x9;
		*reinterpret_cast<uint32_t*>(face + 0x1D8) = UINT32_MAX;

		dk::Sprite::initWork(face);
	}
}

void dk::FACE::update(char* face)
{
	auto _fetchCrown = *reinterpret_cast<uint32_t*>(face + 0x0308);
	auto _fetchIsDraw = *(face + 0x304);

	if (_fetchIsDraw)
		dk::Sprite::update(face);
}

void dk::FACE::draw(char* face)
{
	auto _fetchCrown = *reinterpret_cast<char**>(face + 0x0308);
	auto _fetchCrownIMD = *reinterpret_cast<char**>(face + 0x0310);
	auto _fetchIsDraw = *(face + 0x304);

	if (_fetchIsDraw)
	{
		YI::IMAGE::LoadTexture(face + 0x200);
		dk::Sprite::draw(face);

		if (_fetchCrown)
		{
			auto _fetchCacheId = *reinterpret_cast<int*>(_fetchCrown + 0x02F4);

			auto _fetchFaceNum = *reinterpret_cast<uint32_t*>(face + 0x1D4);
			auto _fetchFaceTime = *reinterpret_cast<float*>(face + 0x168);

			if (*reinterpret_cast<uint32_t*>(_fetchCrown + 0x1D4) != _fetchFaceNum || _fetchFaceTime == 0x00)
				dk::Sprite::setNum(_fetchCrown, _fetchFaceNum);

			*reinterpret_cast<int*>(_fetchCrown + 0x1C0) = *reinterpret_cast<int*>(face + 0x1C0);
			*reinterpret_cast<int*>(_fetchCrown + 0x1C4) = *reinterpret_cast<int*>(face + 0x1C4);

			YI::IMAGE::LoadTexture(_fetchCrown + 0x200);
			dk::Sprite::draw(_fetchCrown);
		}
	}
}

dk::FACE::staticInitializer initialize;