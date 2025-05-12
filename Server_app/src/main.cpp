#include <iostream>
#include <string>
#include <zmq.hpp>
#include <regex>
#include <poll.h>
#include <unistd.h>

int main() {
    try {
        zmq::context_t context(1);
        zmq::socket_t subscriber(context, zmq::socket_type::sub);
        zmq::socket_t pusher(context, zmq::socket_type::push);

        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
        pusher.connect("tcp://benternet.pxl-ea-ict.be:24041");

        subscriber.set(zmq::sockopt::subscribe, "escape_room>commands>");

        std::cout << "Server running. Listening for messages..." << std::endl;

        while (subscriber && pusher) {
            int events = subscriber.get(zmq::sockopt::events);
            if(events & ZMQ_POLLIN){
                zmq::message_t msg;
                if(subscriber.recv(msg, zmq::recv_flags::none))
                {
                    std::string received = msg.to_string();
                    std::cout << "Received: " << received << std::endl;

                    // Match pattern: escape_room>commands>USERNAME?>...
                    std::regex pattern(R"(escape_room>commands>([^>]+)\?>)");
                    std::smatch match;

                    if (std::regex_search(received, match, pattern) && match.size() > 1) {
                        std::string username = match[1];
                        std::string response = "escape_room>commands>" + username + "!>Hello " + username + ", your message was received.";
                        
                        pusher.send(zmq::buffer(response), zmq::send_flags::none);
                        std::cout << "Responded to " << username << std::endl;
                    }
                }
                else perror("recv");     
            }
            
            
            struct pollfd fds;
            fds.fd = STDIN_FILENO;
            fds.events = POLLIN;

            int ret = poll(&fds, 1, 0); // timeout = 0 (non-blocking)
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
