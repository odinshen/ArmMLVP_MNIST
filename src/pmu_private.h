/* Copyright (c) 2016 by ARM Limited. All rights reserved. */

#ifndef INCLUDE_PMU_PRIVATE_H
#define INCLUDE_PMU_PRIVATE_H

#define STRINGIFY_TOKEN(x)  #x
#define STRINGIFY(x)        STRINGIFY_TOKEN(x)

#if  defined(__arm__)

/**
 * @def         MRRC_64(15, op1, CRm, out)
 * @brief       Read from a system register into a 64 bit integer
 * @param       coproc
 *              Is the coproc parameter within the System register encoding space,
 *              in the range 8 to 15, encoded in the coproc" field.
 * @param       op1
 *              Is the op1/opc1 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op1"/"opc1" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       out Type "unsigned long long"
 *              L-value to write the result into.
 *
 * @def         MCRR_64(15, op1, CRm, in)
 * @brief       Writes a 64 bit integer to a system register
 * @param       coproc
 *              Is the coproc parameter within the System register encoding space,
 *              in the range 8 to 15, encoded in the coproc" field.
 * @param       op1
 *              Is the op1/opc1 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op1"/"opc1" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       in Type "unsigned long long"
 *              Value to write into the register.
 */
# define    MRRC_64(coproc, op1, CRm, out)                                          \
                do {                                                                \
                    unsigned int _mrrc_lower;                                       \
                    unsigned int _mrrc_upper;                                       \
                    MRRC(coproc, op1, CRm, _mrrc_lower, _mrrc_upper);               \
                    out = (((unsigned long long) _mrrc_upper) << 32) | _mrrc_lower; \
                } while (0)
# define    MCRR_64(coproc, op1, CRm, in)         MCRR(coproc, op1, CRm, (unsigned int) in, (unsigned int) (in >> 32))
#endif


/**
 * @def         MRC(op0, op1, CRn, CRm, op2, Rt)
 * @brief       MRC instruction (or manually encoded MRS on AArch64)
 * @param       op0
 *              Is the op1/coproc parameter within the System register encoding space,
 *              in the range 0 to 7 or 8 to 15, respectively, encoded in the "op1"/"coproc" field.
 * @param       op1
 *              Is the op1/opc1 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op1"/"opc1" field.
 * @param       CRn
 *              Is the CRn parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRn" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       op2
 *              Is the op2/opc2 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op2"/"opc2" field.
 * @param       Rt Type "unsigned long"
 *              L-value to write the result into.
 *
 * @def         MCR(op0, op1, CRn, CRm, op2, Rt)
 * @brief       MCR instruction (or manually encoded MSR on AArch64)
 * @param       op0
 *              Is the op1/coproc parameter within the System register encoding space,
 *              in the range 0 to 7 or 8 to 15, respectively, encoded in the "op1"/"coproc" field.
 * @param       op1
 *              Is the op1/opc1 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op1"/"opc1" field.
 * @param       CRn
 *              Is the CRn parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRn" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       op2
 *              Is the op2/opc2 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op2"/"opc2" field.
 * @param       Rt Type "unsigned long"
 *              Value to write into the register.
 *
 * @def         MRS(name, Rt)
 * @brief       MRS instruction
 * @param       name
 *              Name of the special purpose register to read.
 * @param       Rt Type "unsigned long"
 *              L-value to write the result into.
 *
 * @def         MSR(name, Rt)
 * @brief       MSR instruction
 * @param       name
 *              Name of the special purpose register to write into.
 * @param       in Type "unsigned long"
 *              Value to write into the register.
 */

#if defined(__aarch64__)
# define    MRC(op0, op1, CRn, CRm, op2, Rt)    __asm volatile ("mrs %0, S" STRINGIFY(op0) "_" STRINGIFY(op1) "_C" STRINGIFY(CRn) "_C" STRINGIFY(CRm) "_" STRINGIFY(op2) : "=r"(Rt))
# define    MCR(op0, op1, CRn, CRm, op2, Rt)    __asm volatile ("msr S" STRINGIFY(op0) "_" STRINGIFY(op1) "_C" STRINGIFY(CRn) "_C" STRINGIFY(CRm) "_" STRINGIFY(op2) ", %0" :: "r"(Rt))
# define    MRS(name, Rt)                       __asm volatile ("mrs %0, " STRINGIFY(name) : "=r"(Rt))
# define    MSR(name, Rt)                       __asm volatile ("msr " STRINGIFY(name) ", %0" :: "r"(Rt))
#elif defined(__arm__)

/**
 * @def         MRRC(op0, op1, CRm, Rt)
 * @brief       MRRC instruction
 * @param       op0
 *              Is the System register encoding space,
 *              in the range 8 to 15, encoded in the "op1"/"coproc" field.
 * @param       op1
 *              Is the op1/opc1 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op1"/"opc1" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       Rt Type "unsigned int"
 *              L-value to write the first result into.
 * @param       Rt2 Type "unsigned int"
 *              L-value to write the second result into.
 *
 * @def         MCRR(op0, op1, CRm, Rt)
 * @brief       MCRR instruction
 * @param       op0
 *              Is the System register encoding space,
 *              in the range 8 to 15, encoded in the "op1"/"coproc" field.
 * @param       op1
 *              Is the op1/opc1 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op1"/"opc1" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       Rt Type "unsigned int"
 *              First value to write into the register.
 * @param       Rt2 Type "unsigned int"
 *              Second value to write into the register.
 */
