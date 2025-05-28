// Rust-like primitive types
#include "rust.h"

// Better C coding
#include "bcclib.h"

// Better SDL2
#include "bsdl.h"

// Slot type
#include "slot_type.c"

// Returns the current allocated size of the given array
#define arrlen(a) sizeof(a[0]) / sizeof(a)
#define prrlen(a) sizeof(*a[0]) / sizeof(a)

// SDL2 SIO
void init(), sdlout();

// SDL Error message
void sdlerr();

// Input
enum sdlk {UP, LFT, DWN, RGT, ESC};

// Color
typedef struct {u8 r, g, b;} coll;
imut coll COLLS[] = {

    {0, 0, 0}, // Black
    {1, 1, 1}, // White
};

//Jump label
typedef struct label {
    
    u8 name, line;
    
} lbl;

bool hasadd(u8 add);

// Returns the index of the slot
u8 addidx(u8 add);

// Runtime error alert
void rnterr(char err[]);

// Converts an number to binary (as an string)
char * itob(u8 n);

// Converts a binary values (as an string) to int
u8 btoi(char * b);