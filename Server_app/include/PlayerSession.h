#pragma once
#include <string>
#include <chrono>
#include <optional>
#include "Room.h"

class PlayerSession {
public:
    PlayerSession();
    PlayerSession(const std::string& username, const Room& room);
    std::string processCommand(const std::string& command);
    std::string getHint();
    std::string getStatus();
    std::string executeAction(const std::string& action, const std::string& target);
    std::string move(const std::string& direction);
    std::string getChatTopic();

private:
    std::string username;
    std::optional<Room> personalRoom;
    std::chrono::system_clock::time_point lastActivityTime;
};