#include "y_ren_command.h"
#include "y_tool.h"
#include "y_disk_operator.h"

YRenCommand::YRenCommand(const std::string& szName)
	: YCommand(szName, 2)
{

}

YRenCommand::~YRenCommand()
{
}

YErrorCode YRenCommand::excultCommand(YCommandInfo & rCommandInfo)
{
	YErrorCode rResultCode = toAbsolutePath(rCommandInfo.rPathList);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		return errorPrint(rResultCode);
	}
	std::string szSrcPath(m_rArgList[0]);
	std::string szNewName(m_rArgList[1]);
	if (!noWildCardNameValidation(szNewName))
	{
		return YERROR_PATH_ILLEGAL;
	}
	rResultCode = g_pDiskOperator->changeName(szSrcPath, szNewName);
	return errorPrint(rResultCode);
}

YErrorCode YRenCommand::toAbsolutePath(const std::vector<std::string>& rOrgrinalArgList)
{
	if ((int)rOrgrinalArgList.size() != m_nMustSize)
		return YERROR_COMMAND_ARG_NUM_ERROR;
	bool IsCheckTypeArg = false;
	std::string szPathOrgrinal = rOrgrinalArgList[0];
	//Â·¾¶´¦Àí
	if (isRealPath(szPathOrgrinal))
	{
		m_rArgList.push_back(szPathOrgrinal);
		return Y_OPERAT_SUCCEED;
	}
	if (rOrgrinalArgList.empty())
	{
		return YERROR_PATH_ILLEGAL;
	}
	transform(szPathOrgrinal.begin(), szPathOrgrinal.end(), szPathOrgrinal.begin(), ::tolower);
	std::vector<std::string> tempStrArr = splitStrByCharacter(rOrgrinalArgList[0], '/');
	std::string AbsString = m_szCurWorkPath;
	for (int LoopCount = 0; LoopCount < (int)tempStrArr.size(); ++LoopCount)
	{
		if (rootDiskPathValidation(tempStrArr[LoopCount]))
		{
			AbsString = tempStrArr[LoopCount];
			continue;
		}
		if (tempStrArr[LoopCount].empty())
			continue;
		if (".." == tempStrArr[LoopCount])
		{
			if (g_pDiskOperator->isRootName(AbsString))
			{
				return YERROR_PATH_ILLEGAL;
			}
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
	m_rArgList.push_back(rOrgrinalArgList[1]);
	return Y_OPERAT_SUCCEED;
}
bool YRenCommand::noWildCardNameValidation(const std::string & szPath)
{
	if (szPath.empty())
		return false;
	if (isRealPath(szPath))
		return true;
	std::vector<std::string> rPathSplitResult = splitStrByCharacter(szPath, '/');
	if (rPathSplitResult.empty())
	{
		return false;
	}
	std::regex rBaseRegex("[\\w+\\._ ]+");
	std::regex rStartRegex("^[\\w\\._][\\w+\\._ ]*");
	std::regex rEndRegex("[\\w+\\._ ]*[\\w\\._]$");
	std::regex rPointRegex("[\\.]*");
	std::regex rRootDiskRegex("[\\w+\\._]+:");
	for (size_t index = 0; index < rPathSplitResult.size(); ++index)
	{
		bool bMatchResult = std::regex_match(rPathSplitResult[index], rBaseRegex);
		bMatchResult = bMatchResult && std::regex_match(rPathSplitResult[index], rStartRegex);
		bMatchResult = bMatchResult && std::regex_match(rPathSplitResult[index], rEndRegex);
		bMatchResult = bMatchResult && !std::regex_match(rPathSplitResult[index], rPointRegex);
		if (!bMatchResult)
		{
			return false;
		}
	}
	return true;
}