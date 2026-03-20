#define _CRT_SECURE_NO_WARNINGS

#include "voice.h"

char* YS::VOICE::Cache = ResolveRelativeAddress<char*>("\x85\xC9\x0F\x84\x93\x01\x00\x00\x41\x56\x48\x83\xEC\x70\x48\x8B", "xxxxxxxxxxxxxxxx", 0x05C);
char** YS::VOICE::CurrentTask = ResolveRelativeAddress<char**>("\x85\xC9\x0F\x84\x93\x01\x00\x00\x41\x56\x48\x83\xEC\x70\x48\x8B", "xxxxxxxxxxxxxxxx", 0x14C);

uint64_t* YS::VOICE::Part = ResolveRelativeAddress<uint64_t*>("\x85\xC9\x0F\x84\x93\x01\x00\x00\x41\x56\x48\x83\xEC\x70\x48\x8B", "xxxxxxxxxxxxxxxx", 0x0F3);

YS::FILE::ReadCallback YS::VOICE::ReadCallback = ResolveRelativeAddress<YS::FILE::ReadCallback>("\x85\xC9\x0F\x84\x93\x01\x00\x00\x41\x56\x48\x83\xEC\x70\x48\x8B", "xxxxxxxxxxxxxxxx", 0x138);

YS::VOICE::staticInitializer YS::VOICE::initialize;

void YS::VOICE::ReadEntryId(uint16_t entryID, char* buff)
{
    auto _fetchConfig = *reinterpret_cast<const uint16_t*>(AREA::SaveData + 0x41A6);

    string _constructPath = _fetchConfig & 0x0004 ? "voice/jp/battle/%s%d_%s.win32.scd" : 
                           (_fetchConfig & 0x0008 ? "voice/es/battle/%s%d_%s.win32.scd" : 
                           (_fetchConfig & 0x0010 ? "voice/de/battle/%s%d_%s.win32.scd" : 
                           (_fetchConfig & 0x0020 ? "voice/bg/battle/%s%d_%s.win32.scd" :"voice/us/battle/%s%d_%s.win32.scd")));

    char _fileNameBuff[48];

    fprintf(stdout, "[YS::VOICE::ReadEntryId] | Requested VOICE read for Object ID: 0x%X\n", entryID);

    if (entryID)
    {
        auto _fetchObject = YS::OBJENTRY::Get(entryID);

        if (_fetchObject)
        {
            auto _entryPart = *reinterpret_cast<uint16_t*>(_fetchObject + 0x4C);

            if (_entryPart <= 0x0E)
            {
                auto _areaInfo = AREAINFO::Get(-1, -1);
                auto _areaVoice = *reinterpret_cast<uint16_t*>(_areaInfo + 0x30);

                uint32_t _queueOffset = 0;

                for (int i = 0; i < 3; i++)
                {
                    auto _currentOffset = 0x0C * i;

                    auto _currentPart = *reinterpret_cast<uint16_t*>(YS::VOICE::Cache + _currentOffset + 0x04);
                    auto _currentVoice = *reinterpret_cast<uint16_t*>(YS::VOICE::Cache + _currentOffset + 0x08);

                    if (_currentPart == _entryPart && _currentVoice == _areaVoice)
                    {
                        fprintf(stdout, "[YS::VOICE::ReadEntryId] | Voice for Object ID 0x%X is already loaded. Aborting!\n", entryID);

                        *reinterpret_cast<uint16_t*>(YS::VOICE::Cache + _currentOffset) |= 1u;
                        return;
                    }
                }

                auto _cacheStart = YS::VOICE::Cache;
                auto _cacheFinish = YS::VOICE::Cache + 0x24;

                while (_cacheStart < _cacheFinish)
                {
                    if ((*reinterpret_cast<uint8_t*>(_cacheStart) & 1) == 0x00)
                        break;

                    _queueOffset++;
                    _cacheStart += 0x0C;
                }

                *reinterpret_cast<uint32_t*>(YS::VOICE::Cache + 0x0C * _queueOffset) |= 1;
                *reinterpret_cast<uint32_t*>(YS::VOICE::Cache + 0x0C * _queueOffset + 0x04) = _entryPart;
                *reinterpret_cast<uint32_t*>(YS::VOICE::Cache + 0x0C * _queueOffset + 0x08) = _areaVoice;

                auto _worldName = WORLD::GetName(AREA::Current->World);

                if (YS::REGION::Get() && YS::REGION::Get() != 0x07)
                {
                    sprintf(_fileNameBuff, _constructPath.c_str(), _worldName, _areaVoice, YS::VOICE::Part[_entryPart]);

                    if (!YS::FILE::GetSize(_fileNameBuff))
                        sprintf(_fileNameBuff, "voice/us/battle/%s%d_%s.win32.scd", _worldName, _areaVoice, YS::VOICE::Part[_entryPart]);

                }

                else
                    sprintf(_fileNameBuff, "voice/jp/battle/%s%d_%s.win32.scd", _worldName, _areaVoice, YS::VOICE::Part[_entryPart]);

                fprintf(stdout, "[YS::VOICE::ReadEntryId] | Currently processing file: %s\n", _fileNameBuff);

                YS::FILE::ReadBack(_fileNameBuff, buff, YS::VOICE::ReadCallback, _queueOffset);

                char* _currTask = *YS::VOICE::CurrentTask;
                *(_currTask + 0x18) = 0x01;

                while (*(_currTask + 0x18))
                {
                    TASK::sleep(_currTask, 0);
                    _currTask = *YS::VOICE::CurrentTask;
                }
            }
        }
    }
}