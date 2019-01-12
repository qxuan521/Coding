#pragma once
#include <vector>
#include <memory>
class YCommand;
enum CommandType
{
	COMMAND_TOUCH,
	COMMAND_DIR,
	COMMAND_MKDISK,
	COMMAND_MD ,
	COMMAND_RD ,
	COMMAND_CD ,
	COMMAND_DEL ,
	COMMAND_COPY ,
	COMMAND_REN ,
	COMMAND_MOVE ,
	COMMAND_MKLINK ,
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

	std::shared_ptr<YCommand> queryCommandPtr(std::string& szCommandName);

private:

	std::vector <std::shared_ptr<YCommand>>		m_rCommandPool;
};

