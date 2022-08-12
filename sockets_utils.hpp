#ifndef SOCKETS_UTILS_HPP
#define SOCKETS_UTILS_HPP


#include <iostream>


constexpr auto DEFAULT_t = "noll";
constexpr auto SONAR_ERROR = -1;
constexpr auto SONAR_OK = 1;

constexpr auto OPT = -2;
constexpr auto BIND = -3;
constexpr auto LISTEN = -4;
constexpr auto LAUNCH = -5;

struct _terminal{

template<class... P>
constexpr static void show(std::ostream& os, P const&... args )
{
     ((os<<args), ...);
}

};


constexpr int socketError(int type) noexcept {
    const char* msg{};
    switch (type)
    {
    case -1:
     msg = "crear";
        break;
    case -2:
     msg = "enlazar";
        break;
    case -3:
     msg = "escuchar el puerto en";
        break;    
    case -4:
    msg = "lanzar"; 
    break;
    default:
        break;
    }
    _terminal::show(std::clog, "Error al " , msg, " el socket!");
    return type;
}



#endif // !SOCKETS_UTILS_HPP
