#include <fstream>
#include <sys/stat.h>
#include <time.h>
#include <queue>
#include "data_disk.h"
#include "link_file_info.h"
#include "link_file_manager.h"
const int TIME_MAX = 32;
std::unique_ptr<DataDisk> g_DataDiskPtr = std::make_unique<DataDisk>();

struct LinkFileData
{
	std::string RealPath;
	std::string DstPath;
	std::string CreateDate;
	std::string ModifyDate;
};

DataDisk::DataDisk()
	: m_RootNode(new FileInfo(std::string("root"), NULL, FOLDER_FILE))
	, m_LinkFileManager(new LinkFileManager)
{
	std::pair<std::string, FileInfo*> FileMapPair("root", m_RootNode);
	m_CurWorkingNode = m_RootNode;
	m_FileMap.insert(FileMapPair);
}

DataDisk::~DataDisk()
{
	FormatDisk();
	m_FileMap.clear();
	delete m_RootNode;
	m_RootNode = NULL;
}

ErrorCode DataDisk::AddFileRealDisk2DataDisk(std::string Src, std::string Dst)
{
	if (m_FileMap.empty())
		return ERROR_DATA_DISK_UNKNOW;
	std::string DstParent =GetParentPath(Dst);
	if (!m_FileMap.count(DstParent))
		return ERROR_DST_PATH_CODE;
	FileInfo* ParentNode = m_FileMap[DstParent];
	if (NULL == ParentNode)
		return ERROR_DATA_DISK_UNKNOW;
	if (m_FileMap.count(Dst))
		return ERROR_THE_NAME_EXIST_CODE;
	//构建一个新的文对象
	std::fstream rFile(Src,std::ios::binary | std::ios::in);
	if (!rFile.is_open())
		return ERROR_SRC_PATH_CODE;
	rFile.seekg(0,std::ios::end);
	long FileSize = (long)rFile.tellg();
	char* DataBuffer = new char[FileSize];
	rFile.seekg(0,std::ios::beg);
	rFile.read(DataBuffer, FileSize);
	rFile.close();
	FileInfo* NewFile = new FileInfo(Dst, ParentNode, DATA_FILE);
	time_t timep;
	time(&timep);
	char stTmp[TIME_MAX];
	stTmp[24] = '\0'; 
	tm TempTime;
	gmtime_s(&TempTime, &timep);
	asctime_s(stTmp, &TempTime);
	NewFile->SetCreateDate(stTmp);
	NewFile->SetFileData(DataBuffer, FileSize);
	ParentNode->AddChild(NewFile);
	std::pair<std::string, FileInfo*> NewPair(NewFile->GetFilePath(), NewFile);
	m_FileMap.insert(NewPair);
	delete [] DataBuffer;
	return SUCCESS_CODE;
}

ErrorCode DataDisk::AddFileDataDisk2RealDisk(std::string Src, std::string Dst)
{
	if (m_FileMap.empty())
		return ERROR_DATA_DISK_UNKNOW;
	if (!m_FileMap.count(Src))
		return ERROR_SRC_PATH_CODE;
	FileInfo* SrcNode = m_FileMap[Src];
	if (NULL == SrcNode)
		return ERROR_SRC_PATH_CODE;
	//构建一个新的文对象
	std::fstream rFile(Dst, std::ios::binary | std::ios::out | std::ios::trunc);
	if (!rFile.is_open())
		return ERROR_SRC_PATH_CODE; 
	char* DataBuffer = SrcNode->GetFileData();
	if (NULL != DataBuffer)
		rFile.write(DataBuffer, SrcNode->GetFileSize());
	rFile.close();
	return SUCCESS_CODE;
}

