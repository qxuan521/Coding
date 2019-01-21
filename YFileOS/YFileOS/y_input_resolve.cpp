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
		{//�����ո�
			if (!bNameWithSpace)
			{//���û��˫���Ű��� 
				if(rResult.szCommandName.empty()&& !bMustBepath)
				{//���ȳ�ʼ����������
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
			{//�����˫���Ű���
				szSubArg += szTempInput[index];
			}
		}
		else if('"' == szTempInput[index])
		{//����˫����
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
		{//���ǿո�Ҳ����˫����
			szSubArg += szTempInput[index];
		}
	}
	if (rResult.szCommandName.empty())
	{//���ȳ�ʼ����������
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
