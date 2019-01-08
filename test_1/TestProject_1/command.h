#pragma once
#include <set>
#include <vector>
#include "model_def.h"
class Command
{
public:
	Command(std::vector<std::string>& ArgList,int MustSize = 0);
	virtual ~Command();
	virtual ErrorCode					Run() = 0;
	//���õ�ǰ����·��
	void								SetCurWorkingPath(std::string &WorkingPath);
	//����·��Ϊ����·��
	virtual ErrorCode					ToAbsolutePath();

protected:
	std::vector<std::string>	m_ArgList;
	std::string					m_CurWorkPath;
	std::set<std::string>		m_TypeArg;
	int							m_MustSize;
};