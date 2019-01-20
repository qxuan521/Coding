#pragma once
#include <map>
#include <vector>
#include <memory>
class YFile;
class YLinkManager
{
public:
	YLinkManager();
	~YLinkManager();
	void addMapped(YFile* pDstFile, YFile* pSymLnkFile);
	void delDstFile(YFile* pDstFile);
	void delSymLnkFile(YFile* pSymLnkFile);
	void changeLnkDst(YFile* pOldLnk, YFile* pNewLnk);
private:
	std::map<YFile*, std::vector<YFile*>> rMap;
	std::map<YFile*, YFile*>			  rSym2DstMap;
};

extern std::unique_ptr<YLinkManager> g_pSymMananger;