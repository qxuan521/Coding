#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include "move_command.h"
#include "data_disk.h"
#include "file_info.h"
MoveCommand::MoveCommand(std::vector<std::string>& ArgList)
	: Command(ArgList, 2)
{
	m_TypeArg.insert("/y");
}

MoveCommand::~MoveCommand()
{

}

ErrorCode MoveCommand::Run()
{
	ErrorCode AbsError = ToAbsolutePath();

	if (SUCCESS_CODE != AbsError)
		return AbsError;
	if (m_ArgList.size() < 2)
		return ERROR_ARG_COUNT_CODE;
	if (m_ArgList.size() == 2)
		m_ArgList.push_back(m_CurWorkPath);
	std::string SrcStr, DstStr;
	bool IsCoverSameFile = false;
	if ("/y" == m_ArgList[1])
	{
		IsCoverSameFile = true;
		if (m_ArgList.size() < 4)
			return ERROR_ARG_COUNT_CODE;
		SrcStr = m_ArgList[2];
		DstStr = m_ArgList[3];
	}
	else
	{
		SrcStr = m_ArgList[1];
		DstStr = m_ArgList[2];
	}
	if (SrcStr.empty() || '@' == SrcStr[0])
		return ERROR_SRC_PATH_CODE;
	if (DstStr.empty() || '@' == DstStr[0])
		return ERROR_DST_PATH_CODE;
	std::string DstParentPath, NewPath;
	int SrcWildCartIndex = SrcStr.find_first_of('*');
	FileInfo* SrcOldNodex = g_DataDiskPtr->GetFileInfo(SrcStr);
	if (std::string::npos != SrcWildCartIndex )
	{
		if (SrcStr.size() - 1 != SrcWildCartIndex || '/' != SrcStr[SrcStr.size() - 2])
			return ERROR_SRC_PATH_CODE; 
		MoveSrcWildCard(SrcStr, DstStr, IsCoverSameFile);
	}
	else if(NULL != SrcOldNodex && FOLDER_FILE == SrcOldNodex->GetRealType())
	{
		FileInfo* SrcNode = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(SrcStr));
		if (NULL == SrcNode)
			return ERROR_SRC_PATH_CODE;
		MoveSrcFolder(SrcStr, DstStr, IsCoverSameFile);
	}else if (NULL != SrcOldNodex && FOLDER_FILE != SrcOldNodex->GetRealType())
	{
		MoveSrcFile(SrcStr, DstStr, IsCoverSameFile);
	}
	else
	{
		return ERROR_SRC_PATH_CODE;
	}
	for (int index = 0; index < (int)m_DstPathList.size();++index)
	{
		if (g_DataDiskPtr->IsPathExsit(m_DstPathList[index]))
		{
			FileInfo* File = g_DataDiskPtr->GetFileInfo(m_DstPathList[index]);
			FileInfo* SrcFile = g_DataDiskPtr->GetFileInfo(m_SrcPathList[index]);
			if (File->GetRealType() != SrcFile->GetRealType())
				return ERROR_PATH_N_TYPE;
			if (File->GetRealPath() == SrcFile->GetRealPath())
				return ERROR_DST_PATH_CODE;
		}
	}
	for (int index = 0; index < (int)m_DstPathList.size() && index<(int)m_SrcPathList.size(); ++index)
	{
		g_DataDiskPtr->ChangeParent(m_SrcPathList[index], m_DstPathList[index]);
	}
	for (int index = m_SrcPathList.size() - 1 ; index >=0;--index)
	{
		FileInfo* srcNode = g_DataDiskPtr->GetFileInfo(m_SrcPathList[index]);
		if (NULL == srcNode ||FOLDER_FILE != srcNode->GetRealType())
			continue;
		if (0 == srcNode->GetChildrenSize())
			g_DataDiskPtr->DelFile(srcNode);
	}
	return SUCCESS_CODE;
}

