#include <iostream>
#include "copy_command.h"
#include "data_disk.h"
#include "file_info.h"

CopyCommand::CopyCommand(std::vector<std::string> & ArgList)
	: Command(ArgList,3)
{

}

CopyCommand::~CopyCommand()
{
}


ErrorCode CopyCommand::Run()
{
	ToAbsolutePath();
	if (m_ArgList.size() != 3)
		return ERROR_ARG_COUNT_CODE;
	std::string SrcStr = m_ArgList[1];
	std::string DstStr = m_ArgList[2];
	if (SrcStr == DstStr)
		return ERROR_SRC_PATH_CODE;
	ErrorCode ResolvePathResult = ResolvePath();
	if (SUCCESS_CODE != ResolvePathResult)
		return ResolvePathResult;
	bool SrcIsRealPath = false;
	bool DstIsRealPath = false;
	if ('@' == SrcStr[0])
		SrcIsRealPath = true;
	if ('@' == DstStr[0])
		DstIsRealPath = true;
	//ErrorCode DoingResult;
	if (SrcIsRealPath && !DstIsRealPath)//Src�Ǳ����ļ�
	{//��Ҫ���Ը�������Ƿ������ж���©
		std::string RealSrcStr = SrcStr.substr(1, SrcStr.size() - 1);
		if ('*' == DstStr[DstStr.size() - 1])
		{
			DstStr = DstStr.substr(0, DstStr.size() - 1);
			int SrcNameIndex = (int)RealSrcStr.find_last_of('/') + 1;
			if (std::string::npos == SrcNameIndex)
				return ERROR_DST_PATH_CODE;
			DstStr.append(RealSrcStr.substr(SrcNameIndex, RealSrcStr.size() - SrcNameIndex));
		}
		if (DstStr.empty())
			return ERROR_DST_PATH_CODE;
		ErrorCode ResultCode = g_DataDiskPtr->AddFileRealDisk2DataDisk(RealSrcStr, DstStr);
		if (SUCCESS_CODE == ResultCode)
			std::cout<<"1 file were successfully copied."<< std::endl;
		return ResultCode;
	}
	else if(!SrcIsRealPath && DstIsRealPath)//Src�����ļ�Dst�����ļ�
	{//���Դ·���Ƕ���ļ�
		std::string RealDst = DstStr.substr(1, DstStr.size() - 1);
		return DataDisk2RealDiskCopy(SrcStr, RealDst);
	}
	else if (!SrcIsRealPath && !DstIsRealPath)
	{
		return DataDist2DataDistCopy(SrcStr, DstStr);
	}
	else if (SrcIsRealPath && SrcIsRealPath)
	{
		return ERROR_COMMAND_USE_CODE;
	}
	return SUCCESS_CODE;
}

