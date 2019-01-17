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
		//�ַ������� Ҫ�ܰ����ո�����
		YCommandInfo rResoveResult = rInputResolve.resolve(szInput);
		//ʵ��������
		std::shared_ptr<YCommand> pCommand = rCommandFactory.queryCommandPtr(rResoveResult);
		if (nullptr == pCommand)
		{
			continue;
		}
		pCommand->setCurWoringPath(szCurWoringPath);
		pCommand->excultCommand(rResoveResult);
	}
}

void YSystem::runCommand(const std::string & szCmd)
{
	YCommandFactory rCommandFactory;
	YInputResolve rInputResolve;
	YCommandInfo rResoveResult = rInputResolve.resolve(szCmd);
	//ʵ��������
	std::string szCurWoringPath = g_pDiskOperator->getCurWorkingPath();
	std::string szInput;
	std::shared_ptr<YCommand> pCommand = rCommandFactory.queryCommandPtr(rResoveResult);
	if (nullptr == pCommand)
	{
		return;
	}
	pCommand->setCurWoringPath(szCurWoringPath);
	pCommand->excultCommand(rResoveResult);
}
