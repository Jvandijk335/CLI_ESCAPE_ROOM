#include <iostream>
#include <string>
#include <zmq.hpp>
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

        // PUSH socket voor het versturen van berichten
        zmq::socket_t ventilator(context, ZMQ_PUSH);
        ventilator.connect("tcp://benternet.pxl-ea-ict.be:24041");
        std::cout << "Connected to tcp://benternet.pxl-ea-ict.be:24041" << std::endl;

        // SUBSCRIBE socket voor het ontvangen van berichten
        std::string sub = "EscapeRoom>Room?>";

        zmq::socket_t subscriber(context, ZMQ_SUB);
        subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042");
        subscriber.setsockopt(ZMQ_SUBSCRIBE, sub.c_str(), sub.length() );  // Subscribe voor alle berichten
        std::cout << "Connected to tcp://benternet.pxl-ea-ict.be:24042" << std::endl;

        zmq::message_t msg;  // Voor het ontvangen van berichten

        while( subscriber.connected() )
        {
            if (subscriber.recv(&msg))
            {
                // SUBSCRIBE berichten ontvangen
                std::cout << "Subscribed: [" << std::string((char*)msg.data(), msg.size()) << "]" << std::endl;
                // PUSH bericht sturen
                std::string message = "EscapeRoom>Room!>Joep van Dijk>"; 
                ventilator.send(message.c_str(), message.length());
                std::cout << "Pushed: [" << message << "]" << std::endl;
            }
        }
    }
    catch (zmq::error_t &ex)
    {
        std::cerr << "Caught an exception: " << ex.what() << std::endl;
    }

    return 0;
}