ErrorCode CopyCommand::ResolvePath()
{
	std::string SrcStr = m_ArgList[1];
	std::string DstStr = m_ArgList[2];
	if (SrcStr.empty())
		return ERROR_SRC_PATH_CODE;
	int SrcLastWildcardIndex = (int)SrcStr.find_last_of('*');
	bool SrcIsRealPath = false;
	if (SrcLastWildcardIndex != std::string::npos)
	{//Դ·���д���ͨ���
		if (SrcLastWildcardIndex != SrcStr.size() - 1)//ͨ����������һ��
			return ERROR_SRC_PATH_CODE;
		if (SrcStr[SrcLastWildcardIndex - 1] != '/')//ͨ���һ������������·����Ϣ
			return ERROR_ARG_COUNT_CODE;
		if (SrcStr[0] == '@')//��ʵ����·������ͨ���
			return ERROR_SRC_PATH_CODE;
		FileInfo* ParentFileInfo = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(SrcStr));
		if (NULL == ParentFileInfo || ParentFileInfo->GetType() != FOLDER_FILE)
			return ERROR_SRC_PATH_CODE;
	}
	else
	{
		if (SrcStr[0] == '@')
		{
			SrcIsRealPath = true;
			if (SrcStr.size() < 2 || SrcStr[1] == '/' || SrcStr[1] == '.')
				return ERROR_SRC_PATH_CODE;
		}
		else
		{
			FileInfo* CurSrcFileInfo = g_DataDiskPtr->GetFileInfo(SrcStr);
			if (NULL == CurSrcFileInfo)
				return ERROR_SRC_PATH_CODE;

		}
	}
	int DstLastWildcardIndex = (int)DstStr.find_last_of('*');
	bool DstIsRealPath = false;
	if (DstLastWildcardIndex != std::string::npos)
	{//Դ·���д���ͨ���
		if (DstLastWildcardIndex != DstStr.size() - 1)//ͨ����������һ��
			return ERROR_DST_PATH_CODE;
		if (DstStr[DstLastWildcardIndex - 1] != '/')//ͨ���һ������������·����Ϣ
			return ERROR_DST_PATH_CODE;
		if (DstStr[0] == '@'&&SrcIsRealPath)//��ʵ����·������ͨ���
			return ERROR_DST_PATH_CODE;
		FileInfo* ParentFileInfo = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(DstStr));
		if (NULL == ParentFileInfo || ParentFileInfo->GetType() != FOLDER_FILE)
			return ERROR_DST_PATH_CODE;
	}
	else
	{
		if (DstStr[0] == '@')
		{
			if (SrcIsRealPath)
				return ERROR_DST_PATH_CODE;
			if (DstStr.size() < 2 || DstStr[1] == '/' || DstStr[1] == '.')
				return ERROR_DST_PATH_CODE;
			FileInfo* SrcFileInfo = NULL;
			if (std::string::npos == SrcLastWildcardIndex)
				SrcFileInfo = g_DataDiskPtr->GetFileInfo(SrcStr);
			else
				SrcFileInfo = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(SrcStr));
			if (NULL == SrcFileInfo )
				return ERROR_SRC_PATH_CODE;
			if (FOLDER_FILE == SrcFileInfo->GetType() || std::string::npos != SrcLastWildcardIndex)
			{
				std::string TempDstRealStr = DstStr.substr(1, DstStr.size() - 1);
				if (!g_DataDiskPtr->CheckRealDiskPathIsFolder(TempDstRealStr))
					return ERROR_DST_PATH_CODE;
			}
		}
		else
		{
			FileInfo* ParentFileInfo = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(DstStr));
			if (NULL == ParentFileInfo || ParentFileInfo->GetType() != FOLDER_FILE)
				return ERROR_DST_PATH_CODE;
		}
	}
	return SUCCESS_CODE;
}

bool CopyCommand::OverWriteSecendAsk(std::string & FileName)
{
	while (true)
	{
		std::cout << "\"" << FileName << "\"" << " has exist.Do you want to overwrite file?<y/n> ";
		std::string InPut;
		std::cin.clear();
		std::cin.sync();
		getline(std::cin, InPut);
		
		if ("y" == InPut)
			return true;
		if ("n" == InPut)
			return false;
		std::cin.clear();
	}
	return false;
}