# if COMPILER_IS_ARMCC
#   define  MRC(op0, op1, CRn, CRm, op2, Rt)    __asm ("mrc p" STRINGIFY(op0) ", " STRINGIFY(op1) ", " STRINGIFY(Rt) ", c" STRINGIFY(CRn) ", c" STRINGIFY(CRm) ", " STRINGIFY(op2))
#   define  MCR(op0, op1, CRn, CRm, op2, Rt)    __asm ("mcr p" STRINGIFY(op0) ", " STRINGIFY(op1) ", " STRINGIFY(Rt) ", c" STRINGIFY(CRn) ", c" STRINGIFY(CRm) ", " STRINGIFY(op2))
#   define  MRRC(op0, op1, CRm, Rt, Rt2)        __asm ("mrrc p" STRINGIFY(op0) ", " STRINGIFY(op1) ", " STRINGIFY(Rt) ", " STRINGIFY(Rt2) ", c" STRINGIFY(CRm))
#   define  MCRR(op0, op1, CRm, Rt, Rt2)        __asm ("mcrr p" STRINGIFY(op0) ", " STRINGIFY(op1) ", " STRINGIFY(Rt) ", " STRINGIFY(Rt2) ", c" STRINGIFY(CRm))
#   define  MRS(name, Rt)                       __asm ("mrs " STRINGIFY(Rt) ", " STRINGIFY(name))
#   define  MSR(name, Rt)                       __asm ("msr " STRINGIFY(name) ", " STRINGIFY(Rt))
# else
#   define  MRC(op0, op1, CRn, CRm, op2, Rt)    __asm volatile ("mrc p" STRINGIFY(op0) ", " STRINGIFY(op1) ", %0, c" STRINGIFY(CRn) ", c" STRINGIFY(CRm) ", " STRINGIFY(op2) : "=r"(Rt))
#   define  MCR(op0, op1, CRn, CRm, op2, Rt)    __asm volatile ("mcr p" STRINGIFY(op0) ", " STRINGIFY(op1) ", %0, c" STRINGIFY(CRn) ", c" STRINGIFY(CRm) ", " STRINGIFY(op2) :: "r"(Rt))
#   define  MRRC(op0, op1, CRm, Rt, Rt2)        __asm volatile ("mrrc p" STRINGIFY(op0) ", " STRINGIFY(op1) ", %0, %1, c" STRINGIFY(CRm) : "=r"(Rt), "=r"(Rt2))
#   define  MCRR(op0, op1, CRm, Rt, Rt2)        __asm volatile ("mcrr p" STRINGIFY(op0) ", " STRINGIFY(op1) ", %0, %1, c" STRINGIFY(CRm) :: "r"(Rt), "r"(Rt2))
#   define  MRS(name, Rt)                       __asm volatile ("mrs %0, " STRINGIFY(name) : "=r"(Rt))
#   define  MSR(name, Rt)                       __asm volatile ("msr " STRINGIFY(name) ", %0" :: "r"(Rt))
# endif
#endif

#define PMU_ISB() __asm ("isb")

#define PMU_CYCLE_COUNTER 31

/**
 * @defgroup    bm_pmu_intrinsics   Macros for reading and writing PMU registers
 * @{
 *
 * @def         PMU_AT_LEAST_V2
 * @brief       Defines whether targeting PMUv2 or later
 *
 * @def         PMU_AT_LEAST_V3
 * @brief       Defines whether targeting PMUv3 or later
 *
 * @def         PMU_AT_LEAST_V3_1
 * @brief       Defines whether targeting PMUv3 with ARMv8.1 extension or later
 *
 * @def         READ_PMxx(out)
 * @brief       Read from PMU register PMxx (or register that architecturally maps to it)
 * @param       out Type "unsigned long"
 *              L-value to write the result into.
 *
 * @def         WRITE_PMxx(in)
 * @brief       Write to PMU register PMxx (or register that architecturally maps to it)
 * @param       in Type "unsigned long"
 *              Value to write into the register.
 *
 * @def         READ_PMxx_64(out)
 * @brief       Read 64 bits from PMU register PMxx (or register that architecturally maps to it)
 * @param       out Type "unsigned long long"
 *              L-value to write the result into.
 *
 * @def         WRITE_PMxx_64(in)
 * @brief       Write 64 bits to PMU register PMxx (or register that architecturally maps to it)
 * @param       in Type "unsigned long long"
 *              Value to write into the register.
 *
 * @}
 */

/** @{ */

#define PMU_AT_LEAST_V2   1

#if defined(__ARM_ARCH) && (__ARM_ARCH >= 8)
#define PMU_AT_LEAST_V3   1
#else
#define PMU_AT_LEAST_V3   0
#endif

#if defined(__ARM_ARCH) && (__ARM_ARCH >= 801)
#define PMU_AT_LEAST_V3_1 1
#else
#define PMU_AT_LEAST_V3_1 0
#endif

#if PMU_AT_LEAST_V2

#if defined(__arm__)

#define READ_PMCR(x)          MRC(15, 0,  9, 12, 0, x)
#define READ_PMCNTENSET(x)    MRC(15, 0,  9, 12, 1, x)
#define READ_PMCNTENCLR(x)    MRC(15, 0,  9, 12, 2, x)
#define READ_PMOVSR(x)        MRC(15, 0,  9, 12, 3, x)
/* WRITE ONLY PMSWINC */
#define READ_PMSELR(x)        MRC(15, 0,  9, 12, 5, x)
#define READ_PMCEID0(x)       MRC(15, 0,  9, 12, 6, x)
#define READ_PMCEID1(x)       MRC(15, 0,  9, 12, 7, x)

#define READ_PMCCNTR(x)       MRC(15, 0,  9, 13, 0, x)
#define READ_PMXEVTYPER(x)    MRC(15, 0,  9, 13, 1, x)
#define READ_PMXEVCNTR(x)     MRC(15, 0,  9, 13, 2, x)

#define READ_PMUSERENR(x)     MRC(15, 0,  9, 14, 0, x)
#define READ_PMINTENSET(x)    MRC(15, 0,  9, 14, 1, x)
#define READ_PMINTENCLR(x)    MRC(15, 0,  9, 14, 2, x)
#define READ_PMOVSSET(x)      MRC(15, 0,  9, 14, 3, x)

