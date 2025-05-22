#pragma once
#include <string>
#include <chrono>
#include <optional>
#include "Room.h"

class PlayerSession {
public:
    PlayerSession();
    PlayerSession(const std::string& username, const Room& room);
    std::string getHint();
    std::string getStatus();
    std::string executeAction(const std::string& action, const std::string& target);
    std::string move(const std::string& direction);
    std::string joinChat();
    bool isInChat() const { return inChat; }

private:
    std::string searchAction(const std::string& args);    
    std::string interactAction(const std::string& args);
    std::string solveAction(const std::string& args);

    std::string username;
    std::optional<Room> personalRoom;
    bool inChat;
    std::chrono::system_clock::time_point lastActivityTime;
};