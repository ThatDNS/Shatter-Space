// @file: util.h
//
// @brief: Header file for utility functions used throughout the project.

#pragma once
#ifndef _UTIL_H_
#define _UTIL_H_

// Macro for representing hash code
#define STRCODE unsigned int
// Macro to declare a class as singleton
#define DECLARE_SINGLETON(className)\
public:\
	inline static className& Get() \
	{\
		static className instance;\
		return instance;\
	}\
private:\
	className() = default;\
	~className() = default;\
	inline explicit className(className const&) = delete;\
	inline className& operator=(className const&) = delete;

#pragma comment(lib, "rpcrt4.lib")
#include <windows.h>
#include <string>
#include "Engine/Components/Component.h"

/**
 * @brief Generate a hash code from the given string using FVN-1.
 * (https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function)
 *
 * @param str String to be hashed.
 * @return an unsigned integer.
 */
STRCODE GetHashCode(const char* str);

/*
 * @brief Generate a UUID using UuidCreate.
 * 
 * @param uid Placeholder for the generated UUID.
 */
void CreateUUID(UUID* uid);

/*
 * @brief Convert a UUID/GUID to string.
 *
 * @param guid Input UUID.
 * @return String form of the given UUID.
 */
std::string GUIDTostring(UUID& guid);

/*
 * @brief Convert a UUID/GUID to hash code.
 * Converts it first to string format. Then hashes the string.
 *
 * @param guid Input UUID.
 * @return an unsigned integer
 */
STRCODE GUIDToSTRCODE(UUID& guid);

/*
 * @brief Create a component given the component type.
 *
 * @param componentType ComponentType enum
 * @return Component pointer
 */
Component* CreateComponent(ComponentType);

/*
 * @brief Convert component type value to a string value.
 *
 * @param componentType ComponentType enum
 * @return string value
 */
std::string ComponentTypeToStr(ComponentType);

#endif // !_UTIL_H_
