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
	if (SrcIsRealPath && !DstIsRealPath)//Src是本地文件
	{//需要测试各种情况是否是有判断遗漏
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
	else if(!SrcIsRealPath && DstIsRealPath)//Src虚拟文件Dst本地文件
	{//如果源路径是多个文件
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
	{//源路径中存在通配符
		if (SrcLastWildcardIndex != SrcStr.size() - 1)//通配符不是最后一个
			return ERROR_SRC_PATH_CODE;
		if (SrcStr[SrcLastWildcardIndex - 1] != '/')//通配符一级还存在其他路径信息
			return ERROR_ARG_COUNT_CODE;
		if (SrcStr[0] == '@')//真实磁盘路径存在通配符
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
	{//源路径中存在通配符
		if (DstLastWildcardIndex != DstStr.size() - 1)//通配符不是最后一个
			return ERROR_DST_PATH_CODE;
		if (DstStr[DstLastWildcardIndex - 1] != '/')//通配符一级还存在其他路径信息
			return ERROR_DST_PATH_CODE;
		if (DstStr[0] == '@'&&SrcIsRealPath)//真实磁盘路径存在通配符
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
	{//源路径是文件夹，或最后存在通配符，说明需要将多个文件拷贝另一个文件夹下,RealType是因为源文件如果是链接文件则直接拷贝文件
		if ((NULL == DstNode || DstNode->GetType() != FOLDER_FILE)&& '*' != DstStr[DstStr.size() - 1])
		{	//目标路径不存在，或者存在但是不是文件夹，Type是因为如果目标路径是链接文件如果他指向文件夹被视为文件夹路径
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
				{//覆盖询问
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
			{//覆盖询问
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
	{//源路径是文件夹，或最后存在通配符，说明需要将多个文件拷贝另一个文件夹下
		if (!g_DataDiskPtr->CheckRealDiskPathIsFolder(DstStr))// check real path
			return ERROR_DST_PATH_CODE;
		//获得路径前缀
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
		//获得路径前缀 end
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
		//进行拷贝
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
	{//源是单个文件
		std::string FullDstStr = DstStr;
		if (g_DataDiskPtr->CheckRealDiskPathIsFolder(DstStr))
		{//目标路径是个目录
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
