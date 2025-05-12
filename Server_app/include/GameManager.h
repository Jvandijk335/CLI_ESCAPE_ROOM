#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Room.h"
#include "PlayerSession.h"

class GameManager {
public:
    void initializeGame();
    std::string handleMessage(const std::string &username);
    std::string handleMessage(const std::string &username, const std::string &message);

private:
    std::vector<Room> roomTemplates;
    std::unordered_map<std::string, PlayerSession> players;
};