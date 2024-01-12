// @file: Object.h
//
// @brief: Header file for the Object class. Everything in a game is essentially an object.

#pragma once

#ifndef _OBJECT_H_
#define _OBJECT_H_

class Object
{
	DECLARE_ABSTRACT_BASE_CLASS(Object)

protected:
	// Variables used for object pooling
	bool isFreeInObjectPool = true;

	bool isActive = true;

	bool isEntity = false;  // useful in derived class Entity

	std::string name = "";
	std::string guid = "";
	STRCODE uid = 0;

	Object();
	Object(std::string& guid);
	Object(std::string& guid, std::string& name);


public:
	virtual ~Object() = default;

	virtual void Initialize() = 0;
	virtual void Destroy() = 0;

	void SetActive(bool value) { isActive = value; }
	bool IsActive() const { return isActive; }
	const std::string& GetName() { return name; }
	void SetName(const std::string& n) { name = n; }
	const std::string& GetGUID() { return guid; }
	const STRCODE GetUid() { return uid; }

	/*bool IsFreeInObjectPool() const { return isFreeInObjectPool; }
	void MarkOccupiedInObjectPool() { isFreeInObjectPool = false; }
	void MarkFreeInObjectPool() { isFreeInObjectPool = true; }*/

	bool IsEntity() const { return isEntity; }
};

#endif // !_OBJECT_H_
