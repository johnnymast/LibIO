#pragma once

#ifdef _WIN32
    #ifdef LIBIO_EXPORTS
        #define LIBIO_API __declspec(dllexport)
    #else
        #define LIBIO_API __declspec(dllimport)
    #endif
#else
    #define LIBIO_API
#endif

