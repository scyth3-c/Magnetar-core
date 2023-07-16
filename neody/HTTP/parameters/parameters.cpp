#include "parameters.hpp"

#include <utility>

param_box::param_box(string _name, string _value){
      name = std::move(_name);
      value = std::move(_value);
}

Param_t::Param_t(){
    _list.emplace_back("NULL","NULL");
}
param_box Param_t:: operator[](int index) {  
       param_box nuevo(_list[index]);
       return nuevo;
}

void Param_t::setConten(vector<std::pair<string,string>> &list) {
    _list.clear();
    _list = list;
}


[[maybe_unused]] bool Param_t::exist(const string& param){
  std::cout << std::flush;
    for(auto &it : _list){
      if(it.first == param) return true;
    }
    return false;
}


param_box Param_t::get(const string& param){
    for(auto &it : _list){
        if(it.first == param) {
            return {it.first, it.second};
        }
    }
    param_box bad("null","null");
    return bad;
}