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
//! @file       gos_kernel.h
//! @author     Gabor Repasi
//! @date       2022-12-15
//! @version    1.7
//!
//! @brief      GOS kernel header.
//! @details    The GOS kernel is the core of the GOS system. It contains the basic type
//!             definitions of the system (these are used across the OS in other services), and it
//!             handles the system tick, tasks, and scheduling. The kernel module also provides an
//!             interface for registering tasks, changing their states, registering hook functions,
//!             disabling the scheduler. It also provides a service to dump the kernel
//!             configuration and task information on the log output.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2022-10-21    Gabor Repasi    Initial version created.
// 1.1        2022-11-14    Gabor Repasi    +    Task previous state added to task descriptor type
//                                          +    Dump signal sender enumerators added
// 1.2        2022-11-15    Gabor Repasi    +    License added
// 1.3        2022-12-03    Gabor Repasi    +    gos_kernelTaskRegisterTasks added
//                                          +    GOS_PARAM_IGNORE macro added
//                                          +    taskIdEx added to gos_taskDescriptor_t
// 1.4        2022-12-04    Gabor Repasi    *    Sleep, wake-up, suspend, resume, block, unblock,
//                                               delete hook functions replaced with signals
// 1.5        2022-12-08    Gabor Repasi    +    Task original priority added
//                                          +    Task priority setter and getter functions added
// 1.6        2022-12-11    Gabor Repasi    -    GOS_PARAM_IGNORE removed
//                                          +    Interface descriptions added
// 1.7        2022-12-15    Gabor Repasi    +    Task privilege levels and privilege control macros
//                                               and functions added
//*************************************************************************************************
//
// Copyright (c) 2022 Gabor Repasi
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//*************************************************************************************************
#ifndef GOS_KERNEL_H
#define GOS_KERNEL_H
/*
 * Includes
 */
#include <stdint.h>
#include "gos_config.h"

/*
 * Macros
 */
#ifndef NULL
/**
 * NULL pointer.
 */
#define NULL ( (void *) 0 )
#endif

/**
 * Default task ID.
 */
#define GOS_DEFAULT_TASK_ID         ( (gos_tid_t)0x8000 )

/**
 * valid task ID.
 */
#define GOS_INVALID_TASK_ID         ( (gos_tid_t)0x0100 )

/**
 * Maximum task priority levels.
 */
#define GOS_TASK_MAX_PRIO_LEVELS       ( UINT8_MAX )

/**
 * Idle task priority.
 */
#define GOS_TASK_IDLE_PRIO             ( GOS_TASK_MAX_PRIO_LEVELS )

/**
 * Static macro.
 */
#define GOS_STATIC                     static

/**
 * Inline macro.
 */
#define GOS_INLINE                     inline __attribute__((always_inline))

/**
 * Static in-line macro.
 */
#define GOS_STATIC_INLINE              GOS_STATIC GOS_INLINE

/**
 * Extern macro.
 */
#define GOS_EXTERN                     extern

/**
 * Naked.
 */
#define GOS_NAKED                      __attribute__ ((naked))

/**
 * Unused.
 */
#define GOS_UNUSED                     __attribute__ ((unused))

/**
 * NOP.
 */
#define GOS_NOP                        __asm volatile ("NOP")

/**
 * ASM.
 */
#define GOS_ASM                        __asm volatile

/**
 * Disable scheduling.
 */
#define GOS_DISABLE_SCHED              {                                            \
                                           GOS_EXTERN bool_t schedulingDisabled;    \
                                           schedulingDisabled = GOS_TRUE;           \
                                       }
/**
 * Enable scheduling.
 */
#define GOS_ENABLE_SCHED               {                                            \
                                           GOS_EXTERN bool_t schedulingDisabled;    \
                                           schedulingDisabled = GOS_FALSE;          \
                                       }
/**
 * Request privileged access to next function.
 */
#define GOS_PRIVILEGED_ACCESS          {                                            \
                                           GOS_EXTERN u8_t privilegedAccess;        \
                                           privilegedAccess++;                      \
                                       }

