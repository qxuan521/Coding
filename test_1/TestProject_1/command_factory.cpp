#include "command_factory.h"
#include "copy_command.h"
#include "md_command.h"
#include "dir_command.h"
#include "cd_command.h"
#include "mklink_command.h"
#include "move_command.h"
#include "del_command.h"
#include "rd_command.h"
#include "save_command.h"
#include "load_command.h"
#include "ren_command.h"
#include "cls_command.h"

CommandFactory::CommandFactory()
{
	m_CommandArr.resize(COMMAND_MAX);
	m_CommandArr[COMMAND_DIR]	=	"dir";
	m_CommandArr[COMMAND_MD]	=	"md";
	m_CommandArr[COMMAND_RD]	=	"rd";
	m_CommandArr[COMMAND_CD]	=	"cd";
	m_CommandArr[COMMAND_DEL]	=	"del";
	m_CommandArr[COMMAND_COPY]	=	"copy";
	m_CommandArr[COMMAND_REN]	=	"ren";
	m_CommandArr[COMMAND_MOVE]	=	"move";
	m_CommandArr[COMMAND_MKLINK] =	"mklink";
	m_CommandArr[COMMAND_SAVE]	=	"save";
	m_CommandArr[COMMAND_LOAD]	=	"load";
	m_CommandArr[COMMAND_CLS]	=	"cls";
	
}

CommandFactory::~CommandFactory()
{
}

std::unique_ptr<Command> CommandFactory::Factory(std::vector<std::string> & ArgList)
{
	if (ArgList.size() < 1 || ArgList[0].empty())
		return NULL;
	switch (CalcCommandNum(ArgList[0]))
	{
	case COMMAND_DIR:
		return std::make_unique<Dircomman>(ArgList);
	case COMMAND_MD:
		return std::make_unique <MdCommand>(ArgList);
	case COMMAND_RD:
		return std::make_unique <RdCommand>(ArgList);
	case COMMAND_CD:
		return std::make_unique < CDCommand>(ArgList);
	case COMMAND_DEL:
		return std::make_unique < DelCommand>(ArgList);
	case COMMAND_COPY:
		return std::make_unique < CopyCommand>(ArgList);
	case COMMAND_REN:
		return std::make_unique < RenCommand>(ArgList);
	case COMMAND_MOVE:
		return std::make_unique < MoveCommand>(ArgList);
	case COMMAND_MKLINK:
		return std::make_unique < MklinkCommand>(ArgList);
	case COMMAND_SAVE:
		return std::make_unique < SaveCommand>(ArgList);
	case COMMAND_LOAD:
		return std::make_unique < Loadcommand>(ArgList);
	case COMMAND_CLS:
		return std::make_unique < ClsCommand>(ArgList);
		break;
	case ERROR_COMMAND_NUM:
	case COMMAND_MAX:
	default:
		return NULL;
	}
	return NULL;
}

CommandType CommandFactory::CalcCommandNum(std::string& CommandStr)
{
	if (CommandStr.empty() || m_CommandArr.empty())
		return ERROR_COMMAND_NUM;
	for (int index = COMMAND_DIR; index < COMMAND_MAX; ++index)
	{
		if (m_CommandArr[index] == CommandStr)
			return (CommandType)index;
	}
	return ERROR_COMMAND_NUM;
}
