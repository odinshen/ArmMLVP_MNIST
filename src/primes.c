// ------------------------------------------------------------
// Prime Number Generator Code
//
// Copyright (c) 2011-2016 ARM Ltd.  All rights reserved.
// ------------------------------------------------------------

#include <stdio.h>

#include "primes.h"

#define TARGET_COUNT (100) // Number of primes to find

// ------------------------------------------------------------

static unsigned int prime_numbers[TARGET_COUNT];
static unsigned int next_number;
static unsigned int prime_count;

// ------------------------------------------------------------

static unsigned int getNextNumber(void)
{
  return __atomic_fetch_add(&next_number, 2, __ATOMIC_ACQ_REL);
}

// ------------------------------------------------------------

static void addPrime(unsigned int number, unsigned int id)
{

  unsigned int current_prime_count = __atomic_fetch_add(&prime_count, 1, __ATOMIC_RELAXED);

  // It is possible a CPU could skid past the target number of primes
  // so check to avoid potential writes past the end of the array
  if (current_prime_count < TARGET_COUNT)
  {
    prime_numbers[current_prime_count] = number;
    // Let the PMU know we found a prime
    asm ("msr PMSWINC_EL0, %0" :: "r"(0xfffffffful));
  }
  else // Reset to go indefinitely
  {
      __atomic_store_n(&next_number, 31, __ATOMIC_RELEASE);
      __atomic_store_n(&prime_count, 10, __ATOMIC_RELEASE);
  }

  return;
}

// ------------------------------------------------------------

void initPrimes(void)
{
  // Give it the first few primes because there is no guarantee
  // from here on the primes will be stored in order.
  // The algorithm uses all the primes up to the square root of the number being tested,
  // This guarantees we will have no problem finding primes up to 841
  prime_numbers[0] = 2;
  prime_numbers[1] = 3;
  prime_numbers[2] = 5;
  prime_numbers[3] = 7;
  prime_numbers[4] = 11;
  prime_numbers[5] = 13;
  prime_numbers[6] = 17;
  prime_numbers[7] = 19;
  prime_numbers[8] = 23;
  prime_numbers[9] = 29;
  next_number = 31;
  prime_count = 10;

  return;
}

// ------------------------------------------------------------

void calculatePrimes(unsigned int id)
{

  while (1)
  {
    // Get the next number
    int number = getNextNumber();
    int root = 1;
    int square = 1;
    int local_prime_count = __atomic_load_n(&prime_count, __ATOMIC_ACQUIRE);
    int i;

    while (number > square)
    {
      root++;
      square = root * root;
    }

    for (i = 1; i < local_prime_count; i++)
    {
      int remainder;
      int prime = prime_numbers[i];

      if (prime > root)
      {
        addPrime(number, id);
        break;
      }

      remainder = number % prime;

      if (remainder == 0)
      {
        // not a prime, so discard number
        break;
      }
    }
  }
  return;
}

// ------------------------------------------------------------
// End of primes.c
// ------------------------------------------------------------