/**
 * Decrease privileged access requests.
 */
#define GOS_UNPRIVILEGED_ACCESS        {                                            \
                                           GOS_EXTERN u8_t privilegedAccess;        \
                                           GOS_EXTERN u8_t inIsr;                   \
                                           if (privilegedAccess > 0 && inIsr == 0){ \
                                               privilegedAccess--;                  \
                                           }                                        \
                                       }
/**
 * Interrupt Service Routine enter.
 */
#define GOS_ISR_ENTER                  {                                            \
                                           GOS_EXTERN u8_t inIsr;                   \
                                           if (inIsr == 0) { GOS_DISABLE_SCHED }    \
                                           inIsr++;                                 \
                                       }
/**
 * Interrupt service routine exit.
 */
#define GOS_ISR_EXIT                   {                                            \
                                           GOS_EXTERN u8_t inIsr;                   \
                                           if (inIsr > 0) { inIsr--; }              \
                                           if (inIsr == 0) { GOS_ENABLE_SCHED }     \
                                       }
/**
 * Determine if caller is an ISR.
 */
#define GOS_IS_CALLER_ISR(x)           {                                            \
                                          GOS_EXTERN u8_t inIsr;                    \
                                           x = inIsr > 0 ? GOS_TRUE : GOS_FALSE;    \
                                       }
/**
 * Determine if caller has privileged access.
 */
#define GOS_IS_ACCESS_PRIVILEGED(x)    {                                                    \
                                           GOS_EXTERN u8_t privilegedAccess;                \
                                           x = privilegedAccess > 0 ? GOS_TRUE : GOS_FALSE; \
                                       }

/**
 * Atomic operation enter - disable interrupts and kernel rescheduling.
 */
#define GOS_ATOMIC_ENTER               {                                            \
                                           GOS_ASM( " cpsid i " ::: "memory" );     \
                                           GOS_ASM( "dsb" ::: "memory" );           \
                                           GOS_ASM( "isb" );                        \
                                       }

/**
 * Atomic operation exit - enable interrupts kernel rescheduling.
 */
#define GOS_ATOMIC_EXIT                {                                            \
                                           GOS_ASM( " cpsie i " ::: "memory" );     \
                                       }

/**
 * Task manipulation privilege flag.
 */
#define GOS_PRIV_TASK_MANIPULATE       ( 1 << 15 )

/**
 * Task priority change privilege flag.
 */
#define GOS_PRIV_TASK_PRIO_CHANGE      ( 1 << 14 )

/**
 * Logging privilege flag.
 */
#define GOS_PRIV_LOG                   ( 1 << 13 )

/**
 * Task privilege change privilege flag.
 */
#define GOS_PRIV_MODIFY                ( 1 << 12 )

/**
 * Task signal invoking privilege flag.
 */
#define GOS_PRIV_SIGNALING             ( 1 << 11 )

/**
 * Kernel reserved.
 */
#define GOS_PRIV_RESERVED_3            ( 1 << 10 )

/**
 * Kernel reserved.
 */
#define GOS_PRIV_RESERVED_4            ( 1 << 9 )

/**
 * Kernel reserved.
 */
#define GOS_PRIV_RESERVED_5            ( 1 << 8 )

/*
 * Type definitions
 */

/*
 * Basic data types.
 */
typedef uint8_t  bool_t;    //!< Boolean logic type.
typedef uint8_t  u8_t;      //!< 8-bit unsigned type.
typedef uint16_t u16_t;     //!< 16-bit unsigned type.
typedef uint32_t u32_t;     //!< 32-bit unsigned type.
typedef uint64_t u64_t;     //!< 64-bit unsigned type.
typedef int8_t   s8_t;      //!< 8-bit signed type.
typedef int16_t  s16_t;     //!< 16-bit signed type.
typedef int32_t  s32_t;     //!< 32-bit signed type.
typedef int64_t  s64_t;     //!< 64-bit signed type.
typedef char     char_t;    //!< 8-bit character type.
typedef float    float_t;   //!< Single precision float type.
typedef double   double_t;  //!< Double precision float type.
typedef void     void_t;    //!< Void type.

