// MIT License
// 
// Copyright(c)2016 William Barry
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

#include "stdafx.h"
#include "TypeClass.h"
#include "Object.h"

std::map<STRCODE, TypeClass*>* TypeClass::classTable;

TypeClass::TypeClass(std::string _className, std::function<Object*()> ctor)
{
	className = _className;
	classhash = GetHashCode(_className.c_str());
	_ctor = ctor;
	Register();
}

const TypeClass& TypeClass::GetTypeClass(std::string _className)
{
	return TypeClass::GetTypeClass(_className.c_str());
}

const TypeClass& TypeClass::GetTypeClass(const char* _class)
{
	return GetTypeClass(GetHashCode(_class));
}

const TypeClass& TypeClass::GetTypeClass(const STRCODE _class)
{
	std::map<STRCODE, TypeClass*>::iterator _item = classTable->find(_class);
	if (_item == classTable->end())
	{
		assert(0);
	}

	return *(*_item).second;
}

Object* CreateObject(const char* _class)
{
	return TypeClass::ConstructObject(_class);
}

Object* TypeClass::ConstructObject(const char* _class)
{
	return ConstructObject(GetHashCode(_class));
}

Object* TypeClass::ConstructObject(const STRCODE _class)
{
	const TypeClass& typeClass = TypeClass::GetTypeClass(_class);
	//ASSERT(typeClass.HasConstructor(), "TypeClass has no Constructor to create");

	Object* obj = typeClass.ConstructObject();
	return obj;
}

Object* TypeClass::ConstructObject() const
{
	return _ctor ? _ctor() : 0;
}


