#include "y_command_factory.h"
#include "y_touch_command.h"
YCommandFactory::YCommandFactory()
{
	m_rCommandPool.resize(COMMAND_MAX);
	m_rCommandPool[COMMAND_TOUCH] = std::make_shared<YTouchCommand>("touch");
	m_rCommandPool[COMMAND_MKDISK];
// 	m_rCommandPool[COMMAND_DIR] = "dir";
// 	m_rCommandPool[COMMAND_MD] = "md";
// 	m_rCommandPool[COMMAND_RD] = "rd";
// 	m_rCommandPool[COMMAND_CD] = "cd";
// 	m_rCommandPool[COMMAND_DEL] = "del";
// 	m_rCommandPool[COMMAND_COPY] = "copy";
// 	m_rCommandPool[COMMAND_REN] = "ren";
// 	m_rCommandPool[COMMAND_MOVE] = "move";
// 	m_rCommandPool[COMMAND_MKLINK] = "mklink";
// 	m_rCommandPool[COMMAND_SAVE] = "save";
// 	m_rCommandPool[COMMAND_LOAD] = "load";
// 	m_rCommandPool[COMMAND_CLS] = "cls";

}

YCommandFactory::~YCommandFactory()
{

}

std::shared_ptr<YCommand> YCommandFactory::queryCommandPtr(std::string & szCommandName)
{
	if (szCommandName.size() < 1 || szCommandName.empty())
		return NULL;
	if (!m_rCommandPool.empty())
	{
		for (size_t index = 0; index < m_rCommandPool.size(); ++index)
		{
			if (m_rCommandPool[index]->isThisCommand(szCommandName))
				return m_rCommandPool[index];
		}
	}
	return NULL;
}


