#include <stdio.h>
#include <stdlib.h>

#include "main.h"

#define SCALE 8    // Rendering scale
#define SZMAX 4000 // Max supported size

// Runtime data
u8 * code, cmax = 0, cchr = 0, copr;

//Jump-line labels
u8 * lbls, lblc = 0;

// Console memory
slot ** slots;
u8   cslt = 0;

// File
FILE * fptr = nil;

int main(int argc, char ** args){

    // Nullity checker
    if(args[1] == nil or isblnk(args[1])){
        
        rnterr("No file name was given");

    } else fptr = fopen(args[1], "r");

    // Open file
    if(fptr == nil){
        
        puts("Given file does not exist");
        exit(-1);
    }

    // Temp alloc
    code = malloc(SZMAX * sizeof(u8));
    lbls = malloc(127   * sizeof(u8));

    /* Get bytecode */ {

        for(char c = fgetc(fptr); c != EOF; c = fgetc(fptr)){

            //lbl
            if(c == 0b110010 && cchr % 5 == 0){

                lbls[lblc++] = cchr;
            }

            if(c != '\n') code[cmax++] = c;
        }

        // Free memory
        fclose(fptr); fptr = nil;

        // Set to minimum size
        u8 _code[cmax];
        u8 _lbls[lblc];

        // Store at placeholders
        for(u8 c = 0; c < cmax; c++){

            _code[c] = code[c];
        }

        for(u8 l = 0; l < lblc; l++){

            _lbls[l] = lbls[l];
        }

        // Set to minimum size
        realloc(code, code[cmax - 4] * sizeof(u8));
        realloc(lbls, code[cmax - 3] * sizeof(u8));

        // Restore values
        for(u8 c = 0; c < cmax; c++){

            code[c] = _code[c];
        }

        for(u8 l = 0; l < lblc; l++){

            lbls[l] = _lbls[l];
        }

        // Get total runtime memory
        slots = malloc(code[cmax - 1]);
        
        // Avoid fragmentation
        for(u8 i = 0; i < arrlen(slots); i++){

            memcpy(slots[i], &EMPTY, sizeof(slot));
        }
    }

    puts("Running byte code...");

    // Flow control
    bool quit = f, jmpd = f;

    while(!quit && cchr < cmax){

        jmpd = f;
        copr = code[cchr];

        // Match operator
        switch(copr){
        
            // del
            case(0b010000):{

                //del add

                u8 add = addidx(code[cchr + 1]);
                memcpy(slots[add], &EMPTY, sizeof(slot));

                break;
            }

            // mov
            case(0b010001):

                break;

            // cls
            case(0b010010):

                break;

            // get
            case(0b010011):

                break;

            // pxl
            case(0b010100):

                break;

            // snd
            case(0b010101):

                break;

            // sum
            case(0b000001):{

                // sum add val val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt, * rslt;
                s8 lraw, rraw;

                bool lisr = f, risr = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Another variable
                if(hasadd(code[cchr + 3])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 3]);
                    
                    rslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 3]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Left is a variable
                if(!lisr){

                    // Both are variables
                    if(!risr){

                        slot_sum(lslt, rslt);
                        
                        memcpy(add, lslt, sizeof(slot));

                        // Undo operation
                        slot_sub(lslt, rslt);
                    
                    // Only left is
                    } else {
                        
                        slot_rsum(lslt, rraw);
                        add->val.vint = lslt->val.vint;

                        // Undo operation
                        slot_rsub(lslt, rraw);
                    }

                } else {

                    // Only right is a variable
                    if(!risr){

                        slot hold = {0, "int", lraw};
                        *add = *slot_sum(&hold, rslt);
                    
                    // None of them is a variable
                    } else {
                        
                        add->val.vint = lraw;
                        slot_rsum(add, rraw);
                    }
                }

                break;
            }

            // sub
            case(0b000010):{

                // sub add val val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt, * rslt;
                s8 lraw, rraw;

                bool lisr = f, risr = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Another variable
                if(hasadd(code[cchr + 3])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 3]);
                    
                    rslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 3]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Left is a variable
                if(!lisr){

                    // Both are variables
                    if(!risr){

                        slot_sub(lslt, rslt);
                        
                        memcpy(add, lslt, sizeof(slot));

                        // Undo operation
                        slot_sum(lslt, rslt);
                    
                    // Only left is
                    } else {
                        
                        slot_rsub(lslt, rraw);
                        add->val.vint = lslt->val.vint;

                        // Undo operation
                        slot_rsum(lslt, rraw);
                    }

                } else {

                    // Only right is a variable
                    if(!risr){

                        slot hold = {0, "int", lraw};
                        *add = *slot_sub(&hold, rslt);
                    
                    // None of them is a variable
                    } else {
                        
                        add->val.vint = lraw;
                        slot_rsub(add, rraw);
                    }
                }

                break;
            }

            // mul
            case(0b000011):{

                // mul add val val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt, * rslt;
                s8 lraw, rraw;

                bool lisr = f, risr = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Another variable
                if(hasadd(code[cchr + 3])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 3]);
                    
                    rslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 3]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Left is a variable
                if(!lisr){

                    // Both are variables
                    if(!risr){

                        slot_mul(lslt, rslt);
                        
                        memcpy(add, lslt, sizeof(slot));

                        // Undo operation
                        slot_div(lslt, rslt);
                    
                    // Only left is
                    } else {
                        
                        slot_rmul(lslt, rraw);
                        add->val.vint = lslt->val.vint;

                        // Undo operation
                        slot_rdiv(lslt, rraw);
                    }

                } else {

                    // Only right is a variable
                    if(!risr){

                        slot hold = {0, "int", lraw};
                        *add = *slot_mul(&hold, rslt);
                    
                    // None of them is a variable
                    } else {
                        
                        add->val.vint = lraw;
                        slot_rmul(add, rraw);
                    }
                }

                break;
            }

            // div
            case(0b000100):{

                // div add val val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt, * rslt;
                s8 lraw, rraw;

                bool lisr = f, risr = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Another variable
                if(hasadd(code[cchr + 3])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 3]);
                    
                    rslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 3]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Left is a variable
                if(!lisr){

                    // Both are variables
                    if(!risr){

                        slot_div(lslt, rslt);
                        
                        memcpy(add, lslt, sizeof(slot));

                        // Undo operation
                        slot_mul(lslt, rslt);
                    
                    // Only left is
                    } else {
                        
                        slot_rdiv(lslt, rraw);
                        add->val.vint = lslt->val.vint;

                        // Undo operation
                        slot_rmul(lslt, rraw);
                    }

                } else {

                    // Only right is a variable
                    if(!risr){

                        slot hold = {0, "int", lraw};
                        *add = *slot_div(&hold, rslt);
                    
                    // None of them is a variable
                    } else {
                        
                        add->val.vint = lraw;
                        slot_rdiv(add, rraw);
                    }
                }

                break;
            }
                
            // mod
            case(0b000101):{

                // mod add val val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt, * rslt;
                s8 lraw, rraw;

                bool lisr = f, risr = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Another variable
                if(hasadd(code[cchr + 3])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 3]);
                    
                    rslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 3]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Left is a variable
                if(!lisr){

                    // Both are variables
                    if(!risr){

                        slot temp = *lslt;
                        slot_mod(lslt, rslt);
                        
                        memcpy(add, lslt, sizeof(slot));

                        // Undo operation
                        *lslt = temp;
                    
                    // Only left is
                    } else {
                        
                        slot temp = *lslt;

                        slot_rmod(lslt, rraw);
                        add->val.vint = lslt->val.vint;

                        // Undo operation
                        *lslt = temp;
                    }

                } else {

                    // Only right is a variable
                    if(!risr){

                        slot hold = {0, "int", lraw};
                        *add = *slot_mod(&hold, rslt);
                    
                    // None of them is a variable
                    } else {
                        
                        add->val.vint = lraw;
                        slot_rmod(add, rraw);
                    }
                }

                break;
            }

            // bin
            case(0b000110):{

                // bin add idx val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt, * rslt;
                s8 lraw, rraw;

                bool lisr = f, risr = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Another variable
                if(hasadd(code[cchr + 3])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 3]);
                    
                    rslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 3]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Left is a variable
                if(!lisr){

                    // Both are variables
                    if(!risr){
                        
                        u8 idx = slot_getv(lslt);
                        u8 val = slot_getv(rslt);

                        // Keep index under 8
                        if(idx > 8) idx = idx % 8;

                        if(!strcmp(add->mst, "int"))
                        add->val.vint = itob(val)[idx] == '1' ? 1 : 0;

                        if(!strcmp(add->mst, "flt"))
                        add->val.vflt = itob(val)[idx] == '1' ? 1 : 0;

                        if(!strcmp(add->mst, "chr"))
                        add->val.vchr = itob(val)[idx];
                    
                    // Only left is
                    } else {
                        
                        u8 idx = slot_getv(lslt);

                        // Keep index under 8
                        if(idx > 8) idx = idx % 8;

                        if(!strcmp(add->mst, "int"))
                        add->val.vint = itob(lraw)[idx] == '1' ? 1 : 0;

                        if(!strcmp(add->mst, "flt"))
                        add->val.vflt = itob(lraw)[idx] == '1' ? 1 : 0;

                        if(!strcmp(add->mst, "chr"))
                        add->val.vchr = itob(lraw)[idx];
                    }

                } else {

                    // Only right is a variable
                    if(!risr){

                        u8 val = slot_getv(rslt);

                        if(rraw > 8) rraw = rraw % 8;

                        if(!strcmp(add->mst, "int"))
                        add->val.vint = itob(val)[rraw] == '1' ? 1 : 0;

                        if(!strcmp(add->mst, "flt"))
                        add->val.vflt = itob(val)[rraw] == '1' ? 1 : 0;

                        if(!strcmp(add->mst, "chr"))
                        add->val.vchr = itob(val)[rraw];
                    
                    // None of them is a variable
                    } else {
                        
                        if(rraw > 8) rraw = rraw % 8;

                        if(!strcmp(add->mst, "int"))
                        add->val.vint = itob(lraw)[rraw] == '1' ? 1 : 0;

                        if(!strcmp(add->mst, "flt"))
                        add->val.vflt = itob(lraw)[rraw] == '1' ? 1 : 0;

                        if(!strcmp(add->mst, "chr"))
                        add->val.vchr = itob(lraw)[rraw];
                    }
                }

                break;
            }
            
            // grt
            case(0b000111):{

                // grt add val val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt, * rslt;
                s8 lraw, rraw;

                bool lisr = f, risr = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Another variable
                if(hasadd(code[cchr + 3])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 3]);
                    
                    rslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 3]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Left is a variable
                if(!lisr){

                    // Both are variables
                    if(!risr){

                        slot temp = *lslt;
                        slot_grt(lslt, rslt);
                        
                        memcpy(add, lslt, sizeof(slot));

                        // Undo operation
                        *lslt = temp;
                    
                    // Only left is
                    } else {
                        
                        slot temp = *lslt;

                        slot_rgrt(lslt, rraw);
                        add->val.vint = lslt->val.vint;

                        // Undo operation
                        *lslt = temp;
                    }

                } else {

                    // Only right is a variable
                    if(!risr){

                        slot hold = {0, "int", lraw};
                        *add = *slot_grt(&hold, rslt);
                    
                    // None of them is a variable
                    } else {
                        
                        add->val.vint = lraw;
                        slot_rgrt(add, rraw);
                    }
                }

                break;
            }

            // sml
            case(0b001000):{

                // grt add val val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt, * rslt;
                s8 lraw, rraw;

                bool lisr = f, risr = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Another variable
                if(hasadd(code[cchr + 3])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 3]);
                    
                    rslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 3]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Left is a variable
                if(!lisr){

                    // Both are variables
                    if(!risr){

                        slot temp = *lslt;
                        slot_sml(lslt, rslt);
                        
                        memcpy(add, lslt, sizeof(slot));

                        // Undo operation
                        *lslt = temp;
                    
                    // Only left is
                    } else {
                        
                        slot temp = *lslt;

                        slot_rsml(lslt, rraw);
                        add->val.vint = lslt->val.vint;

                        // Undo operation
                        *lslt = temp;
                    }

                } else {

                    // Only right is a variable
                    if(!risr){

                        slot hold = {0, "int", lraw};
                        *add = *slot_sml(&hold, rslt);
                    
                    // None of them is a variable
                    } else {
                        
                        add->val.vint = lraw;
                        slot_rsml(add, rraw);
                    }
                }

                break;
            }

            // gte
            case(0b001001):{

                // grt add val val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt, * rslt;
                s8 lraw, rraw;

                bool lisr = f, risr = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Another variable
                if(hasadd(code[cchr + 3])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 3]);
                    
                    rslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 3]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Left is a variable
                if(!lisr){

                    // Both are variables
                    if(!risr){

                        slot temp = *lslt;
                        slot_gte(lslt, rslt);
                        
                        memcpy(add, lslt, sizeof(slot));

                        // Undo operation
                        *lslt = temp;
                    
                    // Only left is
                    } else {
                        
                        slot temp = *lslt;

                        slot_rgte(lslt, rraw);
                        add->val.vint = lslt->val.vint;

                        // Undo operation
                        *lslt = temp;
                    }

                } else {

                    // Only right is a variable
                    if(!risr){

                        slot hold = {0, "int", lraw};
                        *add = *slot_gte(&hold, rslt);
                    
                    // None of them is a variable
                    } else {
                        
                        add->val.vint = lraw;
                        slot_rgte(add, rraw);
                    }
                }

                break;
            }

            // sle
            case(0b001010):{

                // grt add val val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt, * rslt;
                s8 lraw, rraw;

                bool lisr = f, risr = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Another variable
                if(hasadd(code[cchr + 3])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 3]);
                    
                    rslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 3]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Left is a variable
                if(!lisr){

                    // Both are variables
                    if(!risr){

                        slot temp = *lslt;
                        slot_sml(lslt, rslt);
                        
                        memcpy(add, lslt, sizeof(slot));

                        // Undo operation
                        *lslt = temp;
                    
                    // Only left is
                    } else {
                        
                        slot temp = *lslt;

                        slot_rsml(lslt, rraw);
                        add->val.vint = lslt->val.vint;

                        // Undo operation
                        *lslt = temp;
                    }

                } else {

                    // Only right is a variable
                    if(!risr){

                        slot hold = {0, "int", lraw};
                        *add = *slot_sml(&hold, rslt);
                    
                    // None of them is a variable
                    } else {
                        
                        add->val.vint = lraw;
                        slot_rsml(add, rraw);
                    }
                }

                break;
            }

            // eql
            case(0b001011):{

                // grt add val val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt, * rslt;
                s8 lraw, rraw;

                bool lisr = f, risr = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Another variable
                if(hasadd(code[cchr + 3])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 3]);
                    
                    rslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 3]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Left is a variable
                if(!lisr){

                    // Both are variables
                    if(!risr){

                        slot temp = *lslt;
                        slot_eql(lslt, rslt);
                        
                        memcpy(add, lslt, sizeof(slot));

                        // Undo operation
                        *lslt = temp;
                    
                    // Only left is
                    } else {
                        
                        slot temp = *lslt;

                        slot_reql(lslt, rraw);
                        add->val.vint = lslt->val.vint;

                        // Undo operation
                        *lslt = temp;
                    }

                } else {

                    // Only right is a variable
                    if(!risr){

                        slot hold = {0, "int", lraw};
                        *add = *slot_eql(&hold, rslt);
                    
                    // None of them is a variable
                    } else {
                        
                        add->val.vint = lraw;
                        slot_reql(add, rraw);
                    }
                }

                break;
            }

            // dif
            case(0b001100):{

                // grt add val val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt, * rslt;
                s8 lraw, rraw;

                bool lisr = f, risr = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Another variable
                if(hasadd(code[cchr + 3])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 3]);
                    
                    rslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 3]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Left is a variable
                if(!lisr){

                    // Both are variables
                    if(!risr){

                        slot temp = *lslt;
                        slot_dif(lslt, rslt);
                        
                        memcpy(add, lslt, sizeof(slot));

                        // Undo operation
                        *lslt = temp;
                    
                    // Only left is
                    } else {
                        
                        slot temp = *lslt;

                        slot_rdif(lslt, rraw);
                        add->val.vint = lslt->val.vint;

                        // Undo operation
                        *lslt = temp;
                    }

                } else {

                    // Only right is a variable
                    if(!risr){

                        slot hold = {0, "int", lraw};
                        *add = *slot_dif(&hold, rslt);
                    
                    // None of them is a variable
                    } else {
                        
                        add->val.vint = lraw;
                        slot_rdif(add, rraw);
                    }
                }

                break;
            }

            // and
            case(0b001101):{

                // grt add val val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt, * rslt;
                s8 lraw, rraw;

                bool lisr = f, risr = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Another variable
                if(hasadd(code[cchr + 3])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 3]);
                    
                    rslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 3]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Left is a variable
                if(!lisr){

                    // Both are variables
                    if(!risr){

                        slot temp = *lslt;
                        slot_and(lslt, rslt);
                        
                        memcpy(add, lslt, sizeof(slot));

                        // Undo operation
                        *lslt = temp;
                    
                    // Only left is
                    } else {
                        
                        slot temp = *lslt;

                        slot_rand(lslt, rraw);
                        add->val.vint = lslt->val.vint;

                        // Undo operation
                        *lslt = temp;
                    }

                } else {

                    // Only right is a variable
                    if(!risr){

                        slot hold = {0, "int", lraw};
                        *add = *slot_and(&hold, rslt);
                    
                    // None of them is a variable
                    } else {
                        
                        add->val.vint = lraw;
                        slot_rand(add, rraw);
                    }
                }

                break;
            }

            // _or
            case(0b001110):{

                // grt add val val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt, * rslt;
                s8 lraw, rraw;

                bool lisr = f, risr = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Another variable
                if(hasadd(code[cchr + 3])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 3]);
                    
                    rslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 3]);
                    sscanf(phr, "%x", &rraw);

                    // Fix sign
                    if(rraw > 0x10) rraw = -(rraw - 0x10);
                    
                    risr = t;
                }

                // Left is a variable
                if(!lisr){

                    // Both are variables
                    if(!risr){

                        slot temp = *lslt;
                        slot__or(lslt, rslt);
                        
                        memcpy(add, lslt, sizeof(slot));

                        // Undo operation
                        *lslt = temp;
                    
                    // Only left is
                    } else {
                        
                        slot temp = *lslt;

                        slot__ror(lslt, rraw);
                        add->val.vint = lslt->val.vint;

                        // Undo operation
                        *lslt = temp;
                    }

                } else {

                    // Only right is a variable
                    if(!risr){

                        slot hold = {0, "int", lraw};
                        *add = *slot__or(&hold, rslt);
                    
                    // None of them is a variable
                    } else {
                        
                        add->val.vint = lraw;
                        slot__ror(add, rraw);
                    }
                }

                break;
            }

            // not
            case(0b001111):{

                // not add val
                slot * add = get_slot(code[cchr + 1]);

                // Values
                slot * lslt; s8 lraw;

                bool israw = f;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    slot * hold = malloc(sizeof(slot));
                    *hold = *get_slot(code[cchr + 2]);
                    
                    lslt = hold;

                // A raw value
                } else {

                    char phr[8];

                    sprintf(phr, "%x", code[cchr + 2]);
                    sscanf(phr, "%x", &lraw);

                    // Fix sign
                    if(lraw > 0x10) lraw = -(lraw - 0x10);
                    
                    israw = t;
                }


                // Left is a variable
                if(!israw)

                    slot_rnot(lslt);

                else {

                    if(!strcmp(lslt->mst, "int"))
                    lslt->val.vint = !lraw;

                    else if(!strcmp(lslt->mst, "flt"))
                    lslt->val.vflt = !lraw;

                    else if(!strcmp(lslt->mst, "chr"))
                    lslt->val.vchr = !lraw;
                }

                break;
            }

            // ifs
            case(0b110000):{


                break;
            }

            // jmp
            case(0b110001):

                break;

            // int
            case(0b100001):{

                slot * this = malloc(sizeof(slot));

                // Set current type            
                this->mst = "int";

                // Assign adress
                this->add = code[cchr + 1];

                bool israw = f;
                void * add, * lft, * rgt;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    lft  = get_slot(code[cchr + 2]);
                    this = (slot *)lft;

                // A raw value
                } else {

                    s8 out;
                    char phr[8];

                    // Set the value
                    lft = &code[cchr + 2];
                    rgt = &code[cchr + 3];

                    sprintf(phr, "%x%x", *(u8 *)lft, *(u8 *)rgt);
                    sscanf(phr, "%x", &out);

                    // Fix signal
                    if(out > 0x1000) out = -(out - 0x1000);

                    this->val.vint = out;
                }

                slots[cslt] = this;

                // Next slot
                cslt++;
                break;
            }

            // flt
            case(0b100010):{

                slot * this = malloc(sizeof(slot));

                // Set current type            
                this->mst = "flt";

                // Assign adress
                this->add = code[cchr + 1];

                bool israw = f;
                void * add, * lft, * rgt;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    lft  = get_slot(code[cchr + 2]);
                    this = (slot *)lft;

                // A raw value
                } else {

                    s8 out;
                    char phr[8];

                    // Set the value
                    lft = &code[cchr + 2];
                    rgt = &code[cchr + 3];

                    sprintf(phr, "%x%x", *(u8 *)lft, *(u8 *)rgt);
                    sscanf(phr, "%x", &out);

                    // Fix signal
                    if(out > 0x1000) out = -(out - 0x1000);

                    this->val.vflt = out / 1000;
                }

                slots[cslt] = this;

                // Next slot
                cslt++;
                break;
            }

            // chr
            case(0b100011):{

                slot * this = malloc(sizeof(slot));

                // Set current type            
                this->mst = "chr";

                // Assign adress
                this->add = code[cchr + 1];

                bool israw = f;
                void * add, * lft, * rgt;

                // Another variable
                if(hasadd(code[cchr + 2])){

                    lft  = get_slot(code[cchr + 2]);
                    this = (slot *)lft;

                // A raw value
                } else {

                    u8 oph;
                    char out[8], phr[8];

                    // Set the value
                    lft = &code[cchr + 2];
                    rgt = &code[cchr + 3];

                    // fix strange bug
                    sprintf(phr, "%x", *(u8 *)rgt);
                    sscanf(phr, "%02x%02x", &oph, rgt);

                    // Join two sides
                    sprintf(phr, "%02x%02x", *(u8 *)lft, *(u8 *)rgt);

                    // Turn number into string
                    sscanf(phr, "%x", &oph);
                    sprintf(out, "%d", oph);

                    printf("%d %s %d\n", oph, out, btoi(out));

                    this->val.vchr = btoi(out);
                }

                slots[cslt] = this;

                // Next slot
                cslt++;
                break;
            }

            // ext
            case(0b110011):

                printf("program exit [LST OPR: %d]\n", copr);
                break;
            
            default:
                break;
        }

        // Loop control
        if(copr == 0b110011) quit = t;
        else if(!jmpd) cchr += 4;
    }

    printf("x001: %d\n",slots[0]->val.vint);
    
    // Free memory
    free(slots); free(code); free(lbls);
    return 0;
}

