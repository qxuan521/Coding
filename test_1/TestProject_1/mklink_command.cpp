#include <iostream>
#include "mklink_command.h"
#include "data_disk.h"
#include "link_file_info.h"
const int LNK_PATH_INDEX = 2;
const int SRC_PATH_INDEX = 1;

MklinkCommand::MklinkCommand(std::vector<std::string>& ArgList)
	: Command(ArgList,3)
{

}

MklinkCommand::~MklinkCommand()
{

}

ErrorCode MklinkCommand::Run()
{
	ToAbsolutePath();
	if (m_ArgList.size() != 3)
		return ERROR_LNK_CREATE_CODE;
	if (m_ArgList[SRC_PATH_INDEX].empty()|| std::string::npos != m_ArgList[SRC_PATH_INDEX].find_first_of('*'))
		return ERROR_SRC_PATH_CODE;
	if (m_ArgList[LNK_PATH_INDEX].empty() || std::string::npos != m_ArgList[LNK_PATH_INDEX].find_first_of('*'))
		return ERROR_DST_PATH_CODE;
	if (g_DataDiskPtr->IsPathExsit(m_ArgList[LNK_PATH_INDEX]))
		return ERROR_THE_NAME_EXIST_CODE;
	FileInfo* SrcFile(g_DataDiskPtr->GetFileInfo(m_ArgList[SRC_PATH_INDEX]));
	if (NULL == SrcFile)
		return ERROR_SRC_PATH_CODE;
	std::string LinkParentPath = g_DataDiskPtr->GetParentPath(m_ArgList[LNK_PATH_INDEX]);
	FileInfo* LinkFileParent = g_DataDiskPtr->GetFileInfo(LinkParentPath);
	if (NULL == LinkFileParent || DATA_FILE == LinkFileParent->GetType())
		return ERROR_SRC_PATH_CODE;
	FileInfo* LinkFile =static_cast<FileInfo*>(new LinkFileInfo(m_ArgList[LNK_PATH_INDEX], LinkFileParent, SrcFile));
	ErrorCode Result = g_DataDiskPtr->AddNewNode(LinkFile, LinkFileParent);
	if (SUCCESS_CODE == Result)
		std::cout<<"Create Link "<<LinkFile->GetRealPath()<<"  >>>>>>>>  "<<SrcFile->GetFilePath()<<"."<<std::endl;;
	return Result;
}
