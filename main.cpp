#include "neody/neocore.h"

int main() {

    Router router;
    router.setPort(8080);



    router.get("/",{ [&](Query &http) {
        http.readFile("./views/index.html", "text/html");
    }});


    router.get("/cpp",{ [&](Query &http) {
        http.readFileX("./views/cpp.html", "text/html");
    }});



    std::string id = "22";

    router.get("/verify",{
       [&](Query &http) {

           JSON_s error = {"error", "no id"};
           JSON_s invalid = {"error", "invalid"};

           auto params = http.body.getParams();


           if (!params.exist("id")) {
               http.json(error());
           }

           if (params.get("id").value == id){
               http.next();
            } else{
               http.json(invalid());
            }

           },

        // method 2
        [&](Query &http) {

             auto params = http.body.getParams();
             string nombre = params.get("nombre").value;

             if (nombre.length() > 5)
                http.html("<h1> hola "+  nombre  +" </h1>");
             else
                http.html("<p> adios </p>");

     }}
    );




    router.get("/user",{  [&](Query &http) {
        JSON_s data = {
        "lang", "c++",
            "role", "Full-Stack",
            "github", "github.com/scyth3-c"
        };
        JSON_s user = {
            "name", "kevin",
            "lastname", "bohorquez",
            "data", data()
        };

        http.json(user());
    }});


    router.listen();

    return 0;
}