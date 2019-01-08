#pragma once
#include "command.h"

class FileInfo;
struct MoveDstChild
{
	std::string DstParent;
	std::vector<FileInfo*>  SrcChildList;
};

class MoveCommand : public Command
{
public:
	MoveCommand(std::vector<std::string>& ArgList);
	virtual ~MoveCommand();
	virtual ErrorCode	Run();
private:
	ErrorCode MoveSrcFolder(std::string & SrcStr, std::string & DstStr, bool & IsCoverSameFile);
	ErrorCode MoveSrcWildCard(std::string & SrcStr, std::string & DstStr, bool & IsCoverSameFile);
	ErrorCode MoveSrcFile(std::string & SrcStr, std::string & DstStr, bool & IsCoverSameFile);
	bool	  SameCheck(std::string SrcStr, std::string DstStr, bool& IsCoverSameFile);
	ErrorCode DelChildNode(std::string Str);

	std::vector<std::string>		m_SrcPathList;
	std::vector<std::string>		m_DstPathList;
};
