#ifndef PARAMTERS_HPP  
#define PARAMTERS_HPP

#include <vector>
#include <string>

using std::string, std::vector;

class param_box{
    private:
     std::pair<string,string> _body;

    public:
    param_box(string _name, string _value){ 
      name = std::move(_name);
      value = std::move(_value);
    }
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
  Param_t(){
    _list.push_back(std::make_pair("NULL","NULL"));
  }
  Param_t(vector<std::pair<string,string>> list) :  _list(list) {}

  param_box operator[](int index) {  
       param_box nuevo(_list[index]);
       return nuevo;
    }
  void setConten(vector<std::pair<string,string>> list) {
     _list.clear();
    _list = list;
  }
  void clear(){
    _list.clear();
  }
  bool empty(){
    return _list.empty() ? true : false;
  }

  bool exist(string param){
    for(auto &it : _list){
      if(it.first == param) return true;
    }
    return false;
  }

  param_box get(string param){
    for(auto &it : _list){
      if(it.first == param) return it;
    }
    param_box bad("null","null");
    return bad;
  }

  vector<std::pair<string,string>> toArray(){
    return _list;
  }
  
};


struct utility_t {
  static string prepare_basic(string _txt, string _type, string headers, string status="200") {
    return         "HTTP/1.1 "+status+" OK\n"
                   "Server: Magnetar/0.5\n"
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
                           "Server: Magnetar/0.5\n"
                           "Content-Type: application/json\n"
                           "Content-Length: 25\n"
                           "Accept-Ranges: bytes\n"
                           "Connection: close\n"
                           "\n"
                           "this route is not defined";



#endif // ! PARAMTERS_HPP