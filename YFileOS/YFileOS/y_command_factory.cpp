#include "y_command_factory.h"
#include "y_touch_command.h"
#include "y_dir_command.h"
#include "y_mkdir_command.h"
#include "y_mkdisk_command.h"
#include "y_mklink_command.h"
#include "y_cd_command.h"
#include "y_cddisk_command.h"
YCommandFactory::YCommandFactory()
{
	m_rCommandPool.resize(COMMAND_MAX);
	m_rCommandPool[COMMAND_TOUCH] = std::make_shared<YTouchCommand>("touch");
 	m_rCommandPool[COMMAND_DIR] = std::make_shared<YDirCommand>("dir");
 	m_rCommandPool[COMMAND_MD] = std::make_shared<YMkdirCommand>("mkdir");
	m_rCommandPool[COMMAND_MKDISK] = std::make_shared<YMkdiskCommand>("mkdisk");
	m_rCommandPool[COMMAND_MKLINK] = std::make_shared<YMklinkCommand>("mklink");
	m_rCommandPool[COMMAND_CD] = std::make_shared<YCdCommand>("cd");
	m_rCommandPool[COMMAND_CDDISK] = std::make_shared<YCddiskCommand>("[a-zA-Z]+:");

// 	m_rCommandPool[COMMAND_RD] = "rd";
// 	m_rCommandPool[COMMAND_CD] = "cd";
// 	m_rCommandPool[COMMAND_DEL] = "del";
// 	m_rCommandPool[COMMAND_COPY] = "copy";
// 	m_rCommandPool[COMMAND_REN] = "ren";
// 	m_rCommandPool[COMMAND_MOVE] = "move";
// 	m_rCommandPool[COMMAND_SAVE] = "save";
// 	m_rCommandPool[COMMAND_LOAD] = "load";
// 	m_rCommandPool[COMMAND_CLS] = "cls";

}

YCommandFactory::~YCommandFactory()
{

}

std::shared_ptr<YCommand> YCommandFactory::queryCommandPtr(YCommandInfo& rCommandInfo)
{
	std::string szCommandName = rCommandInfo.szCommandName;
	if (szCommandName.size() < 1 || szCommandName.empty())
		return NULL;
	if (!m_rCommandPool.empty())
	{
		for (size_t index = 0; index < m_rCommandPool.size(); ++index)
		{
			if (m_rCommandPool[index]->isThisCommand(szCommandName))
			{
				m_rCommandPool[index]->resetCommand();
				return m_rCommandPool[index];
			}
		}
	}
	return NULL;
}


