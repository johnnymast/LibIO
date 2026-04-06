#pragma once

#if defined(_WIN32) || defined(_WIN64)
    #ifdef LIBIO_BUILD
        #define LIBGRAPHICS_API __declspec(dllexport)
    #else
        #define LIBGRAPHICS_API __declspec(dllimport)
    #endif
#else
    #ifdef LIBIO_BUILD
        #define LIBGRAPHICS_API __attribute__((visibility("default")))
    #else
        #define LIBGRAPHICS_API
    #endif
#endif
