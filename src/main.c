/*
 * ARMv8-A - main
 *
 * Copyright (c) 2011-2016 ARM Ltd.  All rights reserved.
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include "arm_cnn_inference.h"
#include "primes.h"
#include "v8_aarch64.h"
#include "MP_Mutexes.h"
#include "GICv3.h"
#include "GICv3_gicc.h"
#include "pmu_interrupt.h"
#include "pmu.h"
#include "timer_interrupt.h"
#include "mnist.h"
#include "cnn_api_c.h"

// compile-time control for the max number of CPUs in the device
#define nCPUs 8

#define CNN_MODE       1


static unsigned int cpu_active_count = 0;
static unsigned int cpu_finished_count = 0;

static unsigned int next_image;
//static unsigned int conv_mode;


// printf lock to regulate CPU access to an output device
mutex print_lock __attribute__ ((aligned (64)));

// example code from fixstars
// http://www.fixstars.com/en/news/?p=125

void add_scalar(uint8_t * A, uint8_t * B, uint8_t * C)
{
    for(int i=0; i<16; i++)
    {
        C[i] = A[i] + B[i];
    }
}


void test_scalar_neon()
{

}

/*
 * void MainApp(void)
 *
 *    called by all CPUs to run SMP applications, this is the
 *    application start point for all secondary CPUs, and is called
 *    after other startup code has executed on the primary CPU
 *
 * Inputs
 *   <none>
 *
 * Returns
 *   <function does not return>
 */


#define MEMTEST_START        ((unsigned int*) 0x80100000) // to 0x8120_0000 // len: 0x0110_0000



