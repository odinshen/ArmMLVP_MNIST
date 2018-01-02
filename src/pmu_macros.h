/* Copyright (c) 2016 by ARM Limited. All rights reserved. */

#ifndef INCLUDE_PMU_MACROS
#define INCLUDE_PMU_MACROS

/**
 * @def         READ_PMU_REG(CRn, CRm, op2, out)
 * @brief       Read from a PMU system register with "op1" = 3 (AArch64) or "opc2" = 0 (AArch32)
 * @param       CRn
 *              Is the CRn parameter within the System register encoding space,
 *              in the range 0 to 15,encoded in the "CRn" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       op2
 *              Is the op2/opc2 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op2"/"opc2" field.
 * @param       out Type "unsigned long"
 *              L-value to write the result into.
 *
 * @def         WRITE_PMU_REG(CRn, CRm, op2, in)
 * @brief       Writes to a PMU system register with "op1" = 3 (AArch64) or "opc2" = 0 (AArch32)
 * @param       CRn
 *              Is the CRn parameter within the System register encoding space,
 *              in the range 0 to 15,encoded in the "CRn" field.
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       op2
 *              Is the op2/opc2 parameter within the System register encoding space,
 *              in the range 0 to 7, encoded in the "op2"/"opc2" field.
 * @param       in Type "unsigned long"
 *              Value to write into the register.
 */

#if defined(__aarch64__)
# define    READ_PMU_REG(CRn, CRm, op2, out)    asm volatile ("mrs %0, S3_3_C" #CRn "_C" #CRm "_" #op2 : "=r"(out))
# define    WRITE_PMU_REG(CRn, CRm, op2, in)    asm volatile ("msr S3_3_C" #CRn "_C" #CRm "_" #op2 ", %0" :: "r"(in))
#elif defined(__arm__)