ErrorCode MoveCommand::MoveSrcFolder(std::string& SrcStr, std::string& DstStr, bool& IsCoverSameFile)
{
	int DstWildcardIndex = DstStr.find_first_of('*');
	FileInfo* SrcNode = g_DataDiskPtr->GetFileInfo(SrcStr);
	if (NULL == SrcNode)
		return ERROR_SRC_PATH_CODE;
	FileInfo* DstNode = NULL;
	std::queue<MoveDstChild> WaitMoveQueue;
	if (std::string::npos != DstWildcardIndex)
	{
		DstNode = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(DstStr));
		if (DstStr.size() - 1 != DstWildcardIndex || '/' != DstStr[DstStr.size() - 2])
			return ERROR_DST_PATH_CODE;
		if (NULL == DstNode || FOLDER_FILE != DstNode->GetType())
			return ERROR_DST_PATH_CODE;
		std::string NamePath = DstNode->GetRealPath();
		NamePath.append("/");
		NamePath.append(g_DataDiskPtr->GetName(SrcStr));
		if (!SameCheck(SrcStr, NamePath, IsCoverSameFile))
		{
			return ERROR_USER_STOP_CODE;
		}
		m_SrcPathList.push_back(SrcStr);
		m_DstPathList.push_back(NamePath);
		MoveDstChild ChildrenNode;
		ChildrenNode.DstParent = NamePath;
		ChildrenNode.SrcChildList = SrcNode->GetChildrenList();
		WaitMoveQueue.push(ChildrenNode);
	}
	else
	{
		DstNode = g_DataDiskPtr->GetFileInfo(DstStr);
		std::string NamePath; 
		if (NULL == DstNode)
		{
			DstNode = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(DstStr));
			if (NULL == DstNode || FOLDER_FILE != DstNode->GetType())
				return ERROR_DST_PATH_CODE;
			NamePath = DstNode->GetRealPath();
			NamePath.append("/");
			NamePath.append(g_DataDiskPtr->GetName(DstStr));
		}
		else
		{
			if (NULL != DstNode && FOLDER_FILE != DstNode->GetType())
				return ERROR_DST_PATH_CODE;
			NamePath = DstNode->GetRealPath();
			NamePath.append("/");
			NamePath.append(g_DataDiskPtr->GetName(SrcStr));
		}
		if (!SameCheck(SrcStr, NamePath, IsCoverSameFile))
		{
			return ERROR_USER_STOP_CODE;
		}
		m_SrcPathList.push_back(SrcStr);
		m_DstPathList.push_back(NamePath);
		MoveDstChild ChildrenNode;
		ChildrenNode.DstParent = NamePath;
		ChildrenNode.SrcChildList = SrcNode->GetChildrenList(); 
		WaitMoveQueue.push(ChildrenNode);
	}
	while (!WaitMoveQueue.empty())
	{
		MoveDstChild& tempNode = WaitMoveQueue.front();
		if (tempNode.SrcChildList.empty())
		{
			WaitMoveQueue.pop();
			continue;
		}
		for (int index = 0; index < (int)tempNode.SrcChildList.size();++index)
		{
			std::string NewPath = tempNode.DstParent;
			NewPath.append("/");
			NewPath.append(tempNode.SrcChildList[index]->GetFileName());
			if (!SameCheck(tempNode.SrcChildList[index]->GetRealPath(), NewPath, IsCoverSameFile))
			{
				continue;
			}
			m_SrcPathList.push_back(tempNode.SrcChildList[index]->GetRealPath());
			m_DstPathList.push_back(NewPath);
			if (FOLDER_FILE == tempNode.SrcChildList[index]->GetRealType())
			{
				MoveDstChild ChildrenNode;
				ChildrenNode.DstParent = NewPath;
				ChildrenNode.SrcChildList = tempNode.SrcChildList[index]->GetChildrenList();
				WaitMoveQueue.push(ChildrenNode);
			}
		}
		WaitMoveQueue.pop();
	}
	return SUCCESS_CODE;
}

