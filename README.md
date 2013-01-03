VERY in progress C++ port of RanvierMUD. Oh god, seriously, don't use this. Seriously, Ranvier sucked enough, imagine how bad a C++ port is.

Build with

    clang++ cran.cpp server.cpp -o cran -lboost_system -lboost_thread -lanachronism

Run with

    ./cran <port>

Requirements:

 * libboost-filesystem
 * libboost-thread
 * Anachronism - https://github.com/Twisol/anachronism
