#include "PlayerSession.h"

PlayerSession::PlayerSession() 
    : username(""), lastActivityTime(std::chrono::system_clock::now()) {
}

PlayerSession::PlayerSession(const std::string& username, const Room& room) 
    : username(username), personalRoom(room), lastActivityTime(std::chrono::system_clock::now()) {
}

std::string PlayerSession::processCommand(const std::string& command) {
    return "Command processed: " + command;
}

std::string PlayerSession::getHint() {
    return "Here's your hint.";
}

std::string PlayerSession::getStatus() {
    return "Player status: Active";
}

std::string PlayerSession::executeAction(const std::string& action, const std::string& target) {
    return "Executed action: " + action + " on " + target;
}

std::string PlayerSession::move(const std::string& direction) {
    return "Moved to: " + direction;
}

std::string PlayerSession::getChatTopic() {
    return "Chat topic for this room.";
}
