#pragma once
#include "command.h"

class Loadcommand : public Command
{
public:
	Loadcommand(std::vector<std::string>& Arglist);
	virtual ~Loadcommand();
	virtual ErrorCode	Run();
	bool				SecondAsk();
	ErrorCode			ResolveRealPath(std::string& ResultPath);
private:

};

