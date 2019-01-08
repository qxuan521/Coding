#include <iostream>
#include <iomanip>
#include <xiosbase>
#include "dir_command.h"
Dircomman::Dircomman(std::vector<std::string>& ArgList)
	: Command(ArgList)
	, m_Seacher(NULL)
{
	m_FileTypeStd.push_back("     ");
	m_FileTypeStd.push_back("<DIR>");
	m_FileTypeStd.push_back("<LNK>");
	m_TypeArg.insert("/ad");
	m_TypeArg.insert("/s");
}

Dircomman::~Dircomman()
{
	if (NULL != m_Seacher)
	{
		delete m_Seacher;
		m_Seacher = NULL;
	}
}

ErrorCode Dircomman::Run()
{
	m_PathList.clear();
	ToAbsolutePath();
	ResolveArg();
	if (NULL == m_Seacher)
		return ERROR_COMMAND_USE_CODE;
	m_Seacher->DoSearch(m_PathList);
	PrintResult();
	return SUCCESS_CODE;
}

ErrorCode Dircomman::ResolveArg()
{
	if (1 == m_ArgList.size())
	{
		m_PathList.push_back(m_CurWorkPath);
		m_Seacher = new NormalDirSearcher();
		return SUCCESS_CODE;
	}
	bool IsAllChild = false;
	bool IsFolderSearch = false;
	int PathStartIndex = 1;
	for (int Index = 1; Index < (int)m_ArgList.size() && Index < 3;++ Index)
	{
		if("/s"!= m_ArgList[Index] && "/ad" != m_ArgList[Index])
			break;
		if ("/s" == m_ArgList[Index])
			IsAllChild = true;
		if ("/ad" == m_ArgList[Index])
			IsFolderSearch = true;
		++PathStartIndex;
	}
	if ((int)m_ArgList.size() < PathStartIndex + 1)
	{
		m_PathList.push_back(m_CurWorkPath);
	}
	else
	{
		for (int Index = PathStartIndex;Index < (int)m_ArgList.size();++Index)
		{
			int WildcardIndex = (int)m_ArgList[Index].find_last_of('*');
			if (std::string::npos != WildcardIndex)
				return ERROR_SRC_PATH_CODE;
			FileInfo* CurNode = g_DataDiskPtr->GetFileInfo(m_ArgList[Index]);
			if (NULL == CurNode)
				return ERROR_SRC_PATH_CODE;
			m_PathList.push_back(CurNode->GetFilePath());
		}
	}
	if (IsAllChild && IsFolderSearch)
	{
		m_Seacher = new AllFolderDirSearcher;
	}
	else if (!IsAllChild && IsFolderSearch)
	{
		m_Seacher = new FolderDirSearcher;
	}
	else if (IsAllChild && !IsFolderSearch)
	{
		m_Seacher = new AllChildDirSearcher;
	}
	else
	{
		m_Seacher = new NormalDirSearcher;
	}
	return SUCCESS_CODE;
}


void Dircomman::PrintResult()
{
	if (m_Seacher->SearchResult.empty())
		return;
	for (int index = 0; index < (int)m_Seacher->SearchResult.size(); ++index)
	{
		DirSearchResult& SingleResult = m_Seacher->SearchResult[index];
		std::cout<<"Directories of" << " \"" << SingleResult.FilePtr->GetRealPath() << "\":" << std::endl;
		std::cout<<"" << std::endl;
		for (int LoopCount = 0; LoopCount < (int)SingleResult.CurLevelResult.size(); ++LoopCount)
		{
			FileType Type = SingleResult.CurLevelResult[LoopCount]->GetRealType();
			int FileSize = SingleResult.CurLevelResult[LoopCount]->GetFileSize();
			std::string DateStr = SingleResult.CurLevelResult[LoopCount]->GetCreateDate();
 			int DateEnterIndex = (int)DateStr.find_first_of('\n');
			if (std::string::npos != DateEnterIndex)
				DateStr[DateEnterIndex] = '\0';
			for (int index = 0; DateStr.size() < 24; ++index)
			{
				DateStr += '\0';
			}
			std::cout << "  ";
			std::cout << std::setw(25) << DateStr.c_str();
			std::cout << "    ";
			std::cout << std::setw(6) << m_FileTypeStd[Type];
 			if (DATA_FILE == Type)
 			{
				std::cout << std::setw(9) << std::setiosflags(std::ios::right) << FileSize;
 			}
 			else
 			{
				std::cout << std::setw(9) << std::setiosflags(std::ios::right) <<" ";
 			}
			std::string Name = SingleResult.CurLevelResult[LoopCount]->GetFileName();
		
			if (LINK_FILE == Type)
			{
				std::cout << " " << SingleResult.CurLevelResult[LoopCount]->GetFileName() << "[" << SingleResult.CurLevelResult[LoopCount]->GetFilePath() << "]" << std::endl;
			}
			else
			{
				std::cout << " " << SingleResult.CurLevelResult[LoopCount]->GetFileName() << std::endl;
			}
		}
		std::cout << "" << std::endl;
		std::cout << SingleResult.FileCount << " files,  " << SingleResult.FolderCount << " Folders."<<std::endl;
	}
}


