#include "parameters.hpp"

param_box::param_box(string _name, string _value){ 
      name = std::move(_name);
      value = std::move(_value);
}



Param_t::Param_t(){
    _list.push_back(std::make_pair("NULL","NULL"));
}


param_box Param_t:: operator[](int index) {  
       param_box nuevo(_list[index]);
       return nuevo;
}

void Param_t::setConten(vector<std::pair<string,string>> list) {
     _list.clear();
    _list = list;
}



bool Param_t::exist(string param){
  std::cout << std::flush;
    for(auto &it : _list){
      if(it.first == param) return true;
    }
    return false;
}



param_box Param_t::get(string param){
  std::cout << std::flush;
    for(auto &it : _list){
      if(it.first == param) return it;
    }
    param_box bad("null","null");
    return bad;
}