ErrorCode DataDisk::AddFileDataDisk2DataDisk(std::string Src, std::string Dst)
{
	if (m_FileMap.empty())
		return ERROR_DATA_DISK_UNKNOW;
	std::string DstParent = GetParentPath(Dst);
	if (!m_FileMap.count(DstParent))
		return ERROR_DST_PATH_CODE;
	FileInfo* ParentNode = m_FileMap[DstParent];
	if (NULL == ParentNode)
		return ERROR_DATA_DISK_UNKNOW;
	if (ParentNode->CheckSameNameWithChildren(Dst))
		return ERROR_THE_NAME_EXIST_CODE;
	if (!m_FileMap.count(Src))
		return ERROR_SRC_PATH_CODE;
	FileInfo* SrcNode = m_FileMap[Src];
	FileInfo* NewNode = NULL;
	if (LINK_FILE == SrcNode->GetRealType())
	{
		FileInfo* DstFile = static_cast<LinkFileInfo*>(SrcNode)->GetDstFile();
		if (NULL == DstFile)
			return ERROR_SRC_PATH_CODE;
		NewNode = new LinkFileInfo(Dst, ParentNode, DstFile);
	}
	else
	{
		NewNode = new FileInfo(Dst, ParentNode, SrcNode->GetRealType());
		NewNode->SetFileData(SrcNode->GetFileData(), SrcNode->GetFileSize());
	}
	if (NULL == NewNode)
		return ERROR_DATA_DISK_UNKNOW;
	ParentNode->AddChild(NewNode);
	time_t timep;
	time(&timep);
	tm TempTime;
	char stTmp[TIME_MAX];
	stTmp[24] = '\0'; 
	gmtime_s(&TempTime, &timep);
	asctime_s(stTmp, &TempTime);
	NewNode->SetCreateDate(stTmp);
	NewNode->SetModifyDate(stTmp);
	std::pair<std::string, FileInfo*> NewPair(NewNode->GetRealPath(), NewNode);
	m_FileMap.insert(NewPair);
	return SUCCESS_CODE;
}

ErrorCode DataDisk::AddNewNode(FileInfo* NewNode, FileInfo* ParentNode)
{
	if (NULL == NewNode || NULL == ParentNode|| m_FileMap.empty())
		return ERROR_DATA_DISK_UNKNOW;
	if (!m_FileMap.count(ParentNode->GetFilePath()))
		return ERROR_DST_PATH_CODE;
	if (LINK_FILE == NewNode->GetRealType())
	{
		FileInfo* SrcFile = GetFileInfo(NewNode->GetFilePath());
		if (NULL == SrcFile)
			return ERROR_LNK_CREATE_CODE;
		m_LinkFileManager->AddLink(SrcFile, NewNode);
	}
	ParentNode->AddChild(NewNode);
	time_t timep;
	time(&timep);
	tm TempTime;
	char stTmp[TIME_MAX];
	stTmp[24] = '\0';
	gmtime_s(&TempTime, &timep);
	asctime_s(stTmp, &TempTime);
	NewNode->SetCreateDate(stTmp);
	NewNode->SetModifyDate(stTmp);
	std::pair<std::string, FileInfo*> NewPair(NewNode->GetRealPath(), NewNode);
	m_FileMap.insert(NewPair);
	//链接文件处理
	return SUCCESS_CODE;
}

