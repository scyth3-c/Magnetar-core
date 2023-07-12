#ifndef DATA_RENDER_HPP
#define DATA_RENDER_HPP

#include <memory>
#include <fstream>
#include <string>
#include <filesystem>

#include "../../utils/notify.h"
#include "local_utility.h"

class dataRender {

private:
    
        shared_ptr<std::ifstream> reader = nullptr;
        vector<std::tuple<std::string, std::string>> variables = {};

public:

    dataRender(std::function<dataRender(dataRender &data)> _parser) {
        dataRender temp_render = _parser(*this);
        variables = std::move(temp_render.getVariables());
    }
    ~dataRender() {}

    inline std::vector<std::tuple<std::string, std::string>> getVariables() const 
    { return variables; }

    inline void operator()(string nombre, string valor) {
        variables.push_back(std::make_tuple(nombre,valor));
    }

    inline std::string render(string path)  { 

        if(!std::filesystem::exists(path)) return notify_html::noFIle(path);        

        string body = readFile(path);
        string buffer;

        std::cout << std::flush;       
        for (size_t iterator = 0; iterator < variables.size(); iterator++) {
            buffer = body_tratament(body);
            body = buffer;
        }

        return body;
}


 inline string body_tratament(string &body) {

         std::pair<int,int> coords;
         int guard = 0;

        std::cout << std::flush;
        for (size_t iter = 0; iter < body.length(); iter++) {
            string eye{};
            eye += body[iter];
            eye += body[iter+1];

            if(eye == _OPEN_DATA) {
                guard = 1;
                body[iter] = char(32);
                body[iter+1] = char(32);
                coords.first = iter+2;            

            }else if(eye == _CLOSE_DATA){
                guard = 0;
                body[iter] = char(32);
                body[iter+1] = char(32);
                coords.second = iter-1;
                break;
            }
        }
            if(guard==1) return notify_html::noSafeData();
       
         string name{""};
         string data{""};
         std::cout << std::flush;
            for(int i=coords.first; i<=coords.second; i++ ){
                name += body[i];
                body[i] = char(32);
            }
            for(size_t i=0; i<variables.size(); i++){
                if((std::get<0>(variables[i])) == name){
                    data = std::get<1>(variables[i]);
                    break;
                } else{
                    data = "error";
                }
            }
        string newest = body.insert(coords.first, data);
        return newest;
    } 


    inline string readFile(string target) {

        reader = make_shared<std::ifstream>(target.c_str());
            string chunk = "";
            string module = "";
        std::cout << std::flush;
        while (getline(*reader, chunk)) {
             module += chunk;
                
            }
        reader->close();
        reader.reset();
        return module; 
    }




};



#endif // ! DATA_RENDER_HPP