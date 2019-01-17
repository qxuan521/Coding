#include "VirtualDisk.h"
#include "y_disk_operator.h"
#include "y_system.h"
#include "y_file.h"
VirtualDisk::VirtualDisk()
{
}

VirtualDisk::~VirtualDisk()
{
}

bool VirtualDisk::formatDisk(void)
{
	g_pDiskOperator->formatDisk()
	return true;
}

bool VirtualDisk::executeCmd(std::string cmdStr)
{
	YSystem System;
	System.runCommand(cmdStr);
	return true;
}

std::string VirtualDisk::getCurPath(void)
{
	std::string szPath = g_pDiskOperator->getCurWorkingPath();
	if (g_pDiskOperator->isRootName(szPath))
	{
		szPath += '\\';
	}
	return szPath;
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