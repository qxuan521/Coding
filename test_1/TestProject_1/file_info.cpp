#include "file_info.h"
#include "data_disk.h"
FileInfo::FileInfo(std::string FilePathStr, FileInfo* ParentPtr, FileType eFileType)
	: m_FileType(eFileType),
	m_FilePath(FilePathStr),
	m_ParentPtr(ParentPtr),
	m_FileData(NULL),
	m_FileSize(0)
{
	int lastIndex = m_FilePath.find_last_of('/');
	m_FileName = m_FilePath.substr(lastIndex + 1);
}

FileInfo::~FileInfo()
{
	if (m_FileData != NULL)
	{
		delete [] m_FileData;
		m_FileData = NULL;
	}
}

std::vector<FileInfo*>& FileInfo::GetChildrenList()
{
	return m_ChildrenList;
}

int FileInfo::GetChildrenSize()
{
	return m_ChildrenList.size();
}

bool FileInfo::CheckSameNameWithChildren(const std::string& FileNameStr)
{
	if (FileNameStr.empty() || FOLDER_FILE != m_FileType )
		return false;
	for (size_t index = 0; index < m_ChildrenList.size();++index)
	{
		if (FileNameStr == m_ChildrenList[index]->GetFilePath())
			return true;
	}
	return false;
}

void FileInfo::AddChild(FileInfo* ChildPtr)
{
	if (FOLDER_FILE != m_FileType|| NULL == ChildPtr)
		return;
	//查重
	for (std::vector<FileInfo*>::iterator iter = m_ChildrenList.begin(); iter != m_ChildrenList.end();)
	{
		if (ChildPtr == *(iter))/*ChildPtr != NULL 所有相等时右项不会等于NULL*/
		{
			return;
		}
		else
			++iter;
	}
	m_ChildrenList.push_back(ChildPtr);
}

void FileInfo::DelChild(FileInfo* ChildPtr)
{
	if (FOLDER_FILE != m_FileType|| NULL == ChildPtr ||m_ChildrenList.empty())
		return;
	for (std::vector<FileInfo*>::iterator iter = m_ChildrenList.begin();iter!=m_ChildrenList.end();)
	{
		if (ChildPtr == *(iter))/*ChildPtr != NULL 所有相等时右项不会等于NULL*/
		{
			iter = m_ChildrenList.erase(iter);
		}
		else
			++iter;
	}
}

long FileInfo::GetFileSize()
{
	return m_FileSize;
}

void FileInfo::SetParent(FileInfo* ParentPtr)
{
	m_ParentPtr = ParentPtr;
	UpdateFilePath();
}

FileInfo* FileInfo::GetParent()
{
	return m_ParentPtr;
}

FileInfo* FileInfo::GetRealParent()
{
	return m_ParentPtr;
}

const std::string & FileInfo::GetFileName()
{
	return m_FileName;
}

void FileInfo::SetFileName(std::string & FileNameStr)
{
	if (FileNameStr == m_FileName)
		return;
	m_FileName = FileNameStr;
	UpdateFilePath();
}

std::string   FileInfo::GetFilePath()
{
	return m_FilePath;
}

std::string & FileInfo::GetRealPath()
{
	return m_FilePath;
}

FileType FileInfo::GetRealType() const
{
	return m_FileType;
}

FileType FileInfo::GetType() const
{
	return m_FileType;
}

char * FileInfo::GetFileData()
{
	if (FOLDER_FILE == m_FileType)
		return NULL;
	return m_FileData;
}

void FileInfo::SetFileData(char * FileData, long DataSize)
{
	if (FOLDER_FILE == m_FileType || DataSize <= 0 || NULL == FileData)
		return;
	m_FileSize = DataSize;
	m_FileData = new char[DataSize];
	memcpy(m_FileData, FileData, DataSize);
}

const std::string & FileInfo::GetModifyDate()
{
	return m_CreateDate;
}

void FileInfo::SetModifyDate(const std::string & ModifyDateStr)
{
	if (ModifyDateStr.empty() || m_ModifyDate == ModifyDateStr )
		return;
	m_ModifyDate = ModifyDateStr;
}

const std::string & FileInfo::GetCreateDate() const
{
	return m_CreateDate;
}

void FileInfo::SetCreateDate(const std::string & CreateDateStr)
{
	if (CreateDateStr.empty() || CreateDateStr == m_CreateDate)
		return;
	m_CreateDate = CreateDateStr;
}

int FileInfo::GetSize()
{
	return sizeof(*this);
}

void FileInfo::SetFilePath(std::string & FilePath)
{
		m_FilePath = FilePath;
}

void FileInfo::UpdateFilePath()
{
	if (NULL == m_ParentPtr)
	{
		m_FilePath = m_FileName;
	}
	else
	{
		std::string OldPath = m_FilePath;
		m_FilePath = m_ParentPtr->GetFilePath();
		m_FilePath.append("/");
		m_FilePath.append(m_FileName);
	}
}
