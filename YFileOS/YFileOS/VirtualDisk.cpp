#include "VirtualDisk.h"
#include "y_disk_operator.h"
#include "y_system.h"
#include "y_file.h"
#include "y_tool.h"
#include <fstream>
VirtualDisk::VirtualDisk()
{
}

VirtualDisk::~VirtualDisk()
{
}

bool VirtualDisk::formatDisk(void)
{
	g_pDiskOperator->formatDisk();
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
	std::replace_if(szPath.begin(), szPath.end(), [](char in) {return in == '/'; }, '\\');
	return szPath;
}

bool VirtualDisk::containNode(std::string path, int & size, int & type)
{
	std::vector<YIFile*> rResult;
	if (isRealPath(path))
	{
		std::string szTempPath = getPathFromRealPath(path);
		dealRealPath(szTempPath, size,type);
		return true;
	}
	std::replace_if(path.begin(), path.end(), [](char in) {return in == '\\'; }, '/' );
	transform(path.begin(), path.end(), path.begin(), ::tolower);
	g_pDiskOperator->queryAllNode(path, rResult);
	if (rResult.empty() || NULL == rResult[0])
	{
		size = -1;
		type = 0;
		return false;
	}
	size = rResult[0]->getFileSize();
	if (rResult[0]->IsRealFolder())
	{
		type = 1;
	}
	else if(rResult[0]->IsRealFile())
	{
		type = 2;
	}
	else
	{
		type = 3;
	}
	return true;
}
std::string VirtualDisk::getLinkNode(std::string path)
{
	std::vector<YIFile*> rResult;
	std::replace_if(path.begin(), path.end(), [](char in) {return in == '\\'; }, '/');
	transform(path.begin(), path.end(), path.begin(), ::tolower);
	g_pDiskOperator->queryAllNode(path, rResult);
	if (rResult.empty() || NULL == rResult[0])
	{
		return std::string("");
	}
	std::string szDstPath = rResult[0]->getShowName();
	std::replace_if(szDstPath.begin(), szDstPath.end(), [](char in) {return in == '/'; }, '\\');
	return szDstPath;
}

void VirtualDisk::dealRealPath(std::string path, int & size, int & type)
{
	std::fstream rFileReader(path, std::ios::binary | std::ios::in);
	if (!rFileReader.is_open())
	{
		return;
	}
	rFileReader.seekg(0, std::ios::end);
	size = (long)rFileReader.tellg();
	type = 2;
	rFileReader.close();
}

