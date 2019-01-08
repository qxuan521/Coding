#pragma once
#include <iostream>
#include "model_def.h"

struct PathChangeAsk
{
	bool ChangeAsk(String& str,bool& AllFlag)
	{
		OUT_PUT(str<< "is Exist. Do you want to cover or merge file ?<yes/no/all>:");
		String InPut;
		std::cin.clear();
		std::cin >> InPut;
		if ("no" == InPut)
			return false;
		if ("all" == InPut)
		{
			AllFlag = true;
			return true;
		}
		else if ("yes" == InPut)
		{
			return true;
		}
		return false;
	}
};