#define WRITE_PMCR(x)         MCR(15, 0,  9, 12, 0, x)
#define WRITE_PMCNTENSET(x)   MCR(15, 0,  9, 12, 1, x)
#define WRITE_PMCNTENCLR(x)   MCR(15, 0,  9, 12, 2, x)
#define WRITE_PMOVSR(x)       MCR(15, 0,  9, 12, 3, x)
#define WRITE_PMSWINC(x)      MCR(15, 0,  9, 12, 4, x)
#define WRITE_PMSELR(x)       MCR(15, 0,  9, 12, 5, x)
/* READ ONLY PMCEID0 */
/* READ ONLY PMCEID1 */

#define WRITE_PMCCNTR(x)      MCR(15, 0,  9, 13, 0, x)
#define WRITE_PMXEVTYPER(x)   MCR(15, 0,  9, 13, 1, x)
#define WRITE_PMXEVCNTR(x)    MCR(15, 0,  9, 13, 2, x)

#define WRITE_PMUSERENR(x)    MCR(15, 0,  9, 14, 0, x)
#define WRITE_PMINTENSET(x)   MCR(15, 0,  9, 14, 1, x)
#define WRITE_PMINTENCLR(x)   MCR(15, 0,  9, 14, 2, x)
#define WRITE_PMOVSSET(x)     MCR(15, 0,  9, 14, 3, x)

#elif defined(__aarch64__)

#define READ_PMCR(x)          MRS(PMCR_EL0,       x)
#define READ_PMCNTENSET(x)    MRS(PMCNTENSET_EL0, x)
#define READ_PMCNTENCLR(x)    MRS(PMCNTENCLR_EL0, x)
#define READ_PMOVSR(x)        MRS(PMOVSCLR_EL0,   x)
/* WRITE ONLY PMSWINC */
#define READ_PMSELR(x)        MRS(PMSELR_EL0,     x)
#define READ_PMCEID0(x)       MRS(PMCEID0_EL0,    x)
#define READ_PMCEID1(x)       MRS(PMCEID1_EL0,    x)

#define READ_PMCCNTR(x)       MRS(PMCCNTR_EL0,    x)
#define READ_PMXEVTYPER(x)    MRS(PMXEVTYPER_EL0, x)
#define READ_PMXEVCNTR(x)     MRS(PMXEVCNTR_EL0,  x)

#define READ_PMUSERENR(x)     MRS(PMUSERENR_EL0,  x)
#define READ_PMINTENSET(x)    MRS(PMINTENSET_EL1, x)
#define READ_PMINTENCLR(x)    MRS(PMINTENCLR_EL1, x)
#define READ_PMOVSSET(x)      MRS(PMOVSSET_EL0,   x)

#define WRITE_PMCR(x)         MSR(PMCR_EL0,       x)
#define WRITE_PMCNTENSET(x)   MSR(PMCNTENSET_EL0, x)
#define WRITE_PMCNTENCLR(x)   MSR(PMCNTENCLR_EL0, x)
#define WRITE_PMOVSR(x)       MSR(PMOVSCLR_EL0,   x)
#define WRITE_PMSWINC(x)      MSR(PMSWINC_EL0,    x)
#define WRITE_PMSELR(x)       MSR(PMSELR_EL0,     x)
/* READ ONLY PMCEID0 */
/* READ ONLY PMCEID1 */

#define WRITE_PMCCNTR(x)      MSR(PMCCNTR_EL0,    x)
#define WRITE_PMXEVTYPER(x)   MSR(PMXEVTYPER_EL0, x)
#define WRITE_PMXEVCNTR(x)    MSR(PMXEVCNTR_EL0,  x)

#define WRITE_PMUSERENR(x)    MSR(PMUSERENR_EL0,  x)
#define WRITE_PMINTENSET(x)   MSR(PMINTENSET_EL1, x)
#define WRITE_PMINTENCLR(x)   MSR(PMINTENCLR_EL1, x)
#define WRITE_PMOVSSET(x)     MSR(PMOVSSET_EL0,   x)

#endif

/* NOT reentrant */
#define READ_PMEVCNTR_NR(n, x)    do { WRITE_PMSELR(n);  PMU_ISB(); READ_PMXEVCNTR(x);   } while (0)
#define READ_PMEVTYPER_NR(n, x)   do { WRITE_PMSELR(n);  PMU_ISB(); READ_PMXEVTYPER(x);  } while (0)
#define READ_PMCCFILTR_NR(x)      do { WRITE_PMSELR(31); PMU_ISB(); READ_PMXEVTYPER(x);  } while (0)
#define WRITE_PMEVCNTR_NR(n, x)   do { WRITE_PMSELR(n);  PMU_ISB(); WRITE_PMXEVCNTR(x);  } while (0)
#define WRITE_PMEVTYPER_NR(n, x)  do { WRITE_PMSELR(n);  PMU_ISB(); WRITE_PMXEVTYPER(x); } while (0)
#define WRITE_PMCCFILTR_NR(x)     do { WRITE_PMSELR(31); PMU_ISB(); WRITE_PMXEVTYPER(x); } while (0)

#endif /* PMU_AT_LEAST_V2 */

#if PMU_AT_LEAST_V3

#if defined(__arm__)

#define READ_PMEVCNTR0(x)      MRC(15, 0, 14,  8, 0, x)
#define READ_PMEVCNTR1(x)      MRC(15, 0, 14,  8, 1, x)
#define READ_PMEVCNTR2(x)      MRC(15, 0, 14,  8, 2, x)
#define READ_PMEVCNTR3(x)      MRC(15, 0, 14,  8, 3, x)
#define READ_PMEVCNTR4(x)      MRC(15, 0, 14,  8, 4, x)
#define READ_PMEVCNTR5(x)      MRC(15, 0, 14,  8, 5, x)
#define READ_PMEVCNTR6(x)      MRC(15, 0, 14,  8, 6, x)
#define READ_PMEVCNTR7(x)      MRC(15, 0, 14,  8, 7, x)

