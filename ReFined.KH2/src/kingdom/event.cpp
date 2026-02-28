#define _CRT_SECURE_NO_WARNINGS

#include "event.h"
#include "SigScan.h"

char** YS::EVENT::Event = ResolveRelativeAddress<char**>("\x40\x57\x48\x83\xEC\x20\x48\x8B\xF9\x48\x8B\x0D\x00\x00\x00\x00\x48\x85\xC9\x0F\x84\xAF\x00\x00\x00\xE8\x00\x00\x00\x00\x84\xC0\x0F\x85\xAF\x00\x00\x00\x48\x89\x5C\x24\x30\x48\x8B\x1D\x00\x00\x00\x00\x48\x89\x74\x24\x38", "xxxxxxxxxxxx????xxxxxxxxxx????xxxxx???xxxxxxxx????xxxxx", 0x0C);

sa::EVENT::get_read_wk_motion_t sa::EVENT::get_read_wk_motion = ResolveFunctionFromCall<sa::EVENT::get_read_wk_motion_t>("\x40\x53\x55\x56\x57\x41\x56\x48\x83\xEC\x70\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x60\x41", "xxxxxxxxxxxxxx????xxxxxxxxx", 0x25);
sa::EVENT::get_read_wk_audio_t sa::EVENT::get_read_wk_audio = ResolveFunctionFromCall<sa::EVENT::get_read_wk_audio_t>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x8B\xEA\x48\x8B\xF9\xBA\x03\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x1E);

sa::EVENT::getSkeletonName_t sa::EVENT::getSkeletonName = ResolveFunctionFromCall<sa::EVENT::getSkeletonName_t>("\x40\x53\x55\x56\x57\x41\x56\x48\x83\xEC\x70\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x60\x41", "xxxxxxxxxxxxxx????xxxxxxxxx", 0x4D);

sa::EVENT::readBuffAlloc_t sa::EVENT::readBuffAlloc = ResolveFunctionFromCall<sa::EVENT::readBuffAlloc_t>("\x40\x53\x55\x56\x57\x41\x56\x48\x83\xEC\x70\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x60\x41", "xxxxxxxxxxxxxx????xxxxxxxxx", 0x259);

YS::FILE::ReadCallback sa::EVENT::ReadMotionCallback = ResolveRelativeAddress<YS::FILE::ReadCallback>("\x40\x53\x55\x56\x57\x41\x56\x48\x83\xEC\x70\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x60\x41", "xxxxxxxxxxxxxx????xxxxxxxxx", 0x268);
YS::FILE::ReadCallback sa::EVENT::ReadAudioCallback = ResolveRelativeAddress<YS::FILE::ReadCallback>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x8B\xEA\x48\x8B\xF9\xBA\x03\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x107);

char* sa::EVENT::MotionFilename = ResolveRelativeAddress<char*>("\x40\x53\x55\x56\x57\x41\x56\x48\x83\xEC\x70\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x60\x41", "xxxxxxxxxxxxxx????xxxxxxxxx", 0x9F);
char* sa::EVENT::AudioFilename = ResolveRelativeAddress<char*>("\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x20\x8B\xEA\x48\x8B\xF9\xBA\x03\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x65);

char* sa::EVENT::audio_read_set(char* name, int allocType)
{
	auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);

	string _encodedRegion = _fetchConfig & 0x0004 ? "jp" :
		(_fetchConfig & 0x0008 ? "es" :
			(_fetchConfig & 0x0010 ? "de" :
				(_fetchConfig & 0x0020 ? "bg" : "us")));

	string _constructPath = "voice/%s/event/%s.win32.scd";

	auto _wk = sa::EVENT::get_read_wk_audio(name, 0x03);
	
	if (!YS::REGION::Get() || YS::REGION::Get() == 0x07)
	{
		_constructPath = "voice/fm/event/%s.win32.scd";

		sprintf(sa::EVENT::AudioFilename, _constructPath.c_str(), name);
	}

	else
		sprintf(sa::EVENT::AudioFilename, _constructPath.c_str(), _encodedRegion.c_str(), name);

	if (!YS::FILE::GetSize(sa::EVENT::AudioFilename))
		sprintf(sa::EVENT::AudioFilename, _constructPath.c_str(), "us", name);

	strcpy(_wk + 0x3C, sa::EVENT::AudioFilename);

	char* _buffAlloc = sa::EVENT::readBuffAlloc(sa::EVENT::AudioFilename, _wk, allocType);
	*reinterpret_cast<uint64_t*>(_wk + 0x18) = reinterpret_cast<uint64_t>(_buffAlloc);

	return YS::FILE::ReadBarBack(sa::EVENT::AudioFilename, _buffAlloc, sa::EVENT::ReadAudioCallback, reinterpret_cast<uint32_t*>(_wk));
}

