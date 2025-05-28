#include <stdio.h>
#include <stdlib.h>

#include "main.h"

#define SCALE 8
#define SZMAX 40000

// Screen params
imut u8 WDT = 640, HGT = 640;

// Screen
swin window = nil;
ssur winsur = nil;

// Image renderer
sren render = nil;

// Console memory
u8 inpt = 0b000000;
bool keys[6];

// Runtime data
char ** defs;
u8 intc, fltc, chrc;

// Memory slots
slot * ints, * flts, * chrs;

// Jumpline labels
u8 crt_lbl = 0, lbl_len = 8;
lbl * lbls;

// Game bytecode
FILE * fptr = nil;
u8 cchr = 0, clin = 0, code[SZMAX];

int main(int argc, char ** args){

    // Nullity checker
    if(args[1] == nil or isblnk(args[1])){
        
        rnterr("No file name was given");

    } else fptr = fopen(args[1], "r");

    if(fptr == nil){
        
        puts("Given file does not exist");
        exit(-1);
    }

    // Init labels array
    lbls = malloc(lbl_len * sizeof(lbl));

    // Get bytecode
    char c = fgetc(fptr);
    while(c != EOF){

        //lbl
        if(c == 0b110010 && cchr % 5 == 0){

            // Realloc memory if is full
            if(crt_lbl == lbl_len){

                lbl_len++;
                realloc(lbls, lbl_len * sizeof(lbl));
            }

            crt_lbl++;

            // Store index and label name
            lbl l = {cchr + 1, cchr + 5};
            lbls[crt_lbl] = l;
        }

        code[++cchr] = c;
        c = fgetc(fptr);
    }

    // Free memory
    fclose(fptr); fptr = nil;

    // Get memory data
    intc = code[cchr - 4];
    fltc = code[cchr - 3];
    chrc = code[cchr - 2];

    // Allocate 6 bytes per slot
    defs = malloc(code[cchr - 1] * 6);

    // Game data
    if(intc > 0) ints = malloc((intc + 1) * sizeof(slot));
    if(fltc > 0) flts = malloc((fltc + 1) * sizeof(slot));
    if(chrc > 0) chrs = malloc((chrc + 1) * sizeof(slot));

    // Use count as array pointer
    intc = 0, fltc = 0, chrc = 0;

    puts("Running byte code...");

    // Initialize SDL
    //init();

    // Init all slots
    for(u8 i = 0; i < 64; i++){

        defs[i] = "EMPTY";
    }

    evnt sdle;

    // Runtime data
    bool quit = f, jmpd = t;

    // Program loop
    while(!quit){

        // SDL Events
        /*while(sdl_poll(&sdle)){

            //Quit application
            if(sdle.type == SDL_QUIT) quit = t;

            // On key down
            else if(sdle.type == SDL_KEYDOWN){

                // Update inputs
                u32 key = sdle.key.keysym.sym;

                if(key == KEY_ESC) quit = t;
                else {

                    if(key == KEY_UP)  inpt = inpt | 0b000001; // key 00
                    if(key == KEY_LFT) inpt = inpt | 0b000010; // key 01
                    if(key == KEY_DWN) inpt = inpt | 0b000100; // key 02
                    if(key == KEY_RGT) inpt = inpt | 0b001000; // key 03
                    if(key == KEY_X)   inpt = inpt | 0b010000; // key 04
                    if(key == KEY_C)   inpt = inpt | 0b100000; // key 05
                }

            // On key up
            } else if(sdle.type == SDL_KEYUP){

                // Update inputs
                u32 key = sdle.key.keysym.sym;

                if(key == KEY_UP)  inpt = inpt & 0b000001 ? inpt ^ 0b000001 : inpt;
                if(key == KEY_LFT) inpt = inpt & 0b000010 ? inpt ^ 0b000010 : inpt;
                if(key == KEY_DWN) inpt = inpt & 0b000100 ? inpt ^ 0b000100 : inpt;
                if(key == KEY_RGT) inpt = inpt & 0b001000 ? inpt ^ 0b001000 : inpt;
                if(key == KEY_X)   inpt = inpt & 0b010000 ? inpt ^ 0b010000 : inpt;
                if(key == KEY_C)   inpt = inpt & 0b100000 ? inpt ^ 0b100000 : inpt;
            }
        }*/

        // Resset jump
        jmpd = f;

        // Read code
        u8 copr = code[clin];

        printf("%d %s\n", copr, itob(copr));

        //del
        if(copr == 0b010000){
            
            // Get pointer to address
            u8 * memo = getadd(code[clin + 1]);

            // Roll back the stack pointer
            if(!strcmp(defs[code[clin + 1]], "int"))
            intc--;

            if(!strcmp(defs[code[clin + 1]], "flt"))
            fltc--;

            if(!strcmp(defs[code[clin + 1]], "chr"))
            chrc--;

            if(memo == nil)
            rnterr("Attempt to delete a unused memory address");
            
            *memo = 0;
            defs[code[clin + 1]] = "EMPTY";
        }
        
        //mov
        else if(copr == 0b010001){

            // Get pointer to address
            u8 * memo = getadd(code[clin + 1]);

            if(memo == nil) 
            rnterr("Attempt to delete an empty memory address");
            
            // Internal converter to signed values
            if(code[clin + 2] > 99)
                
                *memo =
                -((code[clin + 2] - 100) * 100 + code[clin + 3]);

            else  *memo = code[clin + 2] * 100 + code[clin + 3];     
        }
        
        //cls
        else if(copr == 0b010010){
            
            cls(COLLS[code[clin + 1]]);
        }
        
        //get
        else if(copr == 0b010011){
            
            printf("get %d %d %d\n", code[clin + 1], code[clin + 2], code[clin + 3]);
        }
        
        //pxl
        else if(copr == 0b010100){
            
            printf("pxl %d %d %d\n", code[clin + 1], code[clin + 2], code[clin + 3]);
        }
        
        //snd
        else if(copr == 0b010101){
            
            printf("snd %d %d %d\n", code[clin + 1], code[clin + 2], code[clin + 3]);
        }
        
        //sum
        else if(copr == 0b000001){
            
            _op data = doopr(
                
                code[clin + 1],
                code[clin + 2],
                code[clin + 3]
            );

            *data.vl = data.lt + data.rt;
        }
        
        //sub
        else if(copr == 0b000010){
            
            _op data = doopr(
                
                code[clin + 1],
                code[clin + 2],
                code[clin + 3]
            );

            *data.vl = data.lt - data.rt;
        }
        
        //mul
        else if(copr == 0b000011){

            _op data = doopr(

                code[clin + 1],
                code[clin + 2],
                code[clin + 3]
            );
            
            *data.vl = data.lt * data.rt;
        }
        
        //div
        else if(copr == 0b000100){
            
            _op data = doopr(
                
                code[clin + 1],
                code[clin + 2],
                code[clin + 3]
            );

            *data.vl =
            data.rt != 0 ? data.lt / data.rt : 0;
        }
        
        //mod
        else if(copr == 0b000101){
            
            _op data = doopr(
                
                code[clin + 1],
                code[clin + 2],
                code[clin + 3]
            );

            *data.vl =
            data.rt != 0 ? data.lt % data.rt : 0;
        }
        
        //bin
        else if(copr == 0b000110){
            
            //bin {:} var {=} add {<-} val 

            _op data = doopr(
                
                code[clin + 1],
                code[clin + 2],
                code[clin + 3]
            );

            // Avoid reading out of scope memory
            data.lt = data.lt <= 7 ? data.lt : 7;

            char bin[9];
            *bin = itob(data.rt);

            *data.vl = bin[7 - data.lt] == '1';
        }
        
        //grt
        else if(copr == 0b000111){

            _op data = doopr(
                
                code[clin + 1],
                code[clin + 2],
                code[clin + 3]
            );

            *data.vl = data.lt > data.rt;
        }
        
        //sml
        else if(copr == 0b001000){

            _op data = doopr(
                
                code[clin + 1],
                code[clin + 2],
                code[clin + 3]
            );

            *data.vl = data.lt < data.rt;
        }
        
        //gte
        else if(copr == 0b001001){

            _op data = doopr(
                
                code[clin + 1],
                code[clin + 2],
                code[clin + 3]
            );

            *data.vl = data.lt >= data.rt;
        }
        
        //sle
        else if(copr == 0b001010){

            _op data = doopr(
                
                code[clin + 1],
                code[clin + 2],
                code[clin + 3]
            );

            *data.vl = data.lt <= data.rt;
        }
        
        //eql
        else if(copr == 0b001011){

            _op data = doopr(
                
                code[clin + 1],
                code[clin + 2],
                code[clin + 3]
            );

            *data.vl = data.lt == data.rt;
        }
        
        //dif
        else if(copr == 0b001100){

            _op data = doopr(
                
                code[clin + 1],
                code[clin + 2],
                code[clin + 3]
            );

            *data.vl = data.lt != data.rt;
        }
        
        //and
        else if(copr == 0b001101){
            
            _op data = doopr(
                
                code[clin + 1],
                code[clin + 2],
                code[clin + 3]
            );

            *data.vl = data.lt and data.rt;
        }
        
        //_or
        else if(copr == 0b001110){

            _op data = doopr(
                
                code[clin + 1],
                code[clin + 2],
                code[clin + 3]
            );

            *data.vl = data.lt or data.rt;
        }

        //ifs
        else if(copr == 0b110000){
            
            jmpd = t;

            s8 vall = getval(code[clin + 1]);

            if(vall){
                
                s8 nl = getlbl(code[clin + 2]);

                if(nl == -1)
                rnterr(
                    
                    sprintf("Attempt to jump to an unknown label (%d)",
                    code[clin + 2])
                );

                clin = nl;

            } else {
                
                s8 nl = getlbl(code[clin + 3]);

                if(nl == -1) rnterr(
                    
                    sprintf("Attempt to jump to an unknown label (%d)",
                    code[clin + 3])
                );

                clin = nl;
            }
        }

        //jmp
        else if(copr == 0b110001){

            jmpd = t;
                
            s8 nl = getlbl(code[clin + 1]);
            
            if(nl == -1) rnterr(

                sprintf("Attempt to jump to an unknown label (%d)",
                code[clin + 1])
            );

            clin = nl;
        }
        
        //int
        else if(copr == 0b100001){

            defs[code[clin + 1]] = "int";

            // Internal converter to signed values
            if(code[clin + 2] > 99){
                
                u8 vl =
                -((code[clin + 2] - 100) * 100 + code[clin + 3]);

                slot l = {code[clin + 1], &vl};
                ints[++intc] = l;

            } else {
                
                u8 vl = code[clin + 2] * 100 + code[clin + 3];
                
                slot l = {code[clin + 1], &vl};
                ints[++intc] = l;
            }
        }
        
        //flt
        else if(copr == 0b100010){
            
            printf("flt %d %d %d\n", code[clin + 1], code[clin + 2], code[clin + 3]);
        }
        
        //chr
        else if(copr == 0b100011){
            
            printf("chr %d %d %d\n", code[clin + 1], code[clin + 2], code[clin + 3]);
        }

        // Next char
        if(!jmpd){
            
            clin += 5;

            // Loop
            if(clin > cchr - 5) clin = 0;
        }

        // Exit program
        if(copr == 0b110011) quit = t;
    }

    printf("x001: %d x002: %d\n", *ints[1].val, *ints[2].val);

    //sdlout();
    return 0;
}