ErrorCode MoveCommand::MoveSrcWildCard(std::string& SrcStr, std::string& DstStr, bool& IsCoverSameFile)
{
	//find wildcard
	int DstWildcardIndex = DstStr.find_first_of('*');
	FileInfo* SrcNode = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(SrcStr));
	if (NULL == SrcNode)
		return ERROR_SRC_PATH_CODE;
	FileInfo* DstNode = NULL;
	std::queue<MoveDstChild> WaitMoveQueue;

	if (std::string::npos != DstWildcardIndex)
	{
		//exist
		//verify 
		DstNode = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(DstStr));
		if (DstStr.size() - 1 != DstWildcardIndex || '/' != DstStr[DstStr.size() - 2])
			return ERROR_DST_PATH_CODE;
		if (NULL == DstNode || FOLDER_FILE != DstNode->GetType())
			return ERROR_DST_PATH_CODE;
		//find
		for (int index = 0; index < (int)SrcNode->GetChildrenSize(); ++index)
		{
			std::string SrcPath = SrcNode->GetChildrenList()[index]->GetRealPath();
			std::string NamePath = DstNode->GetRealPath();
			NamePath.append("/");
			NamePath.append(g_DataDiskPtr->GetName(SrcPath));
			if (!SameCheck(SrcPath, NamePath, IsCoverSameFile))
			{
				continue;
			}
			m_SrcPathList.push_back(SrcPath);
			m_DstPathList.push_back(NamePath);
			FileInfo* SrcChildFile = g_DataDiskPtr->GetFileInfo(SrcPath);
			if(NULL == SrcChildFile)
				continue;
			if (FOLDER_FILE == SrcChildFile->GetRealType())
			{
				MoveDstChild ChildrenNode;
				ChildrenNode.DstParent = NamePath;
				ChildrenNode.SrcChildList = SrcChildFile->GetChildrenList();
				WaitMoveQueue.push(ChildrenNode);
			}
		}
	}
	else
	{
		DstNode = g_DataDiskPtr->GetFileInfo(DstStr);
		if (NULL == DstNode)
		{
			return ERROR_DST_PATH_CODE;
		}
		else
		{
			if (NULL != DstNode && FOLDER_FILE != DstNode->GetType())
				return ERROR_DST_PATH_CODE;
		}
		for (int index = 0; index < (int)SrcNode->GetChildrenSize(); ++index)
		{
			std::string SrcPath = SrcNode->GetChildrenList()[index]->GetRealPath();
			std::string NamePath = DstNode->GetRealPath();
			NamePath.append("/");
			NamePath.append(g_DataDiskPtr->GetName(SrcPath));
			if (!SameCheck(SrcPath, NamePath, IsCoverSameFile))
			{
				continue;
			}
			m_SrcPathList.push_back(SrcPath);
			m_DstPathList.push_back(NamePath);
			FileInfo* SrcChildFile = g_DataDiskPtr->GetFileInfo(SrcPath);
			if (NULL == SrcChildFile)
				continue;
			if (FOLDER_FILE == SrcChildFile->GetRealType())
			{
				MoveDstChild ChildrenNode;
				ChildrenNode.DstParent = NamePath;
				ChildrenNode.SrcChildList = SrcChildFile->GetChildrenList();
				WaitMoveQueue.push(ChildrenNode);
			}
		}
	}
	//find children
	while (!WaitMoveQueue.empty())
	{
		MoveDstChild& tempNode = WaitMoveQueue.front();
		if (tempNode.SrcChildList.empty())
		{
			WaitMoveQueue.pop();
			continue;
		}
		for (int index = 0; index < (int)tempNode.SrcChildList.size(); ++index)
		{
			std::string NewPath = tempNode.DstParent;
			NewPath.append("/");
			NewPath.append(tempNode.SrcChildList[index]->GetFileName());
			if (!SameCheck(tempNode.SrcChildList[index]->GetRealPath(), NewPath, IsCoverSameFile))
			{
				continue;
			}
			m_SrcPathList.push_back(tempNode.SrcChildList[index]->GetRealPath());
			m_DstPathList.push_back(NewPath);
			if (FOLDER_FILE == tempNode.SrcChildList[index]->GetRealType())
			{
				MoveDstChild ChildrenNode;
				ChildrenNode.DstParent = NewPath;
				ChildrenNode.SrcChildList = tempNode.SrcChildList[index]->GetChildrenList();
				WaitMoveQueue.push(ChildrenNode);
			}
		}
		WaitMoveQueue.pop();
	}
	return SUCCESS_CODE;
}



