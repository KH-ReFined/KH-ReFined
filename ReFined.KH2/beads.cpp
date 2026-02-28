#include "beads.h"

YS::VM::staticInitializer  YS::VM::initialize;

uint32_t YS::VM::trap_obj_get_entry_id(uint32_t* bdvalue)
{
    if (!bdvalue || (char*)bdvalue > moduleInfo.endAddr || *bdvalue == 0x00 || *bdvalue == UINT32_MAX)
        return 0x00;

    auto _fetchObjectPtr = PC::CONVERTER::INT_TO_LONG_ADDRESS(*bdvalue);

    if (!_fetchObjectPtr || (char*)_fetchObjectPtr > moduleInfo.endAddr)
        return 0x00;

    auto _fetchObjectActual = *reinterpret_cast<uint32_t*>(_fetchObjectPtr + 0x04);

    if (_fetchObjectActual == 0x00 || _fetchObjectActual == UINT32_MAX)
        return 0x00;

    auto _acutalObjectPtr = PC::CONVERTER::INT_TO_LONG_ADDRESS(_fetchObjectActual);

    if (!_acutalObjectPtr || (char*)_acutalObjectPtr > moduleInfo.endAddr)
        return 0x00;

    auto _fetchEntryPtr = PC::CONVERTER::INT_TO_LONG_ADDRESS(*reinterpret_cast<uint32_t*>(_acutalObjectPtr + 0x08));
    auto _fetchEntryId = *reinterpret_cast<uint32_t*>(_fetchEntryPtr);

    *bdvalue = _fetchEntryId;
    bdvalue[1] = 1414416704;

    return _fetchEntryId;
}

uint32_t YS::VM::trap_obj_effect_start_bind(uint32_t* bdvalue)
{
    auto _id = bdvalue[2];
    auto _flag = bdvalue[4];
    auto _priority = bdvalue[6];

    auto _fetchArg1 = *bdvalue;

    if (_fetchArg1 == 0x00 || _fetchArg1 == UINT32_MAX)
        return 0x00;

    auto _objectPtr = PC::CONVERTER::INT_TO_LONG_ADDRESS(_fetchArg1);

    if (!_objectPtr || (char*)_objectPtr > moduleInfo.endAddr)
        return 0x00;

    auto _fetchArg2 = *reinterpret_cast<uint32_t*>(_objectPtr + 0x04);

    if (_fetchArg2 == 0x00 || _fetchArg2 == UINT32_MAX)
        return 0x00;

    auto _objectActual = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(_fetchArg2));

    if (!_objectActual || _objectActual > moduleInfo.endAddr)
        return 0x00;

    auto _fetchPAX = reinterpret_cast<char*>(_objectActual + 0x80);

    if (!_fetchPAX)
        return 0x00;

    if (*reinterpret_cast<uint64_t*>(_fetchPAX) == 0x00)
        return 0x00;

    auto _paxReturn = ryj::PAX::StartBind(_fetchPAX, _id, _flag, 0x01, _priority, _objectActual);
    auto _dwordReturn = PC::CONVERTER::LONG_TO_INT_ADDRESS(reinterpret_cast<uint64_t>(_paxReturn));

    *bdvalue = _dwordReturn;
    bdvalue[1] = 1380204864;

    return _dwordReturn;
}

uint32_t YS::VM::trap_obj_effect_start_bind_other(uint32_t* bdvalue)
{
    auto _id = bdvalue[2];
    auto _targetObj = bdvalue[4];
    auto _flag = bdvalue[6];
    auto _priority = bdvalue[8];

    if (*bdvalue == 0x00 || *bdvalue == UINT32_MAX)
        return 0x00;

    if (_targetObj == 0x00 || _targetObj == UINT32_MAX)
        return 0x00;

    auto _objectPtr = PC::CONVERTER::INT_TO_LONG_ADDRESS(*bdvalue);
    auto _targetObjectPtr = PC::CONVERTER::INT_TO_LONG_ADDRESS(_targetObj);

    if (!_objectPtr || (char*)_objectPtr > moduleInfo.endAddr || !_targetObjectPtr || (char*)_targetObjectPtr > moduleInfo.endAddr)
        return 0x00;

    auto _fetchActual = *reinterpret_cast<uint32_t*>(_objectPtr + 0x04);
    auto _fetchTargetActual = *reinterpret_cast<uint32_t*>(_targetObjectPtr + 0x04);

    if (_fetchActual == 0x00 || _fetchActual == UINT32_MAX || _fetchTargetActual == 0x00 || _fetchTargetActual == UINT32_MAX)
        return 0x00;

    auto _objectActual = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(_fetchActual));
    auto _targetObjectActual = reinterpret_cast<char*>(PC::CONVERTER::INT_TO_LONG_ADDRESS(_fetchTargetActual));

    if (!_objectActual || _objectActual > moduleInfo.endAddr || !_targetObjectActual || _targetObjectActual > moduleInfo.endAddr)
        return 0x00;

    auto _fetchPAX = reinterpret_cast<char*>(_objectActual + 0x80);

    if (!_fetchPAX)
        return 0x00;

    if (*reinterpret_cast<uint64_t*>(_fetchPAX) == 0x00)
        return 0x00;

    auto _paxReturn = ryj::PAX::StartBind(_fetchPAX, _id, _flag, 0x01, _priority, _targetObjectActual);
    auto _dwordReturn = PC::CONVERTER::LONG_TO_INT_ADDRESS(reinterpret_cast<uint64_t>(_paxReturn));

    *bdvalue = _dwordReturn;
    bdvalue[1] = 1380204864;

    return _dwordReturn;
}