/**
 * @def         READ_PMU_REG_64(CRm, out)
 * @brief       Read from a PMU system register
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       out Type "unsigned long long"
 *              L-value to write the result into.
 *
 * @def         WRITE_PMU_REG_64(CRm, in)
 * @brief       Writes to a PMU system register
 * @param       CRm
 *              Is the CRm parameter within the System register encoding space,
 *              in the range 0 to 15, encoded in the "CRm" field.
 * @param       in Type "unsigned long long"
 *              Value to write into the register.
 */

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION < 6000000) /* ARM Compiler 5 */
#   define  READ_PMU_REG(CRn, CRm, op2, out)    __asm ("mrc p15, 0, " #out ", c" #CRn ", c" #CRm ", " #op2)
#   define  WRITE_PMU_REG(CRn, CRm, op2, in)    __asm ("mcr p15, 0, " #in ", c" #CRn ", c" #CRm ", " #op2)
#   define  READ_PMU_REG_64(CRm, out)                                             \
                do {                                                              \
                    unsigned int _pmu_lower;                                      \
                    unsigned int _pmu_upper;                                      \
                    __asm ("mrrc p15, 0, _pmu_lower, _pmu_upper, c" #CRm)         \
                    out = (((unsigned long long) _pmu_upper) << 32) | _pmu_lower; \
                } while (0)
#   define  WRITE_PMU_REG_64(CRm, in)           __asm ("mcrr p15, 0, (unsigned int)" #in ", (unsigned int)((" #in ") >> 32), c" #CRm)
# else
#   define  READ_PMU_REG(CRn, CRm, op2, out)    asm volatile ("mrc p15, 0, %0, c" #CRn ", c" #CRm ", " #op2 : "=r"(out))
#   define  WRITE_PMU_REG(CRn, CRm, op2, in)    asm volatile ("mcr p15, 0, %0, c" #CRn ", c" #CRm ", " #op2 :: "r"(in))
#   define  READ_PMU_REG_64(CRm, out)                                                                         \
                do {                                                                                          \
                    unsigned int _pmu_lower;                                                                  \
                    unsigned int _pmu_upper;                                                                  \
                    asm volatile ("mrrc p15, 0, %0, %1, c" #CRm : "=r"(_pmu_lower), "=r"(_pmu_upper))         \
                    out = (((unsigned long long) _pmu_upper) << 32) | _pmu_lower;                             \
                } while (0)
#   define  WRITE_PMU_REG_64(CRm, in)           asm volatile ("mcrr p15, 0, %0, %1, c" #CRm :: "r"((unsigned int)(in)), "r"((unsigned int)((in) >> 32))
# endif
#endif

/**
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
 */

#define PMU_AT_LEAST_V2   1
#define PMU_AT_LEAST_V3   (__ARM_ARCH >= 8)
#define PMU_AT_LEAST_V3_1 (__ARM_ARCH >= 801)

/*           AArch32 name        AArch64 name    */

#if PMU_AT_LEAST_V2

// #define READ_PMCR(x)          /* PMCR_EL0        */ READ_PMU_REG( 9, 12, 0, x)
#define PMCR 9, 12, 0
#define READ_PMU_REG2(x,y) READ_PMU_REG(x,y)
#define READ_PMCR(x)          /* PMCR_EL0        */ READ_PMU_REG2( PMCR, x)
#define READ_PMCNTENSET(x)    /* PMCNTENSET_EL0  */ READ_PMU_REG( 9, 12, 1, x)
#define READ_PMCNTENCLR(x)    /* PMCNTENCLR_EL0  */ READ_PMU_REG( 9, 12, 2, x)
#define READ_PMOVSR(x)        /* PMOVSCLR_EL0    */ READ_PMU_REG( 9, 12, 3, x)
       /* WO PMSWINC */       /* PMSWINC_EL0     */
#define READ_PMSELR(x)        /* PMSELR_EL0      */ READ_PMU_REG( 9, 12, 5, x)
#define READ_PMCEID0(x)       /* PMCEID0_EL0     */ READ_PMU_REG( 9, 12, 6, x)
#define READ_PMCEID1(x)       /* PMCEID1_EL0     */ READ_PMU_REG( 9, 12, 7, x)

#define READ_PMCCNTR(x)       /* PMCCNTR_EL0     */ READ_PMU_REG( 9, 13, 0, x)
#define READ_PMXEVTYPER(x)    /* PMXEVTYPER_EL0  */ READ_PMU_REG( 9, 13, 1, x)
#define READ_PMXEVCNTR(x)     /* PMXEVCNTR_EL0   */ READ_PMU_REG( 9, 13, 2, x)

#define READ_PMUSERENR(x)     /* PMUSERENR_EL0   */ READ_PMU_REG( 9, 14, 0, x)
#if defined(__arm__)
#define READ_PMINTENSET(x)    /* PMINTENSET_EL1  */ READ_PMU_REG( 9, 14, 1, x)
#define READ_PMINTENCLR(x)    /* PMINTENCLR_EL1  */ READ_PMU_REG( 9, 14, 2, x)
#elif defined(__aarch64__)
#define READ_PMINTENSET(x)    /* PMINTENSET_EL1  */ asm volatile ("mrs %0, PMINTENSET_EL1" : "=r"(x))
#define READ_PMINTENCLR(x)    /* PMINTENCLR_EL1  */ asm volatile ("mrs %0, PMINTENCLR_EL1" : "=r"(x))
#endif
#define READ_PMOVSSET(x)      /* PMOVSSET_EL0    */ READ_PMU_REG( 9, 14, 3, x)

#define WRITE_PMCR(x)         /* PMCR_EL0        */ WRITE_PMU_REG( 9, 12, 0, x)
#define WRITE_PMCNTENSET(x)   /* PMCNTENSET_EL0  */ WRITE_PMU_REG( 9, 12, 1, x)
#define WRITE_PMCNTENCLR(x)   /* PMCNTENCLR_EL0  */ WRITE_PMU_REG( 9, 12, 2, x)
#define WRITE_PMOVSR(x)       /* PMOVSCLR_EL0    */ WRITE_PMU_REG( 9, 12, 3, x)
#define WRITE_PMSWINC(x)      /* PMSWINC_EL0     */ WRITE_PMU_REG( 9, 12, 4, x)
#define WRITE_PMSELR(x)       /* PMSELR_EL0      */ WRITE_PMU_REG( 9, 12, 5, x)
        /* RO PMCEID0 */      /* PMCEID0_EL0     */
        /* RO PMCEID1 */      /* PMCEID1_EL0     */

#define WRITE_PMCCNTR(x)      /* PMCCNTR_EL0     */ WRITE_PMU_REG( 9, 13, 0, x)
#define WRITE_PMXEVTYPER(x)   /* PMXEVTYPER_EL0  */ WRITE_PMU_REG( 9, 13, 1, x)
#define WRITE_PMXEVCNTR(x)    /* PMXEVCNTR_EL0   */ WRITE_PMU_REG( 9, 13, 2, x)

#define WRITE_PMUSERENR(x)    /* PMUSERENR_EL0   */ WRITE_PMU_REG( 9, 14, 0, x)
#if defined(__arm__)
#define WRITE_PMINTENSET(x)   /* PMINTENSET_EL1  */ WRITE_PMU_REG( 9, 14, 1, x)
#define WRITE_PMINTENCLR(x)   /* PMINTENCLR_EL1  */ WRITE_PMU_REG( 9, 14, 2, x)
#elif defined(__aarch64__)
#define WRITE_PMINTENSET(x)   /* PMINTENSET_EL1  */ asm volatile ("msr PMINTENSET_EL1, %0" :: "r"(x))
#define WRITE_PMINTENCLR(x)   /* PMINTENCLR_EL1  */ asm volatile ("msr PMINTENCLR_EL1, %0" :: "r"(x))
#endif
#define WRITE_PMOVSSET(x)     /* PMOVSSET_EL0    */ WRITE_PMU_REG( 9, 14, 3, x)

#endif /* PMU_AT_LEAST_V2 */

#if PMU_AT_LEAST_V3

#define READ_PMEVCNTR0(x)     /* PMEVCNTR0_EL0   */ READ_PMU_REG(14,  8, 0, x)
#define READ_PMEVCNTR1(x)     /* PMEVCNTR1_EL0   */ READ_PMU_REG(14,  8, 1, x)
#define READ_PMEVCNTR2(x)     /* PMEVCNTR2_EL0   */ READ_PMU_REG(14,  8, 2, x)
#define READ_PMEVCNTR3(x)     /* PMEVCNTR3_EL0   */ READ_PMU_REG(14,  8, 3, x)
#define READ_PMEVCNTR4(x)     /* PMEVCNTR4_EL0   */ READ_PMU_REG(14,  8, 4, x)
#define READ_PMEVCNTR5(x)     /* PMEVCNTR5_EL0   */ READ_PMU_REG(14,  8, 5, x)
#define READ_PMEVCNTR6(x)     /* PMEVCNTR6_EL0   */ READ_PMU_REG(14,  8, 6, x)
#define READ_PMEVCNTR7(x)     /* PMEVCNTR7_EL0   */ READ_PMU_REG(14,  8, 7, x)

#define READ_PMEVCNTR8(x)     /* PMEVCNTR8_EL0   */ READ_PMU_REG(14,  9, 0, x)
#define READ_PMEVCNTR9(x)     /* PMEVCNTR9_EL0   */ READ_PMU_REG(14,  9, 1, x)
#define READ_PMEVCNTR10(x)    /* PMEVCNTR10_EL0  */ READ_PMU_REG(14,  9, 2, x)
#define READ_PMEVCNTR11(x)    /* PMEVCNTR11_EL0  */ READ_PMU_REG(14,  9, 3, x)
#define READ_PMEVCNTR12(x)    /* PMEVCNTR12_EL0  */ READ_PMU_REG(14,  9, 4, x)
#define READ_PMEVCNTR13(x)    /* PMEVCNTR13_EL0  */ READ_PMU_REG(14,  9, 5, x)
#define READ_PMEVCNTR14(x)    /* PMEVCNTR14_EL0  */ READ_PMU_REG(14,  9, 6, x)
#define READ_PMEVCNTR15(x)    /* PMEVCNTR15_EL0  */ READ_PMU_REG(14,  9, 7, x)

#define READ_PMEVCNTR16(x)    /* PMEVCNTR16_EL0  */ READ_PMU_REG(14, 10, 0, x)
#define READ_PMEVCNTR17(x)    /* PMEVCNTR17_EL0  */ READ_PMU_REG(14, 10, 1, x)
#define READ_PMEVCNTR18(x)    /* PMEVCNTR18_EL0  */ READ_PMU_REG(14, 10, 2, x)
#define READ_PMEVCNTR19(x)    /* PMEVCNTR19_EL0  */ READ_PMU_REG(14, 10, 3, x)
#define READ_PMEVCNTR20(x)    /* PMEVCNTR20_EL0  */ READ_PMU_REG(14, 10, 4, x)
#define READ_PMEVCNTR21(x)    /* PMEVCNTR21_EL0  */ READ_PMU_REG(14, 10, 5, x)
#define READ_PMEVCNTR22(x)    /* PMEVCNTR22_EL0  */ READ_PMU_REG(14, 10, 6, x)
#define READ_PMEVCNTR23(x)    /* PMEVCNTR23_EL0  */ READ_PMU_REG(14, 10, 7, x)

#define READ_PMEVCNTR24(x)    /* PMEVCNTR24_EL0  */ READ_PMU_REG(14, 11, 0, x)
#define READ_PMEVCNTR25(x)    /* PMEVCNTR25_EL0  */ READ_PMU_REG(14, 11, 1, x)
#define READ_PMEVCNTR26(x)    /* PMEVCNTR26_EL0  */ READ_PMU_REG(14, 11, 2, x)
#define READ_PMEVCNTR27(x)    /* PMEVCNTR27_EL0  */ READ_PMU_REG(14, 11, 3, x)
#define READ_PMEVCNTR28(x)    /* PMEVCNTR28_EL0  */ READ_PMU_REG(14, 11, 4, x)
#define READ_PMEVCNTR29(x)    /* PMEVCNTR29_EL0  */ READ_PMU_REG(14, 11, 5, x)
#define READ_PMEVCNTR30(x)    /* PMEVCNTR30_EL0  */ READ_PMU_REG(14, 11, 6, x)

#define READ_PMEVCNTR(n,x)                          READ_PMEVCNTR ## n(x)

#define READ_PMEVTYPER0(x)    /* PMEVTYPER0_EL0  */ READ_PMU_REG(14, 12, 0, x)
#define READ_PMEVTYPER1(x)    /* PMEVTYPER1_EL0  */ READ_PMU_REG(14, 12, 1, x)
#define READ_PMEVTYPER2(x)    /* PMEVTYPER2_EL0  */ READ_PMU_REG(14, 12, 2, x)
#define READ_PMEVTYPER3(x)    /* PMEVTYPER3_EL0  */ READ_PMU_REG(14, 12, 3, x)
#define READ_PMEVTYPER4(x)    /* PMEVTYPER4_EL0  */ READ_PMU_REG(14, 12, 4, x)
#define READ_PMEVTYPER5(x)    /* PMEVTYPER5_EL0  */ READ_PMU_REG(14, 12, 5, x)
#define READ_PMEVTYPER6(x)    /* PMEVTYPER6_EL0  */ READ_PMU_REG(14, 12, 6, x)
#define READ_PMEVTYPER7(x)    /* PMEVTYPER7_EL0  */ READ_PMU_REG(14, 12, 7, x)

#define READ_PMEVTYPER8(x)    /* PMEVTYPER8_EL0  */ READ_PMU_REG(14, 13, 0, x)
#define READ_PMEVTYPER9(x)    /* PMEVTYPER9_EL0  */ READ_PMU_REG(14, 13, 1, x)
#define READ_PMEVTYPER10(x)   /* PMEVTYPER10_EL0 */ READ_PMU_REG(14, 13, 2, x)
#define READ_PMEVTYPER11(x)   /* PMEVTYPER11_EL0 */ READ_PMU_REG(14, 13, 3, x)
#define READ_PMEVTYPER12(x)   /* PMEVTYPER12_EL0 */ READ_PMU_REG(14, 13, 4, x)
#define READ_PMEVTYPER13(x)   /* PMEVTYPER13_EL0 */ READ_PMU_REG(14, 13, 5, x)
#define READ_PMEVTYPER14(x)   /* PMEVTYPER14_EL0 */ READ_PMU_REG(14, 13, 6, x)
#define READ_PMEVTYPER15(x)   /* PMEVTYPER15_EL0 */ READ_PMU_REG(14, 13, 7, x)

#define READ_PMEVTYPER16(x)   /* PMEVTYPER16_EL0 */ READ_PMU_REG(14, 14, 0, x)
#define READ_PMEVTYPER17(x)   /* PMEVTYPER17_EL0 */ READ_PMU_REG(14, 14, 1, x)
#define READ_PMEVTYPER18(x)   /* PMEVTYPER18_EL0 */ READ_PMU_REG(14, 14, 2, x)
#define READ_PMEVTYPER19(x)   /* PMEVTYPER19_EL0 */ READ_PMU_REG(14, 14, 3, x)
#define READ_PMEVTYPER20(x)   /* PMEVTYPER20_EL0 */ READ_PMU_REG(14, 14, 4, x)
#define READ_PMEVTYPER21(x)   /* PMEVTYPER21_EL0 */ READ_PMU_REG(14, 14, 5, x)
#define READ_PMEVTYPER22(x)   /* PMEVTYPER22_EL0 */ READ_PMU_REG(14, 14, 6, x)
#define READ_PMEVTYPER23(x)   /* PMEVTYPER23_EL0 */ READ_PMU_REG(14, 14, 7, x)

#define READ_PMEVTYPER24(x)   /* PMEVTYPER24_EL0 */ READ_PMU_REG(14, 15, 0, x)
#define READ_PMEVTYPER25(x)   /* PMEVTYPER25_EL0 */ READ_PMU_REG(14, 15, 1, x)
#define READ_PMEVTYPER26(x)   /* PMEVTYPER26_EL0 */ READ_PMU_REG(14, 15, 2, x)
#define READ_PMEVTYPER27(x)   /* PMEVTYPER27_EL0 */ READ_PMU_REG(14, 15, 3, x)
#define READ_PMEVTYPER28(x)   /* PMEVTYPER28_EL0 */ READ_PMU_REG(14, 15, 4, x)
#define READ_PMEVTYPER29(x)   /* PMEVTYPER29_EL0 */ READ_PMU_REG(14, 15, 5, x)
#define READ_PMEVTYPER30(x)   /* PMEVTYPER30_EL0 */ READ_PMU_REG(14, 15, 6, x)
#define READ_PMCCFILTR(x)     /* PMCCFILTR_EL0   */ READ_PMU_REG(14, 15, 7, x)

#define READ_PMEVTYPER(n,x)                         READ_PMEVTYPER ## n(x)

#define WRITE_PMEVCNTR0(x)    /* PMEVCNTR0_EL0   */ WRITE_PMU_REG(14,  8, 0, x)
#define WRITE_PMEVCNTR1(x)    /* PMEVCNTR1_EL0   */ WRITE_PMU_REG(14,  8, 1, x)
#define WRITE_PMEVCNTR2(x)    /* PMEVCNTR2_EL0   */ WRITE_PMU_REG(14,  8, 2, x)
#define WRITE_PMEVCNTR3(x)    /* PMEVCNTR3_EL0   */ WRITE_PMU_REG(14,  8, 3, x)
#define WRITE_PMEVCNTR4(x)    /* PMEVCNTR4_EL0   */ WRITE_PMU_REG(14,  8, 4, x)
#define WRITE_PMEVCNTR5(x)    /* PMEVCNTR5_EL0   */ WRITE_PMU_REG(14,  8, 5, x)
#define WRITE_PMEVCNTR6(x)    /* PMEVCNTR6_EL0   */ WRITE_PMU_REG(14,  8, 6, x)
#define WRITE_PMEVCNTR7(x)    /* PMEVCNTR7_EL0   */ WRITE_PMU_REG(14,  8, 7, x)

#define WRITE_PMEVCNTR8(x)    /* PMEVCNTR8_EL0   */ WRITE_PMU_REG(14,  9, 0, x)
#define WRITE_PMEVCNTR9(x)    /* PMEVCNTR9_EL0   */ WRITE_PMU_REG(14,  9, 1, x)
#define WRITE_PMEVCNTR10(x)   /* PMEVCNTR10_EL0  */ WRITE_PMU_REG(14,  9, 2, x)
#define WRITE_PMEVCNTR11(x)   /* PMEVCNTR11_EL0  */ WRITE_PMU_REG(14,  9, 3, x)
#define WRITE_PMEVCNTR12(x)   /* PMEVCNTR12_EL0  */ WRITE_PMU_REG(14,  9, 4, x)
#define WRITE_PMEVCNTR13(x)   /* PMEVCNTR13_EL0  */ WRITE_PMU_REG(14,  9, 5, x)
#define WRITE_PMEVCNTR14(x)   /* PMEVCNTR14_EL0  */ WRITE_PMU_REG(14,  9, 6, x)
#define WRITE_PMEVCNTR15(x)   /* PMEVCNTR15_EL0  */ WRITE_PMU_REG(14,  9, 7, x)

#define WRITE_PMEVCNTR16(x)   /* PMEVCNTR16_EL0  */ WRITE_PMU_REG(14, 10, 0, x)
#define WRITE_PMEVCNTR17(x)   /* PMEVCNTR17_EL0  */ WRITE_PMU_REG(14, 10, 1, x)
#define WRITE_PMEVCNTR18(x)   /* PMEVCNTR18_EL0  */ WRITE_PMU_REG(14, 10, 2, x)
#define WRITE_PMEVCNTR19(x)   /* PMEVCNTR19_EL0  */ WRITE_PMU_REG(14, 10, 3, x)
#define WRITE_PMEVCNTR20(x)   /* PMEVCNTR20_EL0  */ WRITE_PMU_REG(14, 10, 4, x)
#define WRITE_PMEVCNTR21(x)   /* PMEVCNTR21_EL0  */ WRITE_PMU_REG(14, 10, 5, x)
#define WRITE_PMEVCNTR22(x)   /* PMEVCNTR22_EL0  */ WRITE_PMU_REG(14, 10, 6, x)
#define WRITE_PMEVCNTR23(x)   /* PMEVCNTR23_EL0  */ WRITE_PMU_REG(14, 10, 7, x)

#define WRITE_PMEVCNTR24(x)   /* PMEVCNTR24_EL0  */ WRITE_PMU_REG(14, 11, 0, x)
#define WRITE_PMEVCNTR25(x)   /* PMEVCNTR25_EL0  */ WRITE_PMU_REG(14, 11, 1, x)
#define WRITE_PMEVCNTR26(x)   /* PMEVCNTR26_EL0  */ WRITE_PMU_REG(14, 11, 2, x)
#define WRITE_PMEVCNTR27(x)   /* PMEVCNTR27_EL0  */ WRITE_PMU_REG(14, 11, 3, x)
#define WRITE_PMEVCNTR28(x)   /* PMEVCNTR28_EL0  */ WRITE_PMU_REG(14, 11, 4, x)
#define WRITE_PMEVCNTR29(x)   /* PMEVCNTR29_EL0  */ WRITE_PMU_REG(14, 11, 5, x)
#define WRITE_PMEVCNTR30(x)   /* PMEVCNTR30_EL0  */ WRITE_PMU_REG(14, 11, 6, x)

#define WRITE_PMEVCNTR(n,x)                         WRITE_PMEVCNTR ## n(x)

#define WRITE_PMEVTYPER0(x)   /* PMEVTYPER0_EL0  */ WRITE_PMU_REG(14, 12, 0, x)
#define WRITE_PMEVTYPER1(x)   /* PMEVTYPER1_EL0  */ WRITE_PMU_REG(14, 12, 1, x)
#define WRITE_PMEVTYPER2(x)   /* PMEVTYPER2_EL0  */ WRITE_PMU_REG(14, 12, 2, x)
#define WRITE_PMEVTYPER3(x)   /* PMEVTYPER3_EL0  */ WRITE_PMU_REG(14, 12, 3, x)
#define WRITE_PMEVTYPER4(x)   /* PMEVTYPER4_EL0  */ WRITE_PMU_REG(14, 12, 4, x)
#define WRITE_PMEVTYPER5(x)   /* PMEVTYPER5_EL0  */ WRITE_PMU_REG(14, 12, 5, x)
#define WRITE_PMEVTYPER6(x)   /* PMEVTYPER6_EL0  */ WRITE_PMU_REG(14, 12, 6, x)
#define WRITE_PMEVTYPER7(x)   /* PMEVTYPER7_EL0  */ WRITE_PMU_REG(14, 12, 7, x)

#define WRITE_PMEVTYPER8(x)   /* PMEVTYPER8_EL0  */ WRITE_PMU_REG(14, 13, 0, x)
#define WRITE_PMEVTYPER9(x)   /* PMEVTYPER9_EL0  */ WRITE_PMU_REG(14, 13, 1, x)
#define WRITE_PMEVTYPER10(x)  /* PMEVTYPER10_EL0 */ WRITE_PMU_REG(14, 13, 2, x)
#define WRITE_PMEVTYPER11(x)  /* PMEVTYPER11_EL0 */ WRITE_PMU_REG(14, 13, 3, x)
#define WRITE_PMEVTYPER12(x)  /* PMEVTYPER12_EL0 */ WRITE_PMU_REG(14, 13, 4, x)
#define WRITE_PMEVTYPER13(x)  /* PMEVTYPER13_EL0 */ WRITE_PMU_REG(14, 13, 5, x)
#define WRITE_PMEVTYPER14(x)  /* PMEVTYPER14_EL0 */ WRITE_PMU_REG(14, 13, 6, x)
#define WRITE_PMEVTYPER15(x)  /* PMEVTYPER15_EL0 */ WRITE_PMU_REG(14, 13, 7, x)

#define WRITE_PMEVTYPER16(x)  /* PMEVTYPER16_EL0 */ WRITE_PMU_REG(14, 14, 0, x)
#define WRITE_PMEVTYPER17(x)  /* PMEVTYPER17_EL0 */ WRITE_PMU_REG(14, 14, 1, x)
#define WRITE_PMEVTYPER18(x)  /* PMEVTYPER18_EL0 */ WRITE_PMU_REG(14, 14, 2, x)
#define WRITE_PMEVTYPER19(x)  /* PMEVTYPER19_EL0 */ WRITE_PMU_REG(14, 14, 3, x)
#define WRITE_PMEVTYPER20(x)  /* PMEVTYPER20_EL0 */ WRITE_PMU_REG(14, 14, 4, x)
#define WRITE_PMEVTYPER21(x)  /* PMEVTYPER21_EL0 */ WRITE_PMU_REG(14, 14, 5, x)
#define WRITE_PMEVTYPER22(x)  /* PMEVTYPER22_EL0 */ WRITE_PMU_REG(14, 14, 6, x)
#define WRITE_PMEVTYPER23(x)  /* PMEVTYPER23_EL0 */ WRITE_PMU_REG(14, 14, 7, x)

#define WRITE_PMEVTYPER24(x)  /* PMEVTYPER24_EL0 */ WRITE_PMU_REG(14, 15, 0, x)
#define WRITE_PMEVTYPER25(x)  /* PMEVTYPER25_EL0 */ WRITE_PMU_REG(14, 15, 1, x)
#define WRITE_PMEVTYPER26(x)  /* PMEVTYPER26_EL0 */ WRITE_PMU_REG(14, 15, 2, x)
#define WRITE_PMEVTYPER27(x)  /* PMEVTYPER27_EL0 */ WRITE_PMU_REG(14, 15, 3, x)
#define WRITE_PMEVTYPER28(x)  /* PMEVTYPER28_EL0 */ WRITE_PMU_REG(14, 15, 4, x)
#define WRITE_PMEVTYPER29(x)  /* PMEVTYPER29_EL0 */ WRITE_PMU_REG(14, 15, 5, x)
#define WRITE_PMEVTYPER30(x)  /* PMEVTYPER30_EL0 */ WRITE_PMU_REG(14, 15, 6, x)
#define WRITE_PMCCFILTR(x)    /* PMCCFILTR_EL0   */ WRITE_PMU_REG(14, 15, 7, x)

#define WRITE_PMEVTYPER(n,x)                        WRITE_PMEVTYPER ## n(x)

#if defined(__arm__)

#define READ_PMCCNTR_64(x)    /* PMCCNTR_EL0     */ READ_PMU_REG_64(0, 9, x)
#define WRITE_PMCCNTR_64(x)   /* PMCCNTR_EL0     */ WRITE_PMU_REG_64(0, 9, x)

#elif defined(__aarch64__)

#define READ_PMCCNTR_64(x)    /* PMCCNTR_EL0     */ READ_PMCCNTR(x)
#define WRITE_PMCCNTR_64(x)   /* PMCCNTR_EL0     */ WRITE_PMCCNTR(x)

#endif

#endif /* PMU_AT_LEAST_V3 */

#if PMU_AT_LEAST_V3_1

#if defined(__arm__)

#define READ_PMCEID2(x)       /* PMCEID0_EL0[63:32] */ READ_PMU_REG( 9, 14, 4, x)
#define READ_PMCEID3(x)       /* PMCEID1_EL0[63:32] */ READ_PMU_REG( 9, 14, 5, x)

#define READ_PMCEID0_64(x)    /* PMCEID0_EL0     */                     \
    do {                                                                \
        unsigned int _pmu_pmceid0;                                      \
        unsigned int _pmu_pmceid2;                                      \
        READ_PMCEID0(_pmu_pmcied0);                                     \
        READ_PMCEID2(_pmu_pmcied2);                                     \
        x = (((unsigned long long) _pmu_pmcied2) << 32) | _pmu_pmcied0; \
    } while (0)
#define READ_PMCEID1_64(x)    /* PMCEID1_EL0     */                     \
    do {                                                                \
        unsigned int _pmu_pmceid1;                                      \
        unsigned int _pmu_pmceid3;                                      \
        READ_PMCEID1(_pmu_pmcied1);                                     \
        READ_PMCEID3(_pmu_pmcied3);                                     \
        x = (((unsigned long long) _pmu_pmcied3) << 32) | _pmu_pmcied1; \
    } while (0)

#elif defined(__aarch64__)

#define READ_PMCEID2(x)       /* PMCEID0_EL0[63:32] */ do { READ_PMCEID0(x); x >> 32; } while (0)
#define READ_PMCEID3(x)       /* PMCEID1_EL0[63:32] */ do { READ_PMCEID1(x); x >> 32; } while (0)

#define READ_PMCEID0_64(x)    /* PMCEID0_EL0     */ READ_PMCEID0(x)
#define READ_PMCEID1_64(x)    /* PMCEID1_EL0     */ READ_PMCEID1(x)

#endif

#endif /* PMU_AT_LEAST_V3_1 */

#define PMU_CALL_WITH_COUNTER_LITERAL_1(function,counter,arg1) \
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
    }

#define PMU_ISB() asm ("isb")

#define PMU_CYCLE_COUNTER 31


#endif /* INCLUDE_PMU_MACROS */

#if 0
#if defined(__ARM_ARCH) && __ARM_ARCH >= 801
#define PMUV3_V8_1 1
#else
#define PMUV3_V8_1 0
#endif

#define CYCLE_COUNTER 31

#ifdef __aarch64__

#define PMUV3_READ_PMCEID0(x)     asm volatile ("mrs %0, PMCEID0_EL0" : "=r"(x))
#define PMUV3_READ_PMCEID1(x)     asm volatile ("mrs %0, PMCEID1_EL0" : "=r"(x));
#define PMUV3_READ_PMCEID2(x)     asm volatile ("mrs %0, PMCEID0_EL0" : "=r"(x)); x = x >> 32
#define PMUV3_READ_PMCEID3(x)     asm volatile ("mrs %0, PMCEID1_EL0" : "=r"(x)); x = x >> 32
#define PMUV3_READ_PMCR(x)        asm ("mrs %0, PMCR_EL0"         : "=r"(x))
#define PMUV3_READ_PMOVSCLR(x)    asm ("mrs %0, PMOVSCLR_EL0"     : "=r"(x))
#define PMUV3_READ_PMEVCNTR(n,x)  asm ("mrs %0, PMEVCNTR"#n"_EL0" : "=r"(x))
#define PMUV3_WRITE_PMCR(x)       asm ("msr PMCR_EL0,       %0" :: "r"(x))
#define PMUV3_WRITE_PMCNTENSET(x) asm ("msr PMCNTENSET_EL0, %0" :: "r"(x))
#define PMUV3_WRITE_PMCNTENCLR(x) asm ("msr PMCNTENCLR_EL0, %0" :: "r"(x))
#define PMUV3_WRITE_PMOVSCLR(x)   asm ("msr PMOVSCLR_EL0,   %0" :: "r"(x))
#define PMUV3_WRITE_PMINTENSET(x) asm ("msr PMINTENSET_EL1, %0" :: "r"(x))
#define PMUV3_WRITE_PMINTENCLR(x) asm ("msr PMINTENCLR_EL1, %0" :: "r"(x))

#else

#define PMUV3_READ_PMCEID0(x)     asm volatile ("mrc p15, 0, %0, c9, c12, 6" : "=r"(x))
#define PMUV3_READ_PMCEID1(x)     asm volatile ("mrc p15, 0, %0, c9, c12, 7" : "=r"(x))
#define PMUV3_READ_PMCEID2(x)     asm volatile ("mrc p15, 0, %0, c9, c14, 4" : "=r"(x))
#define PMUV3_READ_PMCEID3(x)     asm volatile ("mrc p15, 0, %0, c9, c14, 5" : "=r"(x))
#define PMUV3_READ_PMCR(x)        asm ("mrc p15, 0, %0,  c9, c12, 0" : "=r"(x))
#define PMUV3_READ_PMOVSCLR(x)    asm ("mrc p15, 0, %0,  c9, c12, 3" : "=r"(x))
#define PMUV3_WRITE_PMCR(x)       asm ("mcr p15, 0, %0,  c9, c12, 0" :: "r"(x))
#define PMUV3_WRITE_PMCNTENSET(x) asm ("mcr p15, 0, %0,  c9, c12, 1" :: "r"(x))
#define PMUV3_WRITE_PMCNTENCLR(x) asm ("mcr p15, 0, %0,  c9, c12, 2" :: "r"(x))
#define PMUV3_WRITE_PMOVSCLR(x)   asm ("mcr p15, 0, %0,  c9, c12, 3" :: "r"(x))
#define PMUV3_WRITE_PMINTENSET(x) asm ("mcr p15, 0, %0,  c9, c14, 1" :: "r"(x))
#define PMUV3_WRITE_PMINTENCLR(x) asm ("mcr p15, 0, %0,  c9, c14, 2" :: "r"(x))

#define PMUV3_READ_PMEVCNTR0(x)   asm ("mrc p15, 0, %0, c14,  c8, 0"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR1(x)   asm ("mrc p15, 0, %0, c14,  c8, 1"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR2(x)   asm ("mrc p15, 0, %0, c14,  c8, 2"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR3(x)   asm ("mrc p15, 0, %0, c14,  c8, 3"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR4(x)   asm ("mrc p15, 0, %0, c14,  c8, 4"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR5(x)   asm ("mrc p15, 0, %0, c14,  c8, 5"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR6(x)   asm ("mrc p15, 0, %0, c14,  c8, 6"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR7(x)   asm ("mrc p15, 0, %0, c14,  c8, 7"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR8(x)   asm ("mrc p15, 0, %0, c14,  c9, 0"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR9(x)   asm ("mrc p15, 0, %0, c14,  c9, 1"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR10(x)  asm ("mrc p15, 0, %0, c14,  c9, 2"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR11(x)  asm ("mrc p15, 0, %0, c14,  c9, 3"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR12(x)  asm ("mrc p15, 0, %0, c14,  c9, 4"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR13(x)  asm ("mrc p15, 0, %0, c14,  c9, 5"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR14(x)  asm ("mrc p15, 0, %0, c14,  c9, 6"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR15(x)  asm ("mrc p15, 0, %0, c14,  c9, 7"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR16(x)  asm ("mrc p15, 0, %0, c14, c10, 0"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR17(x)  asm ("mrc p15, 0, %0, c14, c10, 1"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR18(x)  asm ("mrc p15, 0, %0, c14, c10, 2"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR19(x)  asm ("mrc p15, 0, %0, c14, c10, 3"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR20(x)  asm ("mrc p15, 0, %0, c14, c10, 4"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR21(x)  asm ("mrc p15, 0, %0, c14, c10, 5"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR22(x)  asm ("mrc p15, 0, %0, c14, c10, 6"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR23(x)  asm ("mrc p15, 0, %0, c14, c10, 7"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR24(x)  asm ("mrc p15, 0, %0, c14, c11, 0"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR25(x)  asm ("mrc p15, 0, %0, c14, c11, 1"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR26(x)  asm ("mrc p15, 0, %0, c14, c11, 2"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR27(x)  asm ("mrc p15, 0, %0, c14, c11, 3"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR28(x)  asm ("mrc p15, 0, %0, c14, c11, 4"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR29(x)  asm ("mrc p15, 0, %0, c14, c11, 5"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR30(x)  asm ("mrc p15, 0, %0, c14, c11, 6"  : "=r"(x))
#define PMUV3_READ_PMEVCNTR(n,x)  PMUV3_READ_PMEVCNTR ## n(x)
#endif


#define PMUV3_CALL_WITH_COUNTER_LITERAL_1(function,counter,arg1) \
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
    }

