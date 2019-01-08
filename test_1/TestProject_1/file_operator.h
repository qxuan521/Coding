#pragma once
#include "model_def.h"
#include "string_def.h"
class CommandFactory;
class FileOperator
{
public:
	FileOperator();
	~FileOperator();
	void								RuningLoop();
	bool								ExecuteCmd(std::string cmdStr);
private:
	std::vector<std::string>			ResolveInput(const std::string& InputStr);
	void								PrintResult(ErrorCode Result);
private:
	std::string									m_CurWorkPath;
	std::unique_ptr < CommandFactory>			m_CommandMaker;
};