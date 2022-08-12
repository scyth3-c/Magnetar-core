#ifndef JSON_HPP
#define JSON_HPP

#include <string>
#include <vector>
#include <initializer_list>

using std::string, std::vector, std::initializer_list;



template<class...P>
struct JSON_t {
    JSON_t(initializer_list<P...> list) : body(std::move(list))  {}
    vector<string> body;
    string json(){
        string response{"{ "};

        char comilla = char(34);
        char puntos = char(58);
        char coma_simple = char(44);

        bool anidado = false;

        for (size_t i = 0; i < body.size(); i+=2)
        {
            response +=  comilla + body[i] + comilla;
            response +=  puntos;
            response +=  comilla + body[i+1] + comilla;
            response +=  coma_simple;
        }
        size_t size = response.length();
        response[size-1] = char(127);
        response += " }";

        for (size_t i = 0; i < response.length(); i++)
        {
            if(response[i] == '{' && i>0) {
                response[i-1] = ' ';
            } else if( response[i] == '}'){
                response[i+1] = ' ';
            }
        }
    
        return response;
    }

    string operator()(){
        return json();
    }

};
typedef JSON_t<string> JSON;


#endif // ! JSON_HPP