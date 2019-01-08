#pragma once
#include "model_def.h"

enum CommandType
{
	COMMAND_DIR = 0,
	COMMAND_MD = 1,
	COMMAND_RD = 2,
	COMMAND_CD = 3,
	COMMAND_DEL = 4,
	COMMAND_COPY = 5,
	COMMAND_REN = 6,
	COMMAND_MOVE = 7,
	COMMAND_MKLINK = 8,
	COMMAND_SAVE = 9,
	COMMAND_LOAD = 10,
	COMMAND_CLS = 11,
	COMMAND_MAX ,
	ERROR_COMMAND_NUM
};

class Command;
class CommandFactory
{
public:
	CommandFactory();
	~CommandFactory();
	std::unique_ptr< Command>	Factory(std::vector<std::string>& ArgList);
private:
	CommandType CalcCommandNum(std::string& CommandStr);
	std::vector<std::string>				m_CommandArr;
};