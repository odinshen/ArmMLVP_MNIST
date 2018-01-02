// ------------------------------------------------------------
// Prime Number Generator Code
// Header
//
// Copyright (c) 2011 ARM Ltd.  All rights reserved.
// ------------------------------------------------------------

#ifndef _PRIMES_H
#define _PRIMES_H

#include "MP_Mutexes.h"

// Initializes shared data used during calculation of primes
// Must be called by CPU 0 only, before any CPU calls to calculatePrimes()
void initPrimes(void);

// Starts calculation of primes on that CPU
// Must be called by each participating CPU
void calculatePrimes(unsigned int id);

#endif

// ------------------------------------------------------------
// End of primes.h
// ------------------------------------------------------------