ErrorCode DataDisk::ChangeParent(std::string& Src, std::string& NewPath)
{
	FileInfo* SrcNode = GetFileInfo(Src);
	if (NULL == SrcNode)
		return ERROR_SRC_PATH_CODE;
	FileInfo* SrcParent = SrcNode->GetRealParent();
	if (NULL == SrcParent)
		return ERROR_SRC_PATH_CODE;
	//摘除
	FileInfo* SrcCopy = NULL;
	if (FOLDER_FILE == SrcNode->GetRealType())
	{
		SrcCopy = new FileInfo(Src, SrcParent, FOLDER_FILE);
		SrcCopy->SetCreateDate(SrcNode->GetCreateDate());
		SrcCopy->SetModifyDate(SrcNode->GetModifyDate());
	}
	else
	{
		SrcParent->DelChild(SrcNode);
		SrcCopy = SrcNode;
	}
	FileInfo* DstSame = GetFileInfo(NewPath);
	FileInfo* DstParentNode = GetFileInfo(GetParentPath(NewPath));
	std::string OldPath = SrcCopy->GetRealPath();
	std::string Name = GetName(NewPath);
	SrcCopy->SetFileName(Name);
	if (NULL == DstParentNode || FOLDER_FILE != DstParentNode->GetType())
		return ERROR_DST_PATH_CODE;
	DstParentNode->DelChild(DstSame);
	SrcCopy->SetParent(DstParentNode);
	DstParentNode->AddChild(SrcCopy);
	if (NULL != DstSame)
	{
		if (FOLDER_FILE == DstSame->GetRealType())
		{
			for (int index = 0; index < DstSame->GetChildrenSize();++index)
			{
				SrcCopy->AddChild(DstSame->GetChildrenList()[index]);
				DstSame->GetChildrenList()[index]->SetParent(SrcCopy);
			}
		}
		
		m_FileMap[NewPath] = SrcCopy;
		if (FOLDER_FILE == SrcNode->GetRealType())
		{
		}
		else
		{
			FileMap::iterator iter = m_FileMap.find(Src);
			if (iter != m_FileMap.end())
				m_FileMap.erase(iter);
			m_LinkFileManager->ChangeDstFile(DstSame, SrcCopy);
			DstSame = NULL;
		}
	}
	else
	{
		if (FOLDER_FILE == SrcNode->GetRealType())
		{
		}
		else
		{
			FileMap::iterator iter = m_FileMap.find(Src);
			if (iter != m_FileMap.end())
				m_FileMap.erase(iter);
		}
		std::pair<std::string, FileInfo*> NewNode(NewPath, SrcCopy);
		m_FileMap.insert(NewNode);
	}
	return SUCCESS_CODE;
}

ErrorCode DataDisk::ModifyName(FileInfo* SrcNode, std::string & NewName)
{
	if (NULL == SrcNode)
		return ERROR_SRC_PATH_CODE;
	std::string OldPath = SrcNode->GetFilePath();
	std::string NewPath = GetParentPath(SrcNode->GetRealPath());
	NewPath.append("/");
	NewPath.append(NewName);
	if (IsPathExsit(NewPath))
		return ERROR_THE_NAME_EXIST_CODE;
	FileMap::iterator iter = m_FileMap.find(OldPath);
	if (iter == m_FileMap.end())
		return ERROR_DATA_DISK_UNKNOW;
	SrcNode->SetFileName(NewName);
	m_FileMap.erase(iter);
	std::pair<std::string, FileInfo*> NewNode(NewPath, SrcNode);
	m_FileMap.insert(NewNode);
	return SUCCESS_CODE;
}

ErrorCode DataDisk::DelFile(FileInfo* FileNode)
{
	if (NULL == FileNode)
		return ERROR_SRC_PATH_CODE;
	FileInfo* ParentNode = FileNode->GetRealParent();
	if (NULL == ParentNode)
		return ERROR_SRC_PATH_CODE;
	ParentNode->DelChild(FileNode);
	if (LINK_FILE == FileNode->GetRealType())
		m_LinkFileManager->DelLinkFile(FileNode);
	else
		m_LinkFileManager->DelDstFile(FileNode);
	FileMap::iterator iter = m_FileMap.find(FileNode->GetRealPath());
	if (m_FileMap.end() == iter)
		return ERROR_DATA_DISK_UNKNOW;
	m_FileMap.erase(iter);
	return SUCCESS_CODE;
}


const std::string & DataDisk::GetWorkingPath()
{
	return m_CurWorkingNode->GetRealPath();
}

std::string  DataDisk::GetName(std::string& str)
{
	int NameIndex = str.find_last_of('/');
	if (std::string::npos == NameIndex)
		return str;
	else
		return str.substr(NameIndex + 1, str.size() - NameIndex - 1);
}

