#include "y_system.h"
#include <string>
#include <iostream>
#include <memory>
#include "y_command.h"
#include "y_command_factory.h"
#include "y_disk_operator.h"
YSystem::YSystem()
{
}

YSystem::~YSystem()
{
}

void YSystem::runingLoop()
{
	YCommandFactory rCommandFactory;
	while (true)
	{
		std::string szInput;
		std::cout << g_pDiskOperator->getCurWorkingPath() << ">";
		std::cin.clear();
		getline(std::cin, szInput);
		//字符串处理 要能包含空格名字
		//
		//实例化命令
		std::shared_ptr<YCommand> pCommand = rCommandFactory.queryCommandPtr(szInput/*临时填写需要修改 */);
		//pCommand->excultCommand(szInput);

	}
}
