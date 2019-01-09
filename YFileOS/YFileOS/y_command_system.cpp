#include "y_command_system.h"
#include "y_disk.h"
#include "y_console.h"
#include "y_tool.h"
#include <algorithm>
struct YInputInfo {
	std::string szCommandStr;
	std::vector < std::string > rCommandArgList;
};

YCommandSyster::YCommandSyster()
{

}

YCommandSyster::~YCommandSyster()
{

}

void YCommandSyster::runSystem()
{
	std::string curPath = g_pDiskPtr->getRootName();
	YConsole	viewer;
	while (true)
	{
		viewer.log(curPath);
		std::string szInput = viewer.getInputStr();
		YInputInfo rCommandStructure;
		resolveInput(szInput, rCommandStructure);
	}
}

void YCommandSyster::resolveInput(const std::string & szInput, YInputInfo& rResult)
{
	if (szInput.empty())
		return;
	replace_if(szInput.begin(), szInput.end(), [](char a)->bool {return'\\' == a; }, '/');
	std::vector < std::string > rResultArr =  splitStrByCharacter(szInput, ' ');
	if(rResultArr.empty())
		return;
	rResult.szCommandStr = rResultArr[0];
	for (size_t index = 1; index < rResultArr.size(); ++index)
	{
		rResult.rCommandArgList.push_back(rResultArr[index]);
	}
}