ErrorCode DataDisk::SetWorkingPath(std::string & Path)
{
	if (Path.empty())
		return ERROR_DST_PATH_CODE;
	std::string ParentPath = g_DataDiskPtr->GetParentPath(Path);
	FileInfo* ParentNode = g_DataDiskPtr->GetFileInfo(ParentPath);
	if (NULL == ParentNode)
	{
		FileInfo* WorkingFileInfo = GetFileInfo(Path);
		if (NULL == WorkingFileInfo)
			return ERROR_DST_PATH_CODE;
		if (DATA_FILE == WorkingFileInfo->GetType())
			return ERROR_DST_PATH_CODE;
		m_CurWorkingNode = WorkingFileInfo;
	}
	else
	{
		std::string DstPath = ParentNode->GetFilePath() + "/" +g_DataDiskPtr->GetName(Path);
		FileInfo* WorkingFileInfo = GetFileInfo(DstPath);
		if (NULL == WorkingFileInfo)
			return ERROR_DST_PATH_CODE;
		if (DATA_FILE == WorkingFileInfo->GetType())
			return ERROR_DST_PATH_CODE;
		m_CurWorkingNode = WorkingFileInfo;
	}
	return SUCCESS_CODE;
}

bool DataDisk::IsPathExsit(const std::string & Path)
{
	return m_FileMap.count(Path) != 0;
}