slot * get_slot(s8 add){

    for(u8 i = 0; i < prrlen(slots); i++){

        if(slots[i] != nil){

            if(slots[i]->add == add and slots[i]->mst != nil)
            return slots[i];
        }
    }

    return nil;
}

bool hasadd(u8 add){

    for(u8 a = 0; a < prrlen(slots); a++){

        if(slots[a] != nil){

            if(slots[a]->add == add and slots[a]->mst != nil)
            return t;
        }
    }

    return f;
}

u8 addidx(u8 add){

    for(u8 a = 0; a < prrlen(slots); a++){

        if(slots[a] != nil){
            
            if(slots[a]->add == add)
            return a;
        }
    }

    return 0;
}

char * itob(u8 n){

    char * b = malloc(9);

    for(u8 i = 0; i < 8; i++){
    
        b[7 - i] = n & (u8)SDL_pow(2, i) ? '1' : '0';
    }

    b[8] = '\0';

    return b;
}

u8 btoi(char * b){

    u8 out = 0;

    for(u8 c = 0; c < strlen(b) and c < 8; c++){

        if(b[c] != '0') out += (u8)SDL_pow(2,

            strlen(b) > 8 ? 8 - c : strlen(b) - c
        );

        printf("[%c] c: %d out: %d\n", b[c], c, (u8)SDL_pow(2, 8 - c));
    }

    return out;
}

void rnterr(char err[]){
        
    printf("\n[RUNTIME ERROR][INDX: %d]\n%s\n\nExit program with error code %d\n",
    cchr, err, err);

    //sdlout();
    exit(-1);
}