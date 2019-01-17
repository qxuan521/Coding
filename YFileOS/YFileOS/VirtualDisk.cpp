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
	std::vector<YIFile*> rResult;
	g_pDiskOperator->queryAllNode(path, rResult);
	if (rResult.empty() || NULL == rResult[0])
	{
		size = -1;
		type = 0;
		return false;
	}
	size = rResult[0]->getFileSize();
	if (rResult[0])->IsRealFolder())
	{
		type = 1;
	}
	else if(rResult[0])->IsRealFile())
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
	g_pDiskOperator->queryAllNode(path, rResult);
	if (rResult.empty() || NULL == rResult[0])
	{
		return std::string("");
	}
	return rResult[0]->getShowName();
}