#define PMUV3_ISB() asm ("isb")

#define NUM_KNOWN_COMMON_EVENTS_0 0x31
static const char* const pmuv3_common_event_names_0[NUM_KNOWN_COMMON_EVENTS_0] =
{
    "SW_INCR",
    "L1I_CACHE_REFILL",
    "L1I_TLB_REFILL",
    "L1D_CACHE_REFILL",
    "L1D_CACHE",
    "L1D_TLB_REFILL",
    "LD_RETIRED",
    "ST_RETIRED",
    "INST_RETIRED",
    "EXC_TAKEN",
    "EXC_RETURN",
    "CID_WRITE_RETIRED",
    "PC_WRITE_RETIRED",
    "BR_IMMED_RETIRED",
    "BR_RETURN_RETIRED",
    "UNALIGNED_LDST_RETIRED",
    "BR_MIS_PRED",
    "CPU_CYCLES",
    "BR_PRED",
    "MEM_ACCESS",
    "L1I_CACHE",
    "L1D_CACHE_WB",
    "L2D_CACHE",
    "L2D_CACHE_REFILL",
    "L2D_CACHE_WB",
    "BUS_ACCESS",
    "MEMORY_ERROR",
    "INST_SPEC",
    "TTBR_WRITE_RETIRED",
    "BUS_CYCLES",
    "CHAIN",
    "L1D_CACHE_ALLOCATE",
    "L2D_CACHE_ALLOCATE",
    "BR_RETIRED",
    "BR_MIS_PRED_RETIRED",
    "STALL_FRONTEND",
    "STALL_BACKEND",
    "L1D_TLB",
    "L1I_TLB",
    "L2I_CACHE",
    "L2I_CACHE_REFILL",
    "L3D_CACHE_ALLOCATE",
    "L3D_CACHE_REFILL",
    "L3D_CACHE",
    "L3D_CACHE_WB",
    "L2D_TLB_REFILL",
    "L2I_TLB_REFILL",
    "L2D_TLB",
    "L2I_TLB"
};

