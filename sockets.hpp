#ifndef SOCKETS_HPP
#define SOCKETS_HPP

#include <netinet/in.h>
#include <sys/socket.h>

#include <string>
#include <memory>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "sockets_utils.hpp"

using std::string,
    std::shared_ptr,
    std::make_shared;

class sonar_socket
{
public:
    explicit sonar_socket(uint16_t _port = 0)
    {
        if (_port != 0)
            PORT = make_shared<uint16_t>(_port);
    }
    ~sonar_socket()
    {
        PORT.reset();
    }
    int create(int _domain = AF_INET, int type = SOCK_STREAM, int protocol = 0);
   [[noreturn]] inline void setPort(int _port)
    {
        PORT.reset();
        PORT = make_shared<uint16_t>(_port);
    }
    inline void close() { shutdown(socket_id, SHUT_RDWR); }
    inline void setBuffer(int _max) noexcept { buffer_size = _max; }

protected:
    struct sockaddr_in address;
    shared_ptr<uint16_t> PORT = nullptr;
    int socket_id{},
        new_socket{},
        state_receptor{},
        option_name{1},
        address_lenght{sizeof(address)};
    int buffer_size{2048};
};

class Server : public sonar_socket
{
private:
    int sesions{1};
    string data;

public:
    Server() {}
    Server(uint16_t _port) : sonar_socket(_port) {}

    int on();
    char* getResponse();
    void send_data(string conten, int flags = 0);
    inline void setSesions(int _max) { sesions = _max; }
};

class Client : sonar_socket
{
private:
public:
    Client(uint16_t _port) : sonar_socket(_port) {}
};

int sonar_socket::create(int _domain, int type, int protocol)
{
    int res = socket_id = socket(_domain, type, protocol);
    if (res == 0)
    {
        _terminal::show(std::cout, "el socket fallo al crearse");
        return -1;
    }
    return SONAR_OK;
}

int Server::on()
{
     

    int socket_see = setsockopt(socket_id, SOL_SOCKET,
                                SO_REUSEADDR | SO_REUSEPORT, &option_name,
                                sizeof(option_name));
    if (socket_see)
        return socketError(-1);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(*PORT);

bind(socket_id, (struct sockaddr *)&address, sizeof(address));
    // if (socket_see < 0)
    //     return socketError(-2);

listen(socket_id, sesions);
    // if (socket_see < 0)
    //     return socketError(-3);
 new_socket = accept(socket_id, (struct sockaddr *)&address, (socklen_t *)&address_lenght);
    // if (socket_see < 0)
    //     return socketError(-4);
    return SONAR_OK;
}

char* Server::getResponse()
{   
    char *buffer = new char[buffer_size];
    state_receptor = read(new_socket, buffer, buffer_size);
    return buffer;
}

void Server::send_data(string _msg, int flags){
    auto conten = (char *)_msg.c_str();
    try
    {
        send(new_socket, conten, _msg.size(), flags);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

#endif // !SOCKETS