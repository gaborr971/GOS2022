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
//! @file       gos_config.h
//! @author     Gabor Repasi
//! @date       2023-05-19
//! @version    1.5
//!
//! @brief      GOS configuration header.
//! @details    This header contains the kernel and service configurations of the operating system.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2022-10-24    Gabor Repasi    Initial version created.
// 1.1        2022-12-03    Gabor Repasi    +    CFG_SCHED_COOPERATIVE added
// 1.2        2022-12-11    Gabor Repasi    +    CFG_USE_PRIO_INHERITANCE added
//                                          -    CFG_LOG_PORT removed
// 1.3        2022-12-15    Gabor Repasi    -    Max priority level and idle task priority macros
//                                               moved to gos_kernel.h
// 1.4        2023-04-05    Gabor Repasi    -    Lock- and trigger-related macros removed
//                                          +    CFG_SYSTEM_TASK_STACK_SIZE added
// 1.5        2023-05-19    Ahmed Gazar     +    CFG_SHELL_COMMAND_BUFFER_SIZE added
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
#define CFG_SCHED_COOPERATIVE           ( 0 )
//! Priority inheritance flag for lock.
#define CFG_USE_PRIO_INHERITANCE        ( 1 )

//! Maximum task name length.
#define CFG_TASK_MAX_NAME_LENGTH        ( 32 )
//! Maximum number of tasks.
#define CFG_TASK_MAX_NUMBER             ( 36 )


//! Minimum task stack size.
#define CFG_TASK_MIN_STACK_SIZE         ( 0x200 )
//! Maximum task stack size.
#define CFG_TASK_MAX_STACK_SIZE         ( 0x4000 )
//! Idle task stack size.
#define CFG_IDLE_TASK_STACK_SIZE        ( 0x300 )
//! System task stack size.
#define CFG_SYSTEM_TASK_STACK_SIZE      ( 0x400 )
//! Signal daemon task stack size.
#define CFG_TASK_SIGNAL_DAEMON_STACK    ( 0x400 )
//! Process daemon task stack size.
#define CFG_TASK_PROC_DAEMON_STACK      ( 0x300 )
//! Time daemon task stack size.
#define CFG_TASK_TIME_DAEMON_STACK      ( 0x300 )
//! Kernel dump stack size.
//#define CFG_TASK_KERNEL_DUMP_STACK      ( 0x400 )
//! Process dump stack size.
//#define CFG_TASK_PROC_DUMP_STACK        ( 0x400 )
//! Queue dump stack size.
//#define CFG_TASK_QUEUE_DUMP_STACK       ( 0x300 )
//! Message daemon task stack size.
#define CFG_TASK_MESSAGE_DAEMON_STACK   ( 0x200 )
//! Shell daemon task stack size.
#define CFG_TASK_SHELL_DAEMON_STACK     ( 0x400 )
//! Log daemon task stack size.
#define CFG_TASK_TRACE_DAEMON_STACK     ( 0x400 )

//! Log daemon task priority.
#define CFG_TASK_TRACE_DAEMON_PRIO      ( 194 )
//! Message daemon task priority.
#define CFG_TASK_MESSAGE_DAEMON_PRIO    ( 198 )
//! Signal daemon task priority.
#define CFG_TASK_SIGNAL_DAEMON_PRIO     ( 197 )
//! Process daemon task priority.
#define CFG_TASK_PROC_DAEMON_PRIO       ( 195 )
//! Shell daemon task priority.
#define CFG_TASK_SHELL_DAEMON_PRIO      ( 199 )
//! Time daemon task priority.
#define CFG_TASK_TIME_DAEMON_PRIO       ( 196 )
//! Trigger daemon task priority.
#define CFG_TASK_TRIGGER_DAEMON_PRIO    ( 193 )
//! Kernel dump priority.
//#define CFG_TASK_KERNEL_DUMP_PRIO       ( 200 )
//! Queue dump priority.
//#define CFG_TASK_QUEUE_DUMP_PRIO        ( 200 )
//! Process dump priority.
//#define CFG_TASK_PROC_DUMP_PRIO         ( 200 )

//! Process service use flag.
#define CFG_PROC_USE_SERVICE            ( 1 )
//! Maximum process priority levels.
#define CFG_PROC_MAX_PRIO_LEVELS        ( UINT8_MAX )
//! Idle process priority.
#define CFG_PROC_IDLE_PRIO              ( CFG_PROC_MAX_PRIO_LEVELS )
//! Maximum process name length.
#define CFG_PROC_MAX_NAME_LENGTH        ( 24 )
//! Maximum number of processes.
#define CFG_PROC_MAX_NUMBER             ( 4 )


//! Maximum number of queues.
#define CFG_QUEUE_MAX_NUMBER            ( 4 )
//! Maximum number of queue elements.
#define CFG_QUEUE_MAX_ELEMENTS          ( 40 )
//! Maximum queue length.
#define CFG_QUEUE_MAX_LENGTH            ( 200 )
//! Queue use name flag.
#define CFG_QUEUE_USE_NAME              ( 1 )
//! Maximum queue name length.
#define CFG_QUEUE_MAX_NAME_LENGTH       ( 24 )


//! Maximum number of signals.
#define CFG_SIGNAL_MAX_NUMBER           ( 6 )
//! Maximum number of signal subscribers.
#define CFG_SIGNAL_MAX_SUBSCRIBERS      ( 6 )


//! Maximum number of messages handled at once.
#define CFG_MESSAGE_MAX_NUMBER          ( 4 )
//! Maximum length of a message in bytes.
#define CFG_MESSAGE_MAX_LENGTH          ( 80 )
//! Maximum number of message waiters.
#define CFG_MESSAGE_MAX_WAITERS         ( 10 )
//! Maximum number of message IDs a task can wait for (includes the terminating 0).
#define CFG_MESSAGE_MAX_WAITER_IDS      ( 8 )


//! Shell service use flag.
#define CFG_SHELL_USE_SERVICE           ( 1 )
//! Maximum number of shell commands.
#define CFG_SHELL_MAX_COMMAND_NUMBER    ( 16 )
//! Maximum command length.
#define CFG_SHELL_MAX_COMMAND_LENGTH    ( 20 )
//! Maximum parameters length.
#define CFG_SHELL_MAX_PARAMS_LENGTH     ( 128 )
//! Command buffer size.
#define CFG_SHELL_COMMAND_BUFFER_SIZE   ( 128 )


//! GCP maximum number of channels.
#define CFG_GCP_CHANNELS_MAX_NUMBER     ( 2 )


//! Log maximum (line) length.
#define CFG_TRACE_MAX_LENGTH            ( 200 )

#endif
