#include "slot.h"

// Arithmetics
slot * slot_sum(slot * self, slot * othr){

    // Do a int output
    if(!strcmp(self->mst, "int")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vint += othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vint += othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vint += othr->val.vchr;
    }

    // Do a float output
    else if(!strcmp(self->mst, "flt")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vflt += othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vflt += othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vflt += othr->val.vchr;
    }

    // Do a char output
    else if(!strcmp(self->mst, "chr")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vchr += othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vchr += othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vchr += othr->val.vchr;
    }

    return self;
}

slot * slot_sub(slot * self, slot * othr){

    // Do a int output
    if(!strcmp(self->mst, "int")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vint -= othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vint -= othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vint -= othr->val.vchr;
    }

    // Do a float output
    else if(!strcmp(self->mst, "flt")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vflt -= othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vflt -= othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vflt -= othr->val.vchr;
    }

    // Do a char output
    else if(!strcmp(self->mst, "chr")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vchr -= othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vchr -= othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vchr -= othr->val.vchr;
    }

    return self;
}

slot * slot_mul(slot * self, slot * othr){

    // Do a int output
    if(!strcmp(self->mst, "int")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vint *= othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vint *= othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vint *= othr->val.vchr;
    }

    // Do a float output
    else if(!strcmp(self->mst, "flt")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vflt *= othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vflt *= othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vflt *= othr->val.vchr;
    }

    // Do a char output
    else if(!strcmp(self->mst, "chr")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vchr *= othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vchr *= othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vchr *= othr->val.vchr;
    }

    return self;
}

slot * slot_div(slot * self, slot * othr){

    // Do a int output
    if(!strcmp(self->mst, "int")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
            self->val.vint /=
            othr->val.vint != 0 ? othr->val.vint : 1;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
            self->val.vint /=
            othr->val.vflt != 0 ? othr->val.vflt : 1;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
            self->val.vint /=
            othr->val.vchr != 0 ? othr->val.vchr : 1;
    }

    // Do a float output
    else if(!strcmp(self->mst, "flt")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
            self->val.vflt /=
            othr->val.vint != 0 ? othr->val.vint : 1;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
            self->val.vflt /=
            othr->val.vflt != 0 ? othr->val.vflt : 1;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
            self->val.vflt /=
            othr->val.vchr != 0 ? othr->val.vchr : 1;
    }

    // Do a char output
    else if(!strcmp(self->mst, "chr")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
            self->val.vchr /=
            othr->val.vint != 0 ? othr->val.vint : 1;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
            self->val.vchr /=
            othr->val.vflt != 0 ? othr->val.vflt : 1;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
            self->val.vchr /=
            othr->val.vchr != 0 ? othr->val.vchr : 1;
    }

    return self;
}

slot * slot_mod(slot * self, slot * othr){

    // Do a int output
    if(!strcmp(self->mst, "int")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
            self->val.vint %=
            othr->val.vint != 0 ? othr->val.vint : 1;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vint %=
        othr->val.vflt != 0 ? (s8)othr->val.vflt : 1;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
            self->val.vint %=
            othr->val.vchr != 0 ? othr->val.vchr : 1;
    }

    // Do a float output
    else if(!strcmp(self->mst, "flt")){

        // Othr is an int
        if(!strcmp(othr->mst, "int"))
            self->val.vflt = (s8)self->val.vflt %
            (othr->val.vint != 0 ? othr->val.vint : 1);

        // Othr is an othr->mst
        else if(!strcmp(othr->mst, "flt"))
            self->val.vflt = (s8)self->val.vflt %
            (othr->val.vflt != 0 ? (s8)othr->val.vflt : 1);

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
            self->val.vflt = (s8)self->val.vflt %
            (othr->val.vchr != 0 ? othr->val.vchr : 1);
    }

    // Do a char output
    else if(!strcmp(self->mst, "chr")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vchr %=
        othr->val.vint != 0 ? othr->val.vint : 1;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
            self->val.vchr %=
            othr->val.vflt > 0 ? (u8)othr->val.vflt : 1;

        // Othr is othr->mstfloat
        else if(!strcmp(othr->mst, "chr"))
        self->val.vchr %=
        othr->val.vchr != 0 ? othr->val.vchr : 1;
    }

    return self;
}

// Comparison
slot * slot_grt(slot * self, slot * othr){

    // Do a int output
    if(!strcmp(self->mst, "int")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vint = self->val.vint > othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vint = self->val.vint > othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vint = self->val.vint > othr->val.vchr;
    }

    // Do a float output
    else if(!strcmp(self->mst, "flt")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vflt = self->val.vflt > othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vflt = self->val.vflt > othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vflt = self->val.vflt > othr->val.vchr;
    }

    // Do a char output
    else if(!strcmp(self->mst, "chr")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vchr = self->val.vchr > othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vchr = self->val.vchr > othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vchr = self->val.vchr > othr->val.vchr;
    }

    return self;
}

