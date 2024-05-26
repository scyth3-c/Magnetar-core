#include "mgsockets.h"

Engine::Engine(uint16_t _xport) : PORT(_xport) {}

int Server::Close() {
     try {   

          if (close(*socket_id) < 0) {
               throw std::range_error("Fallo al cerrar el socket, Server");
          }
          return MG_OK;
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return MG_ERROR;
     }
}

int Engine::setPort(uint16_t xPort) {
     try {
          PORT =  xPort;
          if(!PORT) throw std::range_error("error al asignar el puerto");
          return MG_OK;
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return MG_ERROR;
     }
}

int Engine::getPort() const {
     try {
          if (PORT > 0)  {
               return PORT;
          }
          else {
               throw std::range_error("error al intentar obtener el puerto");
          }
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return MG_ERROR;
     }
}



int Engine::setBuffer(int _tamx) {
     try {
          if (buffer_size == nullptr) {
               buffer_size = make_shared<int>(_tamx);
          }
          buffer_size.reset(new int(_tamx));
          if(*buffer_size != _tamx) throw std::range_error("error al intentar asignar el buffer");
          return MG_OK;
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return MG_ERROR;
     }
}



void Server::setSessions(int max) {
     try {
          if (static_sessions == nullptr) {
               static_sessions = make_shared<int>(max);
               return;
          }
          static_sessions.reset(new int(max));
          if(*static_sessions != max) throw std::range_error("error al asignar las sesiones");
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
     }
}

int Server::setNonblocking(const int& socket_id) {
        int flags = fcntl(socket_id, F_GETFL, 0);
        if (flags == -1){
            return MG_ERROR;
        }
        if (fcntl(socket_id, F_SETFL, flags | O_NONBLOCK) < 0){
            return MG_ERROR;
        }
        return MG_OK;
}


int Server::on() {
     try {


         if ((socket_id = make_shared<int>(
                 socket(DOMAIN, TYPE, PROTOCOL))) == nullptr) {
             throw std::range_error("Fallo al crear el socket");
         }

         if (setsockopt(*socket_id,
                        SOL_SOCKET,
                        SO_REUSEADDR |
                        SO_REUSEPORT,
                        &*option_mame,
                        sizeof(*option_mame)) != 0x0) {
             throw std::range_error("error al establecer el servidor");
         }

         if(setNonblocking(*socket_id) == MG_ERROR)
             throw std::runtime_error("Error al establecer el socket principal como no bloqueante");

         address.sin_family = AF_INET;
         address.sin_addr.s_addr = INADDR_ANY;
         address.sin_port = htons(PORT);

         unlink("127.0.0.1");
         if (bind(*socket_id, (struct sockaddr *)&address, sizeof(address)) < 0) {
               throw std::range_error("error al enlazar el servidor");
          }
         if (listen(*socket_id, 3) < 0x0) {
              throw std::range_error("error al escuchar el puerto");
          }

         return MG_OK;
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          lock_guard.unlock();
          return MG_ERROR;
     }
}

void Server::getResponseProcessing() {
    try {
        string base;
        vector<char> buffer;
        buffer.resize(*buffer_size);

        size_t totalbyes = read(*socket_id, buffer.data(), *buffer_size);

        for (int it = 0; it <= totalbyes; it++) {
            if (int(buffer[it]) == 0)
                break;
            if(int(buffer[it]) == UnCATCH_ERROR_CH)
                continue;
            if (int(buffer[it]) == 10)
                continue;
            base += buffer[it];
        }

        std::cout <<  "base: "<< base << std::endl;

        if(base.empty()) throw std::range_error("error, el mensaje no se recibio");
        buffereOd_data = make_shared<string>(base);
    }
    catch (const std::exception &e) { std::cerr << e.what() << '\n'; }
}

void Server::setResponse(char buffer[DEF_BUFFER_SIZE]) {
     std::string raw(buffer);
     if(!raw.empty()) {
          buffereOd_data = make_shared<string>(raw);
     }
}

void Server::sendResponse(const string& _msg) {
     SendData data;
     data.socket = *socket_id;
     data.data = _msg;

     epoll_event event;
     event.events = EPOLLOUT | EPOLLET;
     event.data.ptr = &data;

     if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, *socket_id, &event) == -1) {
          std::cerr << "Error al registrar evento de escritura: " << strerror(errno) << std::endl;
     }

     epoll_event events[1];

     int n = epoll_wait(epoll_fd, events, 1, -1);
     if(n == -1) {
          std::cerr << "epoll_wait" << strerror(errno) << std::endl;
          return;
     }

     if(events[0].events & EPOLLOUT) {
       int bytesw =  send(data.socket, data.data.c_str(), data.data.size(), 0);
          if (bytesw == -1) {
               std::cerr << "Error: Sending data" << std::endl;
               epoll_ctl(epoll_fd, EPOLL_CTL_DEL, data.socket, nullptr);
               close(data.socket);
          } else if(bytesw == 0) {
               epoll_ctl(epoll_fd, EPOLL_CTL_DEL, data.socket, nullptr);
               close(data.socket);
          } else {
               epoll_ctl(epoll_fd, EPOLL_CTL_MOD, data.socket, &events[0]);
          }
     }



}

