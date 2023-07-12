#include <iostream>
#include "neody/neocore.h"

int main() {

    Router router;

    router.setPort(3000);

    std::cout << "servidor corriendo en el puerto: "<< router.getPort() << std::endl;


    router.get("/verify",{ [&](Query &http) {
        http.html("<h1>macaco!</h1>");
    }});


    router.get("/",{ [&](Query &http) {
        http.readFile("./index.html", "text/html");
    }});


    router.listen();
    
    return 0;
}