#pragma once
#include <map>
#include "model_def.h"

class LinkFileManager;
typedef std::map<std::string, FileInfo*> FileMap;
struct FileInOutStructure
{
	int32_t FileType;
	int32_t PathSize;
	int32_t FileSize;
	char CreateTime[25];
	char ModifyTime[25];
};
class DataDisk
{
public:
	DataDisk();
	~DataDisk();
	ErrorCode			AddFileRealDisk2DataDisk(std::string Src, std::string Dst);
	ErrorCode			AddFileDataDisk2RealDisk(std::string Src, std::string Dst);
	ErrorCode			AddFileDataDisk2DataDisk(std::string Src, std::string Dst);
	ErrorCode			AddNewNode(FileInfo* NewNode, FileInfo* ParentNode);
	ErrorCode			ChangeParent(std::string& Src, std::string& NewPath);
	ErrorCode			ModifyName(FileInfo* SrcNode, std::string & NewName);
	ErrorCode			DelFile(FileInfo* FileNode);


	const std::string&	GetWorkingPath();
	ErrorCode			SetWorkingPath(std::string& Path);
	bool				IsPathExsit(const std::string& Path);
	FileInfo*			GetFileInfo(const std::string& Path);

	ErrorCode			LoadData(const std::string& Path);
	ErrorCode			SaveData(const std::string& Path);

	//tool
	std::string			GetParentPath(std::string& PathStr);
	std::string			GetName(std::string& str);
	bool				CheckRealDiskPathIsFolder(std::string& RealPath);
	bool				FormatDisk();
private:
	void				WriteDateInfo(const std::string& CreateDate,const std::string& ModifyDate , FileInOutStructure& WriteStructure);

private:
	FileInfo*			m_RootNode;
	FileMap				m_FileMap;
	FileInfo*			m_CurWorkingNode;
	LinkFileManager*	m_LinkFileManager;
	
};

extern std::unique_ptr<DataDisk> g_DataDiskPtr;