#define READ_PMEVCNTR8(x)      MRC(15, 0, 14,  9, 0, x)
#define READ_PMEVCNTR9(x)      MRC(15, 0, 14,  9, 1, x)
#define READ_PMEVCNTR10(x)     MRC(15, 0, 14,  9, 2, x)
#define READ_PMEVCNTR11(x)     MRC(15, 0, 14,  9, 3, x)
#define READ_PMEVCNTR12(x)     MRC(15, 0, 14,  9, 4, x)
#define READ_PMEVCNTR13(x)     MRC(15, 0, 14,  9, 5, x)
#define READ_PMEVCNTR14(x)     MRC(15, 0, 14,  9, 6, x)
#define READ_PMEVCNTR15(x)     MRC(15, 0, 14,  9, 7, x)

#define READ_PMEVCNTR16(x)     MRC(15, 0, 14, 10, 0, x)
#define READ_PMEVCNTR17(x)     MRC(15, 0, 14, 10, 1, x)
#define READ_PMEVCNTR18(x)     MRC(15, 0, 14, 10, 2, x)
#define READ_PMEVCNTR19(x)     MRC(15, 0, 14, 10, 3, x)
#define READ_PMEVCNTR20(x)     MRC(15, 0, 14, 10, 4, x)
#define READ_PMEVCNTR21(x)     MRC(15, 0, 14, 10, 5, x)
#define READ_PMEVCNTR22(x)     MRC(15, 0, 14, 10, 6, x)
#define READ_PMEVCNTR23(x)     MRC(15, 0, 14, 10, 7, x)

#define READ_PMEVCNTR24(x)     MRC(15, 0, 14, 11, 0, x)
#define READ_PMEVCNTR25(x)     MRC(15, 0, 14, 11, 1, x)
#define READ_PMEVCNTR26(x)     MRC(15, 0, 14, 11, 2, x)
#define READ_PMEVCNTR27(x)     MRC(15, 0, 14, 11, 3, x)
#define READ_PMEVCNTR28(x)     MRC(15, 0, 14, 11, 4, x)
#define READ_PMEVCNTR29(x)     MRC(15, 0, 14, 11, 5, x)
#define READ_PMEVCNTR30(x)     MRC(15, 0, 14, 11, 6, x)

#define READ_PMEVTYPER0(x)     MRC(15, 0, 14, 12, 0, x)
#define READ_PMEVTYPER1(x)     MRC(15, 0, 14, 12, 1, x)
#define READ_PMEVTYPER2(x)     MRC(15, 0, 14, 12, 2, x)
#define READ_PMEVTYPER3(x)     MRC(15, 0, 14, 12, 3, x)
#define READ_PMEVTYPER4(x)     MRC(15, 0, 14, 12, 4, x)
#define READ_PMEVTYPER5(x)     MRC(15, 0, 14, 12, 5, x)
#define READ_PMEVTYPER6(x)     MRC(15, 0, 14, 12, 6, x)
#define READ_PMEVTYPER7(x)     MRC(15, 0, 14, 12, 7, x)

#define READ_PMEVTYPER8(x)     MRC(15, 0, 14, 13, 0, x)
#define READ_PMEVTYPER9(x)     MRC(15, 0, 14, 13, 1, x)
#define READ_PMEVTYPER10(x)    MRC(15, 0, 14, 13, 2, x)
#define READ_PMEVTYPER11(x)    MRC(15, 0, 14, 13, 3, x)
#define READ_PMEVTYPER12(x)    MRC(15, 0, 14, 13, 4, x)
#define READ_PMEVTYPER13(x)    MRC(15, 0, 14, 13, 5, x)
#define READ_PMEVTYPER14(x)    MRC(15, 0, 14, 13, 6, x)
#define READ_PMEVTYPER15(x)    MRC(15, 0, 14, 13, 7, x)

#define READ_PMEVTYPER16(x)    MRC(15, 0, 14, 14, 0, x)
#define READ_PMEVTYPER17(x)    MRC(15, 0, 14, 14, 1, x)
#define READ_PMEVTYPER18(x)    MRC(15, 0, 14, 14, 2, x)
#define READ_PMEVTYPER19(x)    MRC(15, 0, 14, 14, 3, x)
#define READ_PMEVTYPER20(x)    MRC(15, 0, 14, 14, 4, x)
#define READ_PMEVTYPER21(x)    MRC(15, 0, 14, 14, 5, x)
#define READ_PMEVTYPER22(x)    MRC(15, 0, 14, 14, 6, x)
#define READ_PMEVTYPER23(x)    MRC(15, 0, 14, 14, 7, x)

#define READ_PMEVTYPER24(x)    MRC(15, 0, 14, 15, 0, x)
#define READ_PMEVTYPER25(x)    MRC(15, 0, 14, 15, 1, x)
#define READ_PMEVTYPER26(x)    MRC(15, 0, 14, 15, 2, x)
#define READ_PMEVTYPER27(x)    MRC(15, 0, 14, 15, 3, x)
#define READ_PMEVTYPER28(x)    MRC(15, 0, 14, 15, 4, x)
#define READ_PMEVTYPER29(x)    MRC(15, 0, 14, 15, 5, x)
#define READ_PMEVTYPER30(x)    MRC(15, 0, 14, 15, 6, x)
#define READ_PMCCFILTR(x)      MRC(15, 0, 14, 15, 7, x)

#define WRITE_PMEVCNTR0(x)     MCR(15, 0, 14,  8, 0, x)
#define WRITE_PMEVCNTR1(x)     MCR(15, 0, 14,  8, 1, x)
#define WRITE_PMEVCNTR2(x)     MCR(15, 0, 14,  8, 2, x)
#define WRITE_PMEVCNTR3(x)     MCR(15, 0, 14,  8, 3, x)
#define WRITE_PMEVCNTR4(x)     MCR(15, 0, 14,  8, 4, x)
#define WRITE_PMEVCNTR5(x)     MCR(15, 0, 14,  8, 5, x)
#define WRITE_PMEVCNTR6(x)     MCR(15, 0, 14,  8, 6, x)
#define WRITE_PMEVCNTR7(x)     MCR(15, 0, 14,  8, 7, x)