ErrorCode CopyCommand::DataDist2DataDistCopy(std::string & SrcStr, std::string & DstStr)
{
	m_SrcPathArr.clear();
	m_DstPathArr.clear();
	int SucceedCount = 0;
	FileInfo* DstNode = g_DataDiskPtr->GetFileInfo(DstStr);
	FileInfo* SrcNode = g_DataDiskPtr->GetFileInfo(SrcStr);
	if ((NULL != SrcNode && FOLDER_FILE == SrcNode->GetRealType()) || '*' == SrcStr[SrcStr.size() - 1])
	{//Դ·�����ļ��У���������ͨ�����˵����Ҫ������ļ�������һ���ļ�����,RealType����ΪԴ�ļ�����������ļ���ֱ�ӿ����ļ�
		if ((NULL == DstNode || DstNode->GetType() != FOLDER_FILE)&& '*' != DstStr[DstStr.size() - 1])
		{	//Ŀ��·�������ڣ����ߴ��ڵ��ǲ����ļ��У�Type����Ϊ���Ŀ��·���������ļ������ָ���ļ��б���Ϊ�ļ���·��
			return ERROR_SRC_PATH_CODE;
		}
		FileInfo* SrcParent = NULL;
		if (NULL == SrcNode)
			SrcParent = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(SrcStr));
		else
			SrcParent = SrcNode;
		if (NULL == SrcParent)
			return ERROR_SRC_PATH_CODE;
		FileInfo* DstParent = NULL;
		if (NULL == DstNode)
			DstParent = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(DstStr));
		else
			DstParent = DstNode;
		if (NULL == DstParent)
			return ERROR_DST_PATH_CODE;
		std::vector<FileInfo*> SrcChildrenArr = SrcParent->GetChildrenList();
		if (SrcChildrenArr.empty())
			return ERROR_SRC_PATH_CODE;
		m_SrcPathArr.resize(SrcChildrenArr.size());
		m_DstPathArr.resize(SrcChildrenArr.size());
		std::string SrcParentStr = SrcParent->GetFilePath();
		SrcParentStr.append("/");
		std::string DstParentStr = DstParent->GetFilePath();
		DstParentStr.append("/");

		for (int index = 0; index < (int)SrcChildrenArr.size();++index)
		{
			FileInfo* ChildPtr = SrcChildrenArr[index];
			if(NULL == ChildPtr || FOLDER_FILE == ChildPtr->GetRealType())
				continue;
			m_SrcPathArr[index] = SrcParentStr;
			m_SrcPathArr[index].append(ChildPtr->GetFileName());
			m_DstPathArr[index] = DstParentStr;
			m_DstPathArr[index].append(ChildPtr->GetFileName());
		}
		for (int index = 0;index < (int)m_SrcPathArr.size(); ++ index)
		{
			if(m_SrcPathArr[index].empty())
				continue;
			FileInfo* NewDstFile = g_DataDiskPtr->GetFileInfo(m_DstPathArr[index]);
			FileInfo* SrcFile = g_DataDiskPtr->GetFileInfo(m_SrcPathArr[index]);
			if (NULL == SrcFile)
				return ERROR_SRC_PATH_CODE;
			if (NULL != NewDstFile)
			{
				if (NewDstFile->GetRealType() != SrcFile->GetRealType())
					return ERROR_PATH_N_TYPE;
				if (!OverWriteSecendAsk(m_DstPathArr[index]))
				{//����ѯ��
					continue;
				}
				else
				{
					g_DataDiskPtr->DelFile(NewDstFile);
				}
			}
			ErrorCode CopyResult = g_DataDiskPtr->AddFileDataDisk2DataDisk(m_SrcPathArr[index], m_DstPathArr[index]);
			if(SUCCESS_CODE == CopyResult)
			++SucceedCount;
		}
	}
	else
	{
		std::string FullDstStr = DstStr;
		if (NULL != DstNode && DstNode->GetType() == FOLDER_FILE)
		{
			if (NULL == SrcNode)
				return ERROR_SRC_PATH_CODE;
			FullDstStr.append("/");
			FullDstStr.append(SrcNode->GetFileName());
		}
		else if ('*' == DstStr[DstStr.size() - 1])
		{
			FileInfo* DstParentNode = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(DstStr));
			if (NULL == DstParentNode || NULL == SrcNode)
				return ERROR_DST_PATH_CODE;
			FullDstStr = DstParentNode->GetFilePath();
			FullDstStr.append("/");
			FullDstStr.append(SrcNode->GetFileName());
		}
		FileInfo* NewDstFile = g_DataDiskPtr->GetFileInfo(FullDstStr);
		if (NULL != NewDstFile)
		{
			if (NewDstFile->GetRealType() != SrcNode->GetRealType())
				return ERROR_PATH_N_TYPE;
			if (!OverWriteSecendAsk(FullDstStr))
			{//����ѯ��
				return ERROR_USER_STOP_CODE;
			}
			else
			{
				g_DataDiskPtr->DelFile(NewDstFile);
			}
		}
		ErrorCode CopyResult = g_DataDiskPtr->AddFileDataDisk2DataDisk(SrcStr, FullDstStr);
		if(SUCCESS_CODE == CopyResult)
			SucceedCount = 1;
	}
	if (SucceedCount > 0)
		std::cout<<SucceedCount << " files were successfully copied."<<std::endl;;
	return SUCCESS_CODE;
}

