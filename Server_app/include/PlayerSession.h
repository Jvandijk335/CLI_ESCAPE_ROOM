#ifndef PLAYERSESSION_H
#define PLAYERSESSION_H

#include <string>
#include <chrono>
#include "Room.h"

class PlayerSession {
public:
    std::string processCommand(const std::string& command);
    std::string getHint();
    std::string getStatus();
    std::string executeAction(const std::string& action, const std::string& target);

private:
    std::string username;
    Room personalRoom;
    std::chrono::system_clock::time_point lastActivityTime;
};

#endif