#ifndef CPP_READER_HPP
#define CPP_READER_HPP

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


class CppReader {
    
private:
    shared_ptr<std::ifstream> reader = nullptr;
    shared_ptr<std::ofstream> writter = nullptr;

public:
    CppReader() = default;
    

    string processing(const string& path) {

        string nombre,
            chunk,
            body,
            ruta,
            command,
            compile_container,
            code;

        bool init = false;
        std::pair<int, int> coords;
        code = BASE;

        reader = make_shared<std::ifstream>(path);
        while (getline(*reader, chunk))
        {
            body += chunk;
        }
        reader->close();
        for (size_t iterator = 0; iterator < body.length() - 1; iterator++)
        {
            if (init)
            {
                if (body[iterator] == CODE_LOCATE)
                {
                    coords.second = (int)iterator;
                    break;
                }
                else
                {
                    code += body[iterator];
                }
            }

            if (body[iterator] == CODE_LOCATE && !init)
            {
                coords.first = (int)iterator;
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
        
        std::cout << std::flush;
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

        std::cout << std::flush;
        for (int i = coords.first; i <= coords.second; i++)
        {
            body[i] = char(32);
        }

        std::filesystem::remove(ruta);
        std::filesystem::remove(ruta + _CPP_);
        std::filesystem::remove(ruta + _TXX_);
        body.insert(coords.first, compile_container);

        return body;
    }
};

#endif // ! CPP_READER_HPP