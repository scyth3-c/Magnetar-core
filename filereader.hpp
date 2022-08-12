#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <memory>
#include <string>
#include <fstream>

using std::string, std::make_shared, std::shared_ptr;

class Files_t
{
private:
    shared_ptr<std::ifstream> reader = nullptr;
public:
    Files_t() { }
    string extract(string path) {
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
    ~Files_t(){}
};

#endif // !FILEREADER_HPP