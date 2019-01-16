#include "y_move_command.h"

YMoveCommand::YMoveCommand(const std::string& szName)
	: YCommand(szName, 2)
{

}

YMoveCommand::~YMoveCommand()
{

}

YErrorCode YMoveCommand::excultCommand(YCommandInfo & rCommandInfo)
{
	return YErrorCode();
}
