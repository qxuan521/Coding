#pragma once
#include <vector>

using namespace std;
class YIFile
{
public:
	virtual ~YIFile();
	YIFile();

	virtual const int8_t* getFileData() = 0;
	virtual const uint32_t getFileSize() = 0;
	virtual const string  getModifyDate() = 0;
	virtual bool IsFile() = 0;
	virtual bool IsFolder() = 0;
	virtual bool IsSymLnk() = 0;
private:

};

class YFile : public YIFile
{
	enum YFileType
	{
		Y_File,
		Y_Folder,
		Y_SymLnk
	};
public:
	YFile();
	virtual ~YFile();

private:
	vector<int8_t*>	m_pData;
	string			m_szName;
	string			m_szModifyDate;
	const YFileType	m_eFileType;

};

