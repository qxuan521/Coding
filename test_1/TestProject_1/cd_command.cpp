#include "cd_command.h"
#include "data_disk.h"

CDCommand::CDCommand(std::vector<std::string> & ArgList)
	:Command(ArgList)
{
}

CDCommand::~CDCommand()
{
}

ErrorCode CDCommand::Run()
{
	ToAbsolutePath();
	if (m_ArgList.empty())
		return ERROR_DST_PATH_CODE;
	if (m_ArgList.size() > 2)
		return ERROR_DST_PATH_CODE;
	if (m_ArgList.size() == 1)
		return SUCCESS_CODE;
	int WildCardIndex = (int) m_ArgList[1].find_first_of('*');
	if (std::string::npos != WildCardIndex)
		return ERROR_DST_PATH_CODE;
	ErrorCode ChangePathResult = g_DataDiskPtr->SetWorkingPath(m_ArgList[1]);
	return ChangePathResult;
}