#define WRITE_PMEVCNTR8(x)     MCR(15, 0, 14,  9, 0, x)
#define WRITE_PMEVCNTR9(x)     MCR(15, 0, 14,  9, 1, x)
#define WRITE_PMEVCNTR10(x)    MCR(15, 0, 14,  9, 2, x)
#define WRITE_PMEVCNTR11(x)    MCR(15, 0, 14,  9, 3, x)
#define WRITE_PMEVCNTR12(x)    MCR(15, 0, 14,  9, 4, x)
#define WRITE_PMEVCNTR13(x)    MCR(15, 0, 14,  9, 5, x)
#define WRITE_PMEVCNTR14(x)    MCR(15, 0, 14,  9, 6, x)
#define WRITE_PMEVCNTR15(x)    MCR(15, 0, 14,  9, 7, x)

#define WRITE_PMEVCNTR16(x)    MCR(15, 0, 14, 10, 0, x)
#define WRITE_PMEVCNTR17(x)    MCR(15, 0, 14, 10, 1, x)
#define WRITE_PMEVCNTR18(x)    MCR(15, 0, 14, 10, 2, x)
#define WRITE_PMEVCNTR19(x)    MCR(15, 0, 14, 10, 3, x)
#define WRITE_PMEVCNTR20(x)    MCR(15, 0, 14, 10, 4, x)
#define WRITE_PMEVCNTR21(x)    MCR(15, 0, 14, 10, 5, x)
#define WRITE_PMEVCNTR22(x)    MCR(15, 0, 14, 10, 6, x)
#define WRITE_PMEVCNTR23(x)    MCR(15, 0, 14, 10, 7, x)

#define WRITE_PMEVCNTR24(x)    MCR(15, 0, 14, 11, 0, x)
#define WRITE_PMEVCNTR25(x)    MCR(15, 0, 14, 11, 1, x)
#define WRITE_PMEVCNTR26(x)    MCR(15, 0, 14, 11, 2, x)
#define WRITE_PMEVCNTR27(x)    MCR(15, 0, 14, 11, 3, x)
#define WRITE_PMEVCNTR28(x)    MCR(15, 0, 14, 11, 4, x)
#define WRITE_PMEVCNTR29(x)    MCR(15, 0, 14, 11, 5, x)
#define WRITE_PMEVCNTR30(x)    MCR(15, 0, 14, 11, 6, x)

#define WRITE_PMEVTYPER0(x)    MCR(15, 0, 14, 12, 0, x)
#define WRITE_PMEVTYPER1(x)    MCR(15, 0, 14, 12, 1, x)
#define WRITE_PMEVTYPER2(x)    MCR(15, 0, 14, 12, 2, x)
#define WRITE_PMEVTYPER3(x)    MCR(15, 0, 14, 12, 3, x)
#define WRITE_PMEVTYPER4(x)    MCR(15, 0, 14, 12, 4, x)
#define WRITE_PMEVTYPER5(x)    MCR(15, 0, 14, 12, 5, x)
#define WRITE_PMEVTYPER6(x)    MCR(15, 0, 14, 12, 6, x)
#define WRITE_PMEVTYPER7(x)    MCR(15, 0, 14, 12, 7, x)

#define WRITE_PMEVTYPER8(x)    MCR(15, 0, 14, 13, 0, x)
#define WRITE_PMEVTYPER9(x)    MCR(15, 0, 14, 13, 1, x)
#define WRITE_PMEVTYPER10(x)   MCR(15, 0, 14, 13, 2, x)
#define WRITE_PMEVTYPER11(x)   MCR(15, 0, 14, 13, 3, x)
#define WRITE_PMEVTYPER12(x)   MCR(15, 0, 14, 13, 4, x)
#define WRITE_PMEVTYPER13(x)   MCR(15, 0, 14, 13, 5, x)
#define WRITE_PMEVTYPER14(x)   MCR(15, 0, 14, 13, 6, x)
#define WRITE_PMEVTYPER15(x)   MCR(15, 0, 14, 13, 7, x)

#define WRITE_PMEVTYPER16(x)   MCR(15, 0, 14, 14, 0, x)
#define WRITE_PMEVTYPER17(x)   MCR(15, 0, 14, 14, 1, x)
#define WRITE_PMEVTYPER18(x)   MCR(15, 0, 14, 14, 2, x)
#define WRITE_PMEVTYPER19(x)   MCR(15, 0, 14, 14, 3, x)
#define WRITE_PMEVTYPER20(x)   MCR(15, 0, 14, 14, 4, x)
#define WRITE_PMEVTYPER21(x)   MCR(15, 0, 14, 14, 5, x)
#define WRITE_PMEVTYPER22(x)   MCR(15, 0, 14, 14, 6, x)
#define WRITE_PMEVTYPER23(x)   MCR(15, 0, 14, 14, 7, x)

#define WRITE_PMEVTYPER24(x)   MCR(15, 0, 14, 15, 0, x)
#define WRITE_PMEVTYPER25(x)   MCR(15, 0, 14, 15, 1, x)
#define WRITE_PMEVTYPER26(x)   MCR(15, 0, 14, 15, 2, x)
#define WRITE_PMEVTYPER27(x)   MCR(15, 0, 14, 15, 3, x)
#define WRITE_PMEVTYPER28(x)   MCR(15, 0, 14, 15, 4, x)
#define WRITE_PMEVTYPER29(x)   MCR(15, 0, 14, 15, 5, x)
#define WRITE_PMEVTYPER30(x)   MCR(15, 0, 14, 15, 6, x)
#define WRITE_PMCCFILTR(x)     MCR(15, 0, 14, 15, 7, x)

#define READ_PMCCNTR_64(x)     MRRC_64(15, 0, 9, x)
#define WRITE_PMCCNTR_64(x)    MCRR_64(15, 0, 9, x)

