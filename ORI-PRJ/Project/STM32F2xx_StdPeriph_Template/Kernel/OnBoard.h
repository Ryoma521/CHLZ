#ifndef ONBOARD_H
#define ONBOARD_H

/*********************************************************************
 * CONSTANTS
 */

// Timer clock and power-saving definitions
#define TIMER_DECR_TIME    1  // 1ms - has to be matched with TC_OCC

/* OSAL timer defines */
#define TICK_TIME   1000   // Timer per tick - in micro-sec 暂时为1s，注意改回1ms

#define INT_HEAP_LEN  4096


// Memory Allocation Heap
#define MAXMEMHEAP INT_HEAP_LEN  // Typically, 0.70-1.50K

#endif