/*
 * Task-related definitions.
 */
typedef u16_t    gos_tid_t;                                    //!< Task ID type.
typedef char_t   gos_taskName_t [CFG_TASK_MAX_NAME_LENGTH];    //!< Task name type.
typedef void_t   (*gos_task_t)(void_t);                        //!< Task function type.
typedef u8_t     gos_taskPrio_t;                               //!< Task priority type.
typedef u32_t    gos_taskSleepTick_t;                          //!< Sleep tick type.
typedef u32_t    gos_taskAddress_t;                            //!< Memory address type.
typedef u32_t    gos_taskRunCounter_t;                         //!< Run counter type.
typedef u64_t    gos_taskRunTime_t;                            //!< Run-time type.
typedef u32_t    gos_taskCSCounter_t;                          //!< Context-switch counter type.
typedef u16_t    gos_taskStackSize_t;                          //!< Task stack size type.

/**
 *  Task state enumerator.
 *
 *  @note Hamming distance of
 *  - ready and sleeping:       3
 *  - ready and blocked:        3
 *  - ready and suspended:      4
 *  - ready and zombie:         3
 *  - sleeping and blocked:     4
 *  - sleeping and suspended:   3
 *  - sleeping and zombie:      4
 *  - blocked and suspended:    3
 *  - blocked and zombie:       2
 *  - suspended and zombie:     1
 */
typedef enum
{
    GOS_TASK_READY      = 0b01010,   //!< Task is ready to be scheduled.
    GOS_TASK_SLEEPING   = 0b10110,   //!< Task is sleeping (waiting for wake-up ticks).
    GOS_TASK_BLOCKED    = 0b11001,   //!< Task is blocked (waiting for resource).
    GOS_TASK_SUSPENDED  = 0b00101,   //!< Task is suspended (not to be scheduled), but can be resumed.
    GOS_TASK_ZOMBIE     = 0b01101    //!< Task deleted (physically existing in memory, but cannot be resumed).
}gos_taskState_t;

/**
 * Task privilege level enumerator.
 */
typedef enum
{
    GOS_TASK_PRIVILEGE_SUPERVISOR = 0xFFFF,    //!< Task supervisor privilege level.
    GOS_TASK_PRIVILEGE_KERNEL     = 0xFF00,    //!< Task kernel privilege level.
    GOS_TASK_PRIVILEGE_USER       = 0x00FF,    //!< Task user privilege level.
    GOS_TASK_PRIVILEGED_USER      = 0x20FF     //!< User with logging right.
}gos_taskPrivilegeLevel_t;

/*
 * Hook function type definitions.
 */
typedef void_t    (*gos_taskSwapHook_t    )(gos_tid_t, gos_tid_t  );    //!< Task swap hook type.
typedef void_t    (*gos_taskIdleHook_t    )(void_t                );    //!< Task idle hook type.
typedef void_t    (*gos_taskSleepHook_t   )(gos_tid_t             );    //!< Task sleep hook type.
typedef void_t    (*gos_taskWakeupHook_t  )(gos_tid_t             );    //!< Task wake-up hook type.
typedef void_t    (*gos_taskSuspendHook_t )(gos_tid_t             );    //!< Task suspend hook type.
typedef void_t    (*gos_taskResumeHook_t  )(gos_tid_t             );    //!< Task resume hook type.
typedef void_t    (*gos_taskBlockHook_t   )(gos_tid_t             );    //!< Task block hook type.
typedef void_t    (*gos_taskUnblockHook_t )(gos_tid_t             );    //!< Task unblock hook type.
typedef void_t    (*gos_taskDeleteHook_t  )(gos_tid_t             );    //!< Task delete hook type.

/**
 *  Result type enumerator.
 *  @note Hamming distance of
 *  - success and error:    7
 *  - success and busy:     4
 *  - error and busy:       5
 */
