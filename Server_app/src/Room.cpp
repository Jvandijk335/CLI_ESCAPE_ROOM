#include "Room.h"

Room::Room(const std::string& name, const std::string& description) 
    : name(name), description(description) {
}

std::string Room::describeRoom(const PlayerSession& session) {
    return "Room description: " + description;
}

std::string Room::interact(PlayerSession& session, const std::string& action) {
    return "Interacted with action: " + action;
}
