#include "command.h"
#include "data_disk.h"
const int COMMAND_TYPE_INDEX_MAX = 2;

Command::Command(std::vector<std::string>& ArgList, int MustSize)
	: m_ArgList(ArgList)
	, m_MustSize(MustSize)
{
	//ToAbsolutePath();
}

Command::~Command()
{
}

void Command::SetCurWorkingPath(std::string & WorkingPath)
{
	m_CurWorkPath = WorkingPath;
}


ErrorCode Command::ToAbsolutePath()
{
	if ((int)m_ArgList.size() < m_MustSize)
		return ERROR_ARG_COUNT_CODE;
	bool IsCheckTypeArg = false;
	for (int index = 1; index < (int)m_ArgList.size(); ++index)
	{
		//检查命令参数列表
		if(!IsCheckTypeArg && index <= COMMAND_TYPE_INDEX_MAX)
			if (!m_TypeArg.empty() && m_TypeArg.count(m_ArgList[index]))
			{
				continue;
			}
			else
			{
				IsCheckTypeArg = true;
			}
		//路径处理
		if('@' == m_ArgList[index][0])
			continue;
		std::vector<std::string> tempStrArr = SplitStr(m_ArgList[index],'/');
		std::string AbsString = m_CurWorkPath;
		for (int LoopCount = 0; LoopCount < (int)tempStrArr.size();++LoopCount)
		{
			if (0 == LoopCount && "root:" == tempStrArr[LoopCount])
			{
				AbsString = "root:";
				continue;
			}
			if (tempStrArr[LoopCount].empty())
				continue;
			if (".." == tempStrArr[LoopCount])
			{
				if ("root:" ==AbsString)
					return ERROR_SRC_PATH_CODE;
				AbsString =AbsString.substr(0, AbsString.find_last_of('/'));
			}
			else if ("." == tempStrArr[LoopCount])
				continue;
			else
			{
				AbsString.append("/");
				AbsString.append(tempStrArr[LoopCount]);
			}
		}
		m_ArgList[index] = AbsString;
	}
	return SUCCESS_CODE;
}
