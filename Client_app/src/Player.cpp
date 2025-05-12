#include "Player.h"
#include <iostream>

Player::Player(const std::string& username) : username(username), currentRoom("StartRoom") {}

void Player::sendCommand(const std::string& command) {
    std::cout << "[SEND] Command: " << command << std::endl;
}

void Player::move(const std::string& direction) {
    std::cout << username << " moves " << direction << "." << std::endl;
    sendCommand("move " + direction);
}

void Player::interact(const std::string& target) {
    std::cout << username << " interacts with " << target << "." << std::endl;
    sendCommand("interact " + target);
}

void Player::inspect(const std::string& object) {
    std::cout << username << " inspects " << object << "." << std::endl;
    sendCommand("inspect " + object);
}

void Player::manageInventory(const std::string& action, const std::string& item) {
    std::cout << username << " performs '" << action << "' on item: " << item << std::endl;
    sendCommand("inventory " + action + " " + item);
}

void Player::requestHelp() {
    std::cout << username << " requests help." << std::endl;
    sendCommand("help");
}

void Player::lookAround() {
    std::cout << username << " looks around." << std::endl;
    sendCommand("look");
}

std::string Player::joinChatRoom() {
    std::string room = "GeneralChatRoom";
    std::cout << username << " joins chat room: " << room << std::endl;
    return room;
}
