#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <unordered_map>
#include <thread>
#include <string>
#include "GameManager.h"

class GameServer {
public:
    void handlePlayerConnect(const std::string& username);

private:
    GameManager gameLogic;
    std::unordered_map<std::string, std::thread> clientThreads;
};

#endif