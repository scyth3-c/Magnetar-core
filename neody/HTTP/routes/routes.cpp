#include "routes.hpp"


dataRender * Query::_cache = nullptr;


string  Query::getData() const noexcept                   {     return last;        }
bool    Query::getNext() const noexcept                   {     return nexteable;   }
void    Query::next()    noexcept                         {     nexteable = true;   }
void    Query::lock()    noexcept                         {     nexteable = false;  }


void Query::json(string _txt, std::function<void()> callback) noexcept {
    last = string(utility_t::prepare_basic(_txt, "application/json", headers)); 
    callback();
}
void Query::html(string _txt,  std::function<void()> callback) noexcept {
    last = string(utility_t::prepare_basic(_txt, "text/html", headers)); 
    callback();
}
void  Query::send(string _txt,  std::function<void()> callback) noexcept {    
     last = string(utility_t::prepare_basic(_txt, "text/plain", headers));       
     callback();
}



void Query::json(string _txt, int status, std::function<void()> callback) noexcept {
    last = utility_t::prepare_basic(_txt, "application/json", headers, std::to_string(status)); 
    callback();
}
void Query::html(string _txt, int status,  std::function<void()> callback) noexcept {
    last = utility_t::prepare_basic(_txt, "text/html", headers,  std::to_string(status)); 
    callback();
}
void  Query::send(string _txt, int status,  std::function<void()> callback) noexcept {    
     last = utility_t::prepare_basic(_txt, "text/plain", headers,  std::to_string(status));       
     callback();
}


void  Query::readFile(string path,string type, std::function<void()> callback) noexcept {

    BasicRead reader;
    string body = reader.processing(path);
    last = utility_t::prepare_basic(body, type, headers);
    reader.~BasicRead();
    callback();
}

void  Query::readFileX(string path,string type, std::function<void()> callback) noexcept {
    CppReader reader;
    string body = reader.processing(path);
    last = utility_t::prepare_basic(body, type, headers);
    reader.~CppReader();
    callback();
}

void  Query::compose(string path, int reserve, std::function<void()> callback) noexcept {
    
    MgReader reader;
    string body = reader.processing(path, reserve);
    last = utility_t::prepare_basic(body, "text/html" , headers);
    reader.~MgReader();
    callback();
}

void  Query::render(string path, std::function<dataRender(dataRender& data)> callback) noexcept {

std::unique_ptr<dataRender> tasty_temp = std::make_unique<dataRender>(callback);
string body = tasty_temp->render(path);
last = utility_t::prepare_basic(body, "text/html", headers);
tasty_temp.reset();

}


void Query::setHeaders(string body) noexcept {
 headers += body + "\n";
}
void Query::setHeaders(HEADERS box) noexcept{
headers += box.generate();
}

