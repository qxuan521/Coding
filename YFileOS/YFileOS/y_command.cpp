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

void YCommand::errorPrint(YErrorCode rErrorType, std::string szPath)
{

}

YErrorCode YCommand::toAbsolutePath(const std::vector<std::string>& rOrgrinalArgList)
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

bool YCommand::pathValidation(const std::string & szPath)
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
	std::regex rBaseRegex("[\\w+\\._\\?\\* ]+");
	std::regex rStartRegex("^[\\w\\._*?]+");
	std::regex rEndRegex("[\\w\\._*?]+$");
	std::regex rPointRegex("[\\.]*");
	std::regex rRootDiskRegex("[\\w+\\._*?]+:");
	for (size_t index = 1; index < rPathSplitResult.size(); ++index)
	{
		if (0 == index)
		{
			if (!std::regex_match(rPathSplitResult[index], rRootDiskRegex) || std::regex_match(rPathSplitResult[index], rPointRegex))
				return false;
		}
		else
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
	}
	return true;
}

bool YCommand::noWildCardPathValidation(const std::string & szPath)
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
	std::regex rStartRegex("^[\\w\\._]+");
	std::regex rEndRegex("[\\w\\._]+$");
	std::regex rPointRegex("[\\.]*");
	std::regex rRootDiskRegex("[\\w+\\._]+:");
	for (size_t index = 0; index < rPathSplitResult.size(); ++index)
	{
		if (0 == index)
		{
			if (!std::regex_match(rPathSplitResult[index], rRootDiskRegex) || std::regex_match(rPathSplitResult[index], rPointRegex))
				return false;
		}
		else
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
	}
	return true;
}

bool YCommand::rootDiskPathValidation(const std::string & szPath)
{
	if (szPath.empty())
		return false;
	if (isRealPath(szPath))
		return false;
	std::regex rRootDiskRegex("[\\w+\\._]+:");
	std::regex rPointRegex("[\\.]*");
	if (!std::regex_match(szPath, rRootDiskRegex) || std::regex_match(szPath, rPointRegex))
	{
		return false;
	}
	return true;
}

bool YCommand::wildCardOnlyLastLevel(const std::string & szPath)
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
	std::regex rPointRegex("[\\.]*");
	std::regex rRootDiskRegex("[\\w+\\._]+:");
	for (size_t index = 0; index < rPathSplitResult.size(); ++index)
	{
		if (0 == index)
		{
			if (!std::regex_match(rPathSplitResult[index], rRootDiskRegex) || std::regex_match(rPathSplitResult[index], rPointRegex))
				return false;
		}
		else
		{
			if (rPathSplitResult.size() - 1 == index)
			{
				std::regex rBaseRegex("[\\w+\\._\\?\\* ]+");
				std::regex rStartRegex("^[\\w\\._*?]+");
				std::regex rEndRegex("[\\w\\._*?]+$");
				bool bMatchResult = std::regex_match(rPathSplitResult[index], rBaseRegex);
				bMatchResult = bMatchResult && std::regex_match(rPathSplitResult[index], rStartRegex);
				bMatchResult = bMatchResult && std::regex_match(rPathSplitResult[index], rEndRegex);
				bMatchResult = bMatchResult && !std::regex_match(rPathSplitResult[index], rPointRegex);
				if (!bMatchResult)
				{
					return false;
				}
			}
			else
			{
				std::regex rBaseRegex("[\\w+\\._ ]+");
				std::regex rStartRegex("^[\\w\\._]+");
				std::regex rEndRegex("[\\w\\._]+$");
				bool bMatchResult = std::regex_match(rPathSplitResult[index], rBaseRegex);
				bMatchResult = bMatchResult && std::regex_match(rPathSplitResult[index], rStartRegex);
				bMatchResult = bMatchResult && std::regex_match(rPathSplitResult[index], rEndRegex);
				bMatchResult = bMatchResult && !std::regex_match(rPathSplitResult[index], rPointRegex);
				if (!bMatchResult)
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool YCommand::mayHaveWildCard(const std::string & szPath)
{
	return std::string::npos != szPath.find('*') || std::string::npos != szPath.find(',');
}

bool YCommand::pathCanbeRealValid()
{
	if (m_rArgList.size() != 2)
	{
		return false;
	}
	bool validation = mayHaveWildCard(m_rArgList[0]);
	validation = validation && mayHaveWildCard(m_rArgList[1]);
	if (isRealPath(m_rArgList[0]))
	{
		validation = validation && isHaveWildCard(m_rArgList[0]);
	}
	if (isRealPath(m_rArgList[1]))
	{
		validation = validation && isHaveWildCard(m_rArgList[1]);
	}
	if (isHaveWildCard(m_rArgList[0]))
	{
		validation = validation && g_pDiskOperator->isPathExist(m_rArgList[0]);
	}
	return validation;
}

