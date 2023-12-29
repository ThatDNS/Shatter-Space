// @file: Object.h
//
// @brief: Header file for the Object class. Everything in a game is essentially an object.
//
// @author: Divyanshu N Singh (DNS)
// @date: 2023-12-03

#pragma once

#ifndef _OBJECT_H_
#define _OBJECT_H_

class Object
{
	DECLARE_ABSTRACT_BASE_CLASS(Object)

protected:
	bool isActive = true;

	std::string name = "";
	std::string guid = "";
	STRCODE uid = 0;

	Object();
	Object(std::string& guid);
	Object(std::string& guid, std::string& name);


public:
	virtual ~Object() = default;
	virtual void Load(json::JSON&);

	virtual void Initialize() = 0;
	virtual void Destroy() = 0;
	virtual json::JSON GetClassData() = 0;

	json::JSON Save();

	void SetActive(bool value) { isActive = value; }
	bool IsActive() const { return isActive; }
	const std::string& GetName() { return name; }
	const std::string& GetGUID() { return guid; }
	const STRCODE GetUid() { return uid; }
};

#endif // !_OBJECT_H_
