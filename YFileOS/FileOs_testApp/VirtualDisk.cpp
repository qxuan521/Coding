#include "VirtualDisk.h"
#include "data_disk.h"
#include "file_operator.h"
#include "file_info.h"
VirtualDisk::VirtualDisk()
{
}

VirtualDisk::~VirtualDisk()
{
}

bool VirtualDisk::formatDisk(void)
{
	
	return g_DataDiskPtr->FormatDisk();
}

bool VirtualDisk::executeCmd(std::string cmdStr)
{
	FileOperator System;
	System.ExecuteCmd(cmdStr);
	return true;
}

std::string VirtualDisk::getCurPath(void)
{
	return g_DataDiskPtr->GetWorkingPath();
}

bool VirtualDisk::containNode(std::string path, int & size, int & type)
{
	std::shared_ptr< FileInfo> File = g_DataDiskPtr->GetFileInfo(path);
	if (NULL == File)
		return false;
	size = File->GetSize();
	if (FOLDER_FILE == File->GetRealType())
	{
		type = 1;
	}
	else if(DATA_FILE == File->GetRealType())
	{
		type = 2;
	}
	else
	{
		type = 3;
	}
	return true;
}
#include "link_file_info.h"
std::string VirtualDisk::getLinkNode(std::string path)
{
	std::shared_ptr<LinkFileInfo> LinkFile = std::static_pointer_cast<LinkFileInfo>(g_DataDiskPtr->GetFileInfo(path));
	if (NULL == LinkFile)
		return "";
	return LinkFile->GetDstFile()->GetRealPath();
}