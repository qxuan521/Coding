#include "y_input_resolve.h"
#include <algorithm>
YInputResolve::YInputResolve()
{
}

YInputResolve::~YInputResolve()
{
}
//cmd 认为被双引号包括的东西都是路径，
//这里设计与cmd 有区别只将双引号包裹的视为一个整体的参数 可能是命令参数也可能是路径
//原因如下：
//由于设计预处理时无法获得那些是参数
//并且由于cmd在识别时以'/'开头的都视为参数 路径分割必须时'\'由于需求要求能够全部支持为路径所以难以分辨参数所以这里这样设计
//！并且由于需要支持以'/'开头的路径参数，在命令进行绝对路径处理时会将连续的'/'视为同一个分割符分割前后两级路径（这个应该有办法优化）
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
					if (rResult.rPathList.empty() && !bMustBepath && !szSubArg.empty() && '/' == szSubArg[0])
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
		std::replace_if(szSubArg.begin(), szSubArg.end(), [](char in) {return in == '\\'; }, '/');
		rResult.rPathList.push_back(szSubArg);
		szSubArg.clear();
	}
	return rResult;
}
