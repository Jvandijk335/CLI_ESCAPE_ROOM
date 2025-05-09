#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
public:
    // Constructor
    Player(const std::string& username);

    // Acties die de speler kan uitvoeren
    void sendCommand(const std::string& command);
    void move(const std::string& direction);
    void interact(const std::string& target);
    void inspect(const std::string& object);
    void manageInventory(const std::string& action, const std::string& item);
    void requestHelp();
    void lookAround();
    std::string joinChatRoom();

private:
    std::string username;
    std::string currentRoom;
};

#endif