#elif defined(__aarch64__)

#define READ_PMEVCNTR0(x)     MRS(PMEVCNTR0_EL0,   x)
#define READ_PMEVCNTR1(x)     MRS(PMEVCNTR1_EL0,   x)
#define READ_PMEVCNTR2(x)     MRS(PMEVCNTR2_EL0,   x)
#define READ_PMEVCNTR3(x)     MRS(PMEVCNTR3_EL0,   x)
#define READ_PMEVCNTR4(x)     MRS(PMEVCNTR4_EL0,   x)
#define READ_PMEVCNTR5(x)     MRS(PMEVCNTR5_EL0,   x)
#define READ_PMEVCNTR6(x)     MRS(PMEVCNTR6_EL0,   x)
#define READ_PMEVCNTR7(x)     MRS(PMEVCNTR7_EL0,   x)

#define READ_PMEVCNTR8(x)     MRS(PMEVCNTR8_EL0,   x)
#define READ_PMEVCNTR9(x)     MRS(PMEVCNTR9_EL0,   x)
#define READ_PMEVCNTR10(x)    MRS(PMEVCNTR10_EL0,  x)
#define READ_PMEVCNTR11(x)    MRS(PMEVCNTR11_EL0,  x)
#define READ_PMEVCNTR12(x)    MRS(PMEVCNTR12_EL0,  x)
#define READ_PMEVCNTR13(x)    MRS(PMEVCNTR13_EL0,  x)
#define READ_PMEVCNTR14(x)    MRS(PMEVCNTR14_EL0,  x)
#define READ_PMEVCNTR15(x)    MRS(PMEVCNTR15_EL0,  x)

#define READ_PMEVCNTR16(x)    MRS(PMEVCNTR16_EL0,  x)
#define READ_PMEVCNTR17(x)    MRS(PMEVCNTR17_EL0,  x)
#define READ_PMEVCNTR18(x)    MRS(PMEVCNTR18_EL0,  x)
#define READ_PMEVCNTR19(x)    MRS(PMEVCNTR19_EL0,  x)
#define READ_PMEVCNTR20(x)    MRS(PMEVCNTR20_EL0,  x)
#define READ_PMEVCNTR21(x)    MRS(PMEVCNTR21_EL0,  x)
#define READ_PMEVCNTR22(x)    MRS(PMEVCNTR22_EL0,  x)
#define READ_PMEVCNTR23(x)    MRS(PMEVCNTR23_EL0,  x)

#define READ_PMEVCNTR24(x)    MRS(PMEVCNTR24_EL0,  x)
#define READ_PMEVCNTR25(x)    MRS(PMEVCNTR25_EL0,  x)
#define READ_PMEVCNTR26(x)    MRS(PMEVCNTR26_EL0,  x)
#define READ_PMEVCNTR27(x)    MRS(PMEVCNTR27_EL0,  x)
#define READ_PMEVCNTR28(x)    MRS(PMEVCNTR28_EL0,  x)
#define READ_PMEVCNTR29(x)    MRS(PMEVCNTR29_EL0,  x)
#define READ_PMEVCNTR30(x)    MRS(PMEVCNTR30_EL0,  x)

#define READ_PMEVTYPER0(x)    MRS(PMEVTYPER0_EL0,  x)
#define READ_PMEVTYPER1(x)    MRS(PMEVTYPER1_EL0,  x)
#define READ_PMEVTYPER2(x)    MRS(PMEVTYPER2_EL0,  x)
#define READ_PMEVTYPER3(x)    MRS(PMEVTYPER3_EL0,  x)
#define READ_PMEVTYPER4(x)    MRS(PMEVTYPER4_EL0,  x)
#define READ_PMEVTYPER5(x)    MRS(PMEVTYPER5_EL0,  x)
#define READ_PMEVTYPER6(x)    MRS(PMEVTYPER6_EL0,  x)
#define READ_PMEVTYPER7(x)    MRS(PMEVTYPER7_EL0,  x)

#define READ_PMEVTYPER8(x)    MRS(PMEVTYPER8_EL0,  x)
#define READ_PMEVTYPER9(x)    MRS(PMEVTYPER9_EL0,  x)
#define READ_PMEVTYPER10(x)   MRS(PMEVTYPER10_EL0, x)
#define READ_PMEVTYPER11(x)   MRS(PMEVTYPER11_EL0, x)
#define READ_PMEVTYPER12(x)   MRS(PMEVTYPER12_EL0, x)
#define READ_PMEVTYPER13(x)   MRS(PMEVTYPER13_EL0, x)
#define READ_PMEVTYPER14(x)   MRS(PMEVTYPER14_EL0, x)
#define READ_PMEVTYPER15(x)   MRS(PMEVTYPER15_EL0, x)

#define READ_PMEVTYPER16(x)   MRS(PMEVTYPER16_EL0, x)
#define READ_PMEVTYPER17(x)   MRS(PMEVTYPER17_EL0, x)
#define READ_PMEVTYPER18(x)   MRS(PMEVTYPER18_EL0, x)
#define READ_PMEVTYPER19(x)   MRS(PMEVTYPER19_EL0, x)
#define READ_PMEVTYPER20(x)   MRS(PMEVTYPER20_EL0, x)
#define READ_PMEVTYPER21(x)   MRS(PMEVTYPER21_EL0, x)
#define READ_PMEVTYPER22(x)   MRS(PMEVTYPER22_EL0, x)
#define READ_PMEVTYPER23(x)   MRS(PMEVTYPER23_EL0, x)

#define READ_PMEVTYPER24(x)   MRS(PMEVTYPER24_EL0, x)
#define READ_PMEVTYPER25(x)   MRS(PMEVTYPER25_EL0, x)
#define READ_PMEVTYPER26(x)   MRS(PMEVTYPER26_EL0, x)
#define READ_PMEVTYPER27(x)   MRS(PMEVTYPER27_EL0, x)
#define READ_PMEVTYPER28(x)   MRS(PMEVTYPER28_EL0, x)
#define READ_PMEVTYPER29(x)   MRS(PMEVTYPER29_EL0, x)
#define READ_PMEVTYPER30(x)   MRS(PMEVTYPER30_EL0, x)
#define READ_PMCCFILTR(x)     MRS(PMCCFILTR_EL0,   x)

