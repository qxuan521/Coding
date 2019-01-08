#include "link_file_manager.h"
#include "data_disk.h"
LinkFileManager::LinkFileManager()
{

}

LinkFileManager::~LinkFileManager()
{

}

void LinkFileManager::AddLink(FileInfo* SrcFile, FileInfo* LinkFile)
{
	if (NULL == SrcFile || NULL == LinkFile)
		return;
	if (m_ManagerMap.empty() || !m_ManagerMap.count(SrcFile))
	{
		std::vector<FileInfo*> LinkArr;
		LinkArr.push_back(LinkFile);
		std::pair<FileInfo*, std::vector<FileInfo*>> NewLink(SrcFile, LinkArr);
		m_ManagerMap.insert(NewLink);
	}
	else
	{
		std::vector<FileInfo*>& LinkArr = m_ManagerMap[SrcFile];
		for (int index = 0; index <(int)LinkArr.size();++index)
		{
			if (LinkFile == LinkArr[index])
				return;
		}
		LinkArr.push_back(LinkFile);
	}
}

void LinkFileManager::DelLinkFile(FileInfo* LinkFile)
{
	FileInfo* DstFile = g_DataDiskPtr->GetFileInfo(LinkFile->GetFilePath());
	if (NULL == DstFile)
		return;
	if (m_ManagerMap.empty())
		return;
	Src2LinkMap::iterator iter = m_ManagerMap.find(DstFile);
	if (iter != m_ManagerMap.end())
	{
		std::vector<FileInfo*>& List = iter->second;
		std::vector<FileInfo*>::iterator iter = List.begin();
		for (;iter != List.end();++iter)
		{
			if (LinkFile == *iter)
			{
				List.erase(iter);
				return;
			}
		}
	}
}

void LinkFileManager::DelDstFile(FileInfo* SrcFile)
{
	if (m_ManagerMap.empty())
		return;
	Src2LinkMap::iterator iter = m_ManagerMap.find(SrcFile);
	if (iter == m_ManagerMap.end())
		return;
	std::vector<FileInfo*> LinkList = iter->second;
	m_ManagerMap.erase(iter);

	for (int index = 0; index < (int) LinkList.size();++index)
	{
		g_DataDiskPtr->DelFile(LinkList[index]);
	}
	
}


void LinkFileManager::ChangeDstFile(FileInfo* OldFile, FileInfo* NewFile)
{
	if (m_ManagerMap.empty() || !m_ManagerMap.count(OldFile))
		return;
	std::vector<FileInfo*> LinkFileArr = m_ManagerMap[OldFile];
	m_ManagerMap.erase(m_ManagerMap.find(OldFile));
	std::pair <FileInfo*, std::vector<FileInfo*>> NewPair(NewFile,LinkFileArr);
	m_ManagerMap.insert(NewPair);
	for (int index = 0 ;index < (int)LinkFileArr.size();++index)
	{
		if(NULL == LinkFileArr[index])
			continue;
		static_cast<LinkFileInfo*>(LinkFileArr[index])->SetDstFile(NewFile);
	}
}

void LinkFileManager::Clear()
{
	m_ManagerMap.clear();
}
