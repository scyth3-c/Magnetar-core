#ifndef NEO_RENDER
#define NEO_RENDER

#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>


#include <chrono>
#include <random>

#include "local_utility.h"

using std::string;
using std::make_shared;
using std::shared_ptr;


class NeoRender {
private:
    shared_ptr<std::ifstream> reader = nullptr;
    shared_ptr<std::ofstream> writter = nullptr;
   

public:
    NeoRender() = default;

    [[maybe_unused]] string processing(const string& path) {

            string neo_base;

            string nombre,
            chunk,
            code_body,
            ruta,
            command,
            compile_container,
            code;

        reader = make_shared<std::ifstream>(TEMPLATE_NEO);
        while (getline(*reader, chunk))
        {
            neo_base += chunk;
        }
        reader->close();
        

        reader = make_shared<std::ifstream>(path);
        while (getline(*reader, chunk))
        {
            code_body += chunk;
        }
        reader->close();

        

        bool init = false;
        std::pair<int, int> coords;
        code = NEOBASE;


        for (size_t iterator = 0; iterator < neo_base.length() - 1; iterator++) {
            if (init) {
                if (neo_base[iterator] == CODE_LOCATE) {
                    coords.second = (int)iterator;
                    break;
                }
            }
            if (neo_base[iterator] == CODE_LOCATE && !init)
            {
                coords.first = (int)iterator;
                code += code_body;
                init = true;
                continue;
            }
        }


        std::mt19937 gen;
        gen.seed(std::random_device()());
        std::uniform_int_distribution<std::mt19937::result_type> dist;

        nombre = std::to_string(dist(gen));
        ruta = WORK_PATH + nombre;

        writter = make_shared<std::ofstream>();
        writter->open((ruta + _CPP_).c_str());

        for (auto &it : code)
        {
            *writter << it;
        }

        *writter << _CODE_END;
        writter->close();
        writter.reset();

        command = _GPP_  + 
                  ruta   + 
                  _CPP_  + 
                  _OUT   +
                  ruta   + 
                  _AFTER +
                   ruta  + 
                   _DATA + 
                   ruta  + 
                   _TXX_;

        system(command.c_str());
        reader->open(ruta + _TXX_);
        chunk = " ";

        while (getline(*reader, chunk))
        {
            compile_container += chunk;
        }

        reader->close();
        reader.reset();

        for (int i = coords.first; i <= coords.second; i++)
        {
            neo_base[i] = char(32);
        }

        std::filesystem::remove(ruta);
        std::filesystem::remove(ruta + _CPP_);
        std::filesystem::remove(ruta + _TXX_);
        neo_base.insert(coords.first, compile_container);
        return neo_base;

    }



};




#endif // !NEO_RENDER