slot * slot_sml(slot * self, slot * othr){

    // Do a int output
    if(!strcmp(self->mst, "int")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vint = self->val.vint < othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vint = self->val.vint < othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vint = self->val.vint < othr->val.vchr;
    }

    // Do a float output
    else if(!strcmp(self->mst, "flt")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vflt = self->val.vflt < othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vflt = self->val.vflt < othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vflt = self->val.vflt < othr->val.vchr;
    }

    // Do a char output
    else if(!strcmp(self->mst, "chr")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vchr = self->val.vchr < othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vchr = self->val.vchr < othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vchr = self->val.vchr < othr->val.vchr;
    }

    return self;
}

slot * slot_gte(slot * self, slot * othr){

    // Do a int output
    if(!strcmp(self->mst, "int")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vint = self->val.vint >= othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vint = self->val.vint >= othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vint = self->val.vint >= othr->val.vchr;
    }

    // Do a float output
    else if(!strcmp(self->mst, "flt")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vflt = self->val.vflt >= othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vflt = self->val.vflt >= othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vflt = self->val.vflt >= othr->val.vchr;
    }

    // Do a char output
    else if(!strcmp(self->mst, "chr")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vchr = self->val.vchr >= othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vchr = self->val.vchr >= othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vchr = self->val.vchr >= othr->val.vchr;
    }

    return self;
}

slot * slot_sle(slot * self, slot * othr){

    // Do a int output
    if(!strcmp(self->mst, "int")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vint = self->val.vint <= othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vint = self->val.vint <= othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vint = self->val.vint <= othr->val.vchr;
    }

    // Do a float output
    else if(!strcmp(self->mst, "flt")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vflt = self->val.vflt <= othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vflt = self->val.vflt <= othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vflt = self->val.vflt <= othr->val.vchr;
    }

    // Do a char output
    else if(!strcmp(self->mst, "chr")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vchr = self->val.vchr <= othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vchr = self->val.vchr <= othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vchr = self->val.vchr <= othr->val.vchr;
    }

    return self;
}

// Logical
slot * slot_eql(slot * self, slot * othr){

    // Do a int output
    if(!strcmp(self->mst, "int")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vint = self->val.vint == othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vint = self->val.vint == othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vint = self->val.vint == othr->val.vchr;
    }

    // Do a float output
    else if(!strcmp(self->mst, "flt")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vflt = self->val.vflt == othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vflt = self->val.vflt == othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vflt = self->val.vflt == othr->val.vchr;
    }

    // Do a char output
    else if(!strcmp(self->mst, "chr")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vchr = self->val.vchr == othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vchr = self->val.vchr == othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vchr = self->val.vchr == othr->val.vchr;
    }

    return self;
}

slot * slot_dif(slot * self, slot * othr){

    // Do a int output
    if(!strcmp(self->mst, "int")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vint = self->val.vint != othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vint = self->val.vint != othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vint = self->val.vint != othr->val.vchr;
    }

    // Do a float output
    else if(!strcmp(self->mst, "flt")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vflt = self->val.vflt != othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vflt = self->val.vflt != othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vflt = self->val.vflt != othr->val.vchr;
    }

    // Do a char output
    else if(!strcmp(self->mst, "chr")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vchr = self->val.vchr != othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vchr = self->val.vchr != othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vchr = self->val.vchr != othr->val.vchr;
    }

    return self;
}

slot * slot_and(slot * self, slot * othr){

    // Do a int output
    if(!strcmp(self->mst, "int")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vint = self->val.vint and othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vint = self->val.vint and othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vint = self->val.vint and othr->val.vchr;
    }

    // Do a float output
    else if(!strcmp(self->mst, "flt")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vflt = self->val.vflt and othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vflt = self->val.vflt and othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vflt = self->val.vflt and othr->val.vchr;
    }

    // Do a char output
    else if(!strcmp(self->mst, "chr")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vchr = self->val.vchr and othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vchr = self->val.vchr and othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vchr = self->val.vchr and othr->val.vchr;
    }

    return self;
}

slot * slot__or(slot * self, slot * othr){

    // Do a int output
    if(!strcmp(self->mst, "int")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vint = self->val.vint or othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vint = self->val.vint or othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vint = self->val.vint or othr->val.vchr;
    }

    // Do a float output
    else if(!strcmp(self->mst, "flt")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vflt = self->val.vflt or othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vflt = self->val.vflt or othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vflt = self->val.vflt or othr->val.vchr;
    }

    // Do a char output
    else if(!strcmp(self->mst, "chr")){
        
        // Othr is an int
        if(!strcmp(othr->mst, "int"))
        self->val.vchr = self->val.vchr or othr->val.vint;

        // Othr is an float
        else if(!strcmp(othr->mst, "flt"))
        self->val.vchr = self->val.vchr or othr->val.vflt;

        // Othr is an float
        else if(!strcmp(othr->mst, "chr"))
        self->val.vchr = self->val.vchr or othr->val.vchr;
    }

    return self;
}

