// @file: util.cpp
//
// @brief: Cpp file for utility functions used throughout the project.

#include "stdafx.h"
#include "Engine/Core/util.h"

STRCODE GetHashCode(const char* str)
{
	const unsigned int fnv_prime = 0x811C9DC5;
	unsigned int hash = 0;
	char end = '\0';

	for (const char* ref = str; *ref != end; ref++)
	{
		hash *= fnv_prime;
		hash ^= *ref;
	}

	return hash;
}

void CreateUUID(UUID* uid)
{
	UuidCreate(uid);
}

std::string GUIDTostring(UUID& guid)
{
	char guid_string[37];
	sprintf_s(
		guid_string,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	return guid_string;
}

STRCODE GUIDToSTRCODE(UUID& guid)
{
	return GetHashCode(GUIDTostring(guid).c_str());
}
