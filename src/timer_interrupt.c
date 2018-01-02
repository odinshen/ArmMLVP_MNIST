/* Bare-metal example for ARMv8-A */

/* Timer and interrupts */

/* Copyright (C) ARM Limited, 2016. All rights reserved. */

#include <stdio.h>

#include "GICv3.h"
#include "GICv3_gicc.h"
#include "sp804_timer.h"
#include "v8_aarch64.h"

#define TIMER_RELOAD_VALUE 350

static unsigned long interrupt_count;

// LED Base address
#define LED_BASE (volatile unsigned int *)0x1C010008

static void nudgeLeds(void)
{
    static int state = 1;
    static int value = 1;

    if (state)
    {
        int max = (1 << 7);
        value <<= 1;
        if (value == max)
            state = 0;
    }
    else
    {
        value >>= 1;
        if (value == 1)
            state = 1;
    }

    *LED_BASE = value;  // Update LEDs hardware
}

unsigned long long getTimerTicks(void)
{
    const unsigned long count_before = __atomic_load_n(&interrupt_count, __ATOMIC_SEQ_CST);
    unsigned long count_down_ticks = getTimerCount();
    const unsigned long count_after = __atomic_load_n(&interrupt_count, __ATOMIC_SEQ_CST);

    /* If we got interrupted, ticks could be wrong, so assume at reload */
    if (count_before != count_after)
    {
        count_down_ticks = TIMER_RELOAD_VALUE;
    }

    return (((unsigned long long) count_after) + 1 ) * TIMER_RELOAD_VALUE - count_down_ticks;
}

void timerInterruptHandler(void)
{
    __atomic_fetch_add(&interrupt_count, 1, __ATOMIC_SEQ_CST);

    nudgeLeds();

    // Relay this interrupt to all cpus by sending them an SGI
    setICC_SGI1R(0, 0, 0, sgirIRMAll, 0, 14);

    // Also let's send one to ourself
    setICC_SGI1R(0, 0, 0, sgirIRMTarget, 1, 14);

    clearTimerIrq();
}


void initTimerInterrupt(void)
{
    interrupt_count = 0;

    // The SP804 timer generates SPI INTID 34.  Enable
    // this ID, and route it to core 0
    SetSPIRoute(34, 0, gicdirouter_ModeSpecific);  // Route INTID 34 to core 0
    SetSPIPriority(34, 0);                         // Set INTID 34 to priority to 0
    ConfigureSPI(34, gicdicfgr_Level);             // Set INTID 34 as level-sensitive
    EnableSPI(34);                                 // Enable INTID 34

    // Configure the SP804 timer to generate an interrupt
    setTimerBaseAddress(0x1C110000);
    // 35MHz clock so timer goes off at 100kHz
    initTimer(TIMER_RELOAD_VALUE, SP804_AUTORELOAD, SP804_GENERATE_IRQ);
    startTimer();
}

