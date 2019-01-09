#pragma once
#include <string>


class YConsole
{
public:
	YConsole();
	~YConsole();
	void log(std::string& str);
	void logLine(std::string& str);

	std::string& getInputStr();
private:
	std::string   m_szCurPath;
};