typedef enum
{
    GOS_SUCCESS = 0b01010101, //!< Success.
    GOS_ERROR   = 0b10101110, //!< Error.
    GOS_BUSY    = 0b10110001  //!< Busy.
}gos_result_t;

/**
 *  Boolean values.
 *  @note Hamming distance of true and false value: 7.
 */
typedef enum
{
    GOS_TRUE  = 0b00110110,   //!< True value.
    GOS_FALSE = 0b01001001    //!< False value.
}gos_boolValue_t;

/**
 * Task descriptor structure.
 */
typedef struct
{
    gos_task_t               taskFunction;         //!< Task function.
    gos_taskState_t          taskState;            //!< Task state.
    gos_taskState_t          taskPreviousState;    //!< Task previous state (for restoration).
    gos_taskPrio_t           taskPriority;         //!< Task priority.
    gos_taskPrio_t           taskOriginalPriority; //!< Task original priority.
    gos_taskPrivilegeLevel_t taskPrivilegeLevel;   //!< Task privilege level.
    gos_taskName_t           taskName;             //!< Task name.
    gos_tid_t                taskId;               //!< Task ID (internal).
    gos_tid_t*               taskIdEx;             //!< Task ID (external).
    gos_taskSleepTick_t      taskSleepTicks;       //!< Task sleep ticks.
    gos_taskAddress_t        taskPsp;              //!< Task PSP.
    gos_taskRunCounter_t     taskRunCounter;       //!< Task run counter.
    gos_taskRunTime_t        taskRunTime;          //!< Task run-time.
    gos_taskCSCounter_t      taskCsCounter;        //!< Task context-switch counter.
    gos_taskStackSize_t      taskStackSize;        //!< Task stack size.
    u16_t                    taskCpuUsage;         //!< Task processor usage in [%].
}gos_taskDescriptor_t;

/**
 * Dump signal sender enumerator.
 */
typedef enum
{
    GOS_DUMP_SENDER_USER,    //!< Sender is user.
    GOS_DUMP_SENDER_KERNEL,  //!< Sender is the kernel.
    GOS_DUMP_SENDER_PROC,    //!< Sender is the process service.
    GOS_DUMP_SENDER_QUEUE,   //!< Sender is the queue.
    GOS_DUMP_SENDER_SIGNAL,  //!< Sender is the signal service.
    GOS_DUMP_SENDER_MESSAGE, //!< Sender is the message service.
    GOS_DUMP_SENDER_SHELL,   //!< Sender is the shell service.
    GOS_DUMP_SENDER_TIME,    //!< Sender is the time service.
    GOS_DUMP_SENDER_LAST     //!< Sender is the last one.
}gos_dumpSignalSender_t;

/*
 * Function prototypes
 */

/**
 * @brief   This function initializes the kernel.
 * @details Initializes the internal task array, fills out the PSP for the idle task,
 *          and registers the kernel dump task and suspends it.
 *
 * @return  Result of initialization.
 *
 * @retval  GOS_SUCCESS :    Kernel initialization successful.
 * @retval  GOS_ERROR   :    Kernel dump task or task suspension unsuccessful.
 */
gos_result_t gos_kernelInit (void_t);

/**
 * @brief   This function registers an array of tasks for scheduling.
 * @details Checks the task descriptor array pointer and registers the tasks one by one.
 *
 * @param   taskDescriptors : Pointer to a task descriptor structure array.
 * @param   arraySize       : Size of the array in bytes.
 *
 * @return  Result of task registration.
 *
 * @retval  GOS_SUCCESS     : Tasks registered successfully.
 * @retval  GOS_ERROR       : Invalid task descriptor (NULL function pointer,
 *                            invalid priority level, invalid stack size, idle task registration,
 *                            or stack size is not 4-byte-aligned) in one of the array elements or
 *                            task array is full.
 */
gos_result_t gos_kernelTaskRegisterTasks (gos_taskDescriptor_t* taskDescriptors, u16_t arraySize);

