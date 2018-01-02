#ifndef PMU_H
#define PMU_H

#define PMU_EVENT_SW_INCR                   0x000   /* Instruction architecturally executed, condition code check pass, software increment */
#define PMU_EVENT_L1I_CACHE_REFILL          0x001   /* Attributable Level 1 instruction cache refill */
#define PMU_EVENT_L1I_TLB_REFILL            0x002   /* Attributable Level 1 instruction TLB refill */
#define PMU_EVENT_L1D_CACHE_REFILL          0x003   /* Attributable Level 1 data cache refill */
#define PMU_EVENT_L1D_CACHE                 0x004   /* Attributable Level 1 data cache access */
#define PMU_EVENT_L1D_TLB_REFILL            0x005   /* Attributable Level 1 data TLB refill */
#define PMU_EVENT_LD_RETIRED                0x006   /* Instruction architecturally executed, condition code check pass, load */
#define PMU_EVENT_ST_RETIRED                0x007   /* Instruction architecturally executed, condition code check pass, store */
#define PMU_EVENT_INST_RETIRED              0x008   /* Instruction architecturally executed */
#define PMU_EVENT_EXC_TAKEN                 0x009   /* Exception taken */
#define PMU_EVENT_EXC_RETURN                0x00A   /* Instruction architecturally executed, condition code check pass, exception return */
#define PMU_EVENT_CID_WRITE_RETIRED         0x00B   /* Instruction architecturally executed, condition code check pass, write to CONTEXTIDR */
#define PMU_EVENT_PC_WRITE_RETIRED          0x00C   /* Instruction architecturally executed, condition code check pass, software change of the PC */
#define PMU_EVENT_BR_IMMED_RETIRED          0x00D   /* Instruction architecturally executed, immediate branch */
#define PMU_EVENT_BR_RETURN_RETIRED         0x00E   /* Instruction architecturally executed, condition code check pass, procedure return */
#define PMU_EVENT_UNALIGNED_LDST_RETIRED    0x00F   /* Instruction architecturally executed, condition code check pass, unaligned load or store */
#define PMU_EVENT_BR_MIS_PRED               0x010   /* Mispredicted or not predicted branch speculatively executed */
#define PMU_EVENT_CPU_CYCLES                0x011   /* Cycle */
#define PMU_EVENT_BR_PRED                   0x012   /* Predictable branch speculatively executed */
#define PMU_EVENT_MEM_ACCESS                0x013   /* Data memory access */
#define PMU_EVENT_L1I_CACHE                 0x014   /* Attributable Level 1 instruction cache access */
#define PMU_EVENT_L1D_CACHE_WB              0x015   /* Attributable Level 1 data cache write-back */
#define PMU_EVENT_L2D_CACHE                 0x016   /* Attributable Level 2 data cache access */
#define PMU_EVENT_L2D_CACHE_REFILL          0x017   /* Attributable Level 2 data cache refill */
#define PMU_EVENT_L2D_CACHE_WB              0x018   /* Attributable Level 2 data cache write-back */
#define PMU_EVENT_BUS_ACCESS                0x019   /* Bus access */
#define PMU_EVENT_MEMORY_ERROR              0x01A   /* Local memory error */
#define PMU_EVENT_INST_SPEC                 0x01B   /* Operation speculatively executed */
#define PMU_EVENT_TTBR_WRITE_RETIRED        0x01C   /* Instruction architecturally executed, condition code check pass, write to TTBR */
#define PMU_EVENT_BUS_CYCLES                0x01D   /* Bus cycle */
#define PMU_EVENT_CHAIN                     0x01E   /* For odd-numbered counters, increments the count by one for each overflow of the preceding even-numbered counter. For even-numbered counters there is no increment. */
#define PMU_EVENT_L1D_CACHE_ALLOCATE        0x01F   /* Attributable Level 1 data cache allocation without refill */
#define PMU_EVENT_L2D_CACHE_ALLOCATE        0x020   /* Attributable Level 2 data cache allocation without refill */
#define PMU_EVENT_BR_RETIRED                0x021   /* Instruction architecturally executed, branch */
#define PMU_EVENT_BR_MIS_PRED_RETIRED       0x022   /* Instruction architecturally executed, mispredicted branch */
#define PMU_EVENT_STALL_FRONTEND            0x023   /* No operation issued due to the frontend */
#define PMU_EVENT_STALL_BACKEND             0x024   /* No operation issued due to backend */
#define PMU_EVENT_L1D_TLB                   0x025   /* Attributable Level 1 data or unified TLB access */
#define PMU_EVENT_L1I_TLB                   0x026   /* Attributable Level 1 instruction TLB access */
#define PMU_EVENT_L2I_CACHE                 0x027   /* Attributable Level 2 instruction cache access */
#define PMU_EVENT_L2I_CACHE_REFILL          0x028   /* Attributable Level 2 instruction cache refill */
#define PMU_EVENT_L3D_CACHE_ALLOCATE        0x029   /* Attributable Level 3 data or unified cache allocation without refill */
#define PMU_EVENT_L3D_CACHE_REFILL          0x02A   /* Attributable Level 3 data or unified cache refill */
#define PMU_EVENT_L3D_CACHE                 0x02B   /* Attributable Level 3 data or unified cache access */
#define PMU_EVENT_L3D_CACHE_WB              0x02C   /* Attributable Level 3 data or unified cache write-back */
#define PMU_EVENT_L2D_TLB_REFILL            0x02D   /* Attributable Level 2 data or unified TLB refill */
#define PMU_EVENT_L2I_TLB_REFILL            0x02E   /* Attributable Level 2 instruction TLB refill */
#define PMU_EVENT_L2D_TLB                   0x02F   /* Attributable Level 2 data or unified TLB access */
#define PMU_EVENT_L2I_TLB                   0x030   /* Attributable Level 2 instruction TLB access */

