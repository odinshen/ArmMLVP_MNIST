/* Copyright (C) ARM Limited, 2016. All rights reserved. */

#include "GICv3.h"
#include "GICv3_gicc.h"
#include "MP_Mutexes.h"
#include "v8_aarch64.h"
#include "timer_interrupt.h"
#include "pmu.h"

#include <stdio.h>

#define COUNT_OF(array) (sizeof(array) / sizeof(array[0]))

static unsigned int num_pmu_counters[8];

// printf lock to regulate CPU access to an output device
extern mutex print_lock __attribute__ ((aligned (64)));

/* Change these to any event type the processor supports */
static const unsigned int events_to_count[] =
{
    PMU_EVENT_SW_INCR,
    PMU_EVENT_L1D_CACHE_REFILL,
    PMU_EVENT_L1D_CACHE,
    PMU_EVENT_CPU_CYCLES,
    PMU_EVENT_INST_RETIRED,
    PMU_EVENT_PC_WRITE_RETIRED,
    PMU_EVENT_BR_IMMED_RETIRED,
    PMU_EVENT_BR_RETURN_RETIRED,
    PMU_EVENT_L1I_CACHE_REFILL,
    PMU_EVENT_LD_RETIRED,
    PMU_EVENT_ST_RETIRED,
    PMU_EVENT_BUS_CYCLES,
    PMU_EVENT_EXC_TAKEN,
    PMU_EVENT_EXC_RETURN
};

void initPmuInterrupt()
{
    int i;
    unsigned long core = GetCoreNumber();

    pmu_reset();

    num_pmu_counters[core] = pmu_get_number_of_counters();

    _mutex_acquire(&print_lock);
    printf("Core %lu has %u PMU counters\n", core, num_pmu_counters[core]);
    printf("It supports the following common event types:\n");
    for (i = PMU_COMMON_EVENTS_0_MIN; i < PMU_COMMON_EVENTS_1_MAX; ++i)
    {
        if (i == PMU_COMMON_EVENTS_0_MAX)
        {
            i = PMU_COMMON_EVENTS_1_MIN;
        }

        if (pmu_is_common_event_implemented(i))
        {
            printf("    %d (%s)\n", i, pmu_get_event_name(i));
        }
    }
    printf("\n");
    _mutex_release(&print_lock);

    if (num_pmu_counters[core] > COUNT_OF(events_to_count))
    {
        num_pmu_counters[core] = COUNT_OF(events_to_count);
    }

    for (i = 0; i < num_pmu_counters[core]; ++i)
    {
        pmu_counter_set_event_type(i, events_to_count[i]);
        pmu_counter_enable_overflow_interrupt(i);
        pmu_counter_enable(i);
    }

    pmu_cycle_counter_enable_overflow_interrupt();
    pmu_cycle_counter_enable();

    pmu_start();

    // We are going to use SGI 14 for the sample interrupt
    SetPrivateIntPriority(core, 14, 0xb0);  // Set INTID 14 to priority to 0xb0
    EnablePrivateInt(core, 14);             // Enable INTID 14

    SetPrivateIntPriority(core, 23, 0xb0);  // Set INTID 23 to priority to 0xb0
    EnablePrivateInt(core, 23);             // Enable INTID 23
}

void pmuInterruptHandler(void)
{
    int i;
    unsigned int counter_value;
    unsigned int event_type;
    unsigned long core = GetCoreNumber();

    _mutex_acquire(&print_lock);
    printf("Handling PMU sample interrupt on core %lu\n", core);
    printf("core %lu cycle count is %llu\n", core, pmu_cycle_counter_get_count());
    for (i = 0; i < num_pmu_counters[core]; ++i)
    {
        counter_value = pmu_counter_get_event_count(i);
        event_type = pmu_counter_get_event_type(i);

        printf("core %lu counter %d (%s) is %u\n", core, i, pmu_get_event_name(event_type), counter_value);
    }
    printf("\n");
    _mutex_release(&print_lock);
}

void pmuOverflowHandler(void)
{
    int i;
    unsigned int event_type;
    unsigned long core = GetCoreNumber();

    _mutex_acquire(&print_lock);
    printf("Handling PMU overflow interrupt on core %lu\n", core);

    if (pmu_cycle_counter_is_overflow_flag_set())
    {
        printf("core %lu cycle_counter overflowed\n", core);
        pmu_cycle_counter_clear_overflow_flag();
    }

    for (i = 0; i < num_pmu_counters[core]; ++i)
    {
        if (pmu_counter_is_overflow_flag_set(i))
        {
            event_type = pmu_counter_get_event_type(i);
            printf("core %lu counter %d (%s) overflowed\n", core, i, pmu_get_event_name(event_type));
            pmu_counter_clear_overflow_flag(i);
        }
    }
    printf("\n");
    _mutex_release(&print_lock);
}


