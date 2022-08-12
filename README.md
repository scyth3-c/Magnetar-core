# Magnetar-core
Magnetar core, visit [scyth3-c/Magnetar](https://github.com/scyth3-c/Magnetar)

Requeriments:
| system          | compiler            |     ram     |
| :---            |     :---:           |        ---: |
| linux           |  g++                |   195 KB    |


## Latest changes

- now you can render cpp code embedded in html files
- add: post, put, deletex, patch, head, options, link, unlink and purgue with x-www-form-urlconded, geturl, plain/text params each one of them
- added POST and PUT routes with x-www-form-urlencoded and plain/text



## compile and use

```
  make:
  
    $ make 
    $ ./Magnetar
    
  normal:
    
    $ g++ -std=c++17 -pthread main.cpp -o Magnetar
    $ ./Magnetar

```
