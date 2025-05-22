#include "PlayerSession.h"
#include <sstream>
#include <unordered_map>
#include <functional>

PlayerSession::PlayerSession() 
    : username(""), inChat(false),lastActivityTime(std::chrono::system_clock::now()) {
}

PlayerSession::PlayerSession(const std::string& username, const Room& room) 
    : username(username), personalRoom(room), inChat(false), lastActivityTime(std::chrono::system_clock::now()) {
}

std::string PlayerSession::getHint() {
    return "Here's your hint.";
}

std::string PlayerSession::getStatus() {
    auto now = std::chrono::system_clock::now();
    auto  elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastActivityTime);

    int total_seconds = elapsed.count();
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;

    return "Player status: Active\r\nUsername: " 
        + username + "\r\n" +
        "Duration: " + 
        std::to_string(hours) + "h " +
        std::to_string(minutes) + "m " +
        std::to_string(seconds) + "s ";
}



std::string PlayerSession::move(const std::string& direction) {
    return "Moved to: " + direction;
}

std::string PlayerSession::joinChat(){
    inChat = true;
    return "Joined Chatroom";
}

std::string PlayerSession::executeAction(const std::string& action, const std::string& args) {
    
    std::unordered_map<std::string, std::function<std::string(const std::string&)>> ActionHandler;

    ActionHandler["search"] = [&](const std::string& args){
        return this->searchAction(args);
    };
    ActionHandler["interact"] = [&](const std::string& args){
        return this->interactAction(args);
    };
    ActionHandler["solve"] = [&](const std::string& args){
        return this->solveAction(args);
    };

    auto it = ActionHandler.find(action);
    if (it != ActionHandler.end()) {
        return it->second(args);
    }

    return "Error: Can't execute this action!";
}

std::string PlayerSession::searchAction(const std::string& args) {
    std::ostringstream result;

    std::istringstream iss(args);
    std::string target;
    iss >> target;    
        
    if(target == "room"){
        if(!personalRoom.has_value()) {
        return "You are not in a room.";
        }  

        const auto& items = personalRoom->items;
        const auto& puzzles = personalRoom->puzzles;
        if(items.empty() && puzzles.empty()) {
            return "The room is empty.";
        }

        result << "You found:\n";

        for(const auto& item : items) {
            result << "- " << item.getName() << "\n";
        }

        for(const auto& puzzle : puzzles) { 
            result << "- " << puzzle.getName() << "\n";
        }

    } else {
        result << "can't search that";
    }

    return result.str();
}

std::string PlayerSession::interactAction(const std::string& target) {
    return "interact with " + target;
}

std::string PlayerSession::solveAction(const std::string& args) {
    std::istringstream iss(args);
    std::string target;
    iss >> target;
    std::string solution;
    std::getline(iss, solution);

    for(auto& puzzle : personalRoom->puzzles) {
        if(puzzle.getName() == target)
        {
            if(puzzle.attemptSolution(solution)){
                return "correct! The puzzle is solved.";
            } else {
                return "Incorrect solution";
            }
        }
    }
    return "Can't solve this item.";
}