/*
 * Range of common architectural and microarchitectural events
 * allowed by ARMv8.0
 */
#define PMU_COMMON_EVENTS_0_MIN 0x0ul
#define PMU_COMMON_EVENTS_0_MAX 0x3ful

/*
 * Range of common architectural and microarchitectural events
 * additionally allowed by ARMv8.1
 */
#define PMU_COMMON_EVENTS_1_MIN 0x4000ul
#define PMU_COMMON_EVENTS_1_MAX 0x403ful


/*
 * Gives the name of an event, generic value if unknown.
 */
const char* pmu_get_event_name(unsigned long event_type);

/*
 * Returns 1 if common event is implemented, 0 otherwise.
 */
unsigned int pmu_is_common_event_implemented(unsigned long event_type);

/*
 * Reads the number of PMU counters this processing element has.
 */
unsigned int pmu_get_number_of_counters(void);

/*
 * Reset the PMU to a state with everything disabled
 */
void pmu_reset(void);

/*
 * Start PMU event counting for all enabled counters on the current PE
 */
void pmu_start(void);

/*
 * Stop PMU event counting for all counters on the current PE
 */
void pmu_stop(void);

/*
 * Reads the count held by the cycle counter.
 */
unsigned long long pmu_cycle_counter_get_count(void);

/*
 * Reads the count held by a counter.
 */
unsigned int pmu_counter_get_event_count(unsigned long counter);

/*
 * Sets the count held by a counter.
 */
void pmu_counter_set_event_count(unsigned long counter, unsigned long event_count);

/*
 * Reads the event type that a counter is set to count.
 */
unsigned int pmu_counter_get_event_type(unsigned long counter);

/*
 * Sets the event type that a counter should count.
 */
void pmu_counter_set_event_type(unsigned long counter, unsigned long event_type);

/*
 * Enables the cycle counter
 */
void pmu_cycle_counter_enable(void);

/*
 * Enables a counter
 */
void pmu_counter_enable(unsigned long counter);

/*
 * Disables the cycle counter
 */
void pmu_cycle_counter_disable(void);

/*
 * Disables a counter.
 */
void pmu_counter_disable(unsigned long counter);

/*
 * Makes the cycle counter generate an interrupt when it overflows.
 */
void pmu_cycle_counter_enable_overflow_interrupt(void);

/*
 * Makes the counter generate an interrupt when it overflows.
 */
void pmu_counter_enable_overflow_interrupt(unsigned long counter);

/*
 * Stops the cycle counter from generating an interrupt when it overflows.
 */
void pmu_cycle_counter_disable_overflow_interrupt(void);

/*
 * Stops a counter from generating an interrupt when it overflows.
 */
void pmu_counter_disable_overflow_interrupt(unsigned long counter);

/*
 * Read whether the cycle counter's overflow flag is set.
 */
unsigned int pmu_cycle_counter_is_overflow_flag_set(void);

/*
 * Read whether a counter's overflow flag is set.
 */
unsigned int pmu_counter_is_overflow_flag_set(unsigned long counter);

/*
 * Clears the cycle counter's overflow flag.
 */
void pmu_cycle_counter_clear_overflow_flag(void);

/*
 * Clears a counter's overflow flag.
 */
void pmu_counter_clear_overflow_flag(unsigned long counter);

#endif