__attribute__((noreturn)) void MainApp(void)
{
    unsigned long core;
    unsigned int test_mode;
    unsigned int conv_mode;
    unsigned int test_model;
    unsigned int user_cmd;
	unsigned int image_result;
    unsigned int inference_0;
    unsigned int inference_1;
    unsigned int get_image_idx = 0;

    core = GetCoreNumber();

    if (core >= nCPUs) {
      _mutex_acquire(&print_lock);
      printf("Error: More CPU cores are running than the maximum this program expects\n");
      printf("Change the value of nCPUs.  Exiting...\n");
      _mutex_release(&print_lock);
      exit(1);
    }
    
    __atomic_add_fetch(&cpu_active_count, 1, __ATOMIC_RELAXED);

/*
    // Memory test
    unsigned int mem_test_offset  = 0x0;
    unsigned int mem_test_len     = 0x0;
    unsigned int mem_test_pattern = 0x5a5aa5a5;
    unsigned int mem_test_data = 0;

    // Write pattern
//    for (mem_test_len = 0; mem_test_len < 0x01100000; mem_test_len+=4) {
    for (mem_test_len = 0; mem_test_len < 0x01100000; mem_test_len++) {
    	*(MEMTEST_START + mem_test_len) = mem_test_pattern;
    }

    mem_test_len  = 0x0;
    for (mem_test_len = 0; mem_test_len < 0x01100000; mem_test_len+=4) {
    	mem_test_data = *(MEMTEST_START + mem_test_len);
    	if (mem_test_data != mem_test_pattern) {
            printf("offset [0x%x]: %x !!!!!\n", mem_test_len, mem_test_data);
    	}
    }
*/


#ifdef CNN_MODE

    initPmuInterrupt();  // By core
	test_mode = TESTMODE_AUTO;
	test_model = TESTMODEL_MNIST;

    if (core == 0) {
		user_cmd = *AUTOTESTIMG;
        printf("[0x%x]: %x !!!!!\n", AUTOTESTIMG, user_cmd);
		if (user_cmd == 0xFF) {
			test_mode = TESTMODE_IMAGE;
		}

		user_cmd = *CNNSELECTING;
        printf("[0x%x]: %x !!!!!\n", CNNSELECTING, user_cmd);
		if (user_cmd == 0xFF) {
			test_model = TESTMODEL_CIFAR;
		}

		_mutex_acquire(&print_lock);
        printf("\n\n");
        printf("Select test mode\n");
		if (test_mode == TESTMODE_AUTO) {
			printf("CNN Auto Evaluation\n\n");
		}
		else {
			printf("CNN Selected Image Evaluation\n\n");
		}

		conv_mode = *CONVMODE;
		if (conv_mode == 1) {
			printf("Conv mode #1 \n\n");
		}
		else if (conv_mode == 2) {
			printf("Conv mode #2\n\n");
		}
		else if (conv_mode == 3) {
			printf("Conv mode #3\n\n");
		}
		else {
			conv_mode = 2;
			printf("Conv deafult mode #2\n\n");
		}

		printf("Select test model\n");
		if (test_model == TESTMODEL_MNIST) {
			printf("MNIST CNN\n\n");
		}
		else {
			printf("CIFAR CNN\n\n");
		}
		_mutex_release(&print_lock);
    }

    if (test_mode) {
        inference_0 = 0;
        image_result = *TEST_IMAGE_RES(0);
        _mutex_acquire(&print_lock);
        printf("\n---------------------------------------\n");
        printf("Inf selected image [%d] from CPU: %lu\n", image_result, core);
        _mutex_release(&print_lock);
        pmu_reset();
        pmu_start();
        mnist_cnn_eval((unsigned int*)TEST_IMAGE_0, core, &inference_0);
        pmu_stop();
        _mutex_acquire(&print_lock);
        printf("\tselected image [%d] from CPU: %lu, inference: %d, \t\t", image_result, core, inference_0);
    	if (image_result != inference_0) {
    		printf("[Fail !!!]\n");
    	}
    	else {
    		printf("[Pass]\n");
    	}
        printf("\t\tInstr count is %llu\n", pmu_cycle_counter_get_count());
        printf("\t\t\t Cnt 0 is %u\n", pmu_counter_get_event_count(0));
        printf("\t\t\t Cnt 1 is %u\n", pmu_counter_get_event_count(1));
        printf("\t\t\t Cnt 2 is %u\n", pmu_counter_get_event_count(2));
        printf("\t\t\t Cnt 3 is %u\n", pmu_counter_get_event_count(3));
        printf("\t\t\t Cnt 4 is %u\n", pmu_counter_get_event_count(4));
        printf("\n");
        _mutex_release(&print_lock);
    }
    else {

    	while(1) {

        	get_image_idx = __atomic_fetch_add(&next_image, 1, __ATOMIC_RELAXED);
        	if ( get_image_idx >= TESTMODE_IMAGE_NUM ) {
        		break;
        	}

        	inference_1 = 0;
            image_result = *TEST_IMAGE_RES(get_image_idx);
        	_mutex_acquire(&print_lock);
        	printf("\n---------------------------------------\n");
        	printf("\nGet next image: %d\n", get_image_idx);
        	printf("Inf image[%d] from CPU: %lu\n", image_result, core);
        	_mutex_release(&print_lock);
        	pmu_reset();
        	pmu_start();
        	mnist_cnn_eval((unsigned int*)TEST_IMAGE_X(get_image_idx), core, &inference_1);
        	pmu_stop();
        	_mutex_acquire(&print_lock);
        	printf("\timage[%d] from CPU: %lu, result: %d, \t\t", image_result, core, inference_1);
        	if (image_result != inference_1) {
        		printf("[Fail !!!]\n");
        	}
        	else {
        		printf("[Pass]\n");
        	}
            printf("\t\tCycle count is %llu\n", pmu_cycle_counter_get_count());
        	_mutex_release(&print_lock);

      	}
    }
	_mutex_acquire(&print_lock);
	printf("\n\n[CPU: %lu] End of MNIST CNN Evaluation\n\n", core);
	_mutex_release(&print_lock);


#else

    initPmuInterrupt();

    // Enable interrupts
    asm ("msr DAIFClr, #0xF");
    setICC_IGRPEN1_EL1(igrpEnable);

    _mutex_acquire(&print_lock);
    printf("CPU %lu: Starting calculation\n", core);
    _mutex_release(&print_lock);

    test_scalar_neon();

    calculatePrimes(core);

#endif

    _mutex_acquire(&print_lock);
    printf("CPU %lu: finished\n", core);
    _mutex_release(&print_lock);

    if (__atomic_add_fetch(&cpu_finished_count, 1, __ATOMIC_RELAXED) < cpu_active_count)
    {
      /*
       * CPUs that finish early wait for termination
       */
      while(1)
        asm("wfi");
    }
    else
    {
      /*
       * The last CPU to finish terminates the program
       */
      printf("All CPUs finished\n");
      exit(0);
    }
}


/*
 * int main(void)
 *    the application start point for the primary CPU - bring up the
 *    secondary CPUs and then call MainApp
 *
 *  Inputs
 *    <none>
 *
 *  Returns
 *    subroutine does not return
 */
int main(void)
{
    _mutex_initialize(&print_lock);

    printf("\r\nDS-5 PMUv3 Example, based on ARMv8-A SMP Prime Number Generator Example\r\n\r\n");

    initTimerInterrupt();

    initPrimes(); // Initialize the primes just once, including print_lock

    next_image = 0;

    asm("dsb OSHST"); // Ensure the side effects of initPrimes are seen before waking the CPUs

    /*
     * send SGI15 to the other (i.e. secondary) CPUs to
     * wake them up
     *
     * these should be group1 ("non-secure") interrupts
     */
    setICC_SGI1R(0, 0, 0, sgirIRMAll, 0, 15);

    /*
     * start the main application with the other CPUs
     *
     * note: MainApp() does not return
     */
    MainApp();
}
