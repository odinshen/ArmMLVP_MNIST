/* Copyright (C) ARM Limited, 2016. All rights reserved. */

/* Device specific functions */

    .text
    .cfi_sections .debug_frame  // put stack frame info into .debug_frame instead of .eh_frame

    .global mpidr_to_core_number
    .type mpidr_to_core_number, "function"
    .cfi_startproc
mpidr_to_core_number:
#ifdef CORTEX_A73x2_CORTEX_A53x4
    ubfx x1, x0, #8, #8
    ubfx x0, x0, #0, #8
    cmp x1, #0
    b.eq 1f
    add x0, x0, #2
1:
#else /* Assume core number is Aff0 */
    ubfx x0, x0, #0, #8
#endif
    ret
    .cfi_endproc

    .global mpidr_to_cluster_number
    .type mpidr_to_cluster_number, "function"
    .cfi_startproc
mpidr_to_cluster_number:
    ubfx x0, x0, #8, #8
    ret
    .cfi_endproc
