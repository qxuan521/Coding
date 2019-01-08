#pragma once
#include <set>
#include<queue>
#include <iostream>
#include "model_def.h"
#include "data_disk.h"
#include "file_info.h"

extern "C" struct DirSearchResult;
typedef std::vector<DirSearchResult> ResultArr;

struct DirSearchResult
{
	FileInfo*				FilePtr = NULL;
	std::vector<FileInfo*>	CurLevelResult;
	int						FolderCount = 0;
	int						FileCount = 0;
};
struct DirSeacher
{
	virtual ErrorCode DoSearch(std::vector<std::string>& PathArr) = 0;

	ResultArr	SearchResult;
}; 

struct NormalDirSearcher : public DirSeacher
{
	virtual ErrorCode DoSearch(std::vector<std::string>& PathArr)
	{
		SearchResult.clear();
		if (PathArr.empty())
			return ERROR_SRC_PATH_CODE;
		for (int index = 0; index <(int)PathArr.size(); ++index)
		{
			FileInfo* CurFileNode = g_DataDiskPtr->GetFileInfo(PathArr[index]);
			if (NULL == CurFileNode)
			{//lujingbucunzai
				continue;
			}
			if(!CurFileNode->GetChildrenSize())
				continue;
			DirSearchResult Result;
			Result.FilePtr = CurFileNode;
			Result.CurLevelResult = CurFileNode->GetChildrenList();
			for (int index = 0;index < (int)Result.CurLevelResult.size();++index )
			{
				if (FOLDER_FILE == Result.CurLevelResult[index]->GetRealType())
					++Result.FolderCount;
				else
					++Result.FileCount;
			}
			SearchResult.push_back(Result);
		}
		return SUCCESS_CODE;
	}
};

struct AllChildDirSearcher : public DirSeacher
{

	virtual ErrorCode DoSearch(std::vector<std::string>& PathArr)
	{
		SearchResult.clear();
		std::set<FileInfo*> FinishSearch;
		std::queue<FileInfo*>	SearchQueue;
		for (int index = 0; index < (int)PathArr.size(); ++index)
		{
			FileInfo* CurNode = g_DataDiskPtr->GetFileInfo(PathArr[index]);
			if (NULL == CurNode || FOLDER_FILE != CurNode->GetType())
			{
				if (NULL != CurNode)
				{
					std::cout<<""<< std::endl;
					std::cout<<CurNode->GetRealPath() <<" File was error."<<std::endl;;
					std::cout<< ""<< std::endl;
				}
				continue;
			}
			SearchQueue.push(CurNode);
		}
		while (!SearchQueue.empty())
		{
			FileInfo* SearchNode = SearchQueue.front();
			if (!FinishSearch.empty() && FinishSearch.count(SearchNode))
			{
				SearchQueue.pop();
				continue;
			}
			std::vector<FileInfo*>& Children = SearchNode->GetChildrenList();
			if (Children.empty())
			{
				SearchQueue.pop();
				continue;
			}
			//¼ÇÂ¼²éÑ¯¹ý
			FinishSearch.insert(SearchNode);
			DirSearchResult Result;
			Result.FilePtr = SearchNode;
			for (int index = 0; index < (int)Children.size(); ++index)
			{
				if(NULL == Children[index])
					continue;
				if (FOLDER_FILE == Children[index]->GetType())
					SearchQueue.push(Children[index]);
				Result.CurLevelResult.push_back(Children[index]);
				if (FOLDER_FILE == Children[index]->GetRealType())
					++Result.FolderCount;
				else
					++Result.FileCount;
			}
			SearchResult.push_back(Result);
			SearchQueue.pop();
		}
		
		return SUCCESS_CODE;
	}
};

struct FolderDirSearcher : public DirSeacher
{
	virtual ErrorCode DoSearch(std::vector<std::string>& PathArr)
	{
		SearchResult.clear();
		if (PathArr.empty())
			return ERROR_SRC_PATH_CODE;
		for (int index = 0; index < (int)PathArr.size(); ++index)
		{
			FileInfo* CurFileNode = g_DataDiskPtr->GetFileInfo(PathArr[index]);
			if (NULL == CurFileNode)
			{//lujingbucunzai
				continue;
			}
			if (!CurFileNode->GetChildrenSize())
				continue;
			std::vector<FileInfo*>& Children = CurFileNode->GetChildrenList();
			DirSearchResult Result;
			Result.FilePtr = CurFileNode;
			for (int index = 0; index < (int)Children.size();++index)
			{
				if (FOLDER_FILE == Children[index]->GetRealType())
				{
					Result.CurLevelResult.push_back(Children[index]);
					++Result.FolderCount;
				}
			}
			SearchResult.push_back(Result);
		}
		return SUCCESS_CODE;
	}
};

struct AllFolderDirSearcher : public DirSeacher
{
	virtual ErrorCode DoSearch(std::vector<std::string>& PathArr)
	{
		SearchResult.clear();
		std::set<FileInfo*> Waiting4Search;
		std::queue<FileInfo*>	SearchQueue;
		for (int index = 0; index < (int)PathArr.size(); ++index)
		{
			FileInfo* CurNode = g_DataDiskPtr->GetFileInfo(PathArr[index]);
			if (NULL == CurNode || FOLDER_FILE != CurNode->GetRealType())
			{
				//chuli
				continue;
			}
			SearchQueue.push(CurNode);
		}
		while (!SearchQueue.empty())
		{
			FileInfo* SearchNode = SearchQueue.front();
			if (!Waiting4Search.empty() && Waiting4Search.count(SearchNode))
			{
				SearchQueue.pop();
				continue;
			}
			std::vector<FileInfo*>& Children = SearchNode->GetChildrenList();
			if (Children.empty())
			{
				SearchQueue.pop();
				continue;
			}
			Waiting4Search.insert(SearchNode);
			for (int index = 0; index < (int)Children.size(); ++index)
			{
				if (NULL != Children[index] && FOLDER_FILE == Children[index]->GetType())
					SearchQueue.push(Children[index]);
			}
			SearchQueue.pop();
		}
		if (Waiting4Search.empty())
			return ERROR_SRC_PATH_CODE;
		std::set<FileInfo*>::iterator itor = Waiting4Search.begin();
		int index = 0;
		for (; itor != Waiting4Search.end(); ++itor)
		{
			DirSearchResult Result;
			Result.FilePtr = *itor;
			std::vector<FileInfo*> Children = (*itor)->GetChildrenList();
			for (index = 0; index < (int)Children.size(); ++index)
			{
				if (FOLDER_FILE == Children[index]->GetRealType())
				{
					Result.CurLevelResult.push_back(Children[index]);
					++Result.FolderCount;
				}
			}
			if (!Result.CurLevelResult.empty())
				SearchResult.push_back(Result);
		}
		return SUCCESS_CODE;
	}
};
