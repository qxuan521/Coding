#pragma once
#include <fstream>
#include "y_error_code.h"
class YFileStream
{
public:
	YFileStream();
	~YFileStream();
	YErrorCode	readDataByPath(const std::string& szPath, int8_t* data, uint32_t size);
	YErrorCode  writeDataByPath(const std::string& szPath, int8_t* data, uint32_t size);
	bool		isFolderPath(const std::string& szPath);
	bool		isPathExsit(const std::string& szPath);
private:
};