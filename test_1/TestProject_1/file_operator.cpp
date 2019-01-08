#include "file_operator.h"
#include <iostream>
#include <algorithm> 
#include "command_factory.h"
#include "command.h"
#include "data_disk.h"
const std::string END_WORKING_PATH(">");

FileOperator::FileOperator()
	: m_CurWorkPath("root"),
	m_CommandMaker(std::make_unique < CommandFactory>())
{

}

FileOperator::~FileOperator()
{

}

void FileOperator::RuningLoop()
{
	while (true)
	{
		std::string TempArg("");
		m_CurWorkPath = g_DataDiskPtr->GetWorkingPath();
		std::cout << m_CurWorkPath << END_WORKING_PATH;
		std::cin.clear();
		std::cin.sync();
		getline(std::cin, TempArg);
		if(TempArg.empty())
			continue;
		std::vector<std::string> ResolveResult = ResolveInput(TempArg);
		if (ResolveResult.empty())
		{
			std::cout << STR_UNUSED_COMMAND << std::endl;
			continue;
		}
		std::unique_ptr< Command> CommandPtr = m_CommandMaker->Factory(ResolveResult);
		if (NULL == CommandPtr.get())
		{
			if( ResolveResult[0] != "cls")
				std::cout << STR_UNUSED_COMMAND << std::endl;
			continue;
		}
		CommandPtr->SetCurWorkingPath(m_CurWorkPath);
		ErrorCode RunningResult = CommandPtr->Run();
		PrintResult(RunningResult);
	}
}

bool FileOperator::ExecuteCmd(std::string cmdStr)
{
	if (cmdStr.empty())
		return false;
	std::vector<std::string> ResolveResult = ResolveInput(cmdStr);
	if (ResolveResult.empty())
	{
		std::cout << STR_UNUSED_COMMAND << std::endl;
		return false;
	}
	std::unique_ptr< Command> CommandPtr = m_CommandMaker->Factory(ResolveResult);
	if (NULL == CommandPtr.get())
	{
		return false;
	}
	CommandPtr->SetCurWorkingPath(m_CurWorkPath);
	ErrorCode RunningResult = CommandPtr->Run();
	PrintResult(RunningResult);
	return true;
}

std::vector<std::string> FileOperator::ResolveInput(const std::string& InputStr)
{
	std::vector<std::string> ArgList;
	std::string tempSt(InputStr);
	if (InputStr.empty())
		return ArgList;
	int ArgStart = 0;
	int ArgEnd = 0;
	std::replace_if(tempSt.begin(), tempSt.end(), [](char in) {return in == '\\'; }, '/');
	ArgList = SplitStr(tempSt,' ');

	return ArgList;
}

void FileOperator::PrintResult(ErrorCode Result)
{
	switch (Result)
	{
	case NULL_CODE:
		return;
	case SUCCESS_CODE:
		return;
	case ERROR_DST_PATH_CODE:
		std::cout<<STR_DST_ERROR<< std::endl;
		break;
	case ERROR_DATA_DISK_UNKNOW:
		std::cout<<STR_UNKONW_DISK_ERROR<< std::endl;
		break;
	case ERROR_SRC_PATH_CODE:
		std::cout<<STR_SRC_ERROR << std::endl;
		break;
	case ERROR_THE_NAME_EXIST_CODE:
		std::cout<<STR_NAME_EXIST << std::endl;
		break;
	case ERROR_ARG_COUNT_CODE:
		std::cout<<STR_ARG_COUNT_ERROR << std::endl;
		break;
	case ERROR_COMMAND_USE_CODE:
		std::cout<<STR_UNUSED_COMMAND << std::endl;
		break;
	case ERROR_USER_STOP_CODE:
		std::cout<<STR_USER_STOP << std::endl;
		break;
	case ERROR_LNK_CREATE_CODE:
		std::cout<<STR_LNK_CREATE_FILE << std::endl;
		break;
	case ERROR_PATH_N_TYPE:
		std::cout<<STR_SAME_NAME_DIF_TYPE << std::endl;
		break;
	case ERROR_FOLDER_NOT_NULL:
		std::cout<<STR_FOLDER_NOT_NULL << std::endl;
		break;
	case ERROR_DATA_DISK_NULL:
		std::cout<<STR_DATA_DISK_NULL << std::endl;
		break;
	case ERROR_DISK_READ_CODE:
		std::cout<<STR_DISK_READ_FAILED << std::endl;
		break;
	case ERROR_NEW_NAME:
		std::cout<<STR_NEW_NAME << std::endl;
		break;
	default:
		break;
	}
}

