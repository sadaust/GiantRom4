#include "MessageSystem.h"

MessageSystem::MessageSystem() {
}

void MessageSystem::post(std::string message) {
	for(int i = 0; i < messages.size(); ++i) {
		if(messages[i] == message)
			return;
	}
	messages.push_back(message);
}

bool MessageSystem::getMessage(std::string message) {
	for(int i = 0; i <messages.size(); ++i) {
		if(messages[i] == message)
			return true;
	}
	return false;
}

void MessageSystem::clear() {
	messages.clear();
}