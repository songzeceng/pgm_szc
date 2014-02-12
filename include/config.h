#pragma once

#define PGM_CONFIG

/*
 Select the primative to use for synchronization.
 - For pthread sleeping-mutex + condition variables = 0
 - For PGM spinlock + PGM condition variables       = 1
 */
#ifndef _USE_LITMUS
	#define PGM_SYNC_METHOD		0
#else
	/* Default to PGM spinlocks under Litmus */
	#define	PGM_SYNC_METHOD		1
#endif

/*
 Select the method tasks use for becoming non-preemptive
 while holding spinlocks.
 - To allow preemption                 = 0
 - Via disabling interrupts            = 1
 - Via Litmus's non-preemtion sections = 2

 Notes:
  1) Options 1 and 2 require tasks to run as root.
  2) Disabling interrupts may be unsafe for IPCs
     that may block on write. (PGM uses non-blocking
	 writes whenever possible.)
 */
#ifndef _USE_LITMUS
	#define PGM_NP_METHOD		0
#else
	/* Default to Litmus NP sections. */
	#define PGM_NP_METHOD		2
#endif

/*
 Select support for synchronization between processes.
 - Private synchronization = 0
 - Shared synchronization  = 1

 Private syncrhonization is faster, but graphs may
 not span multiple processes.
*/
#define PGM_SYNC_SCOPE		0


/*** VALIDATE CONFGURATION ***/

#if (PGM_SYNC_METHOD == 0) && (PGM_NP_METHOD != 0)
#error "Cannot disable interrupts using pthread synchronization."
#endif

#if (PGM_SYNC_METHOD == 1) && (PGM_NP_METHOD == 0)
#error "Spinlocks are unsafe without non-preemption."
#endif


/*** SET UP CONDITIONAL COMPILATION ***/

#if (PGM_SYNC_METHOD == 0)
	#define PGM_USE_PTHREAD_SYNC
#elif (PGM_SYNC_METHOD == 1)
	#define PGM_USE_PGM_SYNC
#else
	#error "Unknown synchronization method selected."
#endif

#if (PGM_NP_METHOD == 0)
	#define PGM_PREEMPTIVE
#elif (PGM_NP_METHOD == 1)
	#define PGM_NP_INTERRUPTS
#elif (PGM_NO_METHOD == 2)
	#define PGM_NP_LITMUS
#else
	#error "Unknown non-preemption method."
#endif

#if (PGM_SYNC_SCOPE == 0)
	#define PGM_PRIVATE
#elif (PGM_SYNC_SCOPE == 1)
	#define PGM_SHARED
#else
	#error "Unknown synchronization scope."
#endif
