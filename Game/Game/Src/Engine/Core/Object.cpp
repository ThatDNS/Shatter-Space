// @file: Object.cpp
//
// @brief: Cpp file for the Object.

#include "stdafx.h"
#include "Engine/Core/Object.h"

IMPLEMENT_ABSTRACT_CLASS(Object)

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

Object::Object(std::string& guid, std::string& name) : guid(guid), name(name)
{
    uid = GetHashCode(this->guid.c_str());
}

