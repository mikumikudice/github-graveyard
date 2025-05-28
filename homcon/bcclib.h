#define bcclib 1

// Check if string is empty
#define isblnk(val) \
(val[0] == '\n' && strlen(val) == 1) or !strcmp(val, "")

#ifndef __cplusplus

    #define and && // Better operators
    #define or  || // Better operators

    // Fortran-like boolean
    typedef enum boolean {f, t} bool;

    // nil for C
    #define nil ((void *) 0)
    
#else

    // nil for C++
    #define nil __null
#endif