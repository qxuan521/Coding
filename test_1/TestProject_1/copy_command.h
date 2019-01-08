#pragma once
#include "command.h"

class CopyCommand : public Command
{
public:
	CopyCommand(std::vector<std::string>& ArgList);
	virtual ~CopyCommand();
	virtual ErrorCode	Run();
	virtual ErrorCode	ResolvePath();
	bool				OverWriteSecendAsk(std::string& FileName);
private:
	ErrorCode			DataDist2DataDistCopy(std::string& SrcStr,std::string& DstStr);
	ErrorCode			DataDisk2RealDiskCopy(std::string& SrcStr, std::string& DstStr);
private:
	std::vector<std::string>		m_SrcPathArr;
	std::vector<std::string>		m_DstPathArr;

};