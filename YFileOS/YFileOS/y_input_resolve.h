#pragma once
#include <string>
#include <vector>
struct YCommandInfo
{
	std::string szCommandName;
	std::vector<std::string> rArglist;

};
class YInputResolve
{
public:
	YInputResolve();
	~YInputResolve();

	YCommandInfo resolve(const std::string& szInput);

private:

};

