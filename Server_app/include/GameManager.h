#pragma once
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include "Room.h"
#include "PlayerSession.h"

class GameManager {
public:
    void initializeGame();
    std::string handleMessage(const std::string &username);
    std::string handleMessage(const std::string &username, const std::string &message);
    void setChatBroadcastCallback(std::function<void(const std::string& message)> callback);

private:
    void handleChatMessage(const PlayerSession& session, const std::string& message);
    std::vector<Room> roomTemplates;
    std::unordered_map<std::string, PlayerSession> players;
    std::function<void(const std::string&)> chatBroadcastCallback;
};