#if PMUV3_V8_1
#define NUM_KNOWN_COMMON_EVENTS_1 0x0
static const char* const pmuv3_common_event_names_1[NUM_KNOWN_COMMON_EVENTS_1] = {};
#endif

struct segment_and_index
{
    unsigned long segment;
    unsigned long index;
};

static struct segment_and_index get_segment_and_index(unsigned long event_type)
{
    struct segment_and_index event;

    if (event_type >= PMUV3_COMMON_EVENTS_0_MIN && event_type <= PMUV3_COMMON_EVENTS_0_MAX)
    {
        event.segment = 0;
        event.index = event_type - PMUV3_COMMON_EVENTS_0_MIN;
    }
#if PMUV3_V8_1
    else if (event_type >= PMUV3_COMMON_EVENTS_1_MIN && event_type <= PMUV3_COMMON_EVENTS_1_MAX)
    {
        event.segment = 1;
        event.index = event_type - PMUV3_COMMON_EVENTS_1_MIN;
    }
#endif
    else
    {
        event.segment = -1ul;
        event.index = 0;
    }

    return event;
}

const char* pmuv3_get_event_name(unsigned long event_type)
{
    struct segment_and_index event = get_segment_and_index(event_type);

    if (event.segment == 0)
    {
        if (event.index < NUM_KNOWN_COMMON_EVENTS_0)
        {
            return pmuv3_common_event_names_0[event.index];
        }

        return "UNKNOWN_COMMON_0";
    }

#if PMUV3_V8_1
    if (event.segment == 1)
    {
        if (event.index < NUM_KNOWN_COMMON_EVENTS_1)
        {
            return pmuv3_common_event_names_1[event.index];
        }

        return "UNKNOWN_COMMON_1";
    }
#endif

    return "IMP_DEF";
}

