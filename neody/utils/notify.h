#ifndef NOTIFY_HPP
#define NOTIFY_HPP

#include <string>

struct notify_html {
      static std::string noPath() noexcept {
        return "Magnetar dice: error no se puedo encontrar la ruta! <br/> Magnetar says: error cant get the path! ";
      }
      static std::string noFIle(const std::string& name) noexcept {
        return "Magnetar dice: error no se puedo encontrar el archivo " + name + " <br/> Magnetar says: error cant get the file  " + name;
      }

      static std::string noSafe() noexcept {
        return "Magnetar dice: error de sintaxis no cerro ];  <br/> Magnetar says: sintax error whiout closing the tag with ];  ";
      }

      static std::string noSafeData() noexcept {
        return "Magnetar dice: error de sintaxis no cerro ]]  <br/> Magnetar says: sintax error whiout closing the tag with ]]  ";
      }
};

#endif // ! NOTIFY_HPP