//SDL2 Functions
void init(){

    // Initialize SDL
    if(sdl_init(SDL_INIT_VIDEO) < 0){

        sdlerr("SDL could not be initialized.");
        exit(-1);

    } else {

        // Initialize window
        window = sdl_cwin("Homcon - DiSo", SDL_WPU, SDL_WPU, WDT, HGT, SDL_WSN);

        // Error creating the window
        if(window == nil){

            sdlerr("Window could not be created.");
            exit(-1);
        
        } else {

            // Window surface
            winsur = sdl_gsur(window);

            // Texture renderer
            render = sdl_cren(window, -1, 0);

            sdl_dcol(render, 255, 255, 255, 255);
            sdl_rend_clear(render);
            
        }
    }
}

void sdlout(){

    // Destroy renderer
    sdl_rend_dstry(render);
    render = nil;

    // Destroy window
    sdl_dwin(window);
    window = nil;

    // Quit SDL subsystems
    sdl_quit();
}

// Homcon functions
void pix(s16 x, s16 y, coll c){

    sdl_dcol(render, c.r, c.g, c.b, 255);

    for(s16 _x = x; _x < x + SCALE; _x++){

        for(s16 _y = y; _y < y + SCALE; _y++){

            sdl_dpxl(render, x, y);
        }
    }

    sdl_rend_pres(render);
}