unsigned int pmuv3_is_common_event_implemented(unsigned long event_type)
{
    unsigned long events_implemented = 0;
    struct segment_and_index event = get_segment_and_index(event_type);


    if (event.segment == 0)
    {
        if (event.index < 32ul)
        {
            PMUV3_READ_PMCEID0(events_implemented);
        }
        else
        {
            event.index -= 32ul;
            PMUV3_READ_PMCEID1(events_implemented);
        }
    }
#if PMUV3_V8_1
    else if (event.segment == 1)
    {
        if (event.index < 32ul)
        {
            PMUV3_READ_PMCEID2(events_implemented);
        }
        else
        {
            event.index -= 32ul;
            PMUV3_READ_PMCEID3(events_implemented);
        }
    }
#endif

    return (events_implemented >> event.index) & 1ul;
}

unsigned int pmuv3_get_number_of_counters(void)
{
    unsigned long control_register;
    PMUV3_READ_PMCR(control_register);
    return (control_register >> 11) & 0x1ful;
}

void pmuv3_reset(void)
{
    unsigned long control_register;
    PMUV3_READ_PMCR(control_register);
    control_register |= 0x46ul;
    control_register &= ~0x39ul;
    PMUV3_WRITE_PMCR(control_register);
    PMUV3_ISB();
    PMUV3_WRITE_PMINTENCLR(0xfffffffful);
    PMUV3_WRITE_PMOVSCLR(0xfffffffful);

}