FileInfo* DataDisk::GetFileInfo(const std::string & Path)
{
	std::vector<std::string> strArr = SplitStr(Path, '/');
	if (strArr.empty())
		return nullptr;
	FileInfo* Result = m_RootNode;
	for (int index = 0;index < strArr.size();++index)
	{
		if(0 == index)
			continue;
		if (FOLDER_FILE != Result->GetType())
			break;
		std::vector<FileInfo*>& ChildrenArr = Result->GetChildrenList();
		FileInfo* tempNode = nullptr;
		for (int loopCount = 0; loopCount < ChildrenArr.size();++loopCount)
		{
			if(nullptr == ChildrenArr[loopCount])
				continue;
			if (strArr[index] == ChildrenArr[loopCount]->GetFileName())
			{
				tempNode = ChildrenArr[loopCount];
				break;
			}
		}
		if (nullptr == tempNode)
			return nullptr;
		Result = tempNode;
	}
	if (strArr[strArr.size() - 1] != Result->GetFileName())
		return nullptr;
	return Result;
}
ErrorCode DataDisk::LoadData(const std::string & Path)
{
	FileInOutStructure ReadStructure;

	std::vector<char> PathBuffer;
	std::vector<char> FiledataBuffer;
	char* PathBufferPtr = NULL;
	char* FileDataBufferPtr = NULL;

	std::queue<LinkFileData> LinkFileQueue;
	std::fstream rFile(Path, std::ios::binary | std::ios::in );
	if (m_FileMap.size() > 1)
	{
		FormatDisk();
		m_FileMap.clear();
		std::pair<std::string, FileInfo*> FileMapPair("root", m_RootNode);
		m_FileMap.insert(FileMapPair);
	}
	if (!rFile.is_open())
		return ERROR_SRC_PATH_CODE;
	while (!rFile.eof())
	{
		rFile.read((char*)&ReadStructure,sizeof(ReadStructure));
		if (ReadStructure.FileType > LINK_FILE || ReadStructure.FileType < DATA_FILE)
			return ERROR_DISK_READ_CODE;
		if (ReadStructure.PathSize < 0)
			return ERROR_DISK_READ_CODE;
		if (ReadStructure.FileSize < 0)
			return ERROR_DISK_READ_CODE;
		if (LINK_FILE == ReadStructure.FileType)
		{
			LinkFileData tempData;
			ReadStructure.CreateTime[24] = '\0';
			ReadStructure.ModifyTime[24] = '\0';
			tempData.CreateDate = ReadStructure.CreateTime;
			tempData.ModifyDate = ReadStructure.ModifyTime;
			if (PathBuffer.empty()|| (int)PathBuffer.size() - 1 < ReadStructure.PathSize)
			{
				PathBuffer.resize((ReadStructure.PathSize) + 1);
				PathBufferPtr = &PathBuffer[0];
			}
			PathBuffer[ReadStructure.PathSize] = '\0';
			rFile.read(PathBufferPtr, ReadStructure.PathSize);
			tempData.RealPath = std::string(PathBufferPtr);
			if (FiledataBuffer.empty() || (int)FiledataBuffer.size() - 1 < ReadStructure.FileSize)
			{
				FiledataBuffer.resize((ReadStructure.FileSize) + 1);
				FileDataBufferPtr = &FiledataBuffer[0];
			}
			FiledataBuffer[ReadStructure.FileSize] = '\0';
			rFile.read(FileDataBufferPtr, ReadStructure.FileSize);
			tempData.DstPath = std::string(FileDataBufferPtr);
			LinkFileQueue.push(tempData);
		}
		else
		{
			if (PathBuffer.empty() || (int)PathBuffer.size() - 1 < ReadStructure.PathSize)
			{
				PathBuffer.resize((ReadStructure.PathSize) + 1);
				PathBufferPtr = &PathBuffer[0];
			}
			PathBuffer[ReadStructure.PathSize] = '\0';
			rFile.read(PathBufferPtr, ReadStructure.PathSize);
			std::string NewNodePath(PathBufferPtr);
			if(NewNodePath.empty())
				continue;
			if (!m_FileMap.empty() && m_FileMap.count(NewNodePath))
			{
				continue;
			}
			if (m_FileMap.empty() || !m_FileMap.count(GetParentPath(NewNodePath)))
				return ERROR_DATA_DISK_UNKNOW;
			FileInfo* ParentNode = m_FileMap[GetParentPath(NewNodePath)];
			if (NULL == ParentNode)
				return ERROR_DATA_DISK_UNKNOW;
			FileInfo* NewFile = new FileInfo(NewNodePath, ParentNode, (FileType)ReadStructure.FileType);
			ReadStructure.CreateTime[24] = '\0';
			ReadStructure.ModifyTime[24] = '\0';
			NewFile->SetModifyDate(ReadStructure.ModifyTime);
			NewFile->SetCreateDate(ReadStructure.CreateTime);
			if (DATA_FILE == NewFile->GetRealType())
			{
				if (FiledataBuffer.empty() || (int)FiledataBuffer.size() - 1 < ReadStructure.FileSize)
				{
					FiledataBuffer.resize((ReadStructure.FileSize) + 1);
					FileDataBufferPtr = &FiledataBuffer[0];
				}

				FiledataBuffer[ReadStructure.FileSize] = '\0';
				rFile.read(FileDataBufferPtr, ReadStructure.FileSize);
				NewFile->SetFileData(FileDataBufferPtr, ReadStructure.FileSize);
			}
			ParentNode->AddChild(NewFile);
			std::pair<std::string, FileInfo*> Newpair(NewNodePath, NewFile);
			m_FileMap.insert(Newpair);
		}
	}
	while (!LinkFileQueue.empty())
	{
		LinkFileData& tempData = LinkFileQueue.front();
		if (!m_FileMap.empty() && m_FileMap.count(tempData.RealPath))
		{
			LinkFileQueue.pop();
			continue;
		}
		FileInfo* parent = GetFileInfo(GetParentPath(tempData.RealPath));
		if (NULL == parent)
		{
			LinkFileQueue.pop();
			continue;
		}
		FileInfo* DstNode = GetFileInfo(tempData.DstPath);
		if (NULL == DstNode)
		{
			LinkFileQueue.pop();
			continue;
		}
		FileInfo* NewNode =new LinkFileInfo(tempData.RealPath, parent, DstNode);
		parent->AddChild(NewNode);
		NewNode->SetCreateDate(tempData.CreateDate);
		NewNode->SetModifyDate(tempData.ModifyDate);
		std::pair<std::string, FileInfo*> Newpair(tempData.RealPath, NewNode);
		m_FileMap.insert(Newpair);
		m_LinkFileManager->AddLink(DstNode, NewNode);
		LinkFileQueue.pop();
	}
	rFile.close();
	return SUCCESS_CODE;
}