ErrorCode CopyCommand::DataDisk2RealDiskCopy(std::string & SrcStr, std::string & DstStr)
{
	int SucceedCount = 0;
	m_SrcPathArr.clear();
	m_DstPathArr.clear();
	//FileInfo* DstNode = g_DataDiskPtr->GetFileInfo(DstStr);
	FileInfo* SrcNode = g_DataDiskPtr->GetFileInfo(SrcStr);
	if ((NULL != SrcNode && FOLDER_FILE == SrcNode->GetRealType()) || '*' == SrcStr[SrcStr.size() - 1])
	{//Դ·�����ļ��У���������ͨ�����˵����Ҫ������ļ�������һ���ļ�����
		if (!g_DataDiskPtr->CheckRealDiskPathIsFolder(DstStr))// check real path
			return ERROR_DST_PATH_CODE;
		//���·��ǰ׺
		FileInfo* SrcParent = NULL;
		if (NULL == SrcNode)
			SrcParent = g_DataDiskPtr->GetFileInfo(g_DataDiskPtr->GetParentPath(SrcStr));
		else
			SrcParent = SrcNode;
		if (NULL == SrcParent)
			return ERROR_SRC_PATH_CODE;
		std::vector<FileInfo*> SrcChildrenArr = SrcParent->GetChildrenList();
		if (SrcChildrenArr.empty())
			return ERROR_SRC_PATH_CODE;
		m_SrcPathArr.resize(SrcChildrenArr.size());
		m_DstPathArr.resize(SrcChildrenArr.size());
		std::string SrcParentStr = SrcParent->GetFilePath();
		SrcParentStr.append("/");
		std::string DstParentStr = DstStr;
		DstParentStr.append("/");
		//���·��ǰ׺ end
		for (int index = 0; index < (int)SrcChildrenArr.size(); ++index)
		{
			FileInfo* ChildPtr = SrcChildrenArr[index];
			if (NULL == ChildPtr || FOLDER_FILE == ChildPtr->GetRealType())
				continue;
			m_SrcPathArr[index] = SrcParentStr;
			m_SrcPathArr[index].append(ChildPtr->GetFileName());
			m_DstPathArr[index] = DstParentStr;
			m_DstPathArr[index].append(ChildPtr->GetFileName());
		}
		//���п���
		for (int index = 0; index < (int)m_SrcPathArr.size(); ++index)
		{
			if (m_SrcPathArr[index].empty())
				continue;

			ErrorCode CopyResult =  g_DataDiskPtr->AddFileDataDisk2RealDisk(m_SrcPathArr[index], m_DstPathArr[index]);
			if (SUCCESS_CODE == CopyResult)
				++SucceedCount;
		}
	}
	else
	{//Դ�ǵ����ļ�
		std::string FullDstStr = DstStr;
		if (g_DataDiskPtr->CheckRealDiskPathIsFolder(DstStr))
		{//Ŀ��·���Ǹ�Ŀ¼
			if (NULL == SrcNode)
				return ERROR_SRC_PATH_CODE;
			FullDstStr.append("/");
			FullDstStr.append(SrcNode->GetFileName());
		}
		ErrorCode CopyResult = g_DataDiskPtr->AddFileDataDisk2RealDisk(SrcStr, FullDstStr);
		if (SUCCESS_CODE == CopyResult)
			SucceedCount = 1;
	}
	if(SucceedCount > 0)
		std::cout<<SucceedCount << " files were successfully copied."<< std::endl;
	return SUCCESS_CODE;
}
