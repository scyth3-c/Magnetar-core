#include "neody/neocore.h"


int main() {

    Router router;
    router.setPort(8080);


    /* EXAMPLES */

    // root
    router.get("/",{ [&](Query &http) {
        http.readFile("./views/base.html", "text/html");
    }});



    // cpp render in html
    router.get("/cpp",{ [&](Query &http) {
        http.readFileX("./views/cpp.html", "text/html");
    }});


    // request parameters get
    router.get("/parameters",  {[&](Query &http) {
        auto list = http.body.getParameters();
        http.send(list.get("kaio").value);
    }});


    // request parameters x-www-form-urlencoded
    router.post("/parameters",  {[&](Query &http) {
        auto list = http.body.getParameters();
        http.send(list.get("kaio").value);
    }});


    // request parameters text/plain, application/json and etc.

    router.post("/parameters/plain",  {[&](Query &http) {
        auto list = http.body.getParameters();
        auto plain = list.get("data"); // 'data' for a general data

    http.send(plain.value);
}});




    //request headers
    router.get("/headers",  {[&](Query &http) {
        auto list = http.body.getHeaders();
        http.send(list.get("kaio").value);
    }});



    // response headers
    router.get("/headers",{ [&](Query &control) {

        HEADERS my_headers = {
            "header-1: value",
            "header-2: value",
            "header-3: value",
            "header-N: value"
        };
        control.setHeaders(my_headers);
        control.send("Hi!");
    }});




    // response headers inline
    router.get("/headers2", {[](Query &control){
            control.setHeaders("header-1: value");
            control.send("hello world");
        }
    });



    // Infinite middlewares
    // localhost:8080/verify?id=22&nombre=kevin

    const string id = "22";
    router.get("/verify",{
       [&](Query &http) {

           const JSON_s error = {"error", "no id"},
                  invalid = {"error", "invalid"};

           auto params = http.body.getParameters();

           if (!params.exist("id"))
               http.json(error());

           if (params.get("id").value == id)
               http.next();
           else
               http.json(invalid());
       },

        [&](Query &http) {

              http.html("<h1> hola "+  http.body.getParameters().get("nombre").value  +" </h1>");

     }}
    );




    // CALLBACKS

    string data = "DATA";
    router.get("/callbacks", {[&](Query &http) {

        http.send(data, [&]() {
            // callback
            data  = "SENT";
        });

    }});




    // data template rendering
router.get("/datarender", {[&](Query &http) {

  http.render("./views/template.html", [&](dataRender &Pack) {

            Pack("name", "kevin");
            Pack("fruit", "oranges");

            return Pack;
        });
    }});




    // compose multi-part html system
    router.get("/compose", {[&](Query &http) {

        int modules_to_render = 1;
        http.compose("./views/template.html", modules_to_render);

    }});




    // return
    router.get("/return",{
    [](Query &control) {

    auto params = control.body.getParameters();

    if(params.exist("id"))    return control.send(params.get("id").value    );
    if(params.exist("param")) return control.send(params.get("param").value);
    if(params.exist("other")) return control.send(params.get("other").value);
    if(params.exist("xd"))    return control.send(params.get("xd").value    );

    control.html("<h1>error!<h1/>", 400);

    }});



    // native json system, recommended not to use
    router.get("/json",{ [&](Query &control) {

        const JSON_s token = {
            "id", "54",
            "token", "asd"
        },

        kevin = {
            "name", "kevin",
            "lastname", "bohorquez",
            "age", "21",
            "token", token()
        },

        dev = {
            "ficha", "123",
            "lang",  "c++",
            "dev", kevin()
        };

        control.json(dev());
    }});







    router.listen();

    return 0;
}