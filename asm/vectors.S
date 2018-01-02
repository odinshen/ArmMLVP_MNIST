// ------------------------------------------------------------
// ARMv8-A Vector tables
//
// Copyright (c) 2014-2016 ARM Ltd.  All rights reserved.
// ------------------------------------------------------------


    .extern irqHandler

    .section  EL1VECTORS, "ax"
    .align 11

.global el1_vectors
el1_vectors:
//
// Current EL with SP0
//
c0sync1:
    b c0sync1

    .org el1_vectors + 0x080
c0irq1:
    stp  x29, x30, [sp, #-16]!
    stp  x18, x19, [sp, #-16]!
    stp  x16, x17, [sp, #-16]!
    stp  x14, x15, [sp, #-16]!
    stp  x12, x13, [sp, #-16]!
    stp  x10, x11, [sp, #-16]!
    stp  x8,  x9,  [sp, #-16]!
    stp  x6,  x7,  [sp, #-16]!
    stp  x4,  x5,  [sp, #-16]!
    stp  x2,  x3,  [sp, #-16]!
    stp  x0,  x1,  [sp, #-16]!

    bl el1IrqHandler

    ldp  x0,  x1,  [sp], #16
    ldp  x2,  x3,  [sp], #16
    ldp  x4,  x5,  [sp], #16
    ldp  x6,  x7,  [sp], #16
    ldp  x8,  x9,  [sp], #16
    ldp  x10, x11, [sp], #16
    ldp  x12, x13, [sp], #16
    ldp  x14, x15, [sp], #16
    ldp  x16, x17, [sp], #16
    ldp  x18, x19, [sp], #16
    ldp  x29, x30, [sp], #16
    eret

    .org el1_vectors + 0x100
c0fiq1:
    b c0fiq1

    .org el1_vectors + 0x180
c0serr1:
    b c0serr1

//
// Current EL with SPx
//
    .org el1_vectors + 0x200
cxsync1:
    b cxsync1

    .org el1_vectors + 0x280
cxirq1:
    stp  x29, x30, [sp, #-16]!
    stp  x18, x19, [sp, #-16]!
    stp  x16, x17, [sp, #-16]!
    stp  x14, x15, [sp, #-16]!
    stp  x12, x13, [sp, #-16]!
    stp  x10, x11, [sp, #-16]!
    stp  x8,  x9,  [sp, #-16]!
    stp  x6,  x7,  [sp, #-16]!
    stp  x4,  x5,  [sp, #-16]!
    stp  x2,  x3,  [sp, #-16]!
    stp  x0,  x1,  [sp, #-16]!

    bl el1IrqHandler

    ldp  x0,  x1,  [sp], #16
    ldp  x2,  x3,  [sp], #16
    ldp  x4,  x5,  [sp], #16
    ldp  x6,  x7,  [sp], #16
    ldp  x8,  x9,  [sp], #16
    ldp  x10, x11, [sp], #16
    ldp  x12, x13, [sp], #16
    ldp  x14, x15, [sp], #16
    ldp  x16, x17, [sp], #16
    ldp  x18, x19, [sp], #16
    ldp  x29, x30, [sp], #16
    eret

    .org el1_vectors + 0x300
cxfiq1:
    b cxfiq1

    .org el1_vectors + 0x380
cxserr1:
    b cxserr1

//
// Lower EL using AArch64
//
    .org el1_vectors + 0x400
l64sync1:
    b l64sync1

    .org el1_vectors + 0x480
l64irq1:
    stp  x29, x30, [sp, #-16]!
    stp  x18, x19, [sp, #-16]!
    stp  x16, x17, [sp, #-16]!
    stp  x14, x15, [sp, #-16]!
    stp  x12, x13, [sp, #-16]!
    stp  x10, x11, [sp, #-16]!
    stp  x8,  x9,  [sp, #-16]!
    stp  x6,  x7,  [sp, #-16]!
    stp  x4,  x5,  [sp, #-16]!
    stp  x2,  x3,  [sp, #-16]!
    stp  x0,  x1,  [sp, #-16]!

    bl el1IrqHandler

    ldp  x0,  x1,  [sp], #16
    ldp  x2,  x3,  [sp], #16
    ldp  x4,  x5,  [sp], #16
    ldp  x6,  x7,  [sp], #16
    ldp  x8,  x9,  [sp], #16
    ldp  x10, x11, [sp], #16
    ldp  x12, x13, [sp], #16
    ldp  x14, x15, [sp], #16
    ldp  x16, x17, [sp], #16
    ldp  x18, x19, [sp], #16
    ldp  x29, x30, [sp], #16
    eret

    .org el1_vectors + 0x500
l64fiq1:
    b l64fiq1

    .org el1_vectors + 0x580
l64serr1:
    b l64serr1

//
// Lower EL using AArch32
//
    .org el1_vectors + 0x600
l32sync1:
    b l32sync1

    .org el1_vectors + 0x680
l32irq1:
    stp  x29, x30, [sp, #-16]!
    stp  x18, x19, [sp, #-16]!
    stp  x16, x17, [sp, #-16]!
    stp  x14, x15, [sp, #-16]!
    stp  x12, x13, [sp, #-16]!
    stp  x10, x11, [sp, #-16]!
    stp  x8,  x9,  [sp, #-16]!
    stp  x6,  x7,  [sp, #-16]!
    stp  x4,  x5,  [sp, #-16]!
    stp  x2,  x3,  [sp, #-16]!
    stp  x0,  x1,  [sp, #-16]!

    bl el1IrqHandler

    ldp  x0,  x1,  [sp], #16
    ldp  x2,  x3,  [sp], #16
    ldp  x4,  x5,  [sp], #16
    ldp  x6,  x7,  [sp], #16
    ldp  x8,  x9,  [sp], #16
    ldp  x10, x11, [sp], #16
    ldp  x12, x13, [sp], #16
    ldp  x14, x15, [sp], #16
    ldp  x16, x17, [sp], #16
    ldp  x18, x19, [sp], #16
    ldp  x29, x30, [sp], #16
    eret

    .org el1_vectors + 0x700
l32fiq1:
    b l32fiq1

    .org el1_vectors + 0x780
l32serr1:
    b l32serr1

//----------------------------------------------------------------

    .section  EL2VECTORS, "ax"
    .align 11

.global el2_vectors
el2_vectors:
//
// Current EL with SP0
//
c0sync2:
    b c0sync2

    .org el2_vectors + 0x080
c0irq2:
    b c0irq2

    .org el2_vectors + 0x100
c0fiq2:
    b c0fiq2

    .org el2_vectors + 0x180
c0serr2:
    b c0serr2

//
// Current EL with SPx
//
    .org el2_vectors + 0x200
cxsync2:
    b cxsync2

    .org el2_vectors + 0x280
cxirq2:
    b cxirq2

    .org el2_vectors + 0x300
cxfiq2:
    b cxfiq2

    .org el2_vectors + 0x380
cxserr2:
    b cxserr2

//
// Lower EL using AArch64
//
    .org el2_vectors + 0x400
l64sync2:
    b l64sync2

    .org el2_vectors + 0x480
l64irq2:
    b l64irq2

    .org el2_vectors + 0x500
l64fiq2:
    b l64fiq2

    .org el2_vectors + 0x580
l64serr2:
    b l64serr2

//
// Lower EL using AArch32
//
    .org el2_vectors + 0x600
l32sync2:
    b l32sync2

    .org el2_vectors + 0x680
l32irq2:
    b l32irq2

    .org el2_vectors + 0x700
l32fiq2:
    b l32fiq2

    .org el2_vectors + 0x780
l32serr2:
    b l32serr2

//----------------------------------------------------------------

    .section  EL3VECTORS, "ax"
    .align 11

.global el3_vectors
el3_vectors:
//
// Current EL with SP0
//
c0sync3:
    b c0sync3

    .org el3_vectors + 0x080
c0irq3:
    b c0irq3

    .org el3_vectors + 0x100
c0fiq3:
    b c0fiq3

    .org el3_vectors + 0x180
c0serr3:
    b c0serr3

//
// Current EL with SPx
//
    .org el3_vectors + 0x200
cxsync3:
    b cxsync3

    .org el3_vectors + 0x280
cxirq3:
    b cxirq3

    .org el3_vectors + 0x300
cxfiq3:
    b cxfiq3

    .org el3_vectors + 0x380
cxserr3:
    b cxserr3

//
// Lower EL using AArch64
//
    .org el3_vectors + 0x400
l64sync3:
    b l64sync3

    .org el3_vectors + 0x480
l64irq3:
    b l64irq3

    .org el3_vectors + 0x500
l64fiq3:
    b l64fiq3

    .org el3_vectors + 0x580
l64serr3:
    b l64serr3

//
// Lower EL using AArch32
//
    .org el3_vectors + 0x600
l32sync3:
    b l32sync3

    .org el3_vectors + 0x680
l32irq3:
    b l32irq3

    .org el3_vectors + 0x700
l32fiq3:
    b l32fiq3

    .org el3_vectors + 0x780
l32serr3:
    b l32serr3
