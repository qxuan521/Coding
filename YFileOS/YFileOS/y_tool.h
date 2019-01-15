#pragma once
#include <string>
#include <vector>
#include <regex>
std::vector<std::string>	splitStrByCharacter(const std::string& srcStr, char spliter);
std::string					getParentPath(const std::string& szPath);
std::string					getNameFromFullPath(const std::string& szPath);
std::string					getPathFromRealPath(const std::string& szPath);
bool						isRealPath(const std::string& szPath);
bool						isHaveWildCard(const std::string& szPath);
std::regex					makeRegexByPath(const std::string & szPath); 
std::regex					makeRepaceRegexByPath(const std::string& szPath , std::string& szRepaceStr);