/**
 * @brief   This function registers a task for scheduling.
 * @details Checks the task descriptor parameters and then tries to find the next empty
 *          slot in the internal task array. When it is found, it registers the task in
 *          that slot.
 *
 * @param   taskDescriptor : Pointer to a task descriptor structure.
 * @param   taskId         : Pointer to a variable to hold to assigned task ID value.
 *
 * @return  Result of task registration.
 *
 * @retval  GOS_SUCCESS    : Task registered successfully.
 * @retval  GOS_ERROR      : Invalid task descriptor (NULL function pointer,
 *                           invalid priority level, invalid stack size, idle task registration,
 *                           or stack size is not 4-byte-aligned) or task array is full.
 */
gos_result_t gos_kernelTaskRegister (gos_taskDescriptor_t* taskDescriptor, gos_tid_t* taskId);

/**
 * @brief   Sends the current task to sleeping state.
 * @details Checks the current task and its state, modifies it to sleeping, and
 *          if there is a sleep hook function registered, it calls it. Then, it
 *          invokes a rescheduling.
 *
 * @param   sleepTicks  : Minimum number of ticks until the task should remain in sleeping state.
 *
 * @return  Result of task sleeping.
 *
 * @retval  GOS_SUCCESS : Task successfully sent to sleeping state.
 * @retval  GOS_ERROR   : Function called from idle task or task state is not ready.
 */
gos_result_t gos_kernelTaskSleep (gos_taskSleepTick_t sleepTicks);

/**
 * @brief   Wakes up the given task.
 * @details Checks the current task and its state, modifies it to ready, and
 *          if there is a wake-up hook function registered, it calls it.
 *
 * @param   taskId      : ID of the task to be waken up.
 *
 * @return  Result of task wake-up.
 *
 * @retval  GOS_SUCCESS : Task waken up successfully.
 * @retval  GOS_ERROR   : Task ID is invalid, or task is not sleeping.
 */
gos_result_t gos_kernelTaskWakeup (gos_tid_t taskId);

/**
 * @brief   Sends the given task to suspended state.
 * @details Checks the given task ID and its state, modified it to suspended, and
 *          if there is a suspend hook function registered, it calls it. If the
 *          suspended function is the currently running one, it invokes a rescheduling.
 *
 * @param   taskId     : ID of the task to be suspended.
 *
 * @return  Result of task suspension.
 *
 * @retval  GOS_SUCESS : Task suspended successfully.
 * @retval  GOS_ERROR  : Task ID is invalid, or task state is not ready or sleeping.
 */
gos_result_t gos_kernelTaskSuspend (gos_tid_t taskId);

/**
 * @brief   Resumes the given task.
 * @details Checks the given task ID and its state, modified it to ready, and
 *          if there is a resume hook function registered, it calls it.
 *
 * @param   taskId     : ID of the task to be resumed.
 *
 * @return  Result of task resumption.
 *
 * @retval  GOS_SUCESS : Task resumed successfully.
 * @retval  GOS_ERROR  : Task ID is invalid, or task is not suspended.
 */
gos_result_t gos_kernelTaskResume (gos_tid_t taskId);

/**
 * @brief   Sends the given task to blocked state.
 * @details Checks the given task ID and its state, modified it to blocked, and
 *          if there is a block hook function registered, it calls it. If the blocked
 *          function is the currently running one, it invokes a rescheduling.
 *
 * @param   taskId     : ID of the task to be blocked.
 *
 * @return  Result of task blocking.
 *
 * @retval  GOS_SUCESS : Task blocked successfully.
 * @retval  GOS_ERROR  : Task ID is invalid, or task state is not ready.
 */
gos_result_t gos_kernelTaskBlock (gos_tid_t taskId);

/**
 * @brief   Unblocks the given task.
 * @details Checks the given task ID and its state, modified it to ready, and
 *          if there is an unblock hook function registered, it calls it.
 *
 * @param   taskId     : ID of the task to be unblocked.
 *
 * @return  Result of task unblocking.
 *
 * @retval  GOS_SUCESS : Task unblocked successfully.
 * @retval  GOS_ERROR  : Task ID is invalid, or task is not blocked.
 */
