#include "mgsockets.h"

[[maybe_unused]] std::mutex Server::VICTORIA;


Engine::Engine(uint16_t _xport) {
     PORT = make_shared<uint16_t>(_xport);
}

int Engine::create() {
     try {
          if ((socket_id = make_shared<int>(
                  socket(DOMAIN, TYPE, PROTOCOL))) < 0) {
               throw std::range_error("Fallo al crear el socket");
          }
          return MG_OK;
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return MG_ERROR;
     }
}


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



int Client::Close() {
     try {
          if (close(*socket_fd) < 0) {
               throw std::range_error("Fallo al cerrar el socket, Client");
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
          PORT.reset(new uint16_t(xPort));
          if(!*PORT) throw std::range_error("error al asignar el puerto");
          return MG_OK;
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return MG_ERROR;
     }
}


[[maybe_unused]] int Engine::setHeapLimit(int _max) {
     try {
          if (heap_limit == nullptr) {
               heap_limit = make_shared<int>(_max);
          } 
          heap_limit.reset(new int(_max));
          if(*heap_limit != _max) throw std::range_error("error al asignar el HeapLimit");
          return MG_OK;
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return MG_ERROR;
     }
}



int Engine::getHeapLimit() {
     try {

          if(*heap_limit < 0) throw std::range_error("no se puedo extraer el HeapLimit"); 
          if (*heap_limit > 0) {
               return *heap_limit;
          }
          else {
               heap_limit = make_shared<int>(DEF_HEAP_LIMIT);
               return *heap_limit;
          }
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return MG_ERROR;
     }
}


int Engine::getPort() {
     try {
          if (*PORT == 0)  {
               return *PORT;
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
          if(*static_sessions != max) throw("error al asignar las sesiones");
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
     }
}

int Server::on(function<void(string*)>optional) {
     try {
          if (setsockopt(*socket_id,
                         SOL_SOCKET,
                         SO_REUSEADDR |
                             SO_REUSEPORT,
                         &*option_mame,
                         sizeof(*option_mame)) != 0) {
               throw std::range_error("error al establecer el servidor");
          }
          address.sin_family = AF_INET;
          address.sin_addr.s_addr = INADDR_ANY;
          address.sin_port = htons(*PORT);

          if (bind(*socket_id, (struct sockaddr *)&address, sizeof(address)) < 0) {
               throw std::range_error("error al enlazar el servidor");
          }
          if (listen(*socket_id, *static_sessions) < 0) {
               throw std::range_error("error al escuchar el puerto");
          }
          if ((new_socket = make_shared<int>(
                   accept(*socket_id,
                          (struct sockaddr *)&address,
                          (socklen_t *)&address_len))) < 0) {
               throw std::range_error("error al conectar el servidor");
          }
               getResponseProcessing();
          optional(buffereOd_data.get());

          return MG_OK;
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return MG_ERROR;
     }
}



void Server::getResponseProcessing() {

     try {    
          string receptor;
          vector<char> buffer = {'1'};
     
          buffer.reserve(*buffer_size);
          
          read(*new_socket, buffer.data(), *buffer_size);
     
          for (int it = 0; it <= *buffer_size-(*heap_limit); it++) {
               if (int(buffer[it] == UnCATCH_ERROR_CH))
                    continue;
               receptor += buffer[it];
          }
           if(receptor.empty()) throw std::range_error("error, el mensaje no se recibio");
          buffereOd_data = make_shared<string>(receptor);
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
     }
}


void Server::sendResponse(const string& _msg) {
     std::cout.clear();
     char* conten = (char *)_msg.c_str();
     try {
          if(strlen(conten) == 0) throw std::range_error("erro al obtener la respuesta");
          send(*new_socket, conten, _msg.size(), 0);
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
     }
}

void Client::getResponseProcessing() {
     
          string receptor;
          vector<char> buffer = {'1'};
          buffer.reserve(*buffer_size);
          read(*socket_id, buffer.data(), *buffer_size);

          for (int it = 0; it <= *buffer_size - (*heap_limit); it++) {
               if (int(buffer[it] == UnCATCH_ERROR_CH))
                    continue;
               receptor += buffer[it];
          }
          if(receptor.empty()) throw std::range_error("error, el mensaje no se recibio");
          buffereOd_data = make_shared<string>(receptor);
}

int Client::on(function<void(string*)>optional) {
     try {
          address.sin_family = AF_INET;
          address.sin_port = htons(*PORT);
          char *IP =  (char *)IP_ADDRRESS->c_str();
          if (inet_pton(AF_INET, IP, &address.sin_addr) <= 0) {
               return MG_ERROR; // throw "error, invalid address";
          }
          *socket_fd = connect(*socket_id, (struct sockaddr *)&address, sizeof(address));
          if(message->empty()) throw std::range_error("error mensaje del cliente vacio");
          send(*socket_id, message->c_str(), std::strlen(message->c_str()), 0);
          getResponseProcessing();
          optional(buffereOd_data.get());
          return MG_OK;
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
          return MG_ERROR;
     }
}


void Client::setIP(const string& _ip) {
     try {
          IP_ADDRRESS.reset(new string(_ip));
          if(*IP_ADDRRESS != _ip) throw std::range_error("error al establecer la IP");
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
     }
}

[[maybe_unused]] void Client::setMessage(const string& conten) {
     try {
          message.reset(new string(conten));
          if(*message != conten) throw std::range_error("error al establecer el mensaje");
     }
     catch (const std::exception &e) {
          std::cerr << e.what() << '\n';
     }
}