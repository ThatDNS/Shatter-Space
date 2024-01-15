// @file: Object.cpp
//
// @brief: Cpp file for the Object.

#include "stdafx.h"
#include "Engine/Core/Object.h"

Object::Object()
{
    UUID _uid;
    CreateUUID(&_uid);

    uid = GUIDToSTRCODE(_uid);
    guid = GUIDTostring(_uid);
}

Object::Object(std::string& guid) : guid(guid)
{
    uid = GetHashCode(guid.c_str());
}
