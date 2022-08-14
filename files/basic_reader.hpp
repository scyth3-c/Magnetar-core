#ifndef BASICREADER_HPP
#define BASICREADER_HPP

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
    BasicRead() { }

    string processing(string path) {

        reader = make_shared<std::ifstream>();
        reader->open(path);
        string chunk{""};
        string body{""};
        while(getline(*reader, chunk)){
            body += chunk;
        }
        reader.reset();
        return body;
    }
    ~BasicRead(){}
};

#endif // !BASICREADER_HPP