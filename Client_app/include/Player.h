#pragma once
#include <string>

class Player {
public:
    std::string username;
    std::string currentRoom;

    void sendCommand(const std::string& command);
    void move(const std::string& direction);
    void interact(const std::string& target);
    void inspect(const std::string& object);
    void manageInventory(const std::string& action, const std::string& item);
    void requestHelp();
    void lookAround();
    std::string joinChatRoom();
};