char* sa::EVENT::motion_read_set(char* name, int allocType, uint32_t entryId, int put_id, uint8_t deleteFlag)
{
	auto _fetchConfig = *reinterpret_cast<const uint16_t*>(YS::AREA::SaveData + 0x41A6);

	string _encodedRegion = _fetchConfig & 0x0004 ? "jp" :
						   (_fetchConfig & 0x0008 ? "es" :
					 	   (_fetchConfig & 0x0010 ? "de" :
						   (_fetchConfig & 0x0020 ? "bg" : "us")));

	string _pathPrefix = _fetchConfig & 0x0200 ? "anm_2nd" : (_fetchConfig & 0x0400 ? "anm_3rd" : "anm");

	string _nameString = string(name);
	string _constructPath = "%s/%s/%s/%s/%s.anb";

	auto _wk = sa::EVENT::get_read_wk_motion();

	*reinterpret_cast<uint32_t*>(_wk) = 0x01;
	*reinterpret_cast<uint32_t*>(_wk + 0x04) = 0x01;

	*reinterpret_cast<uint32_t*>(_wk + 0x0C) = put_id;
	*reinterpret_cast<uint32_t*>(_wk + 0x10) = entryId;

	*reinterpret_cast<uint8_t*>(_wk + 0x08) = deleteFlag;

	auto _skeletonName = sa::EVENT::getSkeletonName(entryId);

	strcpy(_wk + 0x2C, _skeletonName);
	strcpy(_wk + 0x3C, name);

	fill(sa::EVENT::MotionFilename, sa::EVENT::MotionFilename + 0x30, 0x00);

	auto _sizeWorld = _nameString.find('/') - 0x04;
	auto _sizeAnimation = _nameString.size() - _nameString.find('/');

	auto _fetchWorld = _nameString.substr(0x04, _sizeWorld);
	auto _fetchAnimation = _nameString.substr(_nameString.find('/') + 1, _sizeAnimation);

	if (!YS::REGION::Get() || YS::REGION::Get() == 0x07)
	{
		_constructPath = "%s/fm/%s/%s/%s.anb";
		sprintf(sa::EVENT::MotionFilename, _constructPath.c_str(), _pathPrefix.c_str(), _fetchWorld.c_str(), _skeletonName, _fetchAnimation.c_str());

		if (!YS::FILE::GetSize(sa::EVENT::MotionFilename))
			sprintf(sa::EVENT::MotionFilename, _constructPath.c_str(), "anm", _fetchWorld.c_str(), _skeletonName, _fetchAnimation.c_str());

	}

	else
		sprintf(sa::EVENT::MotionFilename, _constructPath.c_str(), _pathPrefix.c_str(), _encodedRegion.c_str(), _fetchWorld.c_str(), _skeletonName, _fetchAnimation.c_str());

	if (!YS::FILE::GetSize(sa::EVENT::MotionFilename))
		sprintf(sa::EVENT::MotionFilename, _constructPath.c_str(), _pathPrefix.c_str(), "us", _fetchWorld.c_str(), _skeletonName, _fetchAnimation.c_str());

	if (!YS::FILE::GetSize(sa::EVENT::MotionFilename))
		sprintf(sa::EVENT::MotionFilename, _constructPath.c_str(), "anm", _encodedRegion.c_str(), _fetchWorld.c_str(), _skeletonName, _fetchAnimation.c_str());

	if (!YS::FILE::GetSize(sa::EVENT::MotionFilename))
		sprintf(sa::EVENT::MotionFilename, _constructPath.c_str(), "anm", "us", _fetchWorld.c_str(), _skeletonName, _fetchAnimation.c_str());

	char* _buffAlloc = sa::EVENT::readBuffAlloc(sa::EVENT::MotionFilename, _wk, allocType);
	*reinterpret_cast<uint64_t*>(_wk + 0x18) = reinterpret_cast<uint64_t>(_buffAlloc);

	return YS::FILE::ReadBarBack(sa::EVENT::MotionFilename, _buffAlloc, sa::EVENT::ReadMotionCallback, reinterpret_cast<uint32_t*>(_wk));
}