void slot_setv(slot * self, void * vall){

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint = *(s8 *)vall;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    self->val.vflt = *(f8 *)vall;

    // Do a char output
    else if(!strcmp(self->mst, "chr"))
    self->val.vchr = *(u8 *)vall;
}

f8 slot_getv(slot * self){

    // Do a int output
    if(!strcmp(self->mst, "int"))
    return self->val.vint;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    return self->val.vflt;

    // Do a char output
    else if(!strcmp(self->mst, "chr"))
    return self->val.vchr;
}

// Arithmetics
void slot_rsum(slot * self, f8 othr){

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint += othr;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    self->val.vflt += othr;

    // Do a char output
    else if(!strcmp(self->mst, "chr"))
        self->val.vchr =
        self->val.vchr + othr > 0 ? self->val.vchr + othr : 0;
}

void slot_rsub(slot * self, f8 othr){

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint -= othr;

    // Do a float output
    else if(!strcmp(self->mst, "int"))
    self->val.vflt -= othr;

    // Do a char output
    else if(!strcmp(self->mst, "int"))
        self->val.vchr =
        self->val.vchr - othr > 0 ? self->val.vchr - othr : 0;
}

void slot_rmul(slot * self, f8 othr){

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint *= othr;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    self->val.vflt *= othr;

    // Do a char output
    else if(!strcmp(self->mst, "chr"))
        self->val.vchr =
        self->val.vchr * othr > 0 ? self->val.vchr * othr : 0;
}

void slot_rdiv(slot * self, f8 othr){

    if(othr == 0) return;

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint /= othr;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    self->val.vflt /= othr;

    // Do a char output
    else if(!strcmp(self->mst, "chr"))
        self->val.vchr =
        self->val.vchr / othr > 0 ? self->val.vchr / othr : 0;
}

void slot_rmod(slot * self, f8 othr){

    if(othr == 0) return;

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint %= (s8)othr;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    self->val.vflt = (s8)self->val.vflt % (s8)othr;

    // Do a char output
    else if(!strcmp(self->mst, "flt"))
    self->val.vchr %= othr > 0 ? (s8)othr : 0;
}

// Comparison
void slot_rgrt(slot * self, f8 othr){

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint = self->val.vint > othr;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    self->val.vflt = self->val.vflt > othr;

    // Do a char output
    else if(!strcmp(self->mst, "chr"))
    self->val.vchr = self->val.vchr > othr;
}

void slot_rsml(slot * self, f8 othr){

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint = self->val.vint < othr;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    self->val.vflt = self->val.vflt < othr;

    // Do a char output
    else if(!strcmp(self->mst, "chr"))
    self->val.vchr = self->val.vchr < othr;
}

void slot_rgte(slot * self, f8 othr){

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint = self->val.vint >= othr;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    self->val.vflt = self->val.vflt >= othr;

    // Do a char output
    else if(!strcmp(self->mst, "chr"))
    self->val.vchr = self->val.vchr >= othr;
}

void slot_rsle(slot * self, f8 othr){

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint = self->val.vint <= othr;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    self->val.vflt = self->val.vflt <= othr;

    // Do a char output
    else if(!strcmp(self->mst, "chr"))
    self->val.vchr = self->val.vchr <= othr;
}

// Logical
void slot_reql(slot * self, f8 othr){

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint = self->val.vint == othr;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    self->val.vflt = self->val.vflt == othr;

    // Do a char output
    else if(!strcmp(self->mst, "chr"))
    self->val.vchr = self->val.vchr == othr;
}

void slot_rdif(slot * self, f8 othr){

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint = self->val.vint != othr;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    self->val.vflt = self->val.vflt != othr;

    // Do a char output
    else if(!strcmp(self->mst, "chr"))
    self->val.vchr = self->val.vchr != othr;
}

void slot_rand(slot * self, f8 othr){

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint = self->val.vint and othr;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    self->val.vflt = self->val.vflt and othr;

    // Do a char output
    else if(!strcmp(self->mst, "chr"))
    self->val.vchr = self->val.vchr and othr;
}

void slot__ror(slot * self, f8 othr){

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint = self->val.vint or othr;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    self->val.vflt = self->val.vflt or othr;

    // Do a char output
    else if(!strcmp(self->mst, "chr"))
    self->val.vchr = self->val.vchr or othr;
}

void slot_rnot(slot * self){

    // Do a int output
    if(!strcmp(self->mst, "int"))
    self->val.vint = !self->val.vint;

    // Do a float output
    else if(!strcmp(self->mst, "flt"))
    self->val.vflt = !self->val.vflt;

    // Do a char output
    else if(!strcmp(self->mst, "chr"))
    self->val.vchr = !self->val.vflt;
}