#include <iostream>
#include "load_command.h"
#include "data_disk.h"
#include "file_info.h"

Loadcommand::Loadcommand(std::vector<std::string>& Arglist)
	: Command(Arglist, 2)
{
}

Loadcommand::~Loadcommand()
{

}

ErrorCode Loadcommand::Run()
{
	ToAbsolutePath();
	if (m_ArgList.size() != 2)
		return ERROR_ARG_COUNT_CODE;
	if (!SecondAsk())
		return ERROR_USER_STOP_CODE;
	std::string PathStr;
	ErrorCode ResultCode = ResolveRealPath(PathStr);
	if (SUCCESS_CODE != ResultCode)
		return ResultCode;
	return g_DataDiskPtr->LoadData(PathStr);
}

bool Loadcommand::SecondAsk()
{
	FileInfo* RootFile = g_DataDiskPtr->GetFileInfo("root:");
	if (NULL == RootFile)
		return false;
	if (0 < RootFile->GetChildrenSize())
	{
		bool IsNo = false;
		while (1)
		{
			std::cout << "load new data will rebuild dist,data maybe lost.Do you want to continue<yes /no>?";
			std::string InPut;
			std::cin.clear();
			std::cin.sync();
			getline(std::cin, InPut);
			std::cin.clear();
			if ("no" == InPut)
			{
				IsNo = true;
				break;
			}
			else if ("yes" == InPut)
			{
				IsNo = false;
				break;
			}
		}
		if (IsNo)
			return false;
	}
	return true;
}

ErrorCode Loadcommand::ResolveRealPath(std::string & ResultPath)
{
	FileInfo* RootFile = g_DataDiskPtr->GetFileInfo("root:");
	if (NULL == RootFile)
		return ERROR_DATA_DISK_UNKNOW;
	int PathWildCardIndex = m_ArgList[1].find_first_of('*');
	if (std::string::npos != PathWildCardIndex)
		return ERROR_SRC_PATH_CODE;
	if ('@' != m_ArgList[1][0])
		return ERROR_SRC_PATH_CODE;
	ResultPath = m_ArgList[1].substr(1, m_ArgList[1].size() - 1);
	return SUCCESS_CODE;
}
