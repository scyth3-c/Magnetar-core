#ifndef UTILITY_HPP
#define UTILITY_HPP


constexpr const char* BASE = "#include <iostream> \n int main() { \n ";
constexpr const char* NEOBASE = "#include \"neobase.h\" \n #include <iostream> \n int main() { \n ";

constexpr const char* WORK_PATH = "./utils/work_space/temp_";
constexpr const char* TEMPLATE_NEO = "./utils/work_space/base.html";
constexpr char CODE_LOCATE = '#';

constexpr const char* _GPP_  = "g++ ";
constexpr const char* _CPP_  = ".cpp";
constexpr const char* _TXX_  = ".txt";
constexpr const char* _OUT   = " -o ";
constexpr const char* _AFTER = " && ./";
constexpr const char* _DATA  = " > ";
constexpr const char* _CODE_END = "\n return 0;\n}";

constexpr const char* _OPEN = "#[";
constexpr const char* _CLOSE = "];";

constexpr const char* _OPEN_DATA = "[[";
constexpr const char* _CLOSE_DATA = "]]";





#endif // ! UTILITY_HPP