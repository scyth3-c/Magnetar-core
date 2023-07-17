#include "request.hpp"

Request::Request()= default;


void Request::setRawParametersData(string _raw)
{
  if (_raw == NOT_PARAMS) {return;}

  string slave;
  vector<string> params;

  for (size_t iterator = 0; iterator <= _raw.length(); iterator++)
  {
    if (_raw[iterator] == '=' || _raw[iterator] == '&' ||iterator==_raw.length()) 
    {
      params.push_back(slave);
      slave.clear();
      continue;
    }
    slave += _raw[iterator];
  }
  for (size_t i = 0; i < params.size(); i += 2) {
      std::pair<string, string> temp_pair = std::make_pair(params[i],params[i + 1]);
    _parameters.push_back(temp_pair);
  }
}

Param_t Request::getParams() {
    Param_t nuevo;
    try {
       nuevo.setConten(_parameters);
       return nuevo;
    }
    catch(const std::exception& e) {
      std::cerr << e.what() << '\n';
    }
    return nuevo;
}