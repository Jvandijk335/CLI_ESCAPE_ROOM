#include <iostream>
#include <string.h>
#include <zmq.hpp>
#include <poll.h>
#include <unistd.h>
#ifndef _WIN32
	#include <unistd.h>
#else
	#include <windows.h>
	#define sleep(n)    Sleep(n)
#endif

int main( void )
{
    try
    {
        zmq::context_t context(1);
        std::string username;

        std::cout << "Enter your username: ";
        std::getline(std::cin, username);
        std::cout << "Hello " << username << std::endl;

        zmq::socket_t ventilator(context, zmq::socket_type::push);
        zmq::socket_t subscriber(context, zmq::socket_type::sub);
        
        ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");
        std::cout << "Connected to tcp://benternet.pxl-ea-ict.be:24041" << std::endl;
        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
        std::cout << "Connected to tcp://benternet.pxl-ea-ict.be:24042" << std::endl;

        std::string sub = "escape_room>commands>" + username + "!>";
        subscriber.set(zmq::sockopt::subscribe, sub);

        std::string message = "escape_room>commands>" + username + "?>";
        ventilator.send(zmq::buffer(message), zmq::send_flags::dontwait);
        std::cout << "Send Username: [" << username << "]" << std::endl;

        struct pollfd fds;
        int events;
        int ret;

        while(subscriber && ventilator){
            events = subscriber.get(zmq::sockopt::events);
            if(events & ZMQ_POLLIN){
                zmq::message_t msg;
                if(subscriber.recv(msg, zmq::recv_flags::dontwait)) 
                {
                    std::string full_msg = msg.to_string();
                    std::string delimiter = "!>";
                    std::size_t pos = full_msg.find(delimiter);
                    if (pos != std::string::npos && pos + delimiter.length() < full_msg.length()) {
                        std::string content = full_msg.substr(pos + delimiter.length());
                        std::cout << "Subscriber: " << content << std::endl;
                    } else {
                        std::cout << "Malformed message or no content after '!>'" << std::endl;
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

                std::string msg = "escape_room>commands>" + username + "?>" + input;
                ventilator.send(zmq::buffer(msg), zmq::send_flags::dontwait);
                //std::cout << "Sent: [" << msg << "]" << std::endl;
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}

