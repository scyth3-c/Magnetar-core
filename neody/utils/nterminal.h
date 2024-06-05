//
// Created by scythe on 6/4/24.
//
#ifndef LITERAL_H
#define LITERAL_H
#include <iostream>

constexpr auto ND_NVALUE = -1;
constexpr auto ND_OK = 0;

constexpr auto ND_EPOLL_RANGE =  "AN ERROR OCCURRED WHEN CREATION OF THE EPOLL_FD FILE DESCRIPTOR";
constexpr auto ND_EPOLL_CTL =    "AN ERROR OCCURRED WHEN EXECUTING EPOLL_CTL";
constexpr auto ND_EPOLL_CERR =   "THE FOLLOWING ERROR WAS FOUND WHEN EXECUTING THE EPOLL METHOD: ";

constexpr auto ND_SOCKET_FAIL =  "A PROBLEM OCCURRED WHEN READING THE SOCKET REQUEST, CHECK THE CONNECTION";
constexpr auto ND_SOCKET_CLOSE = "A PROBLEM OCCURRED WHEN TRYING TO CLOSE THE CONNECTION WITH THE SOCKET: normally it is due to an error in the previous code";

constexpr auto ND_MAIN_THREAD =  "AN ERROR OCCURRED IN THE MAIN PROCESSING THREAD";


template<class...P>
auto terminal(P const&... args) {
    ((std::cout<<"[ "<<args<<"]"<<std::endl),...);
}




#endif //LITERAL_H
