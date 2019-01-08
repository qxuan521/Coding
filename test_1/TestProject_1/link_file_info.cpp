#include "link_file_info.h"

LinkFileInfo::LinkFileInfo(std::string & FilePathStr, FileInfo* ParentPtr, FileInfo* DstFilePtr)
	: FileInfo(FilePathStr, ParentPtr,LINK_FILE),
	m_DstFilePtr(DstFilePtr)
{

}

LinkFileInfo::~LinkFileInfo()
{
}

std::vector<FileInfo*>& LinkFileInfo::GetChildrenList()
{
	return m_DstFilePtr->GetChildrenList();
}

int LinkFileInfo::GetChildrenSize()
{
	return m_DstFilePtr->GetChildrenSize();
}

bool LinkFileInfo::CheckSameNameWithChildren(const std::string & FileNameStr)
{
	return m_DstFilePtr->CheckSameNameWithChildren(FileNameStr);
}

void LinkFileInfo::AddChild(FileInfo* ChildPtr)
{
	m_DstFilePtr->AddChild(ChildPtr);
}

void LinkFileInfo::DelChild(FileInfo* ChildPtr)
{
	m_DstFilePtr->DelChild(ChildPtr);
}

int LinkFileInfo::GetFileSize()
{
	return FileInfo::GetFileSize();
}

char * LinkFileInfo::GetFileData()
{
	return FileInfo::GetFileData();
}



std::string LinkFileInfo::GetFilePath()
{
	if (m_DstFilePtr == NULL)
		return std::string();
	return m_DstFilePtr->GetFilePath();
}

FileInfo* LinkFileInfo::GetParent()
{
	return m_DstFilePtr->GetParent();
}

FileType LinkFileInfo::GetType() const
{
	return m_DstFilePtr->GetType();
}

FileInfo* LinkFileInfo::GetDstFile()
{
	return m_DstFilePtr;
}

void LinkFileInfo::SetDstFile(FileInfo* DstFile)
{
	m_DstFilePtr = DstFile;
}
