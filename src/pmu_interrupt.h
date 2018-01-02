/* Copyright (C) ARM Limited, 2016. All rights reserved. */

#ifndef INCLUDED_PMU_INTERRUPT_H
#define INCLUDED_PMU_INTERRUPT_H

/*
 * Initialize the PMU and an interrupt for sampling this core
 */
void initPmuInterrupt(void);

/*
 * Handler for the sample interrupt
 */
void pmuInterruptHandler(void);

/*
 * Handler for the PMU overflow interrupt
 */
void pmuOverflowHandler(void);

#endif
