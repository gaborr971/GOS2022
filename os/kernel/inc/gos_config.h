//*************************************************************************************************
//
//                            #####             #####             #####
//                          #########         #########         #########
//                         ##                ##       ##       ##
//                        ##                ##         ##        #####
//                        ##     #####      ##         ##           #####
//                         ##       ##       ##       ##                ##
//                          #########         #########         #########
//                            #####             #####             #####
//
//                                      (c) Gabor Repasi, 2022
//
//*************************************************************************************************
//! @file		gos_config.h
//! @author		Gabor Repasi
//! @date		2022-12-11
//! @version	1.2
//!
//! @brief		GOS configuration header.
//! @details	This header contains the kernel and service configurations of the operating system.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-24	Gabor Repasi	Initial version created.
// 1.1		2022-12-03	Gabor Repasi	+	CFG_SCHED_COOPERATIVE added
// 1.2		2022-12-11	Gabor Repasi	+	CFG_USE_PRIO_INHERITANCE added
//										-	CFG_LOG_PORT removed
//*************************************************************************************************
#ifndef GOS_CONFIG_H
#define GOS_CONFIG_H
/*
 * Includes
 */
#include <stdint.h>

/*
 * Macros
 */
//! Cooperative scheduling flag.
#define CFG_SCHED_COOPERATIVE			( 0 )
//! Priority inheritance flag for lock.
#define CFG_USE_PRIO_INHERITANCE		( 0 )

//! Maximum task priority levels.
#define CFG_TASK_MAX_PRIO_LEVELS		( UINT8_MAX )
//! Idle task priority.
#define CFG_TASK_IDLE_PRIO				( CFG_TASK_MAX_PRIO_LEVELS )
//! Maximum task name length.
#define CFG_TASK_MAX_NAME_LENGTH		( 32 )
//! Maximum number of tasks.
#define CFG_TASK_MAX_NUMBER				( 32 )


//! Minimum task stack size.
#define CFG_TASK_MIN_STACK_SIZE			( 0x200 )
//! Maximum task stack size.
#define CFG_TASK_MAX_STACK_SIZE			( 0x4000 )
//! Idle task stack size.
#define CFG_IDLE_TASK_STACK_SIZE		( 0x300 )
//! Signal daemon task stack size.
#define CFG_TASK_SIGNAL_DAEMON_STACK	( 0x300 )
//! Process daemon task stack size.
#define CFG_TASK_PROC_DAEMON_STACK		( 0x300 )
//! Time daemon task stack size.
#define CFG_TASK_TIME_DAEMON_STACK		( 0x300 )
//! Kernel dump stack size.
#define CFG_TASK_KERNEL_DUMP_STACK		( 0x300 )
//! Process dump stack size.
#define CFG_TASK_PROC_DUMP_STACK		( 0x300 )
//! Queue dump stack size.
#define CFG_TASK_QUEUE_DUMP_STACK		( 0x300 )
//! Message daemon task stack size.
#define CFG_TASK_MESSAGE_DAEMON_STACK	( 0x300 )
//! Shell daemon task stack size.
#define CFG_TASK_SHELL_DAEMON_STACK		( 0x300 )
//! Log daemon task stack size.
#define CFG_TASK_LOG_DAEMON_STACK		( 0x400 )

//! Log daemon task priority.
#define CFG_TASK_LOG_DAEMON_PRIO		( 194 )
//! Message daemon task priority.
#define CFG_TASK_MESSAGE_DAEMON_PRIO	( 198 )
//! Signal daemon task priority.
#define CFG_TASK_SIGNAL_DAEMON_PRIO		( 197 )
//! Process daemon task priority.
#define CFG_TASK_PROC_DAEMON_PRIO		( 195 )
//! Shell daemon task priority.
#define CFG_TASK_SHELL_DAEMON_PRIO		( 199 )
//! Time daemon task priority.
#define CFG_TASK_TIME_DAEMON_PRIO		( 196 )
//! Kernel dump priority.
#define CFG_TASK_KERNEL_DUMP_PRIO		( 200 )
//! Queue dump priority.
#define CFG_TASK_QUEUE_DUMP_PRIO		( 200 )
//! Process dump priority.
#define CFG_TASK_PROC_DUMP_PRIO			( 200 )

//! Process service use flag.
#define CFG_PROC_USE_SERVICE			( 1 )
//! Maximum process priority levels.
#define CFG_PROC_MAX_PRIO_LEVELS		( UINT8_MAX )
//! Idle process priority.
#define CFG_PROC_IDLE_PRIO				( CFG_PROC_MAX_PRIO_LEVELS )
//! Maximum process name length.
#define CFG_PROC_MAX_NAME_LENGTH		( 24 )
//! Maximum number of processes.
#define CFG_PROC_MAX_NUMBER				( 4 )


//! Maximum number of locks.
#define CFG_LOCK_MAX_NUMBER				( 8 )
//! Maximum number of lock waiters.
#define CFG_LOCK_MAX_WAITERS			( 32 )


//! Maximum number of queues.
#define CFG_QUEUE_MAX_NUMBER			( 6 )
//! Maximum number of queue elements.
#define CFG_QUEUE_MAX_ELEMENTS			( 72 )
//! Maximum queue length.
#define CFG_QUEUE_MAX_LENGTH			( 200 )
//! Queue use name flag.
#define CFG_QUEUE_USE_NAME				( 1 )
//! Maximum queue name length.
#define CFG_QUEUE_MAX_NAME_LENGTH		( 24 )


//! Maximum number of signals.
#define CFG_SIGNAL_MAX_NUMBER			( 10 )
//! Maximum number of signal subscribers.
#define CFG_SIGNAL_MAX_SUBSCRIBERS		( 6 )


//! Maximum number of messages handled at once.
#define CFG_MESSAGE_MAX_NUMBER			( 12 )
//! Maximum length of a message in bytes.
#define CFG_MESSAGE_MAX_LENGTH			( 80 )
//! Maximum number of message waiters.
#define CFG_MESSAGE_MAX_WAITERS			( 10 )
//! Maximum number of message IDs a task can wait for (includes the terminating 0).
#define CFG_MESSAGE_MAX_WAITER_IDS		( 8 )


//! Shell service use flag.
#define CFG_SHELL_USE_SERVICE			( 1 )
//! Maximum number of shell commands.
#define CFG_SHELL_MAX_COMMAND_NUMBER	( 10 )
//! Maximum command length.
#define CFG_SHELL_MAX_COMMAND_LENGTH	( 24 )


//! GCP maximum number of channels.
#define CFG_GCP_CHANNELS_MAX_NUMBER		( 2 )

//! Kernel dump configuration flag.
#define CFG_KERNEL_DUMP_CONFIG			( 0 )

//! Log maximum (line) length.
#define CFG_LOG_MAX_LENGTH				( 200 )

/**
* DRV_UART1 : 0
* DRV_UART2 : 1
* DRV_UART3 : 2
* DRV_UART4 : 3
 */
//#define CFG_LOG_PORT					( 1 )
#endif