void cls(coll c){

    sdl_dcol(render, c.r, c.g, c.b, 255);
    sdl_rend_clear(render);
}

// Errors
void sdlerr(char err[]){

    puts(err);
    printf("SDL ERROR: %s\n", sdl_gerr());
}

void rnterr(char err[]){
        
    printf("\n[RUNTIME ERROR][OPR: %d]\n%s\n\nExit program with error code %d\n",
    clin, err, err);

    //sdlout();
    exit(-1);
}

// Memory
s8 getval(u8 add){

    s8 * l = getadd(add);
    
    if(l != nil) return *l;
    else return add;
}

s8 * getadd(u8 add){

    if(!strcmp(defs[add], "int")){
        
        for(u8 v = 0; v < intc + 1; v++){

            if(add == ints[v].add)
            return ints[add].val;
        }
    }

    if(!strcmp(defs[add], "flt")){
        
        for(u8 v = 0; v < fltc ; v++){
        
            if(add == flts[v].add)
            return flts[add].val;
        }
    }

    if(!strcmp(defs[add], "chr")){
        
        for(u8 v = 0; v < chrc; v++){
        
            if(add == chrs[v].add)
            return chrs[add].val;
        }
    }

    return nil;
}

char * itob(u8 n){

    char b[9];

    for(u8 l = 0; l < 8; l++){
        
        u8 bin = SDL_pow(2, l);
        b[7 - l] = n & bin ? '1' : '0';
    }

    b[8] = '\0';

    return b;
}

u8 btoi(char s[]){

    u8 i = 0;

    for(u8 b = 0; b < 8; b++){

        if(s[b] == '1')
        i += SDL_pow(2, 7 - b);
    }

    return i;
}

_op doopr(u8 c1, u8 c2, u8 c3){

    s8 lt, rt, * vl;

    // Check if is blank and get adress
    if(c2 < arrlen(defs)){

        if(!strcmp(defs[c2], "EMPTY"))

            lt = getval(c2);

        else lt = c2;

    } else lt = c2;

    if(c3 < arrlen(defs)){

        if(!strcmp(defs[c3], "EMPTY"))
            
            rt = getval(c3);

        else rt = c3;

    } else lt = c2;

    // Get pointer to address
    vl = getadd(c1);

    if(vl == nil)
    rnterr("Attempt use an empty memory address");

    _op data = {lt, rt, vl};
    return data;
}

s8 getlbl(u8 n){

    for(u8 i = 0; i < crt_lbl; i++){

        if(lbls[i].name == n) return lbls[i].line;
    }

    return -1;
}