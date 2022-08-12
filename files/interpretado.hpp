#ifndef INTERPRETADO_HPP
#define INTERPRETADO_HPP

#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <chrono>
#include <random>

using std::string, std::make_shared, std::shared_ptr;

class Interpretado
{
private:
    shared_ptr<std::ifstream> reader = nullptr;
    shared_ptr<std::ofstream> writter = nullptr;

public:
    Interpretado() {}
    string extract(string path)
    {

        string nombre,
            chunk,
            body,
            ruta,
            command,
            compile_container,
            code;

        bool init = false;
        std::pair<int, int> coords;
        code = {"#include <iostream> \n int main() { \n "};

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
                if (body[iterator] == '#')
                {
                    coords.second = iterator;
                    break;
                }
                else
                {
                    code += body[iterator];
                }
            }

            if (body[iterator] == '#' && !init)
            {
                coords.first = iterator;
                init = true;
                continue;
            }
        }

        std::mt19937 gen;
        gen.seed(std::random_device()());
        std::uniform_int_distribution<std::mt19937::result_type> dist;

        nombre = std::to_string(dist(gen));
        ruta = "./magnetar/work_space/temp_" + nombre;

        writter = make_shared<std::ofstream>();
        writter->open((ruta + ".cpp").c_str());

        for (auto &it : code)
        {
            *writter << it;
        }

        *writter << "\n return 0;\n}";
        writter->close();
        writter.reset();

        command = "g++ " + ruta + ".cpp -o " + ruta + " && ./" + ruta + " > " + ruta + ".txt";

        system(command.c_str());
        reader->open(ruta + ".txt");
        chunk = " ";

        while (getline(*reader, chunk))
        {
            compile_container += chunk;
        }

        reader->close();
        reader.reset();

        for (int i = coords.first; i <= coords.second; i++)
        {
            body[i] = char(32);
        }

        std::filesystem::remove(ruta);
        std::filesystem::remove(ruta + ".cpp");
        std::filesystem::remove(ruta + ".txt");
        body.insert(coords.first, compile_container);

        return body;
    }
    ~Interpretado() {}
};

#endif // ! INTERPRETADO_HPP
