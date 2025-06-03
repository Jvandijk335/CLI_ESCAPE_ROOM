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
    std::string getHint();

    std::vector<Puzzle> puzzles;
    std::vector<Item> items;
    std::vector<std::string> hints;

private:
    std::string name;
    std::string description;
    int currentHintIndex = 0;
};