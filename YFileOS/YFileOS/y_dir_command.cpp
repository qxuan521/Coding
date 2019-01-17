#include <iostream>
#include <xiosbase>
#include <iomanip>
#include "y_dir_command.h"
#include "y_disk_operator.h"
#include "y_tool.h"
YDirCommand::YDirCommand(const std::string& szName)
	:YCommand(szName, 0)
{
	m_rTypeArg.insert(std::pair<std::string,bool>("/s", false));
	m_rTypeArg.insert(std::pair<std::string,bool>("/ad", false));
}

YDirCommand::~YDirCommand()
{

}

YErrorCode YDirCommand::excultCommand(YCommandInfo& rCommandInfo)
{
	YErrorCode rResultCode;
	rResultCode = toAbsolutePath(rCommandInfo.rPathList);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		return rResultCode;
	}
	rResultCode = handleCommandArg(rCommandInfo);
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		errorPrint(rResultCode);
		return rResultCode;
	}
	//dir没有路径参数时视为 当前路径
	if (m_rArgList.empty())
	{
		m_rArgList.push_back(m_szCurWorkPath);
	}
	//将路径全路径（有可能包含通配符）转换成节点数组
	std::vector<YIFile*> rQueryResult;

	//进行查询
	std::vector<DirSearchResult> rDirResult;
	if (m_rTypeArg["/s"])
	{
		if (m_rTypeArg["/ad"])
		{//all children folder
			rResultCode = allChildFolderSearch(rQueryResult, rDirResult);
		}
		else
		{//all children file&folder
			rResultCode = allChildSearch(rQueryResult, rDirResult);
		}
	}
	else
	{
		if (m_rTypeArg["/ad"])
		{//folder
			rResultCode = folderSearch(rQueryResult, rDirResult);
		}
		else
		{//file&folder 
			rResultCode = normalSearch(rQueryResult,rDirResult);
		}
	}
	if (Y_OPERAT_SUCCEED != rResultCode)
	{
		return rResultCode;
	}
	printResult(rDirResult);
	return Y_OPERAT_SUCCEED;
}

void YDirCommand::printResult(std::vector<DirSearchResult>& rResult)
{
	if (rResult.empty())
		return;
	for (int index = 0; index < (int)rResult.size(); ++index)
	{
		DirSearchResult& SingleResult = rResult[index];
		std::string szFullPath = g_pDiskOperator->getFullPath(SingleResult.FilePtr);
		std::cout << "Directories of" << " \"" << szFullPath << "\":" << std::endl;
		std::cout << "" << std::endl;
		//输出. .. 如果时盘符则不输出
		std::int32_t rSizeCount = 0;
		if(!g_pDiskOperator->isRootName(szFullPath))
		{
			printSeftNParent(SingleResult.FilePtr);
		}
		for (int LoopCount = 0; LoopCount < (int)SingleResult.CurLevelResult.size(); ++LoopCount)
		{
			int FileSize = SingleResult.CurLevelResult[LoopCount]->getFileSize();
			rSizeCount += FileSize;
			std::string DateStr = SingleResult.CurLevelResult[LoopCount]->getModifyDate();
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
			std::cout << std::setw(6) << fileTypeString(SingleResult.CurLevelResult[LoopCount]);
			if (SingleResult.CurLevelResult[LoopCount]->IsRealFile())
			{
				std::cout << std::setw(9) << std::setiosflags(std::ios::right) << FileSize;
			}
			else
			{
				std::cout << std::setw(9) << std::setiosflags(std::ios::right) << " ";
			}
			std::string Name = SingleResult.CurLevelResult[LoopCount]->getName();

			if (SingleResult.CurLevelResult[LoopCount]->IsRealSymLnk())
			{
				std::cout << " " << SingleResult.CurLevelResult[LoopCount]->getName() << "[" << SingleResult.CurLevelResult[LoopCount]->getShowName() << "]" << std::endl;
			}
			else
			{
				std::cout << " " << SingleResult.CurLevelResult[LoopCount]->getName() << std::endl;
			}
		}
		std::cout << "" << std::endl;
		std::cout << SingleResult.FileCount << " files,  " << SingleResult.FolderCount << " Folders." << std::endl;
		std::cout << " files size count:  " << rSizeCount  << " Bytes." << std::endl;
	}
}

std::string YDirCommand::fileTypeString(YIFile * pFile)
{
	std::string rTypeString;
	if (pFile->IsRealFolder())
	{
		rTypeString = "<DIR>";
	}
	else if (pFile->IsRealSymLnk())
	{
		rTypeString = "<LNK>";
	}
	else
	{
		rTypeString = "     ";
	}
	return rTypeString;
}

void YDirCommand::printSeftNParent(YIFile * pFile)
{
	{// .
		int FileSize = pFile->getFileSize();
		std::string DateStr = pFile->getModifyDate();
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
		std::cout << std::setw(6) << fileTypeString(pFile);
		std::cout << std::setw(9) << std::setiosflags(std::ios::right) << " ";
		std::string Name = ".";
		std::cout << " " << Name << std::endl;
	}
	{// ..
		YIFile* pParent = pFile->getParent();
		int FileSize = pParent->getFileSize();
		std::string DateStr = pParent->getModifyDate();
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
		std::cout << std::setw(6) << fileTypeString(pParent);
		std::cout << std::setw(9) << std::setiosflags(std::ios::right) << " ";
		std::string Name = "..";
		std::cout << " " << Name << std::endl;
	}
}

