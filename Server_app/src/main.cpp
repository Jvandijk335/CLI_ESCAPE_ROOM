#include <iostream>
#include <string>
#include <zmq.hpp>
#include <regex>
#include <poll.h>
#include <unistd.h>
#include "GameManager.h"

int main() {
    try {
        zmq::context_t context(1);
        zmq::socket_t subscriber(context, zmq::socket_type::sub);
        zmq::socket_t pusher(context, zmq::socket_type::push);

        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
        pusher.connect("tcp://benternet.pxl-ea-ict.be:24041");

        subscriber.set(zmq::sockopt::subscribe, "escape_room>commands>");

        std::cout << "Server running. Listening for messages..." << std::endl;

        GameManager gameManager;
        gameManager.initializeGame();

        int events;
        struct pollfd fds;
        int ret;

        while (subscriber && pusher) {
            events = subscriber.get(zmq::sockopt::events);
            if(events & ZMQ_POLLIN){
                zmq::message_t msg;
                if(subscriber.recv(msg, zmq::recv_flags::none))
                {
                    std::string received = msg.to_string();

                    // Match pattern: escape_room>commands>USERNAME?>COMMAND
                    std::regex pattern(R"(escape_room>commands>([^>]+)\?>(.*))");
                    std::smatch match;

                    if (std::regex_search(received, match, pattern) && match.size() >= 2) {
                        std::string username = match[1];
                        std::string command = match.size() > 2 ? std::string(match[2]) : "";
                        
                       
                        std::string response;
                        if (command.empty()) {
                            response = gameManager.handleMessage(username);
                        } else {
                            response = gameManager.handleMessage(username, command);
                        }
                        
                        std::string topic = "escape_room>commands>" + username + "!>" + response;
                        pusher.send(zmq::buffer(topic), zmq::send_flags::none);
                        std::cout << "Responded to " << username << std::endl;
                    }
                }
                else perror("recv");     
            }            
            
            
            fds.fd = STDIN_FILENO;
            fds.events = POLLIN;

            ret = poll(&fds, 1, 0);
            if (ret > 0 && (fds.revents & POLLIN)) {
                std::string input;
                std::getline(std::cin, input);

                if (input == "/exit") break;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
