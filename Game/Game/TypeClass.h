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

#ifndef _RTTI_H_
#define _RTTI_H_
#pragma once

class Object;

/// <summary>
/// Type class that allows a developer to create an instance of an object
/// via a string or STRCODE
/// </summary>
class TypeClass
{
public:
	TypeClass(std::string _className, std::function<Object*()> ctor);

	Object* ConstructObject() const;

	const bool HasConstructor()
	{
		return _ctor ? true : false;
	}

	const std::string& GetClassName() const
	{
		return className;
	}

	const STRCODE GetClassHashCode() const
	{
		return classhash;
	}

	static const TypeClass& GetTypeClass(std::string _className);
	static const TypeClass& GetTypeClass(const char* _class);
	static const TypeClass& GetTypeClass(const STRCODE _class);

	static Object* ConstructObject(const char* _class);
	static Object* ConstructObject(const STRCODE _class);

protected:

	void Register()
	{
		if (classTable == nullptr)
		{
			classTable = new std::map<STRCODE, TypeClass*>();
		}
		classTable->insert(TypeClass_pair(GetHashCode(className.c_str()), this));
	};

private:
	static std::map<STRCODE, TypeClass*>* classTable;

	STRCODE classhash;
	std::string className;
	std::function<Object*()> _ctor;

	typedef std::pair<STRCODE, TypeClass*> TypeClass_pair;
};

///-------------------------------------------------------------------------------------------------
/// <summary>	External Functions for easy implementation. </summary>
///-------------------------------------------------------------------------------------------------
extern Object* CreateObject(const char* _class);

///-------------------------------------------------------------------------------------------------
/// <summary>	Class Definition used by all. </summary>
///-------------------------------------------------------------------------------------------------
#define DECLARE_CLASS_DEFINITION(name)																						\
    public:																													\
		static std::string __class##name;																					\
        static TypeClass staticClassInfo_##name;																			\
		static const std::string& GetClassName() { return name::staticClassInfo_##name.GetClassName(); }					\
		static const STRCODE GetClassHashCode() { return name::staticClassInfo_##name.GetClassHashCode(); }					\
		virtual const std::string& GetDerivedClassName() { return name::__class##name; }									\
		virtual const STRCODE GetDerivedClassHashCode() { return name::staticClassInfo_##name.GetClassHashCode(); }

///-------------------------------------------------------------------------------------------------
/// <summary>	IsA Definition used by all. </summary>
///-------------------------------------------------------------------------------------------------
#define IS_A_BASE_DEFINITION(name)																							\
		virtual bool IsA(STRCODE _className)																				\
		{																													\
			return (_className == name::staticClassInfo_##name.GetClassHashCode() ? true : false);							\
		}

#define IS_A_DEFINITION(name, base)																							\
		bool IsA(STRCODE _className) override																				\
		{																													\
			return (_className == GetClassHashCode() ? true : base::IsA(_className));										\
		}

///-------------------------------------------------------------------------------------------------
/// <summary>	Declaration for Abstract Classes. </summary>
///-------------------------------------------------------------------------------------------------
#define DECLARE_ABSTRACT_BASE_CLASS(name)																					\
    public:																													\
		DECLARE_CLASS_DEFINITION(name)																						\
		IS_A_BASE_DEFINITION(name)

#define DECLARE_ABSTRACT_DERIVED_CLASS(name, base)																			\
    public:																													\
		DECLARE_CLASS_DEFINITION(name)																						\
		IS_A_DEFINITION(name, base)


#define IMPLEMENT_ABSTRACT_CLASS(name)																						\
	std::string name::__class##name = #name;																				\
    TypeClass name::staticClassInfo_##name(std::string(#name), nullptr);

///-------------------------------------------------------------------------------------------------
/// <summary>	Declaration for Dynamic Classes. </summary>
///-------------------------------------------------------------------------------------------------
#define DECLARE_CREATE_CLASS(name)																							\
		static Object* TypeClass_CreateObject();

#define DECLARE_DYNAMIC_BASE_CLASS(name)																					\
    public:																													\
		DECLARE_CLASS_DEFINITION(name)																						\
		DECLARE_CREATE_CLASS(name)																							\
		IS_A_BASE_DEFINITION(name)

#define DECLARE_DYNAMIC_DERIVED_CLASS(name, base)																			\
    public:																													\
		DECLARE_CLASS_DEFINITION(name)																						\
		DECLARE_CREATE_CLASS(name)																							\
		IS_A_DEFINITION(name, base)

#define IMPLEMENT_DYNAMIC_CLASS(name)																						\
	std::string name::__class##name = #name;																				\
	TypeClass name::staticClassInfo_##name(std::string(#name), name::TypeClass_CreateObject);								\
    Object* name::TypeClass_CreateObject() { return new name; }

#define REGISTER_TYPECLASS(name)																							\
   name::staticClassInfo_##name = TypeClass::GetTypeClass(std::string(#name));	

#endif

