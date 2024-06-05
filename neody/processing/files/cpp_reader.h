#ifndef CPP_READER_HPP
#define CPP_READER_HPP

#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <random>

#include "local_utility.h"

using std::string;
using std::make_shared;
using std::shared_ptr;

class CppReader {
public:
    CppReader() = default;
     static std::pair<string,string> processing(const string& path) {

        if(!std::filesystem::exists(std::filesystem::path(path))) {
            return {notify::noPath(path), "404"};
        }

        try {

  string nombre{},
            chunk,
            body,
            ruta{},
            command,
            compile_container,
            code{};

        bool init = false;
        std::pair<int, int> coords;
        code = BASE;

        std::ifstream reader(path);

        while (getline(reader, chunk)){
            body += chunk;
        }
        reader.close();

        for (size_t iterator = 0; iterator < body.length() - 1; iterator++)
        {
            if (init)
            {
                if (body[iterator] == CODE_LOCATE) {
                    coords.second = (int)iterator;
                    break;
                }
                else{
                    code += body[iterator];
                }
            }

            if (body[iterator] == CODE_LOCATE && !init){
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

        std::ofstream writter;
        writter.open((ruta + CPP_).c_str());

        for (auto &it : code){
            writter << it;
        }

        writter << CODE_END;
        writter.close();

        command = GPP_  +
                  ruta   +
                  CPP_  +
                  OUT   +
                  ruta   +
                  AFTER +
                   ruta  +
                   DATA +
                   ruta  +
                   TXX_;

        int result = system(command.c_str());

            reader.open(ruta + TXX_);
        chunk = " ";

        while (getline(reader, chunk))
        {
            compile_container += chunk;
        }
        if(result==-1)
            compile_container += " -1";

        reader.close();

        for (int i = coords.first; i <= coords.second; i++){
            body[i] = 32;
        }

        std::filesystem::remove(ruta);
        std::filesystem::remove(ruta + CPP_);
        std::filesystem::remove(ruta + TXX_);

        body.insert(coords.first, compile_container);

        return {body, "200"};

        }catch (std::exception &e) {

            return {e.what(), "500"};

        }

    }
};

#endif // ! CPP_READER_HPP