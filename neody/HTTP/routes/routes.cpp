#include "routes.hpp"

#include <utility>


dataRender * Query::_cache = nullptr;


string  Query::getData() const noexcept                   {     return last;        }
bool    Query::getNext() const noexcept                   {     return nexteable;   }
void    Query::next()    noexcept                         {     nexteable = true;   }
void    Query::lock()    noexcept                         {     nexteable = false;  }


void Query::json(const string& _txt, const std::function<void()>& callback) noexcept {
    last = string(utility_t::prepare_basic(_txt, "application/json", headers));
    callback();
}
void Query::html(const string& _txt,  const std::function<void()>& callback) noexcept {
    last = string(utility_t::prepare_basic(_txt, "text/html", headers));
    callback();
}
void  Query::send(const string& _txt,  const std::function<void()>& callback) noexcept {
     last = string(utility_t::prepare_basic(_txt, "text/plain", headers));
     callback();
}



void Query::json(const string& _txt, int status, const std::function<void()>& callback) noexcept {
    last = utility_t::prepare_basic(_txt, "application/json", headers, std::to_string(status));
    callback();
}
void Query::html(const string& _txt, int status,  const std::function<void()>& callback) noexcept {
    last = utility_t::prepare_basic(_txt, "text/html", headers,  std::to_string(status));
    callback();
}
void  Query::send(const string& _txt, int status,  const std::function<void()>& callback) noexcept {
     last = utility_t::prepare_basic(_txt, "text/plain", headers,  std::to_string(status));
     callback();
}


void  Query::readFile(string path,const string& type, const std::function<void()>& callback) noexcept {

    BasicRead reader;
    last = utility_t::prepare_basic(reader.processing(std::move(path)), type, headers);
    reader.~BasicRead();
    callback();
}

void  Query::readFileX(string path,const string& type, const std::function<void()>& callback) noexcept {
    CppReader reader;
    last = utility_t::prepare_basic(reader.processing(std::move(path)), type, headers);
    reader.~CppReader();
    callback();
}

void  Query::compose(string path, int reserve, const std::function<void()>& callback) noexcept {
    MgReader reader;
    last = utility_t::prepare_basic(reader.processing(std::move(path), reserve), "text/html" , headers);
    reader.~MgReader();
    callback();
}

void  Query::render(string path, const std::function<dataRender(dataRender& data)>& callback) noexcept {

std::unique_ptr<dataRender> tasty_temp = std::make_unique<dataRender>(callback);
last = utility_t::prepare_basic(tasty_temp->render(std::move(path)), "text/html", headers);
tasty_temp.reset();
}


void Query::setHeaders(const string& _body) noexcept {
 headers += _body + "\n";
}
void Query::setHeaders(HEADERS box) noexcept{
headers += box.generate();
}

