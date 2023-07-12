#ifndef MG_READERS_HPP
#define MG_READERS_HPP

#include <memory>
#include <string>
#include <fstream>
#include <filesystem>
#include <utility>
#include <vector>
#include "../../utils/notify.h"

#include "local_utility.h"

#define SHOW(X) std::cout << X << std::endl;

using std::make_shared;
using std::shared_ptr;
using std::string;

class MgReader
{
private:
    shared_ptr<std::ifstream> reader = nullptr;

public:
    MgReader() {}
    ~MgReader() {}

    inline string processing(string path, int reserve)
    {

        size_t init = 0;
        string folder_base = "";

        std::cout << std::flush;
        for (int iterator = path.length(); iterator >= 0; iterator--)
        {
            if (path[iterator] == '/')
            {
                init = iterator;
                break;
            }
        }
        for (size_t it = 0; it <= init; it++)
        {
            folder_base += path[it];
        }

        if (!std::filesystem::exists(folder_base))
        {
            return notify_html::noPath();
        }

        string body = readFile(path);
        string buffer;

        std::cout << std::flush;

        for (int global = 0; global < reserve; global++)
        {
            buffer = tratament(body, folder_base);
            body = buffer;
        }

        return body;
    }

    inline string tratament(string &body, string &folder_base)
    {

        std::pair<int, int> coord;
        int safe = 0;
        std::cout << std::flush;
        for (size_t it = 0; it < body.length(); it++)
        {

            string eye;

            eye += body[it];
            eye += body[it + 1];

            if (eye == _OPEN)
            {
                body[it] = char(32);
                body[it + 1] = char(32);
                coord.first = it + 2;
                safe = 1;
            }
            else if (eye == _CLOSE)
            {
                safe = 2;
                body[it] = char(32);
                body[it + 1] = char(32);
                coord.second = it - 1;
                break;
            }
        }

        if (safe == 1)
            return notify_html::noSafe();

        string name{""};

        for (int it = coord.first; it <= coord.second; it++)
        {
            name += body[it];
            body[it] = char(32);
        }

        string target = folder_base + name;
        std::cout << std::flush;
        if (!std::filesystem::exists(target))
        {
            return notify_html::noFIle(name);
        }

        body.insert(coord.first, readFile(target));
        string newest = body;
        return newest;
    }

    inline string readFile(string target)
    {

        reader = make_shared<std::ifstream>(target.c_str());
        string chunk = "";
        string module = "";

        while (getline(*reader, chunk))
        {
            module += chunk;
        }
        std::cout << std::flush;
        reader->close();
        reader.reset();
        return module;
    }

    inline string normalize(string target)
    {
        string temp_box = "";
        std::cout << std::flush;
        for (size_t i = 0; i < target.length(); i++)
        {
            if (target[i] == char(32))
                target[i] = '\0';
            temp_box += target[i];
        }
        return temp_box;
    }
};

#endif // ! MG_READERS_HPP