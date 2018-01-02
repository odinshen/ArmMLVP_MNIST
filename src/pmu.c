#include "pmu.h"
#include "pmu_private.h"

#define NUM_KNOWN_COMMON_EVENTS_0 0x31
static const char* const pmu_common_event_names_0[NUM_KNOWN_COMMON_EVENTS_0] =
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

#if PMU_AT_LEAST_V3_1
#define NUM_KNOWN_COMMON_EVENTS_1 0x0
static const char* const pmu_common_event_names_1[NUM_KNOWN_COMMON_EVENTS_1] = {};
#endif

/*
 * From ARMv8.1 the common events are in two segments,
 * The first starting from 0, and the second starting from 0x4000
 */
struct segment_and_index
{
    unsigned long segment; /* Segment or -1 for no segment */
    unsigned long index;   /* Index (or offset) within the segment */
};

/*
 * Find an event type in the common events
 */
static struct segment_and_index get_segment_and_index(unsigned long event_type)
{
    struct segment_and_index event;

    if (event_type >= PMU_COMMON_EVENTS_0_MIN && event_type <= PMU_COMMON_EVENTS_0_MAX)
    {
        event.segment = 0;
        event.index = event_type - PMU_COMMON_EVENTS_0_MIN;
    }
#if PMU_AT_LEAST_V3_1
    else if (event_type >= PMU_COMMON_EVENTS_1_MIN && event_type <= PMU_COMMON_EVENTS_1_MAX)
    {
        event.segment = 1;
        event.index = event_type - PMU_COMMON_EVENTS_1_MIN;
    }
#endif
    else
    {
        event.segment = -1ul;
        event.index = 0;
    }

    return event;
}

const char* pmu_get_event_name(unsigned long event_type)
{
    struct segment_and_index event = get_segment_and_index(event_type);

    if (event.segment == 0)
    {
        if (event.index < NUM_KNOWN_COMMON_EVENTS_0)
        {
            return pmu_common_event_names_0[event.index];
        }

        return "UNKNOWN_COMMON_0";
    }

#if PMU_AT_LEAST_V3_1
    if (event.segment == 1)
    {
        if (event.index < NUM_KNOWN_COMMON_EVENTS_1)
        {
            return pmu_common_event_names_1[event.index];
        }

        return "UNKNOWN_COMMON_1";
    }
#endif

    return "IMP_DEF";
}

unsigned int pmu_is_common_event_implemented(unsigned long event_type)
{
    unsigned long events_implemented = 0;
    struct segment_and_index event = get_segment_and_index(event_type);


    if (event.segment == 0)
    {
        if (event.index < 32ul)
        {
            READ_PMCEID0(events_implemented);
        }
        else
        {
            event.index -= 32ul;
            READ_PMCEID1(events_implemented);
        }
    }
#if PMU_AT_LEAST_V3_1
    else if (event.segment == 1)
    {
        if (event.index < 32ul)
        {
            READ_PMCEID2(events_implemented);
        }
        else
        {
            event.index -= 32ul;
            READ_PMCEID3(events_implemented);
        }
    }
#endif

    return (events_implemented >> event.index) & 1ul;
}

unsigned int pmu_get_number_of_counters(void)
{
    unsigned long control_register;
    READ_PMCR(control_register);
    return (control_register >> 11) & 0x1ful;
}

void pmu_reset(void)
{
    unsigned long control_register;
    READ_PMCR(control_register);
    control_register |= 0x46ul;
    control_register &= ~0x39ul;
    WRITE_PMCR(control_register);
    PMU_ISB();
    WRITE_PMINTENCLR(0xfffffffful);
    WRITE_PMOVSR(0xfffffffful);

}

void pmu_start(void)
{
    unsigned long control_register;
    READ_PMCR(control_register);
    control_register |= 1ul;
    WRITE_PMCR(control_register);
    PMU_ISB();
}

void pmu_stop(void)
{
    unsigned long control_register;
    READ_PMCR(control_register);
    control_register &= ~1ul;
    WRITE_PMCR(control_register);
    PMU_ISB();
}

unsigned int pmu_counter_get_event_count(unsigned long counter)
{
    unsigned long event_count;

#if PMU_AT_LEAST_V3
    READ_PMEVCNTR(counter, event_count);
#else
    READ_PMEVCNTR_NR(counter, event_count);
#endif

    return event_count;
}

void pmu_counter_set_event_count(unsigned long counter, unsigned long event_count)
{
#if PMU_AT_LEAST_V3
    WRITE_PMEVCNTR(counter, event_count);
#else
    WRITE_PMEVCNTR_NR(counter, event_count);
#endif
}

unsigned int pmu_counter_get_event_type(unsigned long counter)
{
    unsigned long event_type;

#if PMU_AT_LEAST_V3
    READ_PMEVTYPER(counter, event_type);
#else
    READ_PMEVTYPER_NR(counter, event_type);
#endif

    return event_type & 0x3fful;
}

void pmu_counter_set_event_type(unsigned long counter, unsigned long event_type)
{
#if PMU_AT_LEAST_V3
    WRITE_PMEVTYPER(counter, event_type);
#else
    WRITE_PMEVTYPER_NR(counter, event_type);
#endif
}

void pmu_counter_enable(unsigned long counter)
{
    WRITE_PMCNTENSET(1ul << counter);
    PMU_ISB();
}

void pmu_counter_disable(unsigned long counter)
{
    WRITE_PMCNTENCLR(1ul << counter);
    PMU_ISB();
}

void pmu_counter_enable_overflow_interrupt(unsigned long counter)
{
    WRITE_PMINTENSET(1ul << counter);
    PMU_ISB();
}

void pmu_counter_disable_overflow_interrupt(unsigned long counter)
{
    WRITE_PMINTENCLR(1ul << counter);
    PMU_ISB();
}


unsigned int pmu_counter_is_overflow_flag_set(unsigned long counter)
{
    unsigned long overflow_flags;
    READ_PMOVSR(overflow_flags);
    return (overflow_flags >> counter) & 1ul;
}

void pmu_counter_clear_overflow_flag(unsigned long counter)
{
    WRITE_PMOVSR(1ul << counter);
    PMU_ISB();
}

unsigned long long pmu_cycle_counter_get_count(void)
{
#if PMU_AT_LEAST_V3
    unsigned long long cycle_count;
    READ_PMCCNTR_64(cycle_count);
#else
    unsigned long cycle_count;
    READ_PMCCNTR(cycle_count);
#endif

    return cycle_count;
}

void pmu_cycle_counter_enable(void)
{
    pmu_counter_enable(PMU_CYCLE_COUNTER);
}

void pmu_cycle_counter_disable(void)
{
    pmu_counter_disable(PMU_CYCLE_COUNTER);
}

void pmu_cycle_counter_enable_overflow_interrupt(void)
{
    pmu_counter_enable_overflow_interrupt(PMU_CYCLE_COUNTER);
}

void pmu_cycle_counter_disable_overflow_interrupt(void)
{
    pmu_counter_disable_overflow_interrupt(PMU_CYCLE_COUNTER);
}

unsigned int pmu_cycle_counter_is_overflow_flag_set(void)
{
    return pmu_counter_is_overflow_flag_set(PMU_CYCLE_COUNTER);
}

void pmu_cycle_counter_clear_overflow_flag(void)
{
    pmu_counter_clear_overflow_flag(PMU_CYCLE_COUNTER);
}

/* TODO: add support to set overflow, filtering */

