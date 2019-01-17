#include "y_input_resolve.h"
#include <algorithm>
YInputResolve::YInputResolve()
{
}

YInputResolve::~YInputResolve()
{
}
//cmd ��Ϊ��˫���Ű����Ķ�������·����
//���������cmd ������ֻ��˫���Ű�������Ϊһ������Ĳ��� �������������Ҳ������·��
//ԭ�����£�
//�������Ԥ����ʱ�޷������Щ�ǲ���
//��������cmd��ʶ��ʱ��'/'��ͷ�Ķ���Ϊ���� ·���ָ����ʱ'\'��������Ҫ���ܹ�ȫ��֧��Ϊ·���������Էֱ�������������������
//������������Ҫ֧����'/'��ͷ��·����������������о���·������ʱ�Ὣ������'/'��Ϊͬһ���ָ���ָ�ǰ������·�������Ӧ���а취�Ż���
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
		std::replace_if(szSubArg.begin(), szSubArg.end(), [](char in) {return in == '\\'; }, '/');
		rResult.rPathList.push_back(szSubArg);
		szSubArg.clear();
	}
	return rResult;
}
