#ifndef rustt

    #include "rust.h"
#endif

#ifndef bcclib

    #include "bcclib.h"
#endif

union let {

    u8 vchr;
    s8 vint;
    f8 vflt;
};

// Memory slot
typedef struct memory_slot {

    u8 add;
    char * mst;
    union let val;

} slot;

// Default initializer
imut slot EMPTY = {128, "NIL", nil, nil};

void slot_setv(slot * self, void * vall);
f8   slot_getv(slot * self);

// Slot "class" operations
slot * slot_sum(slot * self, slot * othr),
     * slot_sub(slot * self, slot * othr),
     * slot_mul(slot * self, slot * othr),
     * slot_div(slot * self, slot * othr),
     * slot_mod(slot * self, slot * othr),
     * slot_grt(slot * self, slot * othr),
     * slot_sml(slot * self, slot * othr),
     * slot_gte(slot * self, slot * othr),
     * slot_sle(slot * self, slot * othr),
     * slot_eql(slot * self, slot * othr),
     * slot_dif(slot * self, slot * othr),
     * slot_and(slot * self, slot * othr),
     * slot__or(slot * self, slot * othr);

// Slot "class" operations with primitives
void slot_rsum(slot * self, f8 othr),
     slot_rsub(slot * self, f8 othr),
     slot_rmul(slot * self, f8 othr),
     slot_rdiv(slot * self, f8 othr),
     slot_rmod(slot * self, f8 othr),
     slot_rgrt(slot * self, f8 othr),
     slot_rsml(slot * self, f8 othr),
     slot_rgte(slot * self, f8 othr),
     slot_rsle(slot * self, f8 othr),
     slot_reql(slot * self, f8 othr),
     slot_rdif(slot * self, f8 othr),
     slot_rand(slot * self, f8 othr),
     slot__ror(slot * self, f8 othr),
     slot_rnot(slot * self);

// Return the slot var at add
slot * get_slot(s8 add);