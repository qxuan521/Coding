#include <regex>
#include <iostream>
#include "md_command.h"
#include "data_disk.h"
#include "file_info.h"
const int  MD_COMMAND_PATH_ARG_INDEX = 1;
MdCommand::MdCommand(std::vector<std::string> & ArgList)
	: Command(ArgList,2)
{

}

MdCommand::~MdCommand()
{
	 
}



ErrorCode MdCommand::Run()
{
	m_PathArr.clear();
	if (m_ArgList.size() != 2)
		return ERROR_ARG_COUNT_CODE;
	ToAbsolutePath();
	std::string FilePath = m_ArgList[MD_COMMAND_PATH_ARG_INDEX];
	if (FilePath.empty())
		return ERROR_DST_PATH_CODE;
	std::regex regPoint("[.]*");
	bool InRulePoit = regex_match(g_DataDiskPtr->GetName( m_ArgList[1]), regPoint);
	if (InRulePoit)
		return ERROR_NEW_NAME;
	if (std::string::npos != m_ArgList[MD_COMMAND_PATH_ARG_INDEX].find_last_of("*") ||
		(std::string::npos != m_ArgList[MD_COMMAND_PATH_ARG_INDEX].find_last_of(":") &&
			4 != m_ArgList[MD_COMMAND_PATH_ARG_INDEX].find_last_of(":")))
	{
		return ERROR_NEW_NAME;
	}
	if (g_DataDiskPtr->IsPathExsit(FilePath))
		return ERROR_THE_NAME_EXIST_CODE;
	m_PathArr.push_back(m_ArgList[MD_COMMAND_PATH_ARG_INDEX]);
	std::string ParentStr = g_DataDiskPtr->GetParentPath(m_ArgList[MD_COMMAND_PATH_ARG_INDEX]);
	while (std::string::npos != ParentStr.find_last_of("/") && !g_DataDiskPtr->IsPathExsit(ParentStr))
	{
		if ( '/' == ParentStr[ParentStr.size() - 1] )
			return ERROR_DST_PATH_CODE;
		m_PathArr.push_back(ParentStr);
		ParentStr = g_DataDiskPtr->GetParentPath(ParentStr);
	}
	if (!g_DataDiskPtr->IsPathExsit(ParentStr))
		return ERROR_DATA_DISK_UNKNOW;
	FileInfo* TempStartFileInfo = g_DataDiskPtr->GetFileInfo(ParentStr);
	if (NULL == TempStartFileInfo)
		return ERROR_DATA_DISK_UNKNOW;
	if (FOLDER_FILE != TempStartFileInfo->GetType())
		return ERROR_DST_PATH_CODE;
	for (int index = (int)m_PathArr.size()-1; index >= 0; --index)
	{
		FileInfo* NewNode = new FileInfo(m_PathArr[index],TempStartFileInfo,FOLDER_FILE);
		if (SUCCESS_CODE == g_DataDiskPtr->AddNewNode(NewNode, TempStartFileInfo))
			std::cout << NewNode->GetFilePath() << " Create Successed"<<std::endl;
		TempStartFileInfo = NewNode;
	}
	return SUCCESS_CODE;
}
