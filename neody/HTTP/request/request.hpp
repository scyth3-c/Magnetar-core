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

  [[nodiscard]] inline size_t total_params() const { return _parameters.size(); }
  [[nodiscard]] Param_t getParams();

   inline void clearParams() noexcept {
       _parameters.clear();
       route.clear();
   };

  void setRawParametersData(string _raw_data);
};

#endif // ! REQUEST_HPP