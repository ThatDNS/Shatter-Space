// MIT License
// 
// Copyright(c) 2016 William Barry
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this softwareand associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright noticeand this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#ifndef _STANDARDTYPES_H_
#define _STANDARDTYPES_H_
#pragma once

#include <cstdlib>

#define STRCODE						unsigned int
#define	NoName						0xf06a6692

#define	MATH_HALF_PI				(1.5707963267948966192313216916398f)
#define	MATH_PI						(3.1415926535897932384626433832795f)
#define MATH_2PI					(6.283185307179586476925286766559f)
#define	MATH_180_OVER_PI			(57.295779513082320876798154814105f)
#define	MATH_PI_OVER_180			(0.01745329251994329576923690768489f)

#define PHYSICS_PIXEL_CONVERSION	0.01f
#define PIXEL_PHYSICS_CONVERSION	100.f
#define PIXEL_TO_METER(pixel)		(pixel * PHYSICS_PIXEL_CONVERSION) 
#define METER_TO_PIXEL(meter)		(meter * PIXEL_PHYSICS_CONVERSION)

#define	RAD_TO_DEG(rad)				(rad * MATH_180_OVER_PI)
#define	DEG_TO_RAD(deg)				(deg * MATH_PI_OVER_180)

///-------------------------------------------------------------------------------------------------
/// <summary>
/// Debug LOG that does nothing in release
/// </summary>
///-------------------------------------------------------------------------------------------------
#ifdef _DEBUG
	#define LOG(out) std::cout << out << std::endl;
#else
	#define LOG(out)
#endif

///-------------------------------------------------------------------------------------------------
/// <summary>	
/// Generates a hash code using FVN-1 
/// (https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function)
/// </summary>
///-------------------------------------------------------------------------------------------------
inline STRCODE GetHashCode(const char* str)
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

/// <summary>
/// Create a UUID
/// </summary>
/// <param name="uid"></param>
inline void CreateUUID(UUID* uid)
{
	UuidCreate(uid);
}

/// <summary>
/// Create UUID from string
/// </summary>
/// <param name="guid"></param>
/// <returns></returns>
inline std::string GUIDTostring(UUID& guid)
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

/// <summary>
/// Convert UUID to STRCODE
/// </summary>
/// <param name="guid"></param>
/// <returns></returns>
inline STRCODE GUIDToSTRCODE(UUID& guid)
{
	return GetHashCode(GUIDTostring(guid).c_str());
}


/// <summary>
/// Runtime exception macro to test condition
/// </summary>
#define THROW_RUNTIME_ERROR(condition, message)			\
	if (condition)										\
	{													\
		throw std::runtime_error(message);				\
	}

#ifndef NDEBUG
///-------------------------------------------------------------------------------------------------
/// <summary>	A macro that defines assert. </summary>
///
/// <remarks>	William Barry, 05/02/2013. </remarks>
///
/// <param name="condition">	The condition. </param>
/// <param name="message">  	The message. </param>
///-------------------------------------------------------------------------------------------------
#define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::exit(EXIT_FAILURE); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

/// <summary>
/// Macro to declare a singleton
/// </summary>
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

#endif

