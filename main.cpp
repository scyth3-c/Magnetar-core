#include "neody/neocore.h"
#include <iostream>

int main() {

    Router router(3000);

    std::string token = "12345";


    // test
    // http://localhost:3000/verify?id=12345&nombre=neodimio


    router.get("/verify",{

       [&](Query &http) {

           JSON_s error = {"error", "no id"};
           JSON_s invalid = {"error", "invalid"};

           auto params = http.body.getParams();

           if (!params.exist("id")) {
               http.json(error());
           }

           if (params.get("id").value == token){
               http.next();
           } else{
               http.json(invalid());
           }

           },

        [&](Query &http) {

             auto params = http.body.getParams();
             string nombre = params.get("nombre").value;

             if (nombre.length() > 5)
                http.html("<h1> hola "+  nombre  +" </h1>");
             else
                http.html("<p> adios </p>");

     }}
    );



    router.get("/",{ [&](Query &http) {
        http.readFile("./index.html", "text/html");
    }});


    router.listen();

    return 0;
}