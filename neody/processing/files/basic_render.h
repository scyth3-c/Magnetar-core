
#ifndef BASIC_READER_HPP
#define BASIC_READER_HPP

#include <string>
#include <fstream>
#include <filesystem>

#include "../../utils/notify.h"

using std::string;

class BasicRead {
public:
    BasicRead() = default;

    static std::pair<string, string> processing(const string& path) {
        if(!std::filesystem::exists(std::filesystem::path(path))) {
            return {notify::noPath(path), "404"};
        }
        try {
            std::ifstream reader;
            reader.open(path);
            string chunk;
            string body;
            while(getline(reader, chunk)){
                body += chunk;
            }
            reader.clear();
            reader.close();
            return {body, "200"};
        } catch (std::exception &e) {
            return {e.what(), "500"};
        }
    }
};

#endif // !BASIC_READER_HPP