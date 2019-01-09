#include <regex>
#include "ren_command.h"
#include "data_disk.h"
#include "file_info.h"
RenCommand::RenCommand(std::vector<std::string>& ArgList)
	: Command(ArgList, 3)
{

}

RenCommand::~RenCommand()
{

}

ErrorCode RenCommand::Run()
{
	ToAbsolutePath();
	if (m_ArgList.size() < 3)
		return ERROR_ARG_COUNT_CODE;
	FileInfo* SrcFile= g_DataDiskPtr->GetFileInfo(m_ArgList[1]);
	if (NULL == SrcFile)
		return ERROR_SRC_PATH_CODE;
	std::regex reg("[A-Za-z0-9_\\.]*");
	bool InRule = regex_match(m_ArgList[2], reg);
	if (!InRule)
		return ERROR_NEW_NAME;
	if (SrcFile->GetFileName() == m_ArgList[2])
	{
		return SUCCESS_CODE;
	}
	else
	{
		return g_DataDiskPtr->ModifyName(SrcFile, m_ArgList[2]);
	}
}

ErrorCode RenCommand::ToAbsolutePath()
{
	if ((int)m_ArgList.size() < m_MustSize)
		return ERROR_ARG_COUNT_CODE;
	bool IsCheckTypeArg = false;
	int index = 1;
		//Â·¾¶´¦Àí
	if ('@' == m_ArgList[index][0])
		return ERROR_SRC_PATH_CODE;
	std::vector<std::string> tempStrArr = SplitStr(m_ArgList[index],'/');
	std::string AbsString = m_CurWorkPath;
	for (int LoopCount = 0; LoopCount < (int)tempStrArr.size(); ++LoopCount)
	{
		if (0 == LoopCount && "root" == tempStrArr[LoopCount])
		{
			AbsString = "root";
			continue;
		}
		if (tempStrArr[LoopCount].empty())
			continue;
		if (".." == tempStrArr[LoopCount])
		{
			if ("root" == AbsString)
				return ERROR_SRC_PATH_CODE;
			AbsString = AbsString.substr(0, AbsString.find_last_of('/'));
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
	return SUCCESS_CODE;
}
