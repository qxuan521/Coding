#include "y_command.h"
#include "y_disk_operator.h"
#include "y_tool.h"
YCommand::YCommand(const std::string& szName, int nMstSize)
	: m_rRegex(szName)
	, m_nMustSize(nMstSize)
{

}

YCommand::~YCommand()
{
}

void YCommand::setCurWoringPath(const std::string & szCurPath)
{
	m_szCurWorkPath = szCurPath;
}

void YCommand::errorPrint(YErrorCode rErrorType, std::string& szPath)
{

}

YErrorCode YCommand::toAbsolutePath(const std::vector<std::string>& rOrgrinalArgList)
{
	if ((int)rOrgrinalArgList.size() < m_nMustSize)
		return YERROR_COMMAND_ARG_NUM_ERROR;
	bool IsCheckTypeArg = false;
	for (int index = 0; index < (int)rOrgrinalArgList.size(); ++index)
	{
		if (!IsCheckTypeArg && index < m_rTypeArg.size())
		{
			if (!m_rTypeArg.empty() && m_rTypeArg.count(rOrgrinalArgList[index]))
			{
				m_rTypeArg[rOrgrinalArgList[index]] = true;
				continue;
			}
			else
			{
				IsCheckTypeArg = true;
			}
		}
		//·������
		if (isRealPath(rOrgrinalArgList[index]))
		{
			m_rArgList.push_back(rOrgrinalArgList[index]);
			continue;
		}
		std::vector<std::string> tempStrArr = splitStrByCharacter(rOrgrinalArgList[index], '/');
		std::string AbsString = m_szCurWorkPath;
		for (int LoopCount = 0; LoopCount < (int)tempStrArr.size(); ++LoopCount)
		{
			if (g_pDiskOperator->isRootName(tempStrArr[LoopCount]))
			{
				AbsString = tempStrArr[LoopCount];
				continue;
			}
			if (tempStrArr[LoopCount].empty())
				continue;
			if (".." == tempStrArr[LoopCount])
			{
				if (g_pDiskOperator->isRootName(AbsString))
					continue;
				AbsString = getParentPath(AbsString);
			}
			else if ("." == tempStrArr[LoopCount])
				continue;
			else
			{
				AbsString.append("/");
				AbsString.append(tempStrArr[LoopCount]);
			}
		}
		m_rArgList.push_back(AbsString);
	}
	return Y_OPERAT_SUCCEED;
}

bool YCommand::isThisCommand(std::string & szCommandStr)
{
	return std::regex_match(szCommandStr,m_rRegex);
}

void YCommand::resetCommand()
{
	m_rArgList.clear();
	resetTypeArg();
}

void YCommand::resetTypeArg()
{
	for (auto iter = m_rTypeArg.begin(); iter != m_rTypeArg.end(); ++iter)
	{
		iter->second = false;
	}
}