void pmuv3_start(void)
{
    unsigned long control_register;
    PMUV3_READ_PMCR(control_register);
    control_register |= 1ul;
    PMUV3_WRITE_PMCR(control_register);
    PMUV3_ISB();
}

void pmuv3_stop(void)
{
    unsigned long control_register;
    PMUV3_READ_PMCR(control_register);
    control_register &= ~1ul;
    PMUV3_WRITE_PMCR(control_register);
    PMUV3_ISB();
}

unsigned int pmuv3_counter_get_event_count(unsigned long counter)
{
    unsigned long event_count;
    PMUV3_CALL_WITH_COUNTER_LITERAL_1(PMUV3_READ_PMEVCNTR, counter, event_count);
    /*
    switch (counter)
    {
        case  0: PMUV3_READ_PMEVCNTR( 0,event_count); break;
        case  1: PMUV3_READ_PMEVCNTR( 1,event_count); break;
        case  2: PMUV3_READ_PMEVCNTR( 2,event_count); break;
        case  3: PMUV3_READ_PMEVCNTR( 3,event_count); break;
        case  4: PMUV3_READ_PMEVCNTR( 4,event_count); break;
        case  5: PMUV3_READ_PMEVCNTR( 5,event_count); break;
        case  6: PMUV3_READ_PMEVCNTR( 6,event_count); break;
        case  7: PMUV3_READ_PMEVCNTR( 7,event_count); break;
        case  8: PMUV3_READ_PMEVCNTR( 8,event_count); break;
        case  9: PMUV3_READ_PMEVCNTR( 9,event_count); break;
        case 10: PMUV3_READ_PMEVCNTR(10,event_count); break;
        case 11: PMUV3_READ_PMEVCNTR(11,event_count); break;
        case 12: PMUV3_READ_PMEVCNTR(12,event_count); break;
        case 13: PMUV3_READ_PMEVCNTR(13,event_count); break;
        case 14: PMUV3_READ_PMEVCNTR(14,event_count); break;
        case 15: PMUV3_READ_PMEVCNTR(15,event_count); break;
        case 16: PMUV3_READ_PMEVCNTR(16,event_count); break;
        case 17: PMUV3_READ_PMEVCNTR(17,event_count); break;
        case 18: PMUV3_READ_PMEVCNTR(18,event_count); break;
        case 19: PMUV3_READ_PMEVCNTR(19,event_count); break;
        case 20: PMUV3_READ_PMEVCNTR(20,event_count); break;
        case 21: PMUV3_READ_PMEVCNTR(21,event_count); break;
        case 22: PMUV3_READ_PMEVCNTR(22,event_count); break;
        case 23: PMUV3_READ_PMEVCNTR(23,event_count); break;
        case 24: PMUV3_READ_PMEVCNTR(24,event_count); break;
        case 25: PMUV3_READ_PMEVCNTR(25,event_count); break;
        case 26: PMUV3_READ_PMEVCNTR(26,event_count); break;
        case 27: PMUV3_READ_PMEVCNTR(27,event_count); break;
        case 28: PMUV3_READ_PMEVCNTR(28,event_count); break;
        case 29: PMUV3_READ_PMEVCNTR(29,event_count); break;
        case 30: PMUV3_READ_PMEVCNTR(30,event_count); break;
    }
    */

    return event_count;
}