#define WRITE_PMEVCNTR0(x)    MSR(PMEVCNTR0_EL0,   x)
#define WRITE_PMEVCNTR1(x)    MSR(PMEVCNTR1_EL0,   x)
#define WRITE_PMEVCNTR2(x)    MSR(PMEVCNTR2_EL0,   x)
#define WRITE_PMEVCNTR3(x)    MSR(PMEVCNTR3_EL0,   x)
#define WRITE_PMEVCNTR4(x)    MSR(PMEVCNTR4_EL0,   x)
#define WRITE_PMEVCNTR5(x)    MSR(PMEVCNTR5_EL0,   x)
#define WRITE_PMEVCNTR6(x)    MSR(PMEVCNTR6_EL0,   x)
#define WRITE_PMEVCNTR7(x)    MSR(PMEVCNTR7_EL0,   x)

#define WRITE_PMEVCNTR8(x)    MSR(PMEVCNTR8_EL0,   x)
#define WRITE_PMEVCNTR9(x)    MSR(PMEVCNTR9_EL0,   x)
#define WRITE_PMEVCNTR10(x)   MSR(PMEVCNTR10_EL0,  x)
#define WRITE_PMEVCNTR11(x)   MSR(PMEVCNTR11_EL0,  x)
#define WRITE_PMEVCNTR12(x)   MSR(PMEVCNTR12_EL0,  x)
#define WRITE_PMEVCNTR13(x)   MSR(PMEVCNTR13_EL0,  x)
#define WRITE_PMEVCNTR14(x)   MSR(PMEVCNTR14_EL0,  x)
#define WRITE_PMEVCNTR15(x)   MSR(PMEVCNTR15_EL0,  x)

#define WRITE_PMEVCNTR16(x)   MSR(PMEVCNTR16_EL0,  x)
#define WRITE_PMEVCNTR17(x)   MSR(PMEVCNTR17_EL0,  x)
#define WRITE_PMEVCNTR18(x)   MSR(PMEVCNTR18_EL0,  x)
#define WRITE_PMEVCNTR19(x)   MSR(PMEVCNTR19_EL0,  x)
#define WRITE_PMEVCNTR20(x)   MSR(PMEVCNTR20_EL0,  x)
#define WRITE_PMEVCNTR21(x)   MSR(PMEVCNTR21_EL0,  x)
#define WRITE_PMEVCNTR22(x)   MSR(PMEVCNTR22_EL0,  x)
#define WRITE_PMEVCNTR23(x)   MSR(PMEVCNTR23_EL0,  x)

#define WRITE_PMEVCNTR24(x)   MSR(PMEVCNTR24_EL0,  x)
#define WRITE_PMEVCNTR25(x)   MSR(PMEVCNTR25_EL0,  x)
#define WRITE_PMEVCNTR26(x)   MSR(PMEVCNTR26_EL0,  x)
#define WRITE_PMEVCNTR27(x)   MSR(PMEVCNTR27_EL0,  x)
#define WRITE_PMEVCNTR28(x)   MSR(PMEVCNTR28_EL0,  x)
#define WRITE_PMEVCNTR29(x)   MSR(PMEVCNTR29_EL0,  x)
#define WRITE_PMEVCNTR30(x)   MSR(PMEVCNTR30_EL0,  x)

#define WRITE_PMEVTYPER0(x)   MSR(PMEVTYPER0_EL0,  x)
#define WRITE_PMEVTYPER1(x)   MSR(PMEVTYPER1_EL0,  x)
#define WRITE_PMEVTYPER2(x)   MSR(PMEVTYPER2_EL0,  x)
#define WRITE_PMEVTYPER3(x)   MSR(PMEVTYPER3_EL0,  x)
#define WRITE_PMEVTYPER4(x)   MSR(PMEVTYPER4_EL0,  x)
#define WRITE_PMEVTYPER5(x)   MSR(PMEVTYPER5_EL0,  x)
#define WRITE_PMEVTYPER6(x)   MSR(PMEVTYPER6_EL0,  x)
#define WRITE_PMEVTYPER7(x)   MSR(PMEVTYPER7_EL0,  x)

#define WRITE_PMEVTYPER8(x)   MSR(PMEVTYPER8_EL0,  x)
#define WRITE_PMEVTYPER9(x)   MSR(PMEVTYPER9_EL0,  x)
#define WRITE_PMEVTYPER10(x)  MSR(PMEVTYPER10_EL0, x)
#define WRITE_PMEVTYPER11(x)  MSR(PMEVTYPER11_EL0, x)
#define WRITE_PMEVTYPER12(x)  MSR(PMEVTYPER12_EL0, x)
#define WRITE_PMEVTYPER13(x)  MSR(PMEVTYPER13_EL0, x)
#define WRITE_PMEVTYPER14(x)  MSR(PMEVTYPER14_EL0, x)
#define WRITE_PMEVTYPER15(x)  MSR(PMEVTYPER15_EL0, x)

#define WRITE_PMEVTYPER16(x)  MSR(PMEVTYPER16_EL0, x)
#define WRITE_PMEVTYPER17(x)  MSR(PMEVTYPER17_EL0, x)
#define WRITE_PMEVTYPER18(x)  MSR(PMEVTYPER18_EL0, x)
#define WRITE_PMEVTYPER19(x)  MSR(PMEVTYPER19_EL0, x)
#define WRITE_PMEVTYPER20(x)  MSR(PMEVTYPER20_EL0, x)
#define WRITE_PMEVTYPER21(x)  MSR(PMEVTYPER21_EL0, x)
#define WRITE_PMEVTYPER22(x)  MSR(PMEVTYPER22_EL0, x)
#define WRITE_PMEVTYPER23(x)  MSR(PMEVTYPER23_EL0, x)

