#pragma once
#include <vector>
#include <memory>
#include "y_command.h"
class YCommand;
enum CommandType
{
	COMMAND_TOUCH,
	COMMAND_DIR,
	COMMAND_MD,
	COMMAND_MKDISK,
	COMMAND_MKLINK, 
	COMMAND_CD,
	COMMAND_CDDISK,
	COMMAND_COPY,
	COMMAND_RD,
	COMMAND_DEL ,
	COMMAND_REN ,
	COMMAND_MOVE ,
	COMMAND_SAVE ,
	COMMAND_LOAD ,
	COMMAND_CLS ,
	COMMAND_MAX,
	ERROR_COMMAND_NUM
};

class YCommandFactory
{
public:
	YCommandFactory();
	~YCommandFactory();

	std::shared_ptr<YCommand> queryCommandPtr(YCommandInfo& rCommandInfo);

private:

	std::vector <std::shared_ptr<YCommand>>		m_rCommandPool;
};

