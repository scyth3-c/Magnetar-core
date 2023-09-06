#ifndef MAGICAL_SOCKETS_HPP
#define MAGICAL_SOCKETS_HPP


#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdexcept>


#include <memory>
#include <unistd.h>
#include <string>
#include <cstring>
#include <functional>

#include <iostream>
#include <vector>
#include <mutex>

using std::string;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::function;

constexpr uint16_t DEFAULT_PORT = 0xBB8;

constexpr int DOMAIN = AF_INET;
constexpr int TYPE = SOCK_STREAM;
constexpr int PROTOCOL = 0;

constexpr int MG_ERROR = -0x1;
constexpr int MG_OK = 0x0;
[[maybe_unused]] constexpr int MG_CONFUSED = 0x1;

constexpr int DEF_BUFFER_SIZE = 0x400;
constexpr int UnCATCH_ERROR_CH = -0x42;

[[maybe_unused]] constexpr const char* SOCK_ERR = "_ERROR";

class Engine {

   protected:

        std::mutex lock_guard;
        std::mutex response_guard;
        uint16_t PORT;
        shared_ptr 
                   <int> 
                         socket_id = nullptr,
                         state_receptor = nullptr,
                         address_len = make_shared<int>(sizeof(address)),
                         option_mame = make_shared<int>(0x1),
                         buffer_size = make_shared<int>(DEF_BUFFER_SIZE);
    public:

        struct sockaddr_in address{};
        explicit Engine(uint16_t);
        virtual ~Engine() = default;


    [[maybe_unused]] int
             setBuffer(int),
             setPort(uint16_t),
             getPort() const;

        virtual int on() = 0;
        virtual void getResponseProcessing() = 0;

        [[maybe_unused]] virtual int Close() = 0;
        [[nodiscard]] virtual string getResponse() const = 0;
};


class Server : public Engine {
  private:

     shared_ptr<string> buffereOd_data;
     shared_ptr
               <int> static_sessions = make_shared<int>(10);

  public:
     
     explicit Server(uint16_t Port) : Engine(Port){}
     Server() : Engine(DEFAULT_PORT){}


    void getResponseProcessing() override;
     int on() override;
     int Close() override;

    [[maybe_unused]] inline int getDescription() {  return *socket_id;  }
    [[maybe_unused]] inline shared_ptr<int> getSocketId() { return socket_id; }
    [[maybe_unused]] inline void setSocketId(int identity) { socket_id.reset(new int(identity)); }

     void setSessions(int);
     void sendResponse(const string&);

     static int setNonblocking(const int&);

     [[nodiscard]] inline string getResponse()  const override {
           return *buffereOd_data;
      }
};


[[maybe_unused]] constexpr const char* const HTML = "text/html; charset=utf-8 ";
constexpr const char* JSON = "application/json ";

struct [[maybe_unused]] WEB {

    explicit WEB()= default;

    [[maybe_unused]] static string json(const string& _txt, const string& status="200 OK") {
      return       "HTTP/1.1 "+status+"\n"
                   "Server: Neody/0.5\n"
                   "Content-Type: "+ JSON  +"\n"
                   "Content-Length: " + std::to_string(_txt.length()) + "\n"
                   "Accept-Ranges: bytes\n" +
                   "Connection: close\n"
                   "\n" +
                   _txt;
     }


    [[maybe_unused]] static string custom(const string& _txt, const string& type, const string& headers,  const string& status="200 OK"){
           return  "HTTP/1.1 "+status+"\n"
                   "Server: Neody/0.5\n"
                   "Content-Type: "+type+"\n"
                   "Content-Length: " + std::to_string(_txt.length()) + "\n"
                   "Accept-Ranges: bytes\n" +
                    headers
                    +
                   "Connection: close\n"
                   "\n" +
                   _txt;
     }     
};

template<class...P>
struct HEADERS_MG {
    HEADERS_MG()= default;
    [[maybe_unused]] HEADERS_MG(std::initializer_list<P...>list): body(list) {}
    vector<string> body;
    [[maybe_unused]] string generate(){
        string response;
        for (auto &it : body) {
            response += it + "\n";
        }
        return response;
    }
};

[[maybe_unused]] typedef HEADERS_MG<string> Headers;


[[maybe_unused]] constexpr const char* const HTTP_ERROR = "HTTP/1.1 400 BAD\n"
                           "Server: Neody/0.5\n"
                           "Content-Type: application/json\n"
                           "Content-Length: 25\n"
                           "Accept-Ranges: bytes\n"
                           "Connection: close\n"
                           "\n"
                           "error!";


#endif // !MAGICAL_SOCKETS_HPP