#include "y_command_factory.h"
#include "y_touch_command.h"
#include "y_dir_command.h"
#include "y_mkdir_command.h"
#include "y_mkdisk_command.h"
#include "y_mklink_command.h"
#include "y_cd_command.h"
#include "y_cddisk_command.h"
#include "y_copy_command.h"
#include "y_load_command.h"
#include "y_save_command.h"
#include "y_ren_command.h"
#include "y_rd_command.h";
#include "y_del_command.h"
#include "y_cls_command.h"
#include "y_move_command.h"
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
	m_rCommandPool[COMMAND_COPY] = std::make_shared<YCopyCommand>("copy");
	m_rCommandPool[COMMAND_RD] = std::make_shared<YRdCommand>("rd");
	m_rCommandPool[COMMAND_REN] = std::make_shared<YRenCommand>("ren");
	m_rCommandPool[COMMAND_DEL] = std::make_shared<YDelCommand>("del");
	m_rCommandPool[COMMAND_SAVE] = std::make_shared<YSaveCommand>("save");
	m_rCommandPool[COMMAND_LOAD] = std::make_shared<YLoadCommand>("load");
	m_rCommandPool[COMMAND_MOVE] = std::make_shared<YMoveCommand>("move");
	m_rCommandPool[COMMAND_CLS] = std::make_shared<YClsCommand>("cls");
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
			if (nullptr == m_rCommandPool[index])
			{
				continue;
			}
			if (m_rCommandPool[index]->isThisCommand(szCommandName))
			{
				m_rCommandPool[index]->resetCommand();
				return m_rCommandPool[index];
			}
		}
	}
	return NULL;
}


