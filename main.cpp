#include "neody/neocore.h"

int main() {

    Router router;
    router.setPort(3000);

    router.get("/verify",{ [&](Query &http) {

        auto params = http.body.getParams();

        std::cout << "total params: " << http.body.total_params() << std::endl;

        string nombre = params.get("nombre").value;


        if (nombre  == "kevin"){
            http.html("<h1> hola kevin </h1>");
        } else{
            http.html("<p> otro usuario </p>");
        }


    }});


    router.get("/",{ [&](Query &http) {
        http.readFile("./index.html", "text/html");
    }});


    router.listen();

    return 0;
}