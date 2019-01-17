#include "y_link_manager.h"
#include "y_disk_operator.h"

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
	}
	std::map<YFile*, std::vector<YFile*>>::iterator rIter = rMap.find(pDstFile);
	if (rIter != rMap.end())
	{
		rIter->second.push_back(pSymLnkFile);
	}
	else
	{
		std::pair<YFile*, YFile*> rNewPair(pSymLnkFile, pDstFile);
		rSym2DstMap.insert(rNewPair);
	}
}

void YLinkManager::delDstFile(YFile * pDstFile)
{
	std::map<YFile*, std::vector<YFile*>>::iterator rIter = rMap.find(pDstFile);
	if (rIter != rMap.end())
	{
		std::vector<YFile*>& rLnks = rIter->second;
		for (std::vector<YFile*>::iterator rvecIter = rLnks.begin(); rvecIter != rLnks.end();)
		{
			std::string szName = g_pDiskOperator->getFullPath((YIFile*)*rvecIter);
			g_pDiskOperator->deleteNode(szName);
			rvecIter = rLnks.erase(rvecIter);
		}
		rMap.erase(rIter);
	}
}

void YLinkManager::delSymLnkFile(YFile * pSymLnkFile)
{
	if (rSym2DstMap.empty() && rSym2DstMap.count(pSymLnkFile))
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

