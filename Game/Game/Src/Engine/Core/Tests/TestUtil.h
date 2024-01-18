#pragma once

#include "stdafx.h"
#include "Engine/Core/util.h"

void TestGetHashCode()
{
	std::string s1 = "UbisoftNext";
	assert(GetHashCode(s1.c_str()) == 533715243);
	
	s1 = "Ubisoft Next";
	assert(GetHashCode(s1.c_str()) == 4108938811);
}
