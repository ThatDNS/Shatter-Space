// @file: Object.h
//
// @brief: Header file for the Object class. Everything in a game is essentially an object.

#pragma once

#ifndef _OBJECT_H_
#define _OBJECT_H_

class Object
{
protected:
	bool isActive = true;
	bool isEntity = false;  // useful in derived class Entity

	std::string guid = "";
	STRCODE uid = 0;

	Object();
	Object(std::string& guid);

public:
	virtual ~Object() = default;

	virtual void Initialize() = 0;
	virtual void Destroy() = 0;

	void SetActive(bool value) { isActive = value; }
	bool IsActive() const { return isActive; }
	const std::string& GetGUID() const { return guid; }
	const STRCODE GetUid() const { return uid; }

	bool IsEntity() const { return isEntity; }
};

#endif // !_OBJECT_H_
