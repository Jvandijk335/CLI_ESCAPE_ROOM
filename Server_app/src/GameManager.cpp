#include "GameManager.h"
#include "PlayerSession.h"
#include "Room.h"
#include <sstream>
#include <unordered_map>
#include <functional>
#include <iostream>


void GameManager::initializeGame() {
    // Create some sample rooms, puzzles, and items
    Room room1("Library", "A dusty library filled with old books.");
    Room room2("Kitchen", "A modern kitchen with stainless steel appliances.");
    
    // Room 1: Library with puzzles and items
    Puzzle puzzle1("Book_Puzzle", "Find the correct book to unlock a secret compartment.", 1998);
    Puzzle puzzle2("Code_Puzzle", "Solve the code to get the combination.", 1234);
    Item item1("Key", "A rusty old key.", true);
    Item item2("Book", "A strange looking book.", true);
    room1.puzzles.push_back(puzzle1);
    room1.puzzles.push_back(puzzle2);
    room1.items.push_back(item1);
    room1.items.push_back(item2);
    room1.hints.push_back("hint1");
    room1.hints.push_back("hint2");
    room1.hints.push_back("hint3");
    room1.hints.push_back("hint4");

    // Room 2: Kitchen with puzzles and items
    Puzzle puzzle3("Recipe_Puzzle", "Recreate the recipe to unlock a clue.", 42123);
    Item item3("Knife", "A sharp kitchen knife.", true);
    Item item4("Recipe Book", "A book of secret recipes.", true);
    room2.puzzles.push_back(puzzle3);
    room2.items.push_back(item3);
    room2.items.push_back(item4);
    room2.hints.push_back("hint1");
    room2.hints.push_back("hint2");
    room2.hints.push_back("hint3");
    room2.hints.push_back("hint4");
    
    // Add rooms to the template vector
    roomTemplates.push_back(room1);
    roomTemplates.push_back(room2);


    std::cout << "Game Initialized!" << std::endl;
}

std::string GameManager::handleMessage(const std::string& username) {
    std::cout << "Player connecting: " << username << std::endl;
    
    if (players.find(username) != players.end()) {
        return "Player already connected!";
    }

    Room startingRoom = roomTemplates.empty() ? Room("Default Room", "A default room.") : roomTemplates[0];
    players[username] = PlayerSession(username, startingRoom);
     

    std::string response = "Welcome " + username + "! You can now start interacting.";

    if (!startingRoom.describeRoom(players[username]).empty()) {
        response += "\n" + startingRoom.describeRoom(players[username]);
    } 
    else { 
        response += "\nNo rooms available at the moment.";
    }

    return response;
}

std::string GameManager::handleMessage(const std::string& username, const std::string& message) {
    if (players.find(username) == players.end()) {
        return "Error: Username not initialized!";
    }

    PlayerSession& session = players.find(username)->second;

    std::istringstream iss(message);
    std::string command;
    iss >> command;
    std::string args;
    std::getline(iss >> std::ws, args);

    std::unordered_map<std::string, std::function<std::string(const std::string&)>> commandHandlers;

    commandHandlers["move"] = [&](const std::string& args) {
        return session.move(args);
    };
    commandHandlers["hint"] = [&](const std::string&) {
        return session.getHint();
    };
    commandHandlers["status"] = [&](const std::string&) {
        return session.getStatus();
    };
    commandHandlers["joinchat"] = [&](const std::string&) {
        return session.joinChat();
    };
    commandHandlers["interact"] = [&](const std::string& args) {
        return session.executeAction("interact", args);
    };
    commandHandlers["search"] = [&](const std::string& args) {
        return session.executeAction("search", args);
    };
    commandHandlers["solve"] = [&](const std::string& args) {
        return session.executeAction("solve", args);
    };
    commandHandlers["say"] = [&](const std::string& args) {
        if(session.isInChat()) handleChatMessage(session, args);
        else return "You haven't joined the chat.";
        return  "";      
    };

    auto it = commandHandlers.find(command);
    if (it != commandHandlers.end()) {
        return it->second(args);
    }

    return "Error: Unknown command!";
}

void GameManager::setChatBroadcastCallback(std::function<void(const std::string& message)> callback) { 
    chatBroadcastCallback = std::move(callback);
}

void GameManager::handleChatMessage(const PlayerSession& session, const std::string& message) {
    std::string topic;
    std::string payload;
    for(const auto& player : players){
        if(player.second.isInChat() && player.first != session.getUsername()) {
            if(chatBroadcastCallback) {
                topic = "escape_room>chat>" + player.first + "!>";
                payload = session.getUsername() + ": " + message;
                chatBroadcastCallback(topic + payload);
            }
        }
    }
}
