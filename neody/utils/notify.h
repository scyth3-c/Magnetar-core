#ifndef NOTIFY_HPP
#define NOTIFY_HPP

#include <string>

struct notify_html {
      static std::string noPath() noexcept {
        return "Neody: error no se puedo encontrar la ruta! <br/> Neody: error cant get the path! ";
      }
      static std::string noFIle(const std::string& name) noexcept {
        return "Neody: error no se puedo encontrar el archivo " + name + " <br/> Neody: error cant get the file  " + name;
      }

      static std::string noSafe() noexcept {
        return "Neody: error de sintaxis no <strong>'];'</strong>  <br/> Neody: sintax error whiout closing the tag with <strong>'];'</strong>  ";
      }

      static std::string noSafeData() noexcept {
        return "Neody: error de sintaxis no <strong>']]'</strong>  <br/> Neody: sintax error whiout closing the tag with <strong>']]'</strong>  ";
      }
};

struct notify {
  static std::string noPath(const string& path) noexcept {
    return  "Can't get path file. '" + path + "',  ";
  }
};

#endif // ! NOTIFY_HPP