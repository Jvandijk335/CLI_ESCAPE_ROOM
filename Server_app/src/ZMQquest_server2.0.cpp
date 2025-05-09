#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <zmq.hpp>
#include <unordered_map>
#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n) Sleep(n)
#endif

void EscapeRoomThread(zmq::context_t *ctx, const std::string& roomTopic, const std::string& username);

int main() {
    try {
        zmq::context_t context(1);
        zmq::socket_t subscriber(context, ZMQ_SUB);
        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");

        subscriber.set(zmq::sockopt::subscribe, "EscapeRoom>Room?>");
        //subscriber.setsockopt(ZMQ_SUBSCRIBE, "EscapeRoom>Room?>", 17); // Subscribe for room creation messages

        std::unordered_map<std::string, std::thread> roomThreads; // Store threads for each room

        while (true) {
            zmq::message_t msg;
            if (subscriber.recv(msg)) {
                std::string receivedMsg((char*)msg.data(), msg.size());
                std::cout << "Received: " << receivedMsg << std::endl;

                // Parse the received message
                size_t pos1 = receivedMsg.find("EscapeRoom>Room?>");
                if (pos1 != std::string::npos) {
                    std::string roomInfo = receivedMsg.substr(pos1 + 17); // Extract room information
                    size_t pos2 = roomInfo.find('>');
                    if (pos2 != std::string::npos) {
                        std::string username = roomInfo.substr(pos2 + 1);
                        std::string roomTopic = "EscapeRoom>Room!>" + username + ">";

                        // Check if a thread for this room already exists
                        if (roomThreads.find(roomTopic) == roomThreads.end()) {
                            // Create a new thread for the room
                            roomThreads[roomTopic] = std::thread(EscapeRoomThread, &context, roomTopic, username);
                            std::cout << "Started new thread for room: " << roomTopic << std::endl;
                        } else {
                            std::cout << "Thread for room " << roomTopic << " already exists." << std::endl;
                        }
                    } else {
                        std::cerr << "Invalid room creation message format." << std::endl;
                    }
                }
            }
        }
    } catch (zmq::error_t& ex) {
        std::cerr << "Caught an exception: " << ex.what() << std::endl;
    }
    return 0;
}

void EscapeRoomThread(zmq::context_t *ctx, const std::string& roomTopic, const std::string& username) {
    zmq::socket_t subscriber(*ctx, ZMQ_SUB);
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, roomTopic.c_str(), roomTopic.length());

    zmq::socket_t ventilator(*ctx, ZMQ_PUSH);
    ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");

    std::cout << "Thread for room " << roomTopic << " started." << std::endl;

    while (subscriber.connected()) {
        zmq::message_t msg;
        if (subscriber.recv(msg)) {
            std::string receivedMsg((char*)msg.data(), msg.size());
            std::cout << "Room " << roomTopic << " received: " << receivedMsg << std::endl;

            // Process the message and send a response if needed
            std::string response = "EscapeRoom>Room?>" + username + ">Response from room";
            ventilator.send(response.c_str(), response.length());
            std::cout << "Room " << roomTopic << " sent: " << response << std::endl;
        }
    }

    std::cout << "Thread for room " << roomTopic << " finished." << std::endl;
}