gos_result_t gos_kernelTaskUnblock (gos_tid_t taskId);

/**
 * @brief   Deletes the given task from the scheduling array.
 * @details Checks the given task ID and its state, modifies it to zombie, and
 *          if there is a delete hook function registered, it calls it.
 *
 * @param   taskId      : ID of the task to be deleted.
 *
 * @return  Result of deletion.
 *
 * @retval  GOS_SUCCESS : Task deleted successfully.
 * @retval  GOS_ERROR   : Task is already a zombie.
 */
gos_result_t gos_kernelTaskDelete (gos_tid_t taskId);

/**
 * @brief   Sets the current priority of the given task to the given value (for temporary change).
 * @details Checks the given parameters and sets the current priority of the given task.
 *
 * @param   taskId       : ID of the task to change the priority of.
 * @param   taskPriority : The desired task priority.
 *
 * @return  Result of priority change.
 *
 * @retval  GOS_SUCCESS  : Current priority changed successfully.
 * @retval  GOS_ERROR    : Invalid task ID or priority.
 */
gos_result_t gos_kernelTaskSetPriority (gos_tid_t taskId, gos_taskPrio_t taskPriority);

/**
 * @brief   Sets the original priority of the given task to the given value (for permanent change).
 * @details Checks the given parameters and sets the original priority of the given task.
 *
 * @param   taskId       : ID of the task to change the priority of.
 * @param   taskPriority : The desired task priority.
 *
 * @return  Result of priority change.
 *
 * @retval  GOS_SUCCESS  : Original priority changed successfully.
 * @retval  GOS_ERROR    : Invalid task ID or priority.
 */
gos_result_t gos_kernelTaskSetOriginalPriority (gos_tid_t taskId, gos_taskPrio_t taskPriority);

/**
 * @brief   Gets the current priority of the given task.
 * @details Checks the given parameters and saves the current priority in the given variable.
 *
 * @param   taskId       : ID of the task to get the priority of.
 * @param   taskPriority : Pointer to a priority variable to store the priority in.
 *
 * @return  Result of priority getting.
 *
 * @retval  GOS_SUCCESS  : Current priority getting successfully.
 * @retval  GOS_ERROR    : Invalid task ID or priority variable is NULL.
 */
gos_result_t gos_kernelTaskGetPriority (gos_tid_t taskId, gos_taskPrio_t* taskPriority);

/**
 * @brief   Gets the original priority of the given task.
 * @details Checks the given parameters and saves the original priority in the given variable.
 *
 * @param   taskId       : ID of the task to get the priority of.
 * @param   taskPriority : Pointer to a priority variable to store the priority in.
 *
 * @return  Result of priority getting.
 *
 * @retval  GOS_SUCCESS  : Original priority getting successfully.
 * @retval  GOS_ERROR    : Invalid task ID or priority variable is NULL.
 */
gos_result_t gos_kernelTaskGetOriginalPriority (gos_tid_t taskId, gos_taskPrio_t* taskPriority);

/**
 * @brief   Adds the given privileges to the given task.
 * @details Checks the caller task if it has the privilege to modify task privileges and
 *          if so, it adds the given privileges to the given task.
 *
 * @param   taskId      : ID of the task to give the privileges to.
 * @param   privileges  : Privileges to be added.
 *
 * @return  Result of privilege adding.
 *
 * @retval  GOS_SUCCESS : Privileges added successfully.
 * @retval  GOS_ERROR   : Invalid task ID or caller does not have the privilege to modify task
 *                        privileges.
 */
gos_result_t gos_kernelTaskAddPrivilege (gos_tid_t taskId, gos_taskPrivilegeLevel_t privileges);