YErrorCode YDirCommand::allChildSearch(std::vector<YIFile*>& rDirArr, std::vector<DirSearchResult>& rDirResult)
{
	std::set<YIFile*> rHistorySet;
	std::function<bool(YIFile*)> rPredicate([](YIFile* pFile)->bool
	{
		return pFile != nullptr;
	});
	YErrorCode rResultCode;
	for (size_t index = 0;index < rDirArr.size(); ++index)
	{
		rResultCode = searchHelpter(rDirArr[index], rDirResult, rHistorySet, rPredicate);
		if (Y_OPERAT_SUCCEED != rResultCode)
		{
			std::string szFileFullPath = g_pDiskOperator->getFullPath(rDirArr[index]);
			errorPrint(rResultCode, szFileFullPath);
		}
	}
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDirCommand::allChildFolderSearch(std::vector<YIFile*>& rDirArr, std::vector<DirSearchResult>& rDirResult)
{
	std::set<YIFile*> rHistorySet;
	std::function<bool(YIFile*)> rPredicate([](YIFile* pFile)->bool
	{
		return (pFile != nullptr && pFile->IsFolder());
	});
	YErrorCode rResultCode;
	for (size_t index = 0; index < rDirArr.size(); ++index)
	{
		rResultCode = searchHelpter(rDirArr[index], rDirResult, rHistorySet, rPredicate);
		if (Y_OPERAT_SUCCEED != rResultCode)
		{
			std::string szFileFullPath = g_pDiskOperator->getFullPath(rDirArr[index]);
			errorPrint(rResultCode, szFileFullPath);
		}
	}
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDirCommand::folderSearch(std::vector<YIFile*>& rDirArr, std::vector<DirSearchResult>& rDirResult)
{
	std::set<YIFile*> rHistorySet;
	for (size_t index = 0;index < rDirArr.size(); ++index)
	{
		DirSearchResult rResult;
		rHistorySet.insert(rDirArr[index]);
		rResult.initializeResult(rDirArr[index]);
		for (auto rIter = rResult.CurLevelResult.begin(); rIter != rResult.CurLevelResult.end();)
		{
			if (nullptr == (*rIter))
			{
				std::string szFileFullPath = g_pDiskOperator->getFullPath(*rIter);
				errorPrint(YERROR_POINTER_NULL, szFileFullPath);
				continue;
			}
			if (!(*rIter)->IsFolder())
			{
				rIter = rResult.CurLevelResult.erase(rIter);
			}
			else
			{
				++rIter;
			}
		}
		rDirResult.push_back(rResult);
	}
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDirCommand::normalSearch(std::vector<YIFile*>& rDirArr, std::vector<DirSearchResult>& rDirResult)
{
	std::set<YIFile*> rHistorySet;
	for (size_t index = 0; index < rDirArr.size(); ++index)
	{
		DirSearchResult rResult;
		rHistorySet.insert(rDirArr[index]);
		rResult.initializeResult(rDirArr[index]);
		for (auto rIter = rResult.CurLevelResult.begin(); rIter != rResult.CurLevelResult.end();++rIter)
		{
			if (nullptr == (*rIter))
			{
				std::string szFileFullPath = g_pDiskOperator->getFullPath(*rIter);
				errorPrint(YERROR_POINTER_NULL, szFileFullPath);
				continue;
			}
		}
		rDirResult.push_back(rResult);
	}
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDirCommand::searchHelpter(YIFile * pFile, std::vector<DirSearchResult>& rDirResultArr, std::set<YIFile*>& rHistorySet, std::function<bool(YIFile*)>& rPredicate)
{
	if (nullptr == pFile )
	{
		return YERROR_POINTER_NULL;
	}
	rHistorySet.insert(pFile);
	DirSearchResult rResult;
	rResult.initializeResult(pFile);
	for (size_t index = 0; index < rResult.CurLevelResult.size();++index)
	{
		if (rResult.CurLevelResult[index]->IsFolder())
		{
			searchHelpter(rResult.CurLevelResult[index], rDirResultArr, rHistorySet, rPredicate);
		}
	}
	for (auto rIter = rResult.CurLevelResult.begin(); rIter != rResult.CurLevelResult.end();)
	{
		if (!rPredicate(*rIter))
		{
			rIter = rResult.CurLevelResult.erase(rIter);
		}
		else
		{
			++rIter;
		}
		rDirResultArr.push_back(rResult);
	}
	return Y_OPERAT_SUCCEED;
}

YErrorCode YDirCommand::handleWildCard(std::vector<YIFile*>& rResult)
{
	for (size_t index = 0; index < m_rArgList.size(); index++)
	{
		std::regex	rMutchRegex = makeRegexByPath("*");
		std::string szDirPath = m_rArgList[index];
		if (isHaveWildCard(m_rArgList[index]))
		{
			std::string szName = getNameFromFullPath(szDirPath);
			rMutchRegex = makeRegexByPath(szName);
			szDirPath = getParentPath(szDirPath);
		}
		YErrorCode rResultCode = g_pDiskOperator->queryFolderNode(szDirPath, rResult);
		if (rResultCode != Y_OPERAT_SUCCEED)
		{
			errorPrint(rResultCode, m_rArgList[index]);
		}
		for (std::vector<YIFile*>::iterator rIter = rResult.begin(); rIter != rResult.end();)
		{
			if (!std::regex_match((*rIter)->getName(), rMutchRegex))
			{
				rIter = rResult.erase(rIter);
			}
			else
			{
				++rIter;
			}
		}
	}
	return Y_OPERAT_SUCCEED;
}

void DirSearchResult::initializeResult(YIFile * pFile)
{
	this->FilePtr = pFile;
	this->ParentPtr = pFile->getParent();
	this->FileCount = pFile->getChildrenFileCount();
	this->FolderCount = pFile->getChildrenFolderCount();
	g_pDiskOperator->getChildren(pFile, this->CurLevelResult);
}
