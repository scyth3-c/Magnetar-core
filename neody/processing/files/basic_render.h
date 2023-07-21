
#ifndef BASIC_READER_HPP
#define BASIC_READER_HPP

#include <memory>
#include <string>
#include <fstream>

using std::string;
using std::make_shared;
using std::shared_ptr;

class BasicRead {
private:
    shared_ptr<std::ifstream> reader = nullptr;
public:
    BasicRead() = default;

    string processing(const string& path) {

        reader = make_shared<std::ifstream>();
        reader->open(path);
        string chunk;
        string body;
        while(getline(*reader, chunk)){
            body += chunk;
        }
        reader.reset();
        return body;
    }
    ~BasicRead()= default;
};

#endif // !BASIC_READER_HPP