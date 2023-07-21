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

    explicit dataRender(const std::function<dataRender(dataRender &data)>& _parser) {
        dataRender temp_render = _parser(*this);
        variables = std::move(temp_render.getVariables());
    }
    ~dataRender() = default;

    [[nodiscard]] inline std::vector<std::tuple<std::string, std::string>> getVariables() const
    { return variables; }

    inline void operator()(const string& nombre, const string& valor) {
        variables.emplace_back(nombre,valor);
    }

    inline std::string render(const string& path)  {

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

        for (size_t iter = 0; iter < body.length(); iter++) {
            string eye{};
            eye += body[iter];
            eye += body[iter+0x1];

            if(eye == _OPEN_DATA) {
                guard = 1;
                body[iter] = char(0x20);
                body[iter+1] = char(0x20);
                coords.first = (int)iter+0x2;

            }else if(eye == _CLOSE_DATA){
                guard = 0;
                body[iter] = char(0x20);
                body[iter+1] = char(0x20);
                coords.second = (int)iter-0x1;
                break;
            }
        }
            if(guard==1) return notify_html::noSafeData();
       
         string name;
         string data;

            for(int i=coords.first; i<=coords.second; i++ ){
                name += body[i];
                body[i] = char(32);
            }
            for(auto & variable : variables){
                if((std::get<0>(variable)) == name){
                    data = std::get<1>(variable);
                    break;
                } else{
                    data = "error";
                }
            }
        string newest = body.insert(coords.first, data);
        return newest;
    } 


    inline string readFile(const string& target) {

        reader = make_shared<std::ifstream>(target.c_str());
            string chunk;
            string module;
        while (getline(*reader, chunk)) {
             module += chunk;
                
            }
        reader->close();
        reader.reset();
        return module; 
    }




};



#endif // ! DATA_RENDER_HPP