/**
 * @brief   Removes the given privileges from the given task.
 * @details Checks the caller task if it has the privilege to modify task privileges and
 *          if so, it removes the given privileges from the given task.
 *
 * @param   taskId      : ID of the task to remove the privileges from.
 * @param   privileges  : Privileges to be removed.
 *
 * @return  Result of privilege removing.
 *
 * @retval  GOS_SUCCESS : Privileges removed successfully.
 * @retval  GOS_ERROR   : Invalid task ID or caller does not have the privilege to modify task
 *                        privileges.
 */
gos_result_t gos_kernelTaskRemovePrivilege (gos_tid_t taskId, gos_taskPrivilegeLevel_t privileges);

/**
 * @brief   Gets the task name of the task with the given ID.
 * @details Copies the task name corresponding with the given task ID to the task name variable.
 *
 * @param   taskId      : Pointer to a task ID variable to store the returned ID.
 * @param   taskName    : Task name pointer to store the returned task name.
 *
 * @return  Success of task name get.
 *
 * @retval  GOS_SUCCESS : Task name found successfully.
 * @retval  GOS_ERROR   : Invalid task ID or task name variable is NULL.
 */
gos_result_t gos_kernelTaskGetName (gos_tid_t taskId, gos_taskName_t taskName);

/**
 * @brief   Gets the task ID of the task with the given name.
 * @details This function loops through the internal task array and tries to find the
 *          given task name to get the corresponding task ID.
 *
 * @param   taskName    : Name of the task (string).
 * @param   taskId      : Pointer to a task ID variable to store the returned ID.
 *
 * @return  Success of task ID get.
 *
 * @retval  GOS_SUCCESS : Task ID found successfully.
 * @retval  GOS_ERROR   : Task name not found.
 */
gos_result_t gos_kernelTaskGetId (gos_taskName_t taskName, gos_tid_t* taskId);

/**
 * @brief   Returns the ID of the currently running task.
 * @details Returns the ID of the currently running task.
 *
 * @param   taskId      : Pointer to a task ID variable to store the current task ID.
 *
 * @return  Result of current task ID get.
 *
 * @retval  GOS_SUCCESS : Current task ID returned successfully.
 * @retval  GOS_ERROR   : Task ID pointer is NULL.
 */
gos_result_t gos_kernelTaskGetCurrentId (gos_tid_t* taskId);

/**
 * @brief   Returns the task data of the given task.
 * @details Based on the task ID, it copies the content of the internal task
 *          descriptor array element to the given task descriptor.
 *
 * @param   taskId      : ID of the task to get the data of.
 * @param   taskData    : Pointer to the task descriptor to save the task data in.
 *
 * @return  Result of task data get.
 *
 * @retval  GOS_SUCCESS : Task data copied successfully.
 * @retval  GOS_ERROR   : Invalid task ID or task data pointer is NULL.
 */
gos_result_t gos_kernelTaskGetData (gos_tid_t taskId, gos_taskDescriptor_t* taskData);

/**
 * @brief   Yields the current task.
 * @details Invokes rescheduling.
 *
 * @return  Result of task yield.
 *
 * @retval  GOS_SUCCESS : Yield successful.
 */
gos_result_t gos_kernelTaskYield (void_t);

/**
 * @brief   Registers a task swap hook function.
 * @details Checks whether the param is NULL pointer and a hook function is already
 *          registered, and both conditions are false, it registers the hook function.
 *
 * @param   swapHookFunction : Swap hook function.
 *
 * @return  Result of registration.
 *
 * @retval  GOS_SUCCESS      : Registration successful.
 * @retval  GOS_ERROR        : Registration failed (hook function already exists or parameter is NULL).
 */
gos_result_t gos_kernelRegisterSwapHook (gos_taskSwapHook_t swapHookFunction);

/**
 * @brief   Registers an idle hook function.
 * @details Checks whether the param is NULL pointer and a hook function is already
 *          registered, and both conditions are false, it registers the hook function.
 *
 * @param   idleHookFunction : Idle hook function.
 *
 * @return  Result of registration.
 *
 * @retval  GOS_SUCCESS      : Registration successful.
 * @retval  GOS_ERROR        : Registration failed (hook function already exists or parameter is NULL).
 */
