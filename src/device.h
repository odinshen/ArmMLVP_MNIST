/* Copyright (C) ARM Limited, 2016. All rights reserved. */

#ifndef INCLUDED_DEVICE_H
#define INCLUDED_DEVICE_H
/**
 * Gets the unique core number of this MPIDR value
 * from the set [0, NUM_CORES)
 */
unsigned long mpidr_to_core_number(unsigned long mpidr);

/**
 * Gets the cluster number of this MPIDR value
 * from the set [0, NUM_CLUSTERS)
 */
unsigned long mpidr_to_cluster_number(unsigned long mpidr);

#endif
