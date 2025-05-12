#include "Player.h"
#include <iostream>

void Player::sendCommand(const std::string& command) {
    std::cout << "Sending command: " << command << std::endl;
}

void Player::move(const std::string& direction) {
    std::cout << username << " moves " << direction << std::endl;
}

void Player::interact(const std::string& target) {
    std::cout << username << " interacts with " << target << std::endl;
}

void Player::inspect(const std::string& object) {
    std::cout << username << " inspects " << object << std::endl;
}

void Player::manageInventory(const std::string& action, const std::string& item) {
    std::cout << username << " performs " << action << " on " << item << std::endl;
}

void Player::requestHelp() {
    std::cout << username << " requests help" << std::endl;
}

void Player::lookAround() {
    std::cout << username << " looks around" << std::endl;
}

std::string Player::joinChatRoom() {
    return "Joined chat room: " + currentRoom;
}