#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <vector>
#include <iostream>
#include "parameters.hpp"

using std::string;
using std::vector;

constexpr const char* NOT_PARAMS = "___s";

class Request
{
private:
  string route{};
  vector<std::pair<string, string>> _parameters;

public:
  Request() {}
  ~Request() {}

  inline size_t total_params() const { return _parameters.size(); }
  inline Param_t getParams() const
  {
    Param_t nuevo;
    try
    {
       nuevo.setConten(_parameters);
       return nuevo;
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
    }
    return nuevo;
  }

  void setRawParametersData(string _raw_data);
  void showParams();
};

void Request::setRawParametersData(string _raw)
{
  if (_raw == NOT_PARAMS) {return;}

  string slave{""}; 
  vector<string> params{};
  
  for (size_t iterator = 0; iterator <= _raw.length(); iterator++)
  {
    if (_raw[iterator] == '=' || _raw[iterator] == '&' ||iterator==_raw.length()) 
    {
      params.push_back(slave);
      slave = "";
      continue;
    }
    slave += _raw[iterator];
  }

  for (size_t i = 0; i < params.size(); i += 2)
  {
    std::pair<string, string> temp_pair = std::make_pair(params[i],params[i + 1]);
    _parameters.push_back(temp_pair);
  }

};



#endif // ! REQUEST_HPP