ErrorCode DataDisk::SaveData(const std::string & Path)
{
	std::queue<FileInfo*>  SaveQueue;
	if (!m_RootNode->GetChildrenSize())
		return ERROR_DATA_DISK_NULL;
	for (int index = 0; index < m_RootNode->GetChildrenSize(); ++index)
	{
		SaveQueue.push(m_RootNode->GetChildrenList()[index]);
	}
	std::fstream rFile(Path, std::ios::binary | std::ios::out | std::ios::trunc);
	if (!rFile.is_open())
		return ERROR_SRC_PATH_CODE;
	while (!SaveQueue.empty())
	{
		FileInfo* TempFileInfo = SaveQueue.front();
		if (NULL == TempFileInfo)
		{
			SaveQueue.pop();
			continue;
		}
		FileInOutStructure Write;
		if (LINK_FILE == TempFileInfo->GetRealType())
		{
			Write.FileType = TempFileInfo->GetRealType();
			Write.PathSize = TempFileInfo->GetRealPath().size();
			Write.FileSize = TempFileInfo->GetFilePath().size();
			WriteDateInfo(TempFileInfo->GetCreateDate(), TempFileInfo->GetModifyDate(), Write);
			rFile.write((char*)&Write, sizeof(Write));
			rFile.write(TempFileInfo->GetRealPath().c_str(), Write.PathSize);
			rFile.write(TempFileInfo->GetFilePath().c_str(), Write.FileSize);
		}
		else
		{
			Write.FileType = TempFileInfo->GetRealType();
			Write.PathSize = TempFileInfo->GetRealPath().size();
			Write.FileSize = TempFileInfo->GetFileSize();
			WriteDateInfo(TempFileInfo->GetCreateDate(), TempFileInfo->GetModifyDate(), Write);
			rFile.write((char*)&Write, sizeof(Write)); 
			rFile.write(TempFileInfo->GetRealPath().c_str(), Write.PathSize);
			rFile.write(TempFileInfo->GetFileData(), Write.FileSize);
		}
		SaveQueue.pop();
		if(FOLDER_FILE != TempFileInfo->GetRealType())
			continue;
		for (int index = 0; index < TempFileInfo->GetChildrenSize(); ++index)
		{
			SaveQueue.push(TempFileInfo->GetChildrenList()[index]);
		}
	}
	rFile.close();
	return SUCCESS_CODE;
}

std::string DataDisk::GetParentPath(std::string & PathStr)
{
	int ParentNameEnd = PathStr.find_last_of("/");
	if (std::string::npos == ParentNameEnd)
		return std::string("");
	std::string DstParent = PathStr.substr(0,ParentNameEnd);
	return DstParent;
}

bool DataDisk::CheckRealDiskPathIsFolder(std::string & RealPath)
{
	if (RealPath.empty())
		return false;
	struct _stat buf;
	int result;
	result = _stat(RealPath.c_str(), &buf);
	return (_S_IFDIR & buf.st_mode);
}

bool DataDisk::FormatDisk()
{
	m_LinkFileManager->Clear();
	if (m_FileMap.size() > 1)
	{
		std::vector<FileInfo*>& Children = m_RootNode->GetChildrenList();
		if (!Children.empty())
			Children.clear();
		std::map<std::string, FileInfo*>::iterator iter = m_FileMap.begin();
		for (;iter!=m_FileMap.end();)
		{
			FileInfo*& TempNode = iter->second;
			if (m_RootNode == TempNode)
				continue;
			if (NULL == TempNode)
			{
				++iter;
				continue;
			}
			else
			{
				delete TempNode;
				TempNode = NULL;
			}
		}
		m_FileMap.clear();
	
		return true;
	}
	return false;
}

void DataDisk::WriteDateInfo(const std::string& CreateDate, const std::string& ModifyDate, FileInOutStructure& WriteStructure)
{
	for (int index = 0; index <(int) CreateDate.size();++index)
	{
		WriteStructure.CreateTime[index] = CreateDate[index];
		WriteStructure.ModifyTime[index] = ModifyDate[index];
	}
}