unsigned int pmuv3_counter_get_event_type(unsigned long counter)
{
    unsigned long event_type;
    switch (counter)
    {
#ifdef __aarch64__
        case  0: asm ("mrs %0,  PMEVTYPER0_EL0" : "=r"(event_type)); break;
        case  1: asm ("mrs %0,  PMEVTYPER1_EL0" : "=r"(event_type)); break;
        case  2: asm ("mrs %0,  PMEVTYPER2_EL0" : "=r"(event_type)); break;
        case  3: asm ("mrs %0,  PMEVTYPER3_EL0" : "=r"(event_type)); break;
        case  4: asm ("mrs %0,  PMEVTYPER4_EL0" : "=r"(event_type)); break;
        case  5: asm ("mrs %0,  PMEVTYPER5_EL0" : "=r"(event_type)); break;
        case  6: asm ("mrs %0,  PMEVTYPER6_EL0" : "=r"(event_type)); break;
        case  7: asm ("mrs %0,  PMEVTYPER7_EL0" : "=r"(event_type)); break;
        case  8: asm ("mrs %0,  PMEVTYPER8_EL0" : "=r"(event_type)); break;
        case  9: asm ("mrs %0,  PMEVTYPER9_EL0" : "=r"(event_type)); break;
        case 10: asm ("mrs %0, PMEVTYPER10_EL0" : "=r"(event_type)); break;
        case 11: asm ("mrs %0, PMEVTYPER11_EL0" : "=r"(event_type)); break;
        case 12: asm ("mrs %0, PMEVTYPER12_EL0" : "=r"(event_type)); break;
        case 13: asm ("mrs %0, PMEVTYPER13_EL0" : "=r"(event_type)); break;
        case 14: asm ("mrs %0, PMEVTYPER14_EL0" : "=r"(event_type)); break;
        case 15: asm ("mrs %0, PMEVTYPER15_EL0" : "=r"(event_type)); break;
        case 16: asm ("mrs %0, PMEVTYPER16_EL0" : "=r"(event_type)); break;
        case 17: asm ("mrs %0, PMEVTYPER17_EL0" : "=r"(event_type)); break;
        case 18: asm ("mrs %0, PMEVTYPER18_EL0" : "=r"(event_type)); break;
        case 19: asm ("mrs %0, PMEVTYPER19_EL0" : "=r"(event_type)); break;
        case 20: asm ("mrs %0, PMEVTYPER20_EL0" : "=r"(event_type)); break;
        case 21: asm ("mrs %0, PMEVTYPER21_EL0" : "=r"(event_type)); break;
        case 22: asm ("mrs %0, PMEVTYPER22_EL0" : "=r"(event_type)); break;
        case 23: asm ("mrs %0, PMEVTYPER23_EL0" : "=r"(event_type)); break;
        case 24: asm ("mrs %0, PMEVTYPER24_EL0" : "=r"(event_type)); break;
        case 25: asm ("mrs %0, PMEVTYPER25_EL0" : "=r"(event_type)); break;
        case 26: asm ("mrs %0, PMEVTYPER26_EL0" : "=r"(event_type)); break;
        case 27: asm ("mrs %0, PMEVTYPER27_EL0" : "=r"(event_type)); break;
        case 28: asm ("mrs %0, PMEVTYPER28_EL0" : "=r"(event_type)); break;
        case 29: asm ("mrs %0, PMEVTYPER29_EL0" : "=r"(event_type)); break;
        case 30: asm ("mrs %0, PMEVTYPER30_EL0" : "=r"(event_type)); break;
#else
        case   0: asm ("mrc p15, 0, %0, c14, c12, 0"  : "=r"(event_type)); break;
        case   1: asm ("mrc p15, 0, %0, c14, c12, 1"  : "=r"(event_type)); break;
        case   2: asm ("mrc p15, 0, %0, c14, c12, 2"  : "=r"(event_type)); break;
        case   3: asm ("mrc p15, 0, %0, c14, c12, 3"  : "=r"(event_type)); break;
        case   4: asm ("mrc p15, 0, %0, c14, c12, 4"  : "=r"(event_type)); break;
        case   5: asm ("mrc p15, 0, %0, c14, c12, 5"  : "=r"(event_type)); break;
        case   6: asm ("mrc p15, 0, %0, c14, c12, 6"  : "=r"(event_type)); break;
        case   7: asm ("mrc p15, 0, %0, c14, c12, 7"  : "=r"(event_type)); break;
        case   8: asm ("mrc p15, 0, %0, c14, c13, 0"  : "=r"(event_type)); break;
        case   9: asm ("mrc p15, 0, %0, c14, c13, 1"  : "=r"(event_type)); break;
        case  10: asm ("mrc p15, 0, %0, c14, c13, 2"  : "=r"(event_type)); break;
        case  11: asm ("mrc p15, 0, %0, c14, c13, 3"  : "=r"(event_type)); break;
        case  12: asm ("mrc p15, 0, %0, c14, c13, 4"  : "=r"(event_type)); break;
        case  13: asm ("mrc p15, 0, %0, c14, c13, 5"  : "=r"(event_type)); break;
        case  14: asm ("mrc p15, 0, %0, c14, c13, 6"  : "=r"(event_type)); break;
        case  15: asm ("mrc p15, 0, %0, c14, c13, 7"  : "=r"(event_type)); break;
        case  16: asm ("mrc p15, 0, %0, c14, c14, 0"  : "=r"(event_type)); break;
        case  17: asm ("mrc p15, 0, %0, c14, c14, 1"  : "=r"(event_type)); break;
        case  18: asm ("mrc p15, 0, %0, c14, c14, 2"  : "=r"(event_type)); break;
        case  19: asm ("mrc p15, 0, %0, c14, c14, 3"  : "=r"(event_type)); break;
        case  20: asm ("mrc p15, 0, %0, c14, c14, 4"  : "=r"(event_type)); break;
        case  21: asm ("mrc p15, 0, %0, c14, c14, 5"  : "=r"(event_type)); break;
        case  22: asm ("mrc p15, 0, %0, c14, c14, 6"  : "=r"(event_type)); break;
        case  23: asm ("mrc p15, 0, %0, c14, c14, 7"  : "=r"(event_type)); break;
        case  24: asm ("mrc p15, 0, %0, c14, c15, 0"  : "=r"(event_type)); break;
        case  25: asm ("mrc p15, 0, %0, c14, c15, 1"  : "=r"(event_type)); break;
        case  26: asm ("mrc p15, 0, %0, c14, c15, 2"  : "=r"(event_type)); break;
        case  27: asm ("mrc p15, 0, %0, c14, c15, 3"  : "=r"(event_type)); break;
        case  28: asm ("mrc p15, 0, %0, c14, c15, 4"  : "=r"(event_type)); break;
        case  29: asm ("mrc p15, 0, %0, c14, c15, 5"  : "=r"(event_type)); break;
        case  30: asm ("mrc p15, 0, %0, c14, c15, 6"  : "=r"(event_type)); break;
#endif
    }

    return event_type & 0x3fful;
}

