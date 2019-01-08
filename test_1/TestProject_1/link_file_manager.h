#pragma once
#include <map>
#include <vector>
#include "link_file_info.h"
class FileInfo;

typedef std::map<FileInfo*, std::vector<FileInfo*>> Src2LinkMap;
class LinkFileManager 
{
public:
	LinkFileManager();
	~LinkFileManager();
	void						AddLink(FileInfo* SrcFile, FileInfo* LinkFile);
	void						DelLinkFile(FileInfo* LinkFile);
	void						DelDstFile(FileInfo* SrcFile);
	void						ChangeDstFile(FileInfo* OldFile, FileInfo* NewFile);
	void						Clear();
private:
	Src2LinkMap					m_ManagerMap;
};

