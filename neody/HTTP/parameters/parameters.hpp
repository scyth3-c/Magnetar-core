#ifndef PARAMTERS_HPP  
#define PARAMTERS_HPP

#include <vector>
#include <string>
#include <iostream>

using std::string;
using std::vector;

class param_box{
    private:
     std::pair<string,string> _body;

    public:

    param_box(string, string);
    param_box(std::pair<string,string> conten) : _body(conten) {
        name = std::move(conten.first);
        value = std::move(conten.second);
 }  

    string name{""};
    string value{""};
    
};


class Param_t {
 private:
  vector<std::pair<string,string>> _list;
  
public: 

  Param_t();
  Param_t(vector<std::pair<string,string>> list) :  _list(list) {}

  param_box operator[](int);

  void setConten(vector<std::pair<string,string>>);

  inline void clear() { _list.clear(); }
  inline bool empty(){return _list.empty() ? true : false;}

  bool exist(string);

  param_box get(string);

  [[nodiscard]] inline vector<std::pair<string,string>> toArray(){return _list;}

};




struct utility_t {
  static string prepare_basic(string _txt, string _type, string headers, string status="200") {
    return         "HTTP/1.1 "+status+" OK\n"
                   "Server: Neody/0.5\n"
                   "Content-Type: "+_type+"\n"
                   "Content-Length: " + std::to_string(_txt.length()) + "\n"
                   "Accept-Ranges: bytes\n" 
                    + headers +
                   "Connection: close\n"
                   "\n" +
                   _txt;
  }
};



constexpr auto ERROR_GET = "HTTP/1.1 404 BAD\n"
                           "Server: Neody/0.5\n"
                           "Content-Type: application/json\n"
                           "Content-Length: 37\n"
                           "Accept-Ranges: bytes\n"
                           "Connection: close\n"
                           "\n"
                           "{\"error\":\"this route is not defined\"}";



#endif // ! PARAMTERS_HPP