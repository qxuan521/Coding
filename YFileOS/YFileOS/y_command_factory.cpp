#include "y_command_factory.h"
#include "y_command.h"

YCommandFactory::YCommandFactory()
{

}

YCommandFactory::~YCommandFactory()
{

}

std::shared_ptr<YCommand> YCommandFactory::queryCommandPtr(std::string & szCommandName)
{
	if (!m_rCommandPool.empty())
	{
		for (size_t index = 0; index < m_rCommandPool.size(); ++index)
		{
			if (m_rCommandPool[index]->getCommandName() == szCommandName)
				return m_rCommandPool[index];
		}
	}
	
	return NULL;
}
