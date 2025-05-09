#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include "Puzzle.h"
#include "Item.h"
#include "PlayerSession.h"

class Room {
public:
    std::string describeRoom(const PlayerSession& session);
    std::string interact(PlayerSession& session, const std::string& action);

private:
    std::string name;
    std::string description;
    std::vector<Puzzle> puzzles;
    std::vector<Item> items;
    std::vector<std::string> connectedPlayers;
};

#endif