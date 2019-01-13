#include "y_cd_command.h"
#include "y_disk_operator.h"
#include "y_tool.h"

YCdCommand::YCdCommand(const std::string& szName)
	: YCommand(szName, 1)
{

}

YCdCommand::~YCdCommand()
{

}

YErrorCode YCdCommand::excultCommand(YCommandInfo& rCommandInfo)
{
	YErrorCode rResultCode;
	rResultCode = toAbsolutePath(rCommandInfo.rArglist);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		return rResultCode;
	}
	if (m_rArgList.size() >= 2)
	{
		return YERROR_COMMAND_ARG_NUM_ERROR;
	}
	if (!noWildCardPathValidation(m_rArgList[0]))
	{
		return YERROR_PATH_ILLEGAL;
	}
	g_pDiskOperator->setCurWorkingPath(m_rArgList[0]);
	return Y_OPERAT_SUCCEED;
}

YErrorCode YCdCommand::toAbsolutePath(const std::vector<std::string>& rOrgrinalArgList)
{
	if ((int)rOrgrinalArgList.size() < m_nMustSize)
		return YERROR_COMMAND_ARG_NUM_ERROR;
	bool IsCheckTypeArg = false;
	for (size_t index = 0; index < (int)rOrgrinalArgList.size(); ++index)
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
		//Â·¾¶´¦Àí
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
