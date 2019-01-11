#pragma once
#include <vector>
#include "y_error_code.h"
class YDisk;
class YIFile;
class YDiskOperator
{
public:
	YDiskOperator();
	~YDiskOperator();

	YErrorCode createNewFile(const std::string& szPathArr,YIFile*& pResult);


//Verify
	bool	isRootName(const std::string& szName);
	bool	isPathExist(const std::string& szPath);
private:
	YDisk*		m_pDisk;
};

extern YDiskOperator*  g_pDiskOperator;