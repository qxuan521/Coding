#pragma once
#include <string>

class VirtualDisk
{
public:
	VirtualDisk();
	~VirtualDisk(); 
	bool formatDisk(void);
	bool executeCmd(std::string cmdStr);
	std::string getCurPath(void);
	bool containNode(std::string path, int& size, int& type);
	std::string getLinkNode(std::string path);
};