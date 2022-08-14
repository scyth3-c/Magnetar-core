#ifndef TEMPLATE_HTML_CPP
#define TEMPLATE_HTML_CPP


#include <memory>
#include <string>
#include <fstream>
#include <filesystem>
#include <utility>
#include <vector>
#include"../utils/notify.hpp"

#define SHOW(X) std::cout << X <<std::endl;

using std::string;
using std::shared_ptr;
using std::make_shared;

constexpr const char* _OPEN = "#[";
constexpr const char* _CLOSE = "];";


class MgReader {
private:

    shared_ptr<std::ifstream> reader = nullptr;
    

public: 
    MgReader() {}
    ~MgReader() {}

     inline string processing (string path, int reserve) {

        size_t init = 0;
        string folder_base = "";
        int insertions = 0;

            std::vector<std::pair<int,int>> coord(reserve);


        for (int iterator=path.length(); iterator >= 0; iterator-- ) {
            if(path[iterator]=='/') {
                init = iterator;
                break;
            }
        }
        for (size_t it = 0; it <= init; it++) {
            folder_base += path[it];
        }
        
        if(!std::filesystem::exists(folder_base)) {
            return notify_html::noPath();
        }


        string body = readFile(path);
        int safe = 0;

        for (size_t it = 0; it < body.length(); it++) {
            
            string eye;

            eye += body[it];
            eye += body[it+1];

            if(eye == _OPEN){
                body[it] = char(32);
                body[it+1] = char(32);
                coord[insertions].first = it+2;
                safe = 1;
           
            } else if(eye == _CLOSE) {
                safe = 2;
                body[it] = char(32);
                body[it+1] = char(32);
                coord[insertions].second = it-1;
                insertions++;
            }
        }

        if(safe == 1) return notify_html::noSafe();
    
        string module = "";
        
        for(int global = 0; global < reserve; global++) {

            string name{""};
            
            for (int it = coord[global].first; it <= coord[global].second; it++) {
                name += body[it];
                body[it] = char(32);
            }
            

            string target = folder_base + name;

            SHOW(target)

            if(!std::filesystem::exists(target)) {
                return notify_html::noFIle(name);
            }
            module += readFile(target);

            name.~basic_string();
        }


        body.insert(coord[0].first, module);

        return body;
    }



   inline string readFile(string target) {

        reader = make_shared<std::ifstream>(target.c_str());
            string chunk = "";
            string module = "";

        while (getline(*reader, chunk)) {
             module += chunk;
                
            }
        reader->close();
        reader.reset();
        return module; 
    }



    inline string normalize(string target) {
        string temp_box = "";
        for (size_t i = 0; i < target.length(); i++) {
            if(target[i] == char(32)) target[i] =  '\0';
            temp_box += target[i];
        }
        return temp_box;
    }
};




#endif // ! TEMPLATE_HTML_CPP