ErrorCode MoveCommand::MoveSrcFile(std::string& SrcStr, std::string& DstStr, bool& IsCoverSameFile)
{
	int DstWildcardIndex = DstStr.find_first_of('*');
	FileInfo* SrcNode = g_DataDiskPtr->GetFileInfo(SrcStr);
	if (NULL == SrcNode)
		return ERROR_SRC_PATH_CODE;
	FileInfo* DstNode = NULL;
	if (std::string::npos != DstWildcardIndex)
	{
		DstNode = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(DstStr));
		if (DstStr.size() - 1 != DstWildcardIndex || '/' != DstStr[DstStr.size() - 2])
			return ERROR_DST_PATH_CODE;
		if (NULL == DstNode || FOLDER_FILE != DstNode->GetType())
			return ERROR_DST_PATH_CODE;
		std::string NamePath = DstNode->GetRealPath();
		NamePath.append("/");
		NamePath.append(g_DataDiskPtr->GetName(SrcStr));
		if (!SameCheck(SrcStr, NamePath, IsCoverSameFile))
			return ERROR_USER_STOP_CODE;
		m_SrcPathList.push_back(SrcStr);
		m_DstPathList.push_back(NamePath);
	}
	else
	{
		DstNode = g_DataDiskPtr->GetFileInfo(DstStr);
		if (NULL == DstNode)
		{
			DstNode = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(DstStr));
			if (NULL == DstNode || FOLDER_FILE != DstNode->GetType())
				return ERROR_DST_PATH_CODE;
			if (!SameCheck(SrcStr, DstStr, IsCoverSameFile))
				return ERROR_USER_STOP_CODE; 
			m_SrcPathList.push_back(SrcStr);
			m_DstPathList.push_back(DstStr);
		}
		else
		{
			if (NULL != DstNode && FOLDER_FILE != DstNode->GetType())
				return ERROR_DST_PATH_CODE;
			std::string NamePath = DstNode->GetRealPath();
			NamePath.append("/");
			NamePath.append(g_DataDiskPtr->GetName(SrcStr));
			if (!SameCheck(SrcStr, NamePath, IsCoverSameFile))
				return ERROR_USER_STOP_CODE; 
			m_SrcPathList.push_back(SrcStr);
			m_DstPathList.push_back(NamePath);
		}
	}

	return SUCCESS_CODE;
}

bool MoveCommand::SameCheck(std::string SrcStr, std::string DstStr, bool& IsCoverSameFile)
{
	if (!IsCoverSameFile && g_DataDiskPtr->IsPathExsit(DstStr))
	{
		bool IsNo = false;
		while (1)
		{
			std::cout << DstStr << " is Exist. Do you want to cover or merge file ?<yes/no/all>:";
			std::string InPut;
			std::cin.clear();
			std::cin.sync();
			getline(std::cin, InPut);
			std::cin.clear();
			if ("no" == InPut)
			{
				return false;
				break;
			}
			else if ("all" == InPut)
			{
				IsCoverSameFile = true;
				return true;
				break;
			}
			else if ("yes" == InPut)
			{
				return true;
				break;
			}
		}
		return IsNo;
	}
	return true;
}

ErrorCode MoveCommand::DelChildNode(std::string Str)
{
	std::set<std::string> DelList;
	std::stack<FileInfo*> WaitDelStack;
	std::queue<FileInfo*> ParentQueue;
	FileInfo* DelNode = g_DataDiskPtr->GetFileInfo(Str);
	if (NULL == DelNode || FOLDER_FILE == DelNode->GetRealType())
		return ERROR_SRC_PATH_CODE;
	ParentQueue.push(DelNode);
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
			for (int index = 0; index < (int)ChildList.size(); ++index)
			{
				if (NULL == ChildList[index])
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
