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
        std::cout << "Pushed: [" << message << "]" << std::endl;

        while(subscriber && ventilator){
            int events = subscriber.get(zmq::sockopt::events);
            if(events & ZMQ_POLLIN){
                zmq::message_t msg;
                if(subscriber.recv(msg, zmq::recv_flags::dontwait)) 
                {
                    std::cout << "Subscribed : [" << msg.to_string() << "]" << std::endl;
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

                std::string msg = "escape_room>commands>" + username + "?>" + input;
                ventilator.send(zmq::buffer(msg), zmq::send_flags::dontwait);
                std::cout << "Sent: [" << msg << "]" << std::endl;
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}

