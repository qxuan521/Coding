#include <stack>
#include <queue>
#include "del_command.h"
#include "data_disk.h"
#include "file_info.h"

DelCommand::DelCommand(std::vector<std::string>& ArgList)
	: Command(ArgList, 2)
{
	m_TypeArg.insert("/s");
}

DelCommand::~DelCommand()
{
}

ErrorCode DelCommand::Run()
{
	ErrorCode AbsResult = ToAbsolutePath();
	if (SUCCESS_CODE != AbsResult)
		return AbsResult;
	bool IsAllChildren = false;
	int PathStartIndex = 1;
	std::string SrcStr, DstStr;
	if ("/s" == m_ArgList[1])
	{
		IsAllChildren = true;
		if (m_ArgList.size() < 3)
			return ERROR_ARG_COUNT_CODE;
		PathStartIndex = 2;
	}
	if (!IsAllChildren)
	{
		for (int index = PathStartIndex; index < (int)m_ArgList.size(); ++index)
		{
			if ('@' == m_ArgList[index][0])
				return ERROR_SRC_PATH_CODE;

			ErrorCode Result = DelOneLevel(m_ArgList[index]);
			if (SUCCESS_CODE != Result)
				return Result;
		}
		
	}
	else
	{
		for (int index = PathStartIndex; index < (int)m_ArgList.size(); ++index)
		{
			if ('@' == m_ArgList[index][0])
				return ERROR_SRC_PATH_CODE;
			FileInfo* FileNode = g_DataDiskPtr->GetFileInfo(m_ArgList[index]);
			if (FileNode != NULL && FOLDER_FILE == FileNode->GetType())
			{
				m_ArgList[index] = m_ArgList[index] + "/" + "*";
			}
			ErrorCode Result = DelAllChild(m_ArgList[index]);
			if (SUCCESS_CODE != Result)
				return Result;
		}
	}
	return SUCCESS_CODE;
}

ErrorCode DelCommand::DelAllChild(std::string Str)
{
	std::set<std::string> DelList;
	std::stack<FileInfo*> WaitDelStack;
	std::queue<FileInfo*> ParentQueue;
	if ('*' == Str[Str.size() - 1])
	{
		FileInfo* DelParent = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(Str));
		if (NULL == DelParent || FOLDER_FILE != DelParent->GetRealType())
			return ERROR_SRC_PATH_CODE;
		std::vector<FileInfo*> Children = DelParent->GetChildrenList();
		for (int index = 0; index < (int)Children.size(); ++index)
		{
			if(NULL == Children[index])
				continue;
			ParentQueue.push(Children[index]);
		}

	}
	else
	{
		FileInfo* DelNode = g_DataDiskPtr->GetFileInfo(Str);
		if (NULL == DelNode || FOLDER_FILE == DelNode->GetRealType())
			return ERROR_SRC_PATH_CODE; 
		ParentQueue.push(DelNode);
	}
	while (!ParentQueue.empty())
	{
		FileInfo* tempFile = ParentQueue.front();
		if (NULL == tempFile)
		{
			ParentQueue.pop();
			continue;
		}
		if (!DelList.empty() && DelList.count(tempFile->GetRealPath()))
		{
			ParentQueue.pop();
			continue;
		}
		WaitDelStack.push(tempFile);
		DelList.insert(tempFile->GetRealPath());
		if (tempFile->GetChildrenSize())
		{
			std::vector<FileInfo*> ChildList = tempFile->GetChildrenList();
			for (int index = 0; index < (int) ChildList.size();++index)
			{
				if(NULL == ChildList[index])
					continue;
				ParentQueue.push(ChildList[index]);
			}
		}
		ParentQueue.pop();
	}
	while (!WaitDelStack.empty())
	{
		FileInfo* DelNode = WaitDelStack.top();
		if (NULL == DelNode)
		{
			WaitDelStack.pop();
			continue;
		}
		g_DataDiskPtr->DelFile(DelNode);
		WaitDelStack.pop();
	}
	return SUCCESS_CODE;
}

ErrorCode DelCommand::DelOneLevel(std::string Str)
{
	
	std::vector<FileInfo*> Children;
	if ( '*' == Str[Str.size() - 1])
	{
		FileInfo* DelParent = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(Str));
		if (NULL == DelParent || FOLDER_FILE != DelParent->GetRealType())
			return ERROR_SRC_PATH_CODE;
		Children = DelParent->GetChildrenList();
	}
	else
	{
		FileInfo* DelNode = g_DataDiskPtr->GetFileInfo(Str);
		if (NULL == DelNode || FOLDER_FILE == DelNode->GetRealType())
			return ERROR_SRC_PATH_CODE; 
		if (FOLDER_FILE == DelNode->GetRealType())
			return ERROR_SRC_PATH_CODE;
		Children.push_back(DelNode);

	}
	for (int index = 0; index <(int) Children.size(); ++index)
	{
		if (NULL == Children[index])
			continue;
		if (DATA_FILE == Children[index]->GetType())
			g_DataDiskPtr->DelFile(Children[index]);
	}
	return SUCCESS_CODE;
}

