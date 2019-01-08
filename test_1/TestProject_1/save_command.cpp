#include "save_command.h"
#include "data_disk.h"
SaveCommand::SaveCommand(std::vector<std::string>& ArgList)
	: Command(ArgList, 2)
{
}

SaveCommand::~SaveCommand()
{
}

ErrorCode SaveCommand::Run()
{
	ToAbsolutePath();
	if (m_ArgList.size() != 2)
		return ERROR_ARG_COUNT_CODE;
	int PathWildCardIndex = m_ArgList[1].find_first_of('*');
	if (std::string::npos != PathWildCardIndex)
		return ERROR_SRC_PATH_CODE;
	if ('@' != m_ArgList[1][0])
		return ERROR_SRC_PATH_CODE;
	std::string PathStr = m_ArgList[1].substr(1, m_ArgList[1].size() - 1);
	return g_DataDiskPtr->SaveData(PathStr);
}
