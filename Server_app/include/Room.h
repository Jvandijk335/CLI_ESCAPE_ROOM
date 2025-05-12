#pragma once
#include <string>
#include <vector>
#include "Puzzle.h"
#include "Item.h"

class PlayerSession;

class Room {
public:
    Room(const std::string& name, const std::string& description);
    std::string describeRoom(const PlayerSession& session);
    std::string interact(PlayerSession& session, const std::string& action);

    std::vector<Puzzle> puzzles;
    std::vector<Item> items;

private:
    std::string name;
    std::string description;
};