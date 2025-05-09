#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <vector>
#include <unordered_map>
#include <string>
#include "Room.h"
#include "PlayerSession.h"

class GameManager {
public:
    void initializeGame();
    void movePlayer(PlayerSession& session, const std::string& direction);
    std::string handleAction(PlayerSession& session, const std::string& action, const std::string& target);
    std::string getHint(PlayerSession& session);
    std::string getStatus(PlayerSession& session);
    std::string getChatTopic(PlayerSession& session);

private:
    std::vector<Room> roomTemplates;
    std::unordered_map<std::string, PlayerSession> players;
};

#endif