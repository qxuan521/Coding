#pragma once
#include <string>
#include <vector>

std::vector<std::string>	splitStrByCharacter(const std::string& srcStr, char spliter);
std::string					getParentPath(const std::string& szPath);
std::string					getNameFromFullPath(const std::string& szPath);
bool						isRealPath(const std::string& szPath);