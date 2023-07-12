#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <vector>
#include <iostream>
#include "../parameters/parameters.hpp"

using std::string;
using std::vector;

constexpr const char* NOT_PARAMS = "___s";

class Request {

private:
  string route{};
  vector<std::pair<string, string>> _parameters;

public:
  Request();
  ~Request();

  inline size_t total_params() const { return _parameters.size(); }

   Param_t getParams() const;

  void setRawParametersData(string _raw_data);
};

#endif // ! REQUEST_HPP