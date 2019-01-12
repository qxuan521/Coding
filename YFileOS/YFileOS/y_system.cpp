#include "y_system.h"
#include <string>
#include <iostream>
#include <memory>
#include "y_command.h"
#include "y_command_factory.h"
#include "y_disk_operator.h"
#include "y_input_resolve.h"
YSystem::YSystem()
{
}

YSystem::~YSystem()
{
}

void YSystem::runingLoop()
{
	YCommandFactory rCommandFactory;
	YInputResolve rInputResolve;
	while (true)
	{
		std::string szCurWoringPath = g_pDiskOperator->getCurWorkingPath();
		std::string szInput;
		std::cout << g_pDiskOperator->getCurWorkingPath() << ">";
		std::cin.clear();
		getline(std::cin, szInput);
		std::cin.clear();
		//字符串处理 要能包含空格名字
		YCommandInfo rResoveResult = rInputResolve.resolve(szInput);
		//实例化命令
		std::shared_ptr<YCommand> pCommand = rCommandFactory.queryCommandPtr(rResoveResult.szCommandName);
		if (nullptr == pCommand)
		{
			continue;
		}
		pCommand->setCurWoringPath(szCurWoringPath);
		pCommand->excultCommand(rResoveResult.rArglist);
	}
}
