/*===========================================
=MessageSystem.h/.cpp						=
=by Tyler van Gastel						=
=latest update 12/1/2015					=
=message manager mostly for button functions=
=============================================*/
#pragma once
#include <string>
#include <vector>

class MessageSystem {
private:
	std::vector<std::string> messages;
public:
	MessageSystem();
	void clear();
	void post(std::string message);
	bool getMessage(std::string message);
};