gos_result_t gos_kernelRegisterIdleHook (gos_taskIdleHook_t idleHookFunction);

/**
 * @brief   Subscribes the given handler to the task delete signal.
 * @details Subscribes the given handler to the task delete signal.
 *
 * @param   deleteSignalHandler : Delete signal handler function.
 *
 * @return  Result of subscription.
 *
 * @retval  GOS_SUCCESS         : Subscription successful.
 * @retval  GOS_ERROR           : Subscription failed or signal handler is NULL.
 */
gos_result_t gos_kernelSubscribeTaskDeleteSignal (void_t (*deleteSignalHandler)(u16_t));

/**
 * @brief   Subscribes the given handler to the dump ready signal.
 * @details Subscribes the given handler to the dump ready signal.
 *
 * @param   dumpReadySignalHandler : Dump ready signal handler function.
 *
 * @return  Result of subscription.
 *
 * @retval  GOS_SUCCESS            : Subscription successful.
 * @retval  GOS_ERROR              : Subscription failed or signal handler is NULL.
 */
gos_result_t gos_kernelSubscribeDumpReadySignal (void_t (*dumpReadySignalHandler)(u16_t));

/**
 * @brief    Invokes the kernel dump signal.
 * @details  Invokes the kernel dump signal.
 *
 * @return   Result of kernel dump.
 *
 * @retval   GOS_SUCCESS : Kernel dump signal invoking successful.
 * @retval   GOS_ERROR   : Signal invoking failed.
 */
gos_result_t gos_kernelDump (void_t);

/**
 * @brief   Returns the system ticks.
 * @details Returns the internal system tick counter value.
 *
 * @return  System ticks.
 */
u32_t gos_kernelGetSysTicks (void_t);

/**
 * @brief   Returns the total system run-time.
 * @details Returns the internal total system run-time variable.
 *
 * @return  Total run-time in microseconds.
 */
u64_t gos_kernelGetTotalSysTime (void_t);

/**
 * @brief   Starts the kernel.
 * @details Prepares the PSP for the first task, changes to unprivileged level,
 *          initializes the system timer value, and starts executing the first task.
 *
 * @return  Result of kernel start.
 *
 * @retval  GOS_ERROR : First task terminated.
 */
gos_result_t gos_kernelStart (void_t);

/**
 * @brief   Resets the microcontroller.
 * @details Sets the reset required flag and gets privileged access to reset
 *          the controller.
 *
 * @return    -
 */
void_t gos_kernelReset (void_t);

/**
 * @brief   Blocking delay in microsecond range.
 * @details This function waits in a while loop until the given number
 *          of milliseconds have elapsed based on the system timer.
 *
 * @param   microseconds    : Microseconds to wait.
 *
 * @return    -
 */
void_t gos_kernelDelayUs (u16_t microseconds);

/**
 * @brief   Blocking delay in millisecond range.
 * @details This function waits in a while loop until the given number
 *          of system ticks have elapsed.
 *
 * @param   milliseconds : Milliseconds to wait.
 *
 * @return    -
 */
void_t gos_kernelDelayMs (u16_t milliseconds);

/**
 * @brief   Platform driver initializer. Used for the platform-specific driver initializations.
 * @details This function is weak and therefore should be over-defined by the user. It prints
 *          a warning message to the log output in case it is not over-defined.
 *
 * @return  -
 *
 * @retval  GOS_ERROR : -
 */
__attribute__((weak)) gos_result_t gos_platformDriverInit (void_t);

/**
 * @brief   User application initializer. Used for the application-related initializations.
 * @details This function is weak and therefore should be over-defined by the user. It prints
 *          a warning message to the log output in case it is not over-defined.
 *
 * @return  -
 *
 * @retval  GOS_ERROR : -
 */
__attribute__((weak)) gos_result_t gos_userApplicationInit (void_t);
#endif
