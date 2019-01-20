#include "y_link_manager.h"
#include "y_disk_operator.h"
#include "y_symlnk_file.h"
std::unique_ptr<YLinkManager> g_pSymMananger = std::make_unique<YLinkManager>();

YLinkManager::YLinkManager()
{

}

YLinkManager::~YLinkManager()
{

}

void YLinkManager::addMapped(YFile * pDstFile, YFile * pSymLnkFile)
{
	if (!rSym2DstMap.empty() && rSym2DstMap.count(pSymLnkFile))
	{
		YFile* pOldDst = rSym2DstMap[pSymLnkFile];
		std::map<YFile*,std::vector<YFile*>>::iterator rIter = rMap.find(pOldDst);
		if (rMap.end() != rIter)
		{
			std::vector<YFile*>& rLnks = rIter->second;
			for (std::vector<YFile*>::iterator rvecIter = rLnks.begin(); rvecIter != rLnks.end();)
			{
				if (*rvecIter == pSymLnkFile)
				{
					rvecIter = rLnks.erase(rvecIter);
					break;
				}
				else
				{
					++rvecIter;
				}
			}
		}
		rSym2DstMap[pSymLnkFile] = pDstFile;
	}
	else
	{
		std::pair<YFile*, YFile*> rNewPair(pSymLnkFile, pDstFile);
		rSym2DstMap.insert(rNewPair);
	}
	std::map<YFile*, std::vector<YFile*>>::iterator rIter = rMap.find(pDstFile);
	if (rIter != rMap.end())
	{
		for (size_t index = 0; index < rIter->second.size();++index)
		{
			if (pSymLnkFile == rIter->second[index])
				return;
		}
		rIter->second.push_back(pSymLnkFile);
	}
	else
	{
		std::vector<YFile*> rLnkVector;
		rLnkVector.push_back(pSymLnkFile);
		std::pair<YFile*, std::vector<YFile*>> rMapPair(pDstFile, rLnkVector);
		rMap.insert(rMapPair);
	}
}

void YLinkManager::delDstFile(YFile * pDstFile)
{
	std::map<YFile*, std::vector<YFile*>>::iterator rIter = rMap.find(pDstFile);
	if (rIter != rMap.end())
	{
		std::vector<YFile*> rLnkList = rIter->second;
		for (std::vector<YFile*>::iterator rVecIter = rLnkList.begin(); rVecIter != rLnkList.end();)
		{
			if (nullptr == *rVecIter)
			{
				continue;
			}
			std::string szName = g_pDiskOperator->getFullPath(*rVecIter);
			g_pDiskOperator->deleteNode(szName);
			rVecIter++;
		}
		if(rMap.find(pDstFile) != rMap.end())
			rMap.erase(rMap.find(pDstFile));
	}
}

void YLinkManager::delSymLnkFile(YFile * pSymLnkFile)
{
	if (!rSym2DstMap.empty() && rSym2DstMap.count(pSymLnkFile))
	{
		YFile* pOldDst = rSym2DstMap[pSymLnkFile];
		std::map<YFile*, std::vector<YFile*>>::iterator rIter = rMap.find(pOldDst);
		if (rMap.end() != rIter)
		{
			std::vector<YFile*>& rLnks = rIter->second;
			for (std::vector<YFile*>::iterator rvecIter = rLnks.begin(); rvecIter != rLnks.end();)
			{
				if (*rvecIter == pSymLnkFile)
				{
					rvecIter = rLnks.erase(rvecIter);
					rSym2DstMap.erase(rSym2DstMap.find(pSymLnkFile));
					break;
				}
				else
				{
					++rvecIter;
				}
			}
		}
	}
}

void YLinkManager::changeLnkDst(YFile * pOldLnk, YFile * pNewLnk)
{
	if (rMap.empty() || !rMap.count(pOldLnk))
	{
		return;
	}
	std::vector<YFile*> rLnkList = rMap[pOldLnk];
	for (size_t index = 0; index < rLnkList.size(); ++index)
	{
		std::string szFullPath = g_pDiskOperator->getFullPath(pNewLnk);
		YFile* pDstNode = g_pDiskOperator->lnkDstFindHelper(szFullPath);
		((YSymlnkFile*)rLnkList[index])->setDstFile(pDstNode);
	}
	rMap.erase(rMap.find(pOldLnk));
}

