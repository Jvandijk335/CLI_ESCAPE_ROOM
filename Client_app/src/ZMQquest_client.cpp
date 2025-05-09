#include <iostream>
#include <string>
#include <zmq.hpp>
#include <thread>
#include <future>
#include "zmq_addon.hpp"

#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

void PushThread(zmq::context_t *ctx, std::string username);
void SubscriberThread(zmq::context_t *ctx, std::string username);

int main(void)
{
    try
    {
        // Maak een ZeroMQ context
        zmq::context_t context(1);

        std::string username; 
        
        std::cout << "Enter your username: ";
        std::getline(std::cin, username);
        std::cout << "Hello " << username << std::endl;

        auto thread1 = std::async(std::launch::async, PushThread, &context, username);
        auto thread2 = std::async(std::launch::async, SubscriberThread, &context, username);

        thread1.wait();
        thread2.wait();
    }
    catch (zmq::error_t &ex)
    {
        std::cerr << "Caught an exception: " << ex.what() << std::endl;
    }

    return 0;
}

void PushThread(zmq::context_t *ctx, std::string username) { 
    // Prepare PUSH socket voor het versturen van berichten
    zmq::socket_t ventilator(*ctx, zmq::socket_type::push);

    int rc = -1;
    rc = ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");
    assert(rc == 0);   
    std::cout << "Connected to tcp://benternet.pxl-ea-ict.be:24041" << std::endl;

    // PUSH bericht sturen
    std::string message = "EscapeRoom>Room?>" + username + ">"; //example>quest?>Joep van Dijk>  EscapeRoom>Room?>
    ventilator.send(zmq::buffer(message), zmq::send_flags::dontwait);
    std::cout << "Pushed: [" << message << "]" << std::endl;
}

void SubscriberThread(zmq::context_t *ctx, std::string username) {
    //  Prepare SUBSCRIBE socket voor het ontvangen van berichten
    zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);

    int rc = -1;
    subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
    assert(rc == 0);   
    std::cout << "Thread Connected to tcp://benternet.pxl-ea-ict.be:24042" << std::endl;

    //  Thread opens "EscapeRoom>Room!>*username>" envelopes
    std::string sub = "EscapeRoom>Room!>" + username + ">";
    subscriber.set(zmq::sockopt::subscribe, sub);

    while (subscriber.handle() != nullptr ) {
        // Receive all parts of the message
        std::vector<zmq::message_t> recv_msgs;
        zmq::recv_result_t result =
          zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
        assert(result && "recv failed");
        assert(*result == 2);

        std::cout << "Thread2: [" << recv_msgs[0].to_string() << "] " << recv_msgs[1].to_string() << std::endl;
    }
}