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

YErrorCode YCommand::toAbsolutePath()
{
	if ((int)m_rArgList.size() < m_nMustSize)
		return YERROR_COMMAND_ARG_NUM_ERROR;
	bool IsCheckTypeArg = false;
	for (int index = 0; index < (int)m_rArgList.size(); ++index)
	{
		if (!IsCheckTypeArg && index < m_rTypeArg.size())
		{
			if (!m_rTypeArg.empty() && m_rTypeArg.count(m_rArgList[index]))
			{
				continue;
			}
			else
			{
				IsCheckTypeArg = true;
			}
		}
		//Â·¾¶´¦Àí
		if (isRealPath(m_rArgList[index]))
			continue;
		std::vector<std::string> tempStrArr = splitStrByCharacter(m_rArgList[index], '/');
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
		m_rArgList[index] = AbsString;
	}
	return Y_OPERAT_SUCCEED;
}

bool YCommand::isThisCommand(std::string & szCommandStr)
{
	return std::regex_match(szCommandStr,m_rRegex);
}

