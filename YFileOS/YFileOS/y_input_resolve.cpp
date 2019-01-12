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
	std::replace_if(szTempInput.begin(), szTempInput.end(), [](char in) {return in == '\\'; }, '/');
	std::vector<std::string> resultArr;
	bool bNameWithSpace = false;
	std::string szSubArg;
	for (size_t index = 0; index < szTempInput.size(); ++index)
	{
		if (' ' == szTempInput[index] )
		{//�����ո�
			if (!bNameWithSpace)
			{//���û��˫���Ű��� 
				if(rResult.szCommandName.empty())
				{//���ȳ�ʼ����������
					rResult.szCommandName = szSubArg;
					szSubArg.clear();
				}
				else
				{
					rResult.rArglist.push_back(szSubArg);
					szSubArg.clear();
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
		rResult.rArglist.push_back(szSubArg);
		szSubArg.clear();
	}
	return rResult;
}
