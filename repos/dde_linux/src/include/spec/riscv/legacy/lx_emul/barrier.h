/*
 * \brief  RISC-V-specific part of the Linux API emulation
 * \author Your Name
 * \date   2023-04-12
 */

/*
 * Copyright (C) 2023 Your Company
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

/*******************
 ** asm/barrier.h **
 *******************/

#define mb() asm volatile ("fence rw, rw" : : : "memory")
#define rmb() asm volatile ("fence r, r" : : : "memory")
#define wmb() asm volatile ("fence w, w" : : : "memory")

#define dma_wmb() __asm__ __volatile__ ("fence o, o" : : : "memory")
#define dma_rmb() dma_wmb()

/*
 * This is the "safe" implementation as needed for a configuration
 * with bufferable DMA memory and SMP enabled.
 */

#define smp_mb() asm volatile ("fence iorw, iorw" : : : "memory")
#define smp_rmb() smp_mb()
#define smp_wmb() asm volatile ("fence iow, iow" : : : "memory")

static inline void barrier() { asm volatile ("": : :"memory"); }
