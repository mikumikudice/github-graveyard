// Rust-like primitive types
#include "rust.h"

// Better C coding
#include "bcclib.h"

// Better SDL2
#include "bsdl.h"

// Compiling error message out
void cmperr(char err[], u8 lin);

s8 getlbl(char lbl[]);
s8 getdef(char adr[]);

bool hasdef(char * adr), deflbl(char lbl[]);