void pmuv3_counter_set_event_type(unsigned long counter, unsigned long event_type)
{
#define AA5B ;
#define CASE(n) AA ## n ## B
//#define CASE(n) asm ("isb")

CASE(5);
    switch (counter)
    {
#ifdef __aarch64__
        case  0: asm ("msr  PMEVTYPER0_EL0, %0" :: "r"(event_type)); break;
        case  1: asm ("msr  PMEVTYPER1_EL0, %0" :: "r"(event_type)); break;
        case  2: asm ("msr  PMEVTYPER2_EL0, %0" :: "r"(event_type)); break;
        case  3: asm ("msr  PMEVTYPER3_EL0, %0" :: "r"(event_type)); break;
        case  4: asm ("msr  PMEVTYPER4_EL0, %0" :: "r"(event_type)); break;
        case  5: asm ("msr  PMEVTYPER5_EL0, %0" :: "r"(event_type)); break;
        case  6: asm ("msr  PMEVTYPER6_EL0, %0" :: "r"(event_type)); break;
        case  7: asm ("msr  PMEVTYPER7_EL0, %0" :: "r"(event_type)); break;
        case  8: asm ("msr  PMEVTYPER8_EL0, %0" :: "r"(event_type)); break;
        case  9: asm ("msr  PMEVTYPER9_EL0, %0" :: "r"(event_type)); break;
        case 10: asm ("msr PMEVTYPER10_EL0, %0" :: "r"(event_type)); break;
        case 11: asm ("msr PMEVTYPER11_EL0, %0" :: "r"(event_type)); break;
        case 12: asm ("msr PMEVTYPER12_EL0, %0" :: "r"(event_type)); break;
        case 13: asm ("msr PMEVTYPER13_EL0, %0" :: "r"(event_type)); break;
        case 14: asm ("msr PMEVTYPER14_EL0, %0" :: "r"(event_type)); break;
        case 15: asm ("msr PMEVTYPER15_EL0, %0" :: "r"(event_type)); break;
        case 16: asm ("msr PMEVTYPER16_EL0, %0" :: "r"(event_type)); break;
        case 17: asm ("msr PMEVTYPER17_EL0, %0" :: "r"(event_type)); break;
        case 18: asm ("msr PMEVTYPER18_EL0, %0" :: "r"(event_type)); break;
        case 19: asm ("msr PMEVTYPER19_EL0, %0" :: "r"(event_type)); break;
        case 20: asm ("msr PMEVTYPER20_EL0, %0" :: "r"(event_type)); break;
        case 21: asm ("msr PMEVTYPER21_EL0, %0" :: "r"(event_type)); break;
        case 22: asm ("msr PMEVTYPER22_EL0, %0" :: "r"(event_type)); break;
        case 23: asm ("msr PMEVTYPER23_EL0, %0" :: "r"(event_type)); break;
        case 24: asm ("msr PMEVTYPER24_EL0, %0" :: "r"(event_type)); break;
        case 25: asm ("msr PMEVTYPER25_EL0, %0" :: "r"(event_type)); break;
        case 26: asm ("msr PMEVTYPER26_EL0, %0" :: "r"(event_type)); break;
        case 27: asm ("msr PMEVTYPER27_EL0, %0" :: "r"(event_type)); break;
        case 28: asm ("msr PMEVTYPER28_EL0, %0" :: "r"(event_type)); break;
        case 29: asm ("msr PMEVTYPER29_EL0, %0" :: "r"(event_type)); break;
        case 30: asm ("msr PMEVTYPER30_EL0, %0" :: "r"(event_type)); break;
#else
        case   0: asm ("mcr p15, 0, %0, c14, c12, 0"  :: "r"(event_type)); break;
        case   1: asm ("mcr p15, 0, %0, c14, c12, 1"  :: "r"(event_type)); break;
        case   2: asm ("mcr p15, 0, %0, c14, c12, 2"  :: "r"(event_type)); break;
        case   3: asm ("mcr p15, 0, %0, c14, c12, 3"  :: "r"(event_type)); break;
        case   4: asm ("mcr p15, 0, %0, c14, c12, 4"  :: "r"(event_type)); break;
        case   5: asm ("mcr p15, 0, %0, c14, c12, 5"  :: "r"(event_type)); break;
        case   6: asm ("mcr p15, 0, %0, c14, c12, 6"  :: "r"(event_type)); break;
        case   7: asm ("mcr p15, 0, %0, c14, c12, 7"  :: "r"(event_type)); break;
        case   8: asm ("mcr p15, 0, %0, c14, c13, 0"  :: "r"(event_type)); break;
        case   9: asm ("mcr p15, 0, %0, c14, c13, 1"  :: "r"(event_type)); break;
        case  10: asm ("mcr p15, 0, %0, c14, c13, 2"  :: "r"(event_type)); break;
        case  11: asm ("mcr p15, 0, %0, c14, c13, 3"  :: "r"(event_type)); break;
        case  12: asm ("mcr p15, 0, %0, c14, c13, 4"  :: "r"(event_type)); break;
        case  13: asm ("mcr p15, 0, %0, c14, c13, 5"  :: "r"(event_type)); break;
        case  14: asm ("mcr p15, 0, %0, c14, c13, 6"  :: "r"(event_type)); break;
        case  15: asm ("mcr p15, 0, %0, c14, c13, 7"  :: "r"(event_type)); break;
        case  16: asm ("mcr p15, 0, %0, c14, c14, 0"  :: "r"(event_type)); break;
        case  17: asm ("mcr p15, 0, %0, c14, c14, 1"  :: "r"(event_type)); break;
        case  18: asm ("mcr p15, 0, %0, c14, c14, 2"  :: "r"(event_type)); break;
        case  19: asm ("mcr p15, 0, %0, c14, c14, 3"  :: "r"(event_type)); break;
        case  20: asm ("mcr p15, 0, %0, c14, c14, 4"  :: "r"(event_type)); break;
        case  21: asm ("mcr p15, 0, %0, c14, c14, 5"  :: "r"(event_type)); break;
        case  22: asm ("mcr p15, 0, %0, c14, c14, 6"  :: "r"(event_type)); break;
        case  23: asm ("mcr p15, 0, %0, c14, c14, 7"  :: "r"(event_type)); break;
        case  24: asm ("mcr p15, 0, %0, c14, c15, 0"  :: "r"(event_type)); break;
        case  25: asm ("mcr p15, 0, %0, c14, c15, 1"  :: "r"(event_type)); break;
        case  26: asm ("mcr p15, 0, %0, c14, c15, 2"  :: "r"(event_type)); break;
        case  27: asm ("mcr p15, 0, %0, c14, c15, 3"  :: "r"(event_type)); break;
        case  28: asm ("mcr p15, 0, %0, c14, c15, 4"  :: "r"(event_type)); break;
        case  29: asm ("mcr p15, 0, %0, c14, c15, 5"  :: "r"(event_type)); break;
        case  30: asm ("mcr p15, 0, %0, c14, c15, 6"  :: "r"(event_type)); break;
#endif
    }
}

void pmuv3_counter_enable(unsigned long counter)
{
    PMUV3_WRITE_PMCNTENSET(1ul << counter);
    PMUV3_ISB();
}

void pmuv3_counter_disable(unsigned long counter)
{
    PMUV3_WRITE_PMCNTENCLR(1ul << counter);
    PMUV3_ISB();
}

void pmuv3_counter_enable_overflow_interrupt(unsigned long counter)
{
    PMUV3_WRITE_PMINTENSET(1ul << counter);
    PMUV3_ISB();
}

void pmuv3_counter_disable_overflow_interrupt(unsigned long counter)
{
    PMUV3_WRITE_PMINTENCLR(1ul << counter);
    PMUV3_ISB();
}


unsigned int pmuv3_counter_is_overflow_flag_set(unsigned long counter)
{
    unsigned long overflow_flags;
    PMUV3_READ_PMOVSCLR(overflow_flags);
    return (overflow_flags >> counter) & 1ul;
}

void pmuv3_counter_clear_overflow_flag(unsigned long counter)
{
    PMUV3_WRITE_PMOVSCLR(1ul << counter);
    PMUV3_ISB();
}

unsigned long long pmuv3_cycle_counter_get_count(void)
{
#ifdef __aarch64__
    unsigned long long cycle_count;
    asm ("mrs %0, PMCCNTR_EL0" : "=r"(cycle_count));
    return cycle_count;
#else
    unsigned long cycle_count_lower;
    unsigned long cycle_count_upper;
    asm ("mrrc p15, 0, %0, %1, c9 " : "=r"(cycle_count_lower), "=r"(cycle_count_upper));
    return ((unsigned long long) cycle_count_upper) << 32 | cycle_count_lower;
#endif
}

void pmuv3_cycle_counter_enable(void)
{
    pmuv3_counter_enable(CYCLE_COUNTER);
}

void pmuv3_cycle_counter_disable(void)
{
    pmuv3_counter_disable(CYCLE_COUNTER);
}

void pmuv3_cycle_counter_enable_overflow_interrupt(void)
{
    pmuv3_counter_enable_overflow_interrupt(CYCLE_COUNTER);
}

void pmuv3_cycle_counter_disable_overflow_interrupt(void)
{
    pmuv3_counter_disable_overflow_interrupt(CYCLE_COUNTER);
}

unsigned int pmuv3_cycle_counter_is_overflow_flag_set(void)
{
    return pmuv3_counter_is_overflow_flag_set(CYCLE_COUNTER);
}

void pmuv3_cycle_counter_clear_overflow_flag(void)
{
    pmuv3_counter_clear_overflow_flag(CYCLE_COUNTER);
}

#endif

/* TODO: add support to set counter, set overflow, filtering */
