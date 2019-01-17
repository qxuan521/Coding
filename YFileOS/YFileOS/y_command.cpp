#include "y_command.h"
#include "y_disk_operator.h"
#include "y_tool.h"
/*
enum YErrorCode
{
	Y_OPERAT_SUCCEED,
	Y_OPERAT_FAILD,
	//ptr
	YERROR_POINTER_NULL,
	//filestream

	//disk
	YERROR_QUERY_FILE_NON_EXISTENT,
	TERROR_DISK_ERROR,
	//file
	YERROR_NO_THIS_CHILD,
	YERROR_FILE_IS_EXIST,
	YERROR_FOLDER_IS_NOT_BE_EMPTY,
	//path
	YERROR_PATH_NOT_EXIST,
	YERROR_PATH_ILLEGAL,
	//command
	YERROR_COMMAND_ARG_NUM_ERROR,
	YERROR_COMMAND_ARG_ILLEGAL,
	Y_COPY_SUCCEED,

};
*/
const std::string rErrorStringArr[] =
{
	" surprise!!!!",
	" Operated Faild.",
	" Pointer is null.",
	" Query file is not existed.",
	" Disk unknow error.",
	" The folder dose not have the file.",
	" The file is exist.",
	" The folder is not be empty.",
	" The path not exist.",
	" The path illegal.",
	" The command arg count error.",
	" The command arg illegal."
};

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

YErrorCode YCommand::errorPrint(YErrorCode rErrorType, std::string szPath)
{
	std::string szOutput;
	if (!szPath.empty())
	{
		szOutput += "\"";
		szOutput += szPath;
		szOutput += "\"";
	}
	std::cout << szOutput << rErrorStringArr[rErrorType] << std::endl;
	return rErrorType;
}

YErrorCode YCommand::toAbsolutePath(const std::vector<std::string>& rOrgrinalArgList)
{
	if ((int)rOrgrinalArgList.size() < m_nMustSize)
		return YERROR_COMMAND_ARG_NUM_ERROR;
	bool IsCheckTypeArg = false;
	for (size_t index = 0; index < rOrgrinalArgList.size(); ++index)
	{
		std::string szPathOrgrinal = rOrgrinalArgList[index];
		//Â·¾¶´¦Àí
		if (rOrgrinalArgList[index].empty())
			continue;
		if (isRealPath(szPathOrgrinal))
		{
			m_rArgList.push_back(szPathOrgrinal);
			continue;
		}
		if (rOrgrinalArgList.empty())
		{
			return YERROR_PATH_ILLEGAL;
		}
		transform(szPathOrgrinal.begin(), szPathOrgrinal.end(), szPathOrgrinal.begin(), ::tolower);
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

YErrorCode YCommand::handleCommandArg(YCommandInfo & rCommandInfo)
{
	if (rCommandInfo.rArgList.empty())
	{
		return Y_OPERAT_SUCCEED;
	}
	if (m_rTypeArg.empty())
	{
		return YERROR_COMMAND_ARG_ILLEGAL;
	}
	if (rCommandInfo.rArgList.size() != m_rTypeArg.size())
	{
		return YERROR_COMMAND_ARG_ILLEGAL;
	}
	for (size_t index = 0;index < rCommandInfo.rArgList.size();++index)
	{
		std::string& szCommandArg = rCommandInfo.rArgList[index];
		if (m_rTypeArg.count(szCommandArg))
		{
			if (!m_rTypeArg[szCommandArg])
			{
				m_rTypeArg[szCommandArg] = true;
			}
			else
			{
				return YERROR_COMMAND_ARG_ILLEGAL;
			}
		}
		else
		{
			return YERROR_COMMAND_ARG_ILLEGAL;
		}
	}
	return Y_OPERAT_SUCCEED;
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
	std::regex rStartRegex("^[\\w\\._*?][\\w+\\._\\?\\* ]*");
	std::regex rEndRegex("[\\w+\\._\\?\\* ]*[\\w\\._*?]$");
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
	std::regex rStartRegex("^[\\w\\._][\\w+\\._ ]*");
	std::regex rEndRegex("[\\w+\\._ ]*[\\w\\._]$");
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
	std::regex rRootDiskRegex("[A-Za-z]+:");
	return std::regex_match(szPath, rRootDiskRegex);
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
				std::regex rStartRegex("^[\\w\\._*?][\\w+\\._\\?\\* ]*");
				std::regex rEndRegex("[\\w+\\._\\?\\* ]*[\\w\\._*?]$");
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
				std::regex rStartRegex("^[\\w\\._][\\w+\\._ ]*");
				std::regex rEndRegex("[\\w+\\._ ]*[\\w\\._]$");
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
	return std::string::npos != szPath.find('*') || std::string::npos != szPath.find('?');
}

bool YCommand::pathCanbeRealValid()
{
	if (m_rArgList.size() != 2)
	{
		return false;
	}
	bool validation = true;
		//mayHaveWildCard(m_rArgList[0]);
// 	validation = validation && mayHaveWildCard(m_rArgList[1]);
	if (isRealPath(m_rArgList[0]))
	{
		validation = validation && noWildCardPathValidation(m_rArgList[0]);
	}
	else
	{
		validation = validation && wildCardOnlyLastLevel(m_rArgList[0]);

	}
	if (isRealPath(m_rArgList[1]))
	{
		validation = validation && noWildCardPathValidation(m_rArgList[1]);
	}
	else
	{
		validation = validation && wildCardOnlyLastLevel(m_rArgList[1]);
	}
	return validation;
}

bool YCommand::userAsk(const std::string & szPath, const std::string & szAskContent)
{
	while (true)
	{
		if (szPath.empty())
		{
			std::cout << "\"" << szPath << "\"" << szAskContent;
		}
		else
		{
			std::cout << "\"" << szPath << "\"" << szAskContent;
		}
		std::string InPut;
		std::cin.clear();
		std::cin.sync();
		getline(std::cin, InPut);
		if ("y" == InPut)
		{
			return true;
		}
		if ("n" == InPut)
		{
			return false;
		}
		std::cin.clear();
	}
	return false;
}

