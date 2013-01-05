VERY in progress C++ port of RanvierMUD. Oh god, seriously, don't use this. Seriously, Ranvier sucked enough, imagine how bad a C++ port is.

Build with

    clang++ cran.cpp interp.cpp  server.cpp session.cpp -o cran -lboost_system -lpthread -lanachronism -Wall

Run with

    ./cran <port>

Requirements:

 * libboost-filesystem
 * libboost-thread
 * Anachronism - https://github.com/Twisol/anachronism
