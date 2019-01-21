#include "y_input_resolve.h"
#include <algorithm>
YInputResolve::YInputResolve()
{
}

YInputResolve::~YInputResolve()
{
}
YCommandInfo YInputResolve::resolve(const std::string & szInput)
{
	YCommandInfo rResult;
	if (szInput.empty())
	{
		return rResult;
	}
	std::string szTempInput(szInput);
	std::vector<std::string> resultArr;
	bool bNameWithSpace = false;
	bool bMustBepath = false;
	std::string szSubArg;
	for (size_t index = 0; index < szTempInput.size(); ++index)
	{
		if (' ' == szTempInput[index] )
		{//遇到空格
			if (!bNameWithSpace)
			{//如果没被双引号包裹 
				if(rResult.szCommandName.empty()&& !bMustBepath)
				{//优先初始话命令名字
					rResult.szCommandName = szSubArg;
					szSubArg.clear();
				}
				else
				{
					if (!bMustBepath && !szSubArg.empty() && '/' == szSubArg[0])
					{
						rResult.rArgList.push_back(szSubArg);
						szSubArg.clear();
					}
					else
					{
						std::replace_if(szSubArg.begin(), szSubArg.end(), [](char in) {return in == '\\'; }, '/');
						rResult.rPathList.push_back(szSubArg);
						szSubArg.clear();
					}
					
				}
			}
			else
			{//如果被双引号包裹
				szSubArg += szTempInput[index];
			}
		}
		else if('"' == szTempInput[index])
		{//遇到双引号
			if (rResult.szCommandName.empty())
			{
				
			}
			else
			{
				bNameWithSpace = !bNameWithSpace;
				if (bNameWithSpace)
				{
					bMustBepath = true;
				}
				else
				{
					bMustBepath = false;
				}
			}
		}
		else
		{//不是空格也不是双引号
			szSubArg += szTempInput[index];
		}
	}
	if (rResult.szCommandName.empty())
	{//优先初始话命令名字
		rResult.szCommandName = szSubArg;
		szSubArg.clear();
	}
	else
	{
		if (!bMustBepath && !szSubArg.empty() && '/' == szSubArg[0])
		{
			rResult.rArgList.push_back(szSubArg);
			szSubArg.clear();
		}
		else
		{
			std::replace_if(szSubArg.begin(), szSubArg.end(), [](char in) {return in == '\\'; }, '/');
			rResult.rPathList.push_back(szSubArg);
			szSubArg.clear();
		}
	}
	return rResult;
}
