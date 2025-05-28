#include <stdio.h>
#include <stdlib.h>

#include "homc.h"
#include <sys/time.h>

#define SZMAX 40000

char code[SZMAX / 16][16];
char slots[64][4];

u8   lblc = 0;
char lbls[127][4];

u8 binc, intc, fltc, chrc;

// File pointer
FILE * fptr = nil;

int main(int argc, char ** args){
    
    struct timeval crnt, oldt;
    gettimeofday(&oldt, nil);

    puts("Compiling game code...");

    // Init all strings
    for(u8 i = 0; i < 127; i++){

        *lbls[i] = "";
    }

    // Nullity checker
    if(args[1] == nil or isblnk(args[1])){
        
        cmperr("No file name was given", 0);

    } else fptr = fopen(args[1], "r");

    if(fptr == nil) cmperr("Given file does not exist", 0);

    // Current line index
    u8 lmax = 0;

    // Get lines
    for(char _eof = fgetc(fptr);
        _eof != EOF and lmax < SZMAX / 16;
        _eof = fgetc(fptr)){

        // Undo fgetc
        ungetc(_eof, fptr);

        char plhd[128];

        // Read current line
        fgets(&plhd, 128, fptr);

        // Avoid breaking long lines
        stpncpy(code[lmax], plhd, 16);

        // Repeat space indicator
        bool s = f;

        // Only do for non blank lines
        if(!isblnk(code[lmax])){

            // Fix spaces
            for(u8 c = 0; c < strlen(code[lmax]) and code[lmax][c] != '\0'; c++){

                // Stucked in a space char
                if(isspace(code[lmax][c]) and code[lmax][c] != '\n') {
                    
                    // Break if repeats
                    if(s)
                    memmove(&code[lmax][c], &code[lmax][c + 1], c - 1);

                    else s = t;

                // Remove breaklines
                } else if(code[lmax][c] == '\n'){
                    
                    memmove(&code[lmax][c], &code[lmax][c + 1], c - 1);
                
                } else s = f;
            }
        }

        // Next line
        lmax++;
    }

    // Close file
    fclose(fptr);

    // Store original name
    char * fname = malloc(strlen(args[1]));
    strcpy(fname, args[1]);

    // Change arg extention to bytecode (.bc)
    args[1][strlen(args[1]) - 2] = 'b';

    // Open bytecode
    fptr = fopen(args[1], "w");

    // Precompilation
    for(u8 l = 0; l < lmax; l++){
        
        // Skip comments
        if(code[l][0] == ';' or isblnk(code[l])) continue;

        // Check syntax
        for(u8 c = 0; c < strlen(code[l]); c++){

            if(!isspace(code[l][c]) and
            (c == 3 or c == 7 or c == 11))
            cmperr("Syntax error in this operation", l + 1);

            if(isspace(code[l][c]) and
            !(c == 3 or c == 7 or c == 11))
            cmperr("Syntax error in this operation", l + 1);
        }

        // Operation keyword
        char opr[4] = {code[l][0], code[l][1], code[l][2], '\0'};

        // Jump labels
        if(!strcmp(opr, "lbl")){

            char name[4] = {code[l][4], code[l][5], code[l][6] , '\0'};

            // Avoid overflows
            if(lblc > 125)
            cmperr("The code arrived the max number of labels", l + 1);

            if(!deflbl(name))

                strcpy(lbls[++lblc], name);

            else cmperr("Attempt to override a label definition", l + 1);
        }
    }

    // Compilation
    u8 tlin = 0;
    for(u8 l = 0; l < lmax; l++){

        // Skip comments
        if(code[l][0] == ';' or isblnk(code[l])) continue;
        tlin++;

        // Operation keyword
        char opr[4] = {code[l][0], code[l][1], code[l][2], '\0'};

        // Memory slot definition
        if(!strcmp(opr, "def")){

            // def var idx
            if(strlen(code[l]) > 11){
                
                char * msg = malloc(32);
                sprintf(msg, "Syntax error in \"%s\" operator", opr);

                cmperr(msg, l + 1);
            }

            char indx[4] = {code[l][4], code[l][5], code[l][6],  '\0'};
            char name[4] = {code[l][8], code[l][9], code[l][10], '\0'};
            
            // Invalid addresses
            if(atoi(indx) > 63)
            cmperr("Memory address out of range", l + 1);

            u8 adrs = atoi(indx);

            // Check for pleonasm
            if(strlen(slots[adrs]) == 0){
                
                strcpy(slots[adrs], name);
                continue;
                
            } else cmperr("Attempt to override a address definition", l + 1);
        }
        
        // Memory manangement
        else if(!strcmp(opr, "del")){

            if(strlen(code[l]) > 7){
                
                char * msg = malloc(32);
                sprintf(msg, "Syntax error in \"%s\" operator", opr);

                cmperr(msg, l + 1);
            }

            bool fndd = f;
            char name[4] = {code[l][4], code[l][5], code[l][6],  '\0'};

            // Name was defined
            if(!hasdef(name))
            cmperr("Attempt to delete a unknown address' name", l + 1);

            // Invalid addresses
            if(getdef(name) == 0)
            cmperr("The 000 memory address is reserved by the system", l + 1);
        }

        // Variable creation
        else if(!strcmp(opr, "int")
             or !strcmp(opr, "chr")
             or !strcmp(opr, "flt")
             or !strcmp(opr, "mov")){

            // opr var val val
            char name[4] = {code[l][4] , code[l][5] , code[l][6], '\0'};
            char vall[8] = {code[l][8] , code[l][9] , code[l][10],
                            code[l][12], code[l][13], code[l][14], '\0'};

            // Name was defined
            if(!hasdef(name))
            cmperr("Attempt to use a unknown address' name", l + 1);

            // Check value
            if(!atoi(vall) and strcmp(vall, "000000") and !hasdef(vall))
            cmperr("Invalid number for initialization", l + 1);

            // Tell compiler how much space is needed     
            if(!strcmp(opr, "int")) intc++;
            else if(!strcmp(opr, "chr")) chrc++;
            else fltc++;
        }
        
        // Arithmetic operations
        else if(!strcmp(opr, "sum")
             or !strcmp(opr, "sub")
             or !strcmp(opr, "mul")
             or !strcmp(opr, "div")
             or !strcmp(opr, "mod")
             
             // Binary operators
             or !strcmp(opr, "bin")
        
             // Comparison operations
             or !strcmp(opr, "grt")
             or !strcmp(opr, "sml")
             or !strcmp(opr, "gte")
             or !strcmp(opr, "sle")
             or !strcmp(opr, "eql")
             or !strcmp(opr, "dif")

             // Boolean operations
             or !strcmp(opr, "and")
             or !strcmp(opr, "_or")){

            // opr var val val
            char name[4] = {code[l][4] , code[l][5] , code[l][6] , '\0'};
            char val1[4] = {code[l][8] , code[l][9] , code[l][10], '\0'};
            char val2[4] = {code[l][12], code[l][13], code[l][14], '\0'};

            // Name was defined
            if(!hasdef(name))
            cmperr("Attempt to use a unknown address' name", l + 1);

            // Check values
            if(!atoi(val1) and strcmp(val1, "000") and !hasdef(val1)){

                cmperr("Invalid value at left", l + 1);
            
            } else if(!atoi(val2) and strcmp(val2, "000") and !hasdef(val2)){

                cmperr("Invalid value at right", l + 1);
            }
        }
        
        else if(!strcmp(opr, "not")){

            // not var val
            if(strlen(code[l]) > 11){
                
                char * msg = malloc(32);
                sprintf(msg, "Syntax error in \"%s\" operator", opr);

                cmperr(msg, l + 1);
            }

            char name[4] = {code[l][4] , code[l][5] , code[l][6] , '\0'};
            char vall[4] = {code[l][8] , code[l][9] , code[l][10], '\0'};

            if(!hasdef(name))
            cmperr("Attempt to use a unknown address' name", l + 1);

            // Check values
            if(!atoi(vall) and strcmp(vall, "000000"))
            cmperr("Invalid value to operate", l + 1);
        
        }
        
        else if(!strcmp(opr, "ifs")   // Logic jump
             or !strcmp(opr, "get")   // Graphics
             or !strcmp(opr, "pxl")
             or !strcmp(opr, "snd")){ // Sound

            // opr val val val
            char val1[4] = {code[l][4] , code[l][5] , code[l][6] , '\0'};
            char val2[4] = {code[l][8] , code[l][9] , code[l][10], '\0'};
            char val3[4] = {code[l][12], code[l][13], code[l][14], '\0'};

            // Check values
            if(!atoi(val1) and strcmp(val1, "000") and !hasdef(val1)){

                if(!strcmp(opr, "ifs"))
                cmperr("Invalid value to evaluate", l + 1);

                else
                cmperr("Invalid value in arg #1", l + 1);
            
            } else if(!atoi(val2) and strcmp(val2, "000") and !hasdef(val2)){

                if(!strcmp(opr, "ifs")){

                    if(!deflbl(val2))
                    cmperr("Invalid true-case line", l + 1);
                }

                else
                cmperr("Invalid value in arg #2", l + 1);

            } else if(!atoi(val3) and strcmp(val3, "000") and !hasdef(val3)){

                if(!strcmp(opr, "ifs")){
                
                    if(!deflbl(val3))
                    cmperr("Invalid false-case line", l + 1);
                }

                else
                cmperr("Invalid value in arg #3", l + 1);
            }
        }
        
        else if(!strcmp(opr, "lbl")){

            // Do nothing
        }

        // Clear screen
        else if(!strcmp(opr, "cls")){

            // cls col
            if(strlen(code[l]) > 7){
                
                char * msg = malloc(32);
                sprintf(msg, "Syntax error in \"%s\" operator", opr);

                cmperr(msg, l + 1);
            }

            char vall[4] = {code[l][4] , code[l][5] , code[l][6], '\0'};

            // Check values
            if(!(!strcmp(vall, "000") ? 1 : atoi(vall)) and !hasdef(vall))
            cmperr("Invalid color", l + 1);

            u8 coll = hasdef(vall) ? 0 : (u8)vall;

            // Check color number
            if(!(0 <= coll and coll < 16))
            cmperr("Color out of range", l + 1);
        }
        
        // Program exit
        else if(strcmp(opr, "ext"))
        cmperr("Invalid operation or keyword", l + 1);

        char * copy = malloc(4); u8 byte;
        strcpy(copy, code[l]);

        char * word[4], * cwrd = strtok(copy, " ");

        // Break into words to write at file
        for(u8 w = 0; w < 4 and cwrd != nil; w++){
            
            word[w] = cwrd;
            cwrd = strtok(NULL, " ");
        }

        // Is first side of an literal integer
        bool is_frst = f;

        // If last operator is an "opr add val val"
        bool last_is = f;

        // Switch word by bytecode
        for(u8 w = 0; w < 4 && word[w] != nil; w++){

            if(!strcmp(word[w], "del")) byte = 0b010000;
            else if(!strcmp(word[w], "mov")) byte = 0b010001;
            else if(!strcmp(word[w], "cls")) byte = 0b010010;
            else if(!strcmp(word[w], "get")) byte = 0b010011;
            else if(!strcmp(word[w], "pxl")) byte = 0b010100;
            else if(!strcmp(word[w], "snd")) byte = 0b010101;

            else if(!strcmp(word[w], "sum")) byte = 0b000001;
            else if(!strcmp(word[w], "sub")) byte = 0b000010;
            else if(!strcmp(word[w], "mul")) byte = 0b000011;
            else if(!strcmp(word[w], "div")) byte = 0b000100;
            else if(!strcmp(word[w], "mod")) byte = 0b000101;

            else if(!strcmp(word[w], "bin")) byte = 0b000110;

            else if(!strcmp(word[w], "grt")) byte = 0b000111;
            else if(!strcmp(word[w], "sml")) byte = 0b001000;
            else if(!strcmp(word[w], "gte")) byte = 0b001001;
            else if(!strcmp(word[w], "sle")) byte = 0b001010;
            else if(!strcmp(word[w], "eql")) byte = 0b001011;
            else if(!strcmp(word[w], "dif")) byte = 0b001100;
            else if(!strcmp(word[w], "and")) byte = 0b001101;
            else if(!strcmp(word[w], "_or")) byte = 0b001110;
            else if(!strcmp(word[w], "not")) byte = 0b001111;

            else if(!strcmp(word[w], "ifs")) byte = 0b110000;
            else if(!strcmp(word[w], "jmp")) byte = 0b110001;
            else if(!strcmp(word[w], "lbl")) byte = 0b110010;

            else if(!strcmp(word[w], "int")) byte = 0b100001;
            else if(!strcmp(word[w], "flt")) byte = 0b100010;
            else if(!strcmp(word[w], "chr")) byte = 0b100011;

            else if(!strcmp(word[w], "ext")) byte = 0b110011;

            else if(hasdef(word[w])) byte = getdef(word[w]);
            else {

                //Placeholder
                u8 * phr = malloc(sizeof(u8));

                // Is an label index
                if(deflbl(word[w]))

                    byte = getlbl(word[w]);

                // Is a numeric value
                else if(sscanf(word[w], "%d", phr)){

                    // Type value
                    if(last_is)
                    if(is_frst){
                        
                        char sraw[8];
                        bool sign = 0;

                        sprintf(sraw, "%s%s", word[w], word[w + 1]); // Strings to number
                        sscanf(sraw, "%d", phr);                     // Stract number
                        sign = *phr < 0;

                        sprintf(sraw, "%04x", *phr);                 // Number to hex
                        sprintf(sraw, "%c%c", sraw[0], sraw[1]);     // Split hex

                        sscanf(sraw, "%x", phr);

                        // Fix signal
                        if(sign) *phr = *phr + 0x10; 

                        byte = *phr;
                        is_frst = f;

                    } else {

                        char sraw[8];

                        sprintf(sraw, "%s%s", word[w - 1], word[w]); // Strings to number
                        sscanf(sraw, "%d", phr);                     // Stract number
                        sprintf(sraw, "%04x", *phr);                 // Number to hex

                        sprintf(sraw, "%c%c", sraw[2], sraw[3]);     // Split hex
                        sscanf(sraw, "%x", phr);

                        byte = *phr;
                        last_is = f;
                    }

                    // 3 Digits number
                    else {

                        char sraw[8];

                        strcpy(sraw, word[w]);

                        sscanf(sraw, "%d", phr);     // Stract number
                        sprintf(sraw, "%04x", *phr); // Number to hex

                        sscanf(sraw, "%x", phr);

                        byte = *phr;
                        last_is = f;
                    }

                // Somwthing else
                } else sprintf(&byte, "%d", (u8)word[w]);
            }

            // Check when it is a slot assignment
            if(byte >= 33 and byte <= 35){

                last_is = t;
                is_frst = t;
            }

            // Write at file
            fprintf(fptr, "%c", byte);
        }

        fprintf(fptr, "\n");
    }

    // Write malloc data
    fprintf(fptr, "%c%c%c%c",
    tlin * 4, lblc, intc + fltc + chrc, 0);
    
    fclose(fptr);
    fptr = nil;

    gettimeofday(&crnt, nil);

    f8 dt =
    (crnt.tv_sec - oldt.tv_sec) * 1000000 + crnt.tv_usec - oldt.tv_usec;

    printf("%s compiled into %s successfully in %.3fs\n",
    fname, args[1], dt / 1000);

    return 0;
}

void cmperr(char * err, u8 lin){

    if(lin > 0)
    
        printf("\n[ERROR]\n%s\n\n\t%d | %s\n\nExit program with error code %d\n",
        err, lin, code[lin - 1], err);

    else
        
        printf("\n[ERROR]\n%s\nExit program with error code %d\n",
        err, err);

    exit(-1);
}

bool deflbl(char lbl[]){

    bool fndd = f;

    // Check all slots
    for(u8 s = 0; s < 127; s++){

        if(!strcmp(lbls[s], lbl)){

            fndd = t;
            break;
        }
    }

    return fndd;
}

s8 getlbl(char lbl[]){

    // Check all slots
    for(u8 s = 0; s < 127; s++){

        if(!strcmp(lbls[s], lbl))
        return s;
    }

    return -1;
}

bool hasdef(char adr[]){

    bool fndd = f;

    // Check all slots
    for(u8 s = 0; s < 64; s++){

        if(!strcmp(slots[s], adr)) {

            fndd = t;
            break;
        }
    }

    return fndd;
}

s8 getdef(char adr[]){

    // Check all slots
    for(u8 s = 0; s < 64; s++){

        if(!strcmp(slots[s], adr)) return s;
    }

    return -1;
}