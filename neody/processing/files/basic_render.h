
#ifndef BASIC_READER_HPP
#define BASIC_READER_HPP

#include <memory>
#include <string>
#include <fstream>

using std::string;
using std::make_shared;
using std::shared_ptr;

class BasicRead {
    shared_ptr<std::ifstream> reader = nullptr;
public:
    BasicRead() = default;

    static string processing(const string& path) {
        std::ifstream reader;
        reader.open(path);
        string chunk;
        string body;
        while(getline(reader, chunk)){
            body += chunk;
        }
        return body;
    }
    ~BasicRead()= default;
};

#endif // !BASIC_READER_HPP