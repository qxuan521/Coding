#pragma once
#include <vector>
#include <memory>
class YCommand;
class YCommandFactory
{
public:
	YCommandFactory();
	~YCommandFactory();

	std::shared_ptr< YCommand> queryCommandPtr(std::string& szCommandName);

private:
	std::vector < std::shared_ptr< YCommand> > m_rCommandPool;
};