#define WRITE_PMEVTYPER24(x)  MSR(PMEVTYPER24_EL0, x)
#define WRITE_PMEVTYPER25(x)  MSR(PMEVTYPER25_EL0, x)
#define WRITE_PMEVTYPER26(x)  MSR(PMEVTYPER26_EL0, x)
#define WRITE_PMEVTYPER27(x)  MSR(PMEVTYPER27_EL0, x)
#define WRITE_PMEVTYPER28(x)  MSR(PMEVTYPER28_EL0, x)
#define WRITE_PMEVTYPER29(x)  MSR(PMEVTYPER29_EL0, x)
#define WRITE_PMEVTYPER30(x)  MSR(PMEVTYPER30_EL0, x)
#define WRITE_PMCCFILTR(x)    MSR(PMCCFILTR_EL0,   x)

#define READ_PMCCNTR_64(x)    READ_PMCCNTR(x)
#define WRITE_PMCCNTR_64(x)   WRITE_PMCCNTR(x)

#endif

#define READ_PMEVCNTR_LITERAL(n,x)     READ_PMEVCNTR ##   n(x)
#define READ_PMEVTYPER_LITERAL(n,x)    READ_PMEVTYPER ##  n(x)
#define WRITE_PMEVCNTR_LITERAL(n,x)    WRITE_PMEVCNTR ##  n(x)
#define WRITE_PMEVTYPER_LITERAL(n,x)   WRITE_PMEVTYPER ## n(x)

#define CALL_WITH_COUNTER_LITERAL_1(function, default_function, counter, arg1) \
    switch (counter)                        \
    {                                       \
        case  0: function( 0, arg1); break; \
        case  1: function( 1, arg1); break; \
        case  2: function( 2, arg1); break; \
        case  3: function( 3, arg1); break; \
        case  4: function( 4, arg1); break; \
        case  5: function( 5, arg1); break; \
        case  6: function( 6, arg1); break; \
        case  7: function( 7, arg1); break; \
        case  8: function( 8, arg1); break; \
        case  9: function( 9, arg1); break; \
        case 10: function(10, arg1); break; \
        case 11: function(11, arg1); break; \
        case 12: function(12, arg1); break; \
        case 13: function(13, arg1); break; \
        case 14: function(14, arg1); break; \
        case 15: function(15, arg1); break; \
        case 16: function(16, arg1); break; \
        case 17: function(17, arg1); break; \
        case 18: function(18, arg1); break; \
        case 19: function(19, arg1); break; \
        case 20: function(20, arg1); break; \
        case 21: function(21, arg1); break; \
        case 22: function(22, arg1); break; \
        case 23: function(23, arg1); break; \
        case 24: function(24, arg1); break; \
        case 25: function(25, arg1); break; \
        case 26: function(26, arg1); break; \
        case 27: function(27, arg1); break; \
        case 28: function(28, arg1); break; \
        case 29: function(29, arg1); break; \
        case 30: function(30, arg1); break; \
        default: default_function(arg1);    \
    }

#define DO_NOTHING_1(x)        (void)(x)
#define SET_TO_ZERO_1(x)       (x) = 0

#define READ_PMEVCNTR(n,   x)   CALL_WITH_COUNTER_LITERAL_1(READ_PMEVCNTR_LITERAL,   SET_TO_ZERO_1, n, x)
#define READ_PMEVTYPER(n,  x)   CALL_WITH_COUNTER_LITERAL_1(READ_PMEVTYPER_LITERAL,  SET_TO_ZERO_1, n, x)
#define WRITE_PMEVCNTR(n,  x)   CALL_WITH_COUNTER_LITERAL_1(WRITE_PMEVCNTR_LITERAL,  DO_NOTHING_1,  n, x)
#define WRITE_PMEVTYPER(n, x)   CALL_WITH_COUNTER_LITERAL_1(WRITE_PMEVTYPER_LITERAL, DO_NOTHING_1,  n, x)

#endif /* PMU_AT_LEAST_V3 */

#if PMU_AT_LEAST_V3_1

#if defined(__arm__)

#define READ_PMCEID2(x)    /* PMCEID0_EL0[63:32] */ MRC(15, 0,  9, 14, 4, x)
#define READ_PMCEID3(x)    /* PMCEID1_EL0[63:32] */ MRC(15, 0,  9, 14, 5, x)

#define READ_PMCEID0_64(x) /* PMCEID0_EL0        */       \
    do {                                                     \
        unsigned int _pmceid0;                               \
        unsigned int _pmceid2;                               \
        READ_PMCEID0(_pmceid0);                        \
        READ_PMCEID2(_pmceid2);                        \
        x = (((unsigned long long) _pmceid2) << 32) | _pmceid0; \
    } while (0)
#define READ_PMCEID1_64(x) /* PMCEID1_EL0        */       \
    do {                                                     \
        unsigned int _pmceid1;                               \
        unsigned int _pmceid3;                               \
        READ_PMCEID1(_pmceid1);                        \
        READ_PMCEID3(_pmceid3);                        \
        x = (((unsigned long long) _pmceid3) << 32) | _pmceid1; \
    } while (0)

#elif defined(__aarch64__)

#define READ_PMCEID2(x)    /* PMCEID0_EL0[63:32] */ do { READ_PMCEID0(x); x >> 32; } while (0)
#define READ_PMCEID3(x)    /* PMCEID1_EL0[63:32] */ do { READ_PMCEID1(x); x >> 32; } while (0)

#define READ_PMCEID0_64(x) /* PMCEID0_EL0        */ READ_PMCEID0(x)
#define READ_PMCEID1_64(x) /* PMCEID1_EL0        */ READ_PMCEID1(x)

#endif

#endif /* PMU_AT_LEAST_V3_1 */

/** @} */

#endif /* INCLUDE_PMU_PRIVATE_H */
