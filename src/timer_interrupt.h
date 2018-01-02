/* Bare-metal example for ARMv8-A */

/* Timer and interrupts */

/* Copyright (C) ARM Limited, 2016. All rights reserved. */

/**
 * Initialize and enable Timer0 interrupt on core 0
 */
void initTimerInterrupt(void);

/**
 * Handle the interrupt
 */
void timerInterruptHandler(void);

/**
 * The number of 35MHz ticks from the timer
 */
unsigned long getTimerTicks(void);
