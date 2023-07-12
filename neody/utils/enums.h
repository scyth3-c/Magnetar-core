//
// Created by scythe on 5/07/23.
//

#ifndef DEPLOYOCEAN_ENUMS_H
#define DEPLOYOCEAN_ENUMS_H

namespace enums {

    class neo {
    public:
        enum eReturn {
            OK = 0,
            ERROR = -1,
            NA = 2,
            UKNOW = 3
        };
        enum eSize {
            BUFFER = 2048,
            SESSION = 1,
            DEF_PORT = 3000,
            DEF_REG = 0,
            MIN_PORT = 1000
        };
        enum eStatus {
            START = 1,
            STOP = 0
        };
    };

} // enums

#endif //DEPLOYOCEAN_ENUMS_H
