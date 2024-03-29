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
//                                      (c) Ahmed Gazar, 2022
//
//*************************************************************************************************
//! @file       gos_kernel.c
//! @author     Ahmed Gazar
//! @date       2023-09-25
//! @version    1.14
//!
//! @brief      GOS kernel source.
//! @details    For a more detailed description of this module, please refer to @ref gos_kernel.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2022-10-21    Ahmed Gazar     Initial version created.
// 1.1        2022-11-15    Ahmed Gazar     +    License added
// 1.2        2022-12-03    Ahmed Gazar     +    gos_kernelTaskRegisterTasks added
//                                          +    Tasks external task ID handling added
//                                          +    Kernel configuration structure array added
//                                          +    Cooperative scheduling introduced
// 1.3        2022-12-04    Ahmed Gazar     *    Sleep, wake-up, suspend, resume, block, unblock
//                                               hook functions removed
//                                          +    Kernel dump ready signal added
// 1.4        2022-12-08    Ahmed Gazar     +    Task priority setter and getter functions added
// 1.5        2022-12-15    Ahmed Gazar     +    Privilege handling introduced, privilege check
//                                               added to kernel functions
// 1.6        2023-01-13    Ahmed Gazar     *    Scheduling disabled flag replaced with counter
//                                          -    Configuration dump removed
// 1.7        2023-05-03    Ahmed Gazar     +    Stack overflow monitoring added
//                                          *    Dump separated to task and stack statistics
// 1.8        2023-05-19    Ahmed Gazar     +    gos_kernelGetCpuUsage added
// 1.9        2023-06-17    Ahmed Gazar     *    Kernel dump moved from task to function
//                                          *    CPU statistics fix
// 1.10       2023-06-30    Ahmed Gazar     +    cpuUseLimit added, CPU usage limit implemented
//                                          *    CPU statistics calculation moved to idle task
//                                          *    gos_kernelCalculateTaskCpuUsages reset required
//                                               flag added
//                                          +    gos_kernelSetMaxCpuLoad added
//                                          +    gos_kernelGetMaxCpuLoad added
//                                          +    gos_kernelIsCallerIsr added
//                                          +    gos_kernelIsCallerPrivileged added
//                                          +    gos_kernelTaskSetPrivileges  added
// 1.11       2023-07-11    Ahmed Gazar     -    privilegedAccess removed
//                                          +    kernelSysTickHookFunction added
//                                          -    Privileged access concept removed
//                                          +    Task block timeout introduced
// 1.12       2023-07-25    Ahmed Gazar     +    gos_kernelTaskGetDataByIndex added
//                                          *    Ported function calls added
// 1.13       2023-09-08    Ahmed Gazar     *    Sleep and block tick handling modified
// 1.14       2023-09-25    Ahmed Gazar     +    Missing else branches added
//*************************************************************************************************
//
// Copyright (c) 2022 Ahmed Gazar
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
/*
 * Includes
 */
#include <gos_error.h>
#include <gos_kernel.h>
#include <gos_port.h>
#include <gos_process.h>
#include <gos_signal.h>
#include <gos_shell_driver.h>
#include <gos_timer_driver.h>
#include <gos_trace.h>
#include <stdio.h>
#include <string.h>

/*
 * Macros
 */
/**
 * System Handler control and state register.
 */
#define SHCSR                   *( (volatile u32_t*) 0xE000ED24u )

/**
 * Interrupt Control and State Register.
 */
#define ICSR                    *( (volatile u32_t*) 0xE000ED04u )

/**
 * RAM start address.
 */
#define RAM_START               ( 0x20000000u )

/**
 * RAM size (128kB).
 */
#define RAM_SIZE                ( 128 * 1024 )

/**
 * Main stack.
 */
#define MAIN_STACK              ( RAM_START + RAM_SIZE )

/**
 * Global stack.
 */
#define GLOBAL_STACK            ( 0x1200 )

/**
 * Task dump separator line.
 */
#define TASK_DUMP_SEPARATOR     "+--------+------------------------------+------+------------------+---------+-----------+\r\n"

/**
 * Max CPU loads dump separator line.
 */
#define MAX_CPU_DUMP_SEPARATOR  "+--------+------------------------------+-------------+\r\n"

/**
 * Stack statistics separator line.
 */
#define STACK_STATS_SEPARATOR   "+--------+------------------------------+--------+----------------+-----------+\r\n"

/**
 * Config dump separator line.
 */
#define CONFIG_DUMP_SEPARATOR   "+----------------------------------+--------+\r\n"

/**
 * Pattern for binary format printing.
 */
#define BINARY_PATTERN          "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s"TRACE_FORMAT_RESET

/**
 * u16_t to binary converter macro.
 */
#define TO_BINARY(word)  \
    (word & 0x8000 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x4000 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x2000 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x1000 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x0800 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x0400 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x0200 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x0100 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x0080 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x0040 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x0020 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x0010 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x0008 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x0004 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x0002 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0"), \
    (word & 0x0001 ? TRACE_FG_GREEN_START"1" : TRACE_FG_RED_START"0")

/*
 * Type definitions
 */
/**
 * Kernel privilege levels.
 */
typedef enum
{
    GOS_PRIVILEGED      = 0b10110,   //!< GOS_PRIVILEDGED
    GOS_UNPRIVILEGED    = 0b01001    //!< GOS_UNPRIVILEDGED
}gos_kernel_privilege_t;

/*
 * Global variables
 */
/**
 * Kernel dump signal.
 */
gos_signalId_t                      kernelDumpSignal;

/**
 * Kernel dump ready signal.
 */
gos_signalId_t                      kernelDumpReadySignal;

/**
 * Kernel task delete signal.
 */
gos_signalId_t                      kernelTaskDeleteSignal;

/**
 * Scheduling disabled counter.
 */
u8_t                                schedDisableCntr            = 0u;

/**
 * In ISR counter.
 */
u8_t                                inIsr                       = 0u;

/**
 * CPU use limit.
 */
u16_t                               cpuUseLimit                 = 10000;

/*
 * Static variables
 */
/**
 * System tick value.
 */
GOS_STATIC u32_t                    sysTicks               = 0u;

/**
 * Current task index - for priority scheduling.
 */
GOS_STATIC u32_t                    currentTaskIndex       = 0u;

/**
 * System timer value for run-time calculations.
 */
GOS_STATIC u16_t                    sysTimerValue          = 0u;

/**
 * Monitoring system time since last statistics calculation.
 */
GOS_STATIC gos_runtime_t            monitoringTime         = {0};

/**
 * Kernel idle hook function.
 */
GOS_STATIC gos_taskIdleHook_t       kernelIdleHookFunction = NULL;

/**
 * Kernel swap hook function.
 */
GOS_STATIC gos_taskSwapHook_t       kernelSwapHookFunction = NULL;

/**
 * Kernel system tick hook function.
 */
GOS_STATIC gos_sysTickHook_t        kernelSysTickHookFunction = NULL;

/**
 * Reset required flag.
 */
GOS_STATIC bool_t                   resetRequired          = GOS_FALSE;

/**
 * Previous tick value for sleep and block tick calculation.
 */
GOS_STATIC u32_t                    previousTick           = 0u;

/*
 * Function prototypes
 */
GOS_STATIC        void_t        gos_kernelCheckTaskStack         (void_t);
GOS_STATIC        gos_result_t  gos_kernelCheckTaskDescriptor    (gos_taskDescriptor_t* taskDescriptor);
GOS_STATIC        u32_t         gos_kernelGetCurrentPsp          (void_t);
GOS_STATIC        void_t        gos_kernelSaveCurrentPsp         (u32_t psp);
GOS_STATIC        void_t        gos_kernelSelectNextTask         (void_t);
GOS_STATIC        void_t        gos_kernelIdleTask               (void_t);
GOS_STATIC_INLINE void_t        gos_kernelReschedule             (gos_kernel_privilege_t privilege);
GOS_STATIC        char_t*       gos_kernelGetTaskStateString     (gos_taskState_t taskState);
GOS_STATIC        void_t        gos_kernelProcessorReset         (void_t);

/**
 * Internal task array.
 */
GOS_STATIC gos_taskDescriptor_t taskDescriptors [CFG_TASK_MAX_NUMBER] =
{
    [0] =
        {
            .taskFunction       = gos_kernelIdleTask,
            .taskId             = GOS_DEFAULT_TASK_ID,
            .taskPriority       = GOS_TASK_IDLE_PRIO,
            .taskName           = "gos_idle_task",
            .taskState          = GOS_TASK_READY,
            .taskStackSize      = CFG_IDLE_TASK_STACK_SIZE,
            .taskPrivilegeLevel = GOS_TASK_PRIVILEGE_KERNEL,
            .taskCpuUsageLimit  = 10000
        }
};

/*
 * Function: gos_kernelInit
 */
gos_result_t gos_kernelInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t  initResult = GOS_ERROR;
    u16_t         taskIndex  = 1u;

    /*
     * Function code.
     */
    // Initialize task descriptors.
    for (taskIndex = 1u; taskIndex < CFG_TASK_MAX_NUMBER; taskIndex++)
    {
        taskDescriptors[taskIndex].taskFunction         = NULL;
        taskDescriptors[taskIndex].taskPriority         = GOS_TASK_MAX_PRIO_LEVELS;
        taskDescriptors[taskIndex].taskOriginalPriority = GOS_TASK_MAX_PRIO_LEVELS;
        taskDescriptors[taskIndex].taskState            = GOS_TASK_SUSPENDED;
        taskDescriptors[taskIndex].taskId               = GOS_INVALID_TASK_ID;
    }

    // Register idle task PSP.
    u32_t* psp = (u32_t*)(MAIN_STACK - GLOBAL_STACK);

    // Fill dummy stack frame.
    *(--psp) = 0x01000000u; // Dummy xPSR, just enable Thumb State bit;
    *(--psp) = (u32_t) gos_kernelIdleTask; // PC
    *(--psp) = 0xFFFFFFFDu; // LR with EXC_RETURN to return to Thread using PSP
    *(--psp) = 0x12121212u; // Dummy R12
    *(--psp) = 0x03030303u; // Dummy R3
    *(--psp) = 0x02020202u; // Dummy R2
    *(--psp) = 0x01010101u; // Dummy R1
    *(--psp) = 0x00000000u; // Dummy R0
    *(--psp) = 0x11111111u; // Dummy R11
    *(--psp) = 0x10101010u; // Dummy R10
    *(--psp) = 0x09090909u; // Dummy R9
    *(--psp) = 0x08080808u; // Dummy R8
    *(--psp) = 0x07070707u; // Dummy R7
    *(--psp) = 0x06060606u; // Dummy R6
    *(--psp) = 0x05050505u; // Dummy R5
    *(--psp) = 0x04040404u; // Dummy R4

    // Save PSP.
    taskDescriptors[0].taskPsp = (u32_t)psp;

    // Calculate stack overflow threshold.
    taskDescriptors[0].taskStackOverflowThreshold = taskDescriptors[0].taskPsp - taskDescriptors[0].taskStackSize + 64;

    // Enable Fault Handlers
    gos_ported_enableFaultHandlers();

    schedDisableCntr = 0u;

    initResult = GOS_SUCCESS;

    return initResult;
}

/*
 * Function: gos_kernelStart
 */
gos_result_t gos_kernelStart (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t kernelStartResult = GOS_ERROR;

    /*
     * Function code.
     */
    // Start with the first task.
    currentTaskIndex = 0;

    // Do low-level initialization.
    gos_ported_kernelStartInit();

    // Get the handler of the first task by tracing back from PSP which is at R4 slot.
    gos_task_t firstTask = taskDescriptors[currentTaskIndex].taskFunction;

    // Initialize system timer value.
    gos_timerDriverSysTimerGet(&sysTimerValue);

    // Enable scheduling.
    GOS_ENABLE_SCHED

    // Execute the handler.
    firstTask();

    // This point should not be reached.
    return kernelStartResult;
}

/*
 * Function: gos_kernelTaskRegisterTasks
 */
gos_result_t gos_kernelTaskRegisterTasks (gos_taskDescriptor_t* taskDescriptors, u16_t arraySize)
{
    /*
     * Local variables.
     */
    gos_result_t registerResult = GOS_ERROR;
    u16_t        numberOfTasks  = 0u;
    u16_t        index          = 0u;

    /*
     * Function code.
     */
    if (taskDescriptors != NULL)
    {
        numberOfTasks = arraySize / sizeof(taskDescriptors[0]);

        for (index = 0u; index < numberOfTasks; index++)
        {
            if (gos_kernelTaskRegister(&taskDescriptors[index], NULL) != GOS_SUCCESS)
            {
                break;
            }
        }

        if (index == numberOfTasks)
        {
            registerResult = GOS_SUCCESS;
        }
    }

    return registerResult;
}

/*
 * Function: gos_kernelInit
 */
gos_result_t gos_kernelTaskRegister (gos_taskDescriptor_t* taskDescriptor, gos_tid_t* taskId)
{
    /*
     * Local variables.
     */
    gos_result_t taskRegisterResult = GOS_SUCCESS;
    u16_t        taskIndex          = 0u;
    u32_t        taskStackOffset    = GLOBAL_STACK;
    u32_t*       psp                = NULL;

    /*
     * Function code.
     */
    // Check descriptor structure.
    if (gos_kernelCheckTaskDescriptor(taskDescriptor) == GOS_ERROR)
    {
        taskRegisterResult = GOS_ERROR;
    }
    else
    {
        // Find empty slot.
        for (taskIndex = 0u; taskIndex < CFG_TASK_MAX_NUMBER; taskIndex++)
        {
            if (taskDescriptors[taskIndex].taskFunction == NULL)
            {
                break;
            }
            taskStackOffset += taskDescriptors[taskIndex].taskStackSize;
        }
        // Check if empty slot was found.
        if (taskIndex >= CFG_TASK_MAX_NUMBER)
        {
            taskRegisterResult = GOS_ERROR;
        }
        else
        {
            // Calculate new PSP.
            psp = (u32_t*)(MAIN_STACK - taskStackOffset);

            // Fill dummy stack frame.
            *(--psp) = 0x01000000u; // Dummy xPSR, just enable Thumb State bit;
            *(--psp) = (u32_t)taskDescriptor->taskFunction; // PC
            *(--psp) = 0xFFFFFFFDu; // LR with EXC_RETURN to return to Thread using PSP
            *(--psp) = 0x12121212u; // Dummy R12
            *(--psp) = 0x03030303u; // Dummy R3
            *(--psp) = 0x02020202u; // Dummy R2
            *(--psp) = 0x01010101u; // Dummy R1
            *(--psp) = 0x00000000u; // Dummy R0
            *(--psp) = 0x11111111u; // Dummy R11
            *(--psp) = 0x10101010u; // Dummy R10
            *(--psp) = 0x09090909u; // Dummy R9
            *(--psp) = 0x08080808u; // Dummy R8
            *(--psp) = 0x07070707u; // Dummy R7
            *(--psp) = 0x06060606u; // Dummy R6
            *(--psp) = 0x05050505u; // Dummy R5
            *(--psp) = 0x04040404u; // Dummy R4

            // Save PSP.
            taskDescriptors[taskIndex].taskPsp      = (u32_t)psp;

            // Initial state.
            taskDescriptors[taskIndex].taskState            = GOS_TASK_READY;
            taskDescriptors[taskIndex].taskFunction         = taskDescriptor->taskFunction;
            taskDescriptors[taskIndex].taskPriority         = taskDescriptor->taskPriority;
            taskDescriptors[taskIndex].taskOriginalPriority = taskDescriptor->taskPriority;
            taskDescriptors[taskIndex].taskStackSize        = taskDescriptor->taskStackSize;
            taskDescriptors[taskIndex].taskId               = (GOS_DEFAULT_TASK_ID + taskIndex);
            taskDescriptors[taskIndex].taskPrivilegeLevel   = taskDescriptor->taskPrivilegeLevel;

            if (taskDescriptor->taskCpuUsageLimit == 0u)
            {
                taskDescriptors[taskIndex].taskCpuUsageLimit = 10000u;
            }
            else
            {
                taskDescriptors[taskIndex].taskCpuUsageLimit = taskDescriptor->taskCpuUsageLimit;
            }

            // Copy task name.
            if (taskDescriptor->taskName != NULL &&
                strlen(taskDescriptor->taskName) <= CFG_TASK_MAX_NAME_LENGTH)
            {
                strcpy(taskDescriptors[taskIndex].taskName, taskDescriptor->taskName);
            }

            // Set task ID.
            taskDescriptor->taskId = taskDescriptors[taskIndex].taskId;
            if (taskId != NULL)
            {
                *taskId = taskDescriptors[taskIndex].taskId;
            }
            if (taskDescriptor->taskIdEx != NULL)
            {
                *taskDescriptor->taskIdEx = taskDescriptors[taskIndex].taskId;
            }

            // Calculate stack overflow threshold value (64 byte reserved for protection).
            taskDescriptors[taskIndex].taskStackOverflowThreshold =
                    taskDescriptors[taskIndex].taskPsp - taskDescriptors[taskIndex].taskStackSize + 64;
        }
    }

    return taskRegisterResult;
}

/*
 * Function: gos_kernelTaskSleep
 */
GOS_INLINE gos_result_t gos_kernelTaskSleep (gos_taskSleepTick_t sleepTicks)
{
    /*
     * Local variables.
     */
    gos_result_t taskSleepResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (currentTaskIndex > 0u)
    {
        GOS_ATOMIC_ENTER
        {
            if (taskDescriptors[currentTaskIndex].taskState == GOS_TASK_READY)
            {
                taskDescriptors[currentTaskIndex].taskState            = GOS_TASK_SLEEPING;
                taskDescriptors[currentTaskIndex].taskSleepTicks       = sleepTicks;
                taskDescriptors[currentTaskIndex].taskSleepTickCounter = 0u;
                taskSleepResult = GOS_SUCCESS;
            }
        }
        GOS_ATOMIC_EXIT

        if (taskSleepResult == GOS_SUCCESS)
        {
            // Unprivileged.
            gos_kernelReschedule(GOS_UNPRIVILEGED);
        }
    }
    return taskSleepResult;
}

/*
 * Function: gos_kernelTaskWakeup
 */
GOS_INLINE gos_result_t gos_kernelTaskWakeup (gos_tid_t taskId)
{
    /*
     * Local variables.
     */
    gos_result_t taskWakeupResult = GOS_ERROR;
    u32_t        taskIndex        = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

        // Check task manipulation privilege.
        if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_MANIPULATE) == GOS_PRIV_TASK_MANIPULATE ||
            inIsr > 0)
        {
            if (taskDescriptors[taskIndex].taskState == GOS_TASK_SLEEPING)
            {
                taskDescriptors[taskIndex].taskState = GOS_TASK_READY;
                taskWakeupResult = GOS_SUCCESS;
            }
        }
        else
        {
            GOS_ATOMIC_EXIT
            gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to wake up <%s>!",
                taskDescriptors[currentTaskIndex].taskName,
                taskDescriptors[taskIndex].taskName
            );
        }
    }

    GOS_ATOMIC_EXIT

    return taskWakeupResult;
}

/*
 * Function: gos_kernelTaskSuspend
 */
GOS_INLINE gos_result_t gos_kernelTaskSuspend (gos_tid_t taskId)
{
    /*
     * Local variables.
     */
    gos_result_t taskSuspendResult = GOS_ERROR;
    u32_t        taskIndex         = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

        // Check task manipulation privilege.
        if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_MANIPULATE) == GOS_PRIV_TASK_MANIPULATE ||
            currentTaskIndex == taskIndex || inIsr > 0)
        {
            if (taskDescriptors[taskIndex].taskState == GOS_TASK_READY ||
                taskDescriptors[taskIndex].taskState == GOS_TASK_SLEEPING ||
                taskDescriptors[taskIndex].taskState == GOS_TASK_BLOCKED)
            {
                taskDescriptors[taskIndex].taskPreviousState = taskDescriptors[taskIndex].taskState;
                taskDescriptors[taskIndex].taskState = GOS_TASK_SUSPENDED;
                taskSuspendResult = GOS_SUCCESS;

                GOS_ATOMIC_EXIT

                if (currentTaskIndex == taskIndex)
                {
                    // Unprivileged.
                    gos_kernelReschedule(GOS_UNPRIVILEGED);
                }
            }
        }
        else
        {
            GOS_ATOMIC_EXIT
            gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to suspend <%s>!",
                taskDescriptors[currentTaskIndex].taskName,
                taskDescriptors[taskIndex].taskName
            );
        }
    }

    if (taskSuspendResult != GOS_SUCCESS)
    {
        GOS_ATOMIC_EXIT
    }

    return taskSuspendResult;
}

/*
 * Function: gos_kernelTaskResume
 */
GOS_INLINE gos_result_t gos_kernelTaskResume (gos_tid_t taskId)
{
    /*
     * Local variables.
     */
    gos_result_t taskResumeResult = GOS_ERROR;
    u32_t        taskIndex        = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

        // Check task manipulation privilege.
        if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_MANIPULATE) == GOS_PRIV_TASK_MANIPULATE ||
            inIsr > 0)
        {
            if (taskDescriptors[taskIndex].taskState == GOS_TASK_SUSPENDED)
            {
                taskDescriptors[taskIndex].taskState = taskDescriptors[taskIndex].taskPreviousState;
                taskResumeResult = GOS_SUCCESS;
            }
        }
        else
        {
            gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to resume <%s>!",
                taskDescriptors[currentTaskIndex].taskName,
                taskDescriptors[taskIndex].taskName
            );
        }
    }

    GOS_ATOMIC_EXIT

    return taskResumeResult;
}

/*
 * Function: gos_kernelTaskBlock
 */
GOS_INLINE gos_result_t gos_kernelTaskBlock (gos_tid_t taskId, gos_blockMaxTick_t blockTicks)
{
    /*
     * Local variables.
     */
    gos_result_t taskBlockResult = GOS_ERROR;
    u32_t        taskIndex       = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

        if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_MANIPULATE) == GOS_PRIV_TASK_MANIPULATE ||
            currentTaskIndex == taskIndex || inIsr > 0)
        {
            if (taskDescriptors[taskIndex].taskState == GOS_TASK_READY)
            {
                taskDescriptors[taskIndex].taskState            = GOS_TASK_BLOCKED;
                taskDescriptors[taskIndex].taskBlockTicks       = blockTicks;
                taskDescriptors[taskIndex].taskBlockTickCounter = 0u;

                taskBlockResult = GOS_SUCCESS;

                GOS_ATOMIC_EXIT

                if (currentTaskIndex == taskIndex)
                {
                    // Unprivileged.
                    gos_kernelReschedule(GOS_UNPRIVILEGED);
                }
            }
            else
            {
                GOS_NOP;
            }
        }
        else
        {
            GOS_ATOMIC_EXIT

            gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to block <%s>!",
                taskDescriptors[currentTaskIndex].taskName,
                taskDescriptors[taskIndex].taskName
            );
        }
    }
    if (taskBlockResult != GOS_SUCCESS)
    {
        GOS_ATOMIC_EXIT
    }

    return taskBlockResult;
}

/*
 * Function: gos_kernelTaskUnblock
 */
GOS_INLINE gos_result_t gos_kernelTaskUnblock (gos_tid_t taskId)
{
    /*
     * Local variables.
     */
    gos_result_t taskUnblockResult = GOS_ERROR;
    u32_t        taskIndex         = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

        if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_MANIPULATE) == GOS_PRIV_TASK_MANIPULATE ||
            inIsr > 0)
        {
            if (taskDescriptors[taskIndex].taskState == GOS_TASK_BLOCKED)
            {
                taskDescriptors[taskIndex].taskState = GOS_TASK_READY;
                taskUnblockResult = GOS_SUCCESS;
            }
            else if (taskDescriptors[taskIndex].taskState == GOS_TASK_SUSPENDED &&
                    taskDescriptors[taskIndex].taskPreviousState == GOS_TASK_BLOCKED)
            {
                taskDescriptors[taskIndex].taskPreviousState = GOS_TASK_READY;
                taskUnblockResult = GOS_SUCCESS;
            }
        }
        else
        {
            GOS_ATOMIC_EXIT

            gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to unblock <%s>!",
                taskDescriptors[currentTaskIndex].taskName,
                taskDescriptors[taskIndex].taskName
            );
        }
    }

    GOS_ATOMIC_EXIT

    return taskUnblockResult;
}

/*
 * Function: gos_kernelTaskDelete
 */
GOS_INLINE gos_result_t gos_kernelTaskDelete (gos_tid_t taskId)
{
    /*
     * Local variables.
     */
    gos_result_t taskDeleteResult = GOS_ERROR;
    u32_t        taskIndex         = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

        // Check task manipulation privilege.
        if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_MANIPULATE) == GOS_PRIV_TASK_MANIPULATE ||
            currentTaskIndex == taskIndex || inIsr > 0)
        {
            if (taskDescriptors[taskIndex].taskState != GOS_TASK_ZOMBIE)
            {
                taskDescriptors[taskIndex].taskState = GOS_TASK_ZOMBIE;
                taskDeleteResult = GOS_SUCCESS;

                // Invoke signal.
                if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_SIGNALING) != GOS_PRIV_SIGNALING)
                {
                    taskDescriptors[currentTaskIndex].taskPrivilegeLevel |= GOS_PRIV_SIGNALING;
                    (void_t) gos_signalInvoke(kernelTaskDeleteSignal, taskId);
                    taskDescriptors[currentTaskIndex].taskPrivilegeLevel &= ~GOS_PRIV_SIGNALING;
                }
                else
                {
                    (void_t) gos_signalInvoke(kernelTaskDeleteSignal, taskId);
                }
            }
        }
        else
        {
            GOS_ATOMIC_EXIT

            gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to delete <%s>!",
                taskDescriptors[currentTaskIndex].taskName,
                taskDescriptors[taskIndex].taskName
            );
        }
    }

    GOS_ATOMIC_EXIT

    if (taskDeleteResult == GOS_SUCCESS)
    {
        if (currentTaskIndex == taskIndex)
        {
            gos_kernelReschedule(GOS_UNPRIVILEGED);
        }
    }

    return taskDeleteResult;
}

/*
 * Function: gos_kernelTaskSetPriority
 */
GOS_INLINE gos_result_t gos_kernelTaskSetPriority (gos_tid_t taskId, gos_taskPrio_t taskPriority)
{
    /*
     * Local variables.
     */
    gos_result_t taskSetPriorityResult = GOS_ERROR;
    u32_t        taskIndex             = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER &&
        taskPriority < GOS_TASK_MAX_PRIO_LEVELS)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

        // Check privilege level.
        if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_PRIO_CHANGE) == GOS_PRIV_TASK_PRIO_CHANGE ||
            inIsr > 0)
        {
            taskDescriptors[taskIndex].taskPriority = taskPriority;
            taskSetPriorityResult = GOS_SUCCESS;
        }
        else
        {
            GOS_ATOMIC_EXIT

            gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to set the priority of <%s>!",
                taskDescriptors[currentTaskIndex].taskName,
                taskDescriptors[taskIndex].taskName
            );
        }
    }

    GOS_ATOMIC_EXIT

    return taskSetPriorityResult;
}

/*
 * Function: gos_kernelTaskSetOriginalPriority
 */
GOS_INLINE gos_result_t gos_kernelTaskSetOriginalPriority (gos_tid_t taskId, gos_taskPrio_t taskPriority)
{
    /*
     * Local variables.
     */
    gos_result_t taskSetPriorityResult = GOS_ERROR;
    u32_t        taskIndex             = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER &&
        taskPriority < GOS_TASK_MAX_PRIO_LEVELS)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

        // Check privilege level.
        if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_PRIO_CHANGE) == GOS_PRIV_TASK_PRIO_CHANGE ||
            inIsr > 0)
        {
            taskDescriptors[taskIndex].taskOriginalPriority = taskPriority;
            taskSetPriorityResult = GOS_SUCCESS;
        }
        else
        {
            GOS_ATOMIC_EXIT

            gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to set the priority of <%s>!",
                taskDescriptors[currentTaskIndex].taskName,
                taskDescriptors[taskIndex].taskName
            );
        }
    }

    GOS_ATOMIC_EXIT

    return taskSetPriorityResult;
}

/*
 * Function: gos_kernelTaskGetPriority
 */
gos_result_t gos_kernelTaskGetPriority (gos_tid_t taskId, gos_taskPrio_t* taskPriority)
{
    /*
     * Local variables.
     */
    gos_result_t taskGetPriorityResult = GOS_ERROR;
    u32_t        taskIndex             = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER &&
        taskPriority != NULL)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);
        *taskPriority = taskDescriptors[taskIndex].taskPriority;
        taskGetPriorityResult = GOS_SUCCESS;
    }
    GOS_ATOMIC_EXIT

    return taskGetPriorityResult;
}

/*
 * Function: gos_kernelTaskGetOriginalPriority
 */
gos_result_t gos_kernelTaskGetOriginalPriority (gos_tid_t taskId, gos_taskPrio_t* taskPriority)
{
    /*
     * Local variables.
     */
    gos_result_t taskGetPriorityResult = GOS_ERROR;
    u32_t        taskIndex             = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER &&
        taskPriority != NULL)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);
        *taskPriority = taskDescriptors[taskIndex].taskOriginalPriority;
        taskGetPriorityResult = GOS_SUCCESS;
    }
    GOS_ATOMIC_EXIT

    return taskGetPriorityResult;
}

/*
 * Function: gos_kernelTaskAddPrivilege
 */
GOS_INLINE gos_result_t gos_kernelTaskAddPrivilege (gos_tid_t taskId, gos_taskPrivilegeLevel_t privileges)
{
    /*
     * Local variables.
     */
    gos_result_t taskAddPrivilegeResult = GOS_ERROR;
    u32_t        taskIndex              = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);
        taskDescriptors[taskIndex].taskPrivilegeLevel |= privileges;
        taskAddPrivilegeResult = GOS_SUCCESS;
    }
    GOS_ATOMIC_EXIT

    return taskAddPrivilegeResult;
}

/*
 * Function: gos_kernelTaskRemovePrivilege
 */
GOS_INLINE gos_result_t gos_kernelTaskRemovePrivilege (gos_tid_t taskId, gos_taskPrivilegeLevel_t privileges)
{
    /*
     * Local variables.
     */
    gos_result_t taskRemovePrivilegeResult = GOS_ERROR;
    u32_t        taskIndex                 = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);
        taskDescriptors[taskIndex].taskPrivilegeLevel &= ~privileges;
        taskRemovePrivilegeResult = GOS_SUCCESS;
    }
    GOS_ATOMIC_EXIT

    return taskRemovePrivilegeResult;
}

/*
 * Function: gos_kernelTaskSetPrivileges
 */
GOS_INLINE gos_result_t gos_kernelTaskSetPrivileges (gos_tid_t taskId, gos_taskPrivilegeLevel_t privileges)
{
    /*
     * Local variables.
     */
    gos_result_t taskSetPrivilegesResult = GOS_ERROR;
    u32_t        taskIndex               = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);
        taskDescriptors[taskIndex].taskPrivilegeLevel = privileges;
        taskSetPrivilegesResult = GOS_SUCCESS;
    }
    GOS_ATOMIC_EXIT

    return taskSetPrivilegesResult;
}

/*
 * Function: gos_kernelTaskGetPrivileges
 */
GOS_INLINE gos_result_t gos_kernelTaskGetPrivileges (gos_tid_t taskId, gos_taskPrivilegeLevel_t* privileges)
{
    /*
     * Local variables.
     */
    gos_result_t taskGetPrivilegesResult = GOS_ERROR;
    u32_t        taskIndex               = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER &&
        privileges != NULL)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

        *privileges = taskDescriptors[taskIndex].taskPrivilegeLevel;
        taskGetPrivilegesResult = GOS_SUCCESS;
    }
    GOS_ATOMIC_EXIT

    return taskGetPrivilegesResult;
}

/*
 * Function: gos_kernelTaskYield
 */
GOS_INLINE gos_result_t gos_kernelTaskYield (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t taskYieldResult = GOS_SUCCESS;

    /*
     * Function code.
     */
    gos_kernelReschedule(GOS_UNPRIVILEGED);
    return taskYieldResult;
}

/*
 * Function: gos_kernelTaskGetName
 */
gos_result_t gos_kernelTaskGetName (gos_tid_t taskId, gos_taskName_t taskName)
{
    /*
     * Local variables.
     */
    gos_result_t taskGetNameResult = GOS_ERROR;
    u32_t        taskIndex         = 0u;

    /*
     * Function code.
     */
    if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER &&
        taskName != NULL)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

        strcpy(taskName, taskDescriptors[taskIndex].taskName);

        taskGetNameResult = GOS_SUCCESS;
    }

    return taskGetNameResult;
}

/*
 * Function: gos_kernelTaskGetId
 */
gos_result_t gos_kernelTaskGetId (gos_taskName_t taskName, gos_tid_t* taskId)
{
    /*
     * Local variables.
     */
    gos_result_t taskGetIdResult = GOS_ERROR;
    u32_t        taskIndex       = 0u;

    /*
     * Function code.
     */
    for (taskIndex = 0u; taskIndex < CFG_TASK_MAX_NUMBER && taskId != NULL; taskIndex++)
    {
        if (strcmp(taskName, taskDescriptors[taskIndex].taskName) == 0u)
        {
            *taskId = taskDescriptors[taskIndex].taskId;
            taskGetIdResult = GOS_SUCCESS;
            break;
        }
    }

    return taskGetIdResult;
}

/*
 * Function: gos_kernelTaskGetCurrentId
 */
GOS_INLINE gos_result_t gos_kernelTaskGetCurrentId (gos_tid_t* taskId)
{
    /*
     * Local variables.
     */
    gos_result_t taskGetCurrentIdResult = GOS_ERROR;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId != NULL)
    {
        *taskId = (gos_tid_t)(GOS_DEFAULT_TASK_ID + currentTaskIndex);
        taskGetCurrentIdResult = GOS_SUCCESS;
    }
    GOS_ATOMIC_EXIT

    return taskGetCurrentIdResult;
}

/*
 * Function: gos_kernelTaskGetData
 */
gos_result_t gos_kernelTaskGetData (gos_tid_t taskId, gos_taskDescriptor_t* taskData)
{
    /*
     * Local variables.
     */
    gos_result_t taskGetDataResult = GOS_ERROR;
    u32_t        taskIndex         = 0u;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskId >= GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER &&
        taskData != NULL)
    {
        taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

        memcpy((void*)taskData, (void*)&taskDescriptors[taskIndex], sizeof(*taskData));

        taskGetDataResult = GOS_SUCCESS;
    }
    GOS_ATOMIC_EXIT

    return taskGetDataResult;
}

/*
 * Function: gos_kernelTaskGetDataByIndex
 */
gos_result_t gos_kernelTaskGetDataByIndex (u16_t taskIndex, gos_taskDescriptor_t* taskData)
{
    /*
     * Local variables.
     */
    gos_result_t taskGetDataResult = GOS_ERROR;

    /*
     * Function code.
     */
    GOS_ATOMIC_ENTER
    if (taskIndex < CFG_TASK_MAX_NUMBER &&
        taskData != NULL &&
        (taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_TASK_PRIVILEGE_KERNEL) == GOS_TASK_PRIVILEGE_KERNEL)
    {
        memcpy((void*)taskData, (void*)&taskDescriptors[taskIndex], sizeof(*taskData));

        taskGetDataResult = GOS_SUCCESS;
    }
    GOS_ATOMIC_EXIT

    return taskGetDataResult;
}

/*
 * Function: gos_kernelRegisterSwapHook
 */
gos_result_t gos_kernelRegisterSwapHook (gos_taskSwapHook_t swapHookFunction)
{
    /*
     * Local variables.
     */
    gos_result_t hookRegisterResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (swapHookFunction != NULL && kernelSwapHookFunction == NULL)
    {
        kernelSwapHookFunction = swapHookFunction;
        hookRegisterResult = GOS_SUCCESS;
    }

    return hookRegisterResult;
}

/*
 * Function: gos_kernelRegisterIdleHook
 */
gos_result_t gos_kernelRegisterIdleHook (gos_taskIdleHook_t idleHookFunction)
{
    /*
     * Local variables.
     */
    gos_result_t hookRegisterResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (idleHookFunction != NULL && kernelIdleHookFunction == NULL)
    {
        kernelIdleHookFunction = idleHookFunction;
        hookRegisterResult = GOS_SUCCESS;
    }

    return hookRegisterResult;
}

/*
 * Function: gos_kernelRegisterSysTickHook
 */
gos_result_t gos_kernelRegisterSysTickHook (gos_sysTickHook_t sysTickHookFunction)
{
    /*
     * Local variables.
     */
    gos_result_t hookRegisterResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (sysTickHookFunction != NULL && kernelSysTickHookFunction == NULL)
    {
        kernelSysTickHookFunction = sysTickHookFunction;
        hookRegisterResult = GOS_SUCCESS;
    }

    return hookRegisterResult;
}

/*
 * Function: gos_kernelSubscribeTaskDeleteSignal
 */
gos_result_t gos_kernelSubscribeTaskDeleteSignal (gos_signalHandler_t deleteSignalHandler)
{
    /*
     * Local variables.
     */
    gos_result_t subscriptionResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (deleteSignalHandler != NULL)
    {
        subscriptionResult = gos_signalSubscribe(
                kernelTaskDeleteSignal,
                deleteSignalHandler,
                GOS_TASK_PRIVILEGED_USER
                );
    }

    return subscriptionResult;
}

/*
 * Function: gos_kernelSubscribeDumpReadySignal
 */
gos_result_t gos_kernelSubscribeDumpReadySignal (gos_signalHandler_t dumpReadySignalHandler)
{
    /*
     * Local variables.
     */
    gos_result_t subscriptionResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (dumpReadySignalHandler != NULL)
    {
        subscriptionResult = gos_signalSubscribe(kernelDumpReadySignal, dumpReadySignalHandler, GOS_TASK_PRIVILEGE_USER);
    }

    return subscriptionResult;
}

/*
 * Function: gos_ported_sysTickInterrupt
 */
void_t gos_ported_sysTickInterrupt (void_t)
{
    /*
     * Function code.
     */
    sysTicks++;

    // Periodic stack overflow check on currently running task.
    gos_kernelCheckTaskStack();

#if CFG_SCHED_COOPERATIVE == 0
    if (schedDisableCntr == 0u)
    {
        // Privileged.
        gos_kernelReschedule(GOS_PRIVILEGED);
    }
#endif

    if (kernelSysTickHookFunction != NULL)
    {
        kernelSysTickHookFunction();
    }
}

/*
 * Function: gos_kernelGetSysTicks
 */
u32_t gos_kernelGetSysTicks (void_t)
{
    /*
     * Function code.
     */
    return sysTicks;
}

/*
 * Function: gos_kernelGetCpuUsage
 */
u16_t gos_kernelGetCpuUsage (void_t)
{
    /*
     * Function code.
     */
    return (10000 - taskDescriptors[0].taskCpuMonitoringUsage);
}

/*
 * Function: gos_kernelReset
 */
void_t gos_kernelReset (void_t)
{
    /*
     * Function code.
     */
    resetRequired = GOS_TRUE;
    gos_kernelReschedule(GOS_UNPRIVILEGED);
}

/*
 * Function: gos_kernelDelayUs
 */
GOS_INLINE void_t gos_kernelDelayUs (u16_t microseconds)
{
    /*
     * Local variables.
     */
    u16_t tmrInitialValue = 0u;
    u16_t tmrActualValue  = 0u;

    /*
     * Function code.
     */
    gos_timerDriverSysTimerGet(&tmrInitialValue);
    do
    {
        gos_timerDriverSysTimerGet(&tmrActualValue);
    }
    while ((u16_t)(tmrActualValue - tmrInitialValue) < microseconds);
}

/*
 * Function: gos_kernelDelayMs
 */
GOS_INLINE void_t gos_kernelDelayMs (u16_t milliseconds)
{
    /*
     * Local variables.
     */
    u32_t sysTickVal = sysTicks;

    /*
     * Function code.
     */
    while ((u16_t)(sysTicks - sysTickVal) < milliseconds);
}

/*
 * Function: gos_kernelCalculateTaskCpuUsages
 */
#include <gos_time.h>
GOS_INLINE void_t gos_kernelCalculateTaskCpuUsages (bool_t isResetRequired)
{
    /*
     * Local variables.
     */
    u16_t taskIndex           = 0u;
    u32_t systemConvertedTime = 0u;
    u32_t taskConvertedTime   = 0u;

    /*
     * Function code.
     */
    // Calculate in microseconds.
    systemConvertedTime = monitoringTime.minutes * 60 * 1000 * 1000 +
                          monitoringTime.seconds * 1000 * 1000 +
                          monitoringTime.milliseconds * 1000 +
                          monitoringTime.microseconds;

    for (taskIndex = 0u; taskIndex < CFG_TASK_MAX_NUMBER; taskIndex++)
    {
        taskConvertedTime   = taskDescriptors[taskIndex].taskMonitoringRunTime.minutes * 60 * 1000 * 1000 +
                              taskDescriptors[taskIndex].taskMonitoringRunTime.seconds * 1000 * 1000 +
                              taskDescriptors[taskIndex].taskMonitoringRunTime.milliseconds * 1000 +
                              taskDescriptors[taskIndex].taskMonitoringRunTime.microseconds;

        if (systemConvertedTime > 0)
        {
            // Calculate CPU usage and then reset runtime counter.
            taskDescriptors[taskIndex].taskCpuMonitoringUsage = (u16_t)(((u32_t)10000 * taskConvertedTime) / systemConvertedTime);

            if (isResetRequired == GOS_TRUE || monitoringTime.seconds > 0)
            {
                taskDescriptors[taskIndex].taskCpuUsage = (u16_t)((u32_t)(10000 * taskConvertedTime) / systemConvertedTime);

                // Increase runtime microseconds.
                gos_runTimeAddMicroseconds(
                        &taskDescriptors[taskIndex].taskRunTime,
                        NULL,
                        (u16_t)taskDescriptors[taskIndex].taskMonitoringRunTime.microseconds);

                // Increase runtime milliseconds.
                gos_runTimeAddMilliseconds(
                        &taskDescriptors[taskIndex].taskRunTime,
                        (u32_t)(taskDescriptors[taskIndex].taskMonitoringRunTime.milliseconds +
                        taskDescriptors[taskIndex].taskMonitoringRunTime.seconds * 1000));

                taskDescriptors[taskIndex].taskMonitoringRunTime.days         = 0u;
                taskDescriptors[taskIndex].taskMonitoringRunTime.hours        = 0u;
                taskDescriptors[taskIndex].taskMonitoringRunTime.minutes      = 0u;
                taskDescriptors[taskIndex].taskMonitoringRunTime.seconds      = 0u;
                taskDescriptors[taskIndex].taskMonitoringRunTime.milliseconds = 0u;
                taskDescriptors[taskIndex].taskMonitoringRunTime.microseconds = 0u;

                // Store the highest CPU usage value.
                if (taskDescriptors[taskIndex].taskCpuUsage > taskDescriptors[taskIndex].taskCpuUsageMax)
                {
                    taskDescriptors[taskIndex].taskCpuUsageMax = taskDescriptors[taskIndex].taskCpuUsage;
                }
                else
                {
                    // Max. value has not been reached.
                }
            }
        }

        if (taskDescriptors[taskIndex].taskFunction == NULL)
        {
            break;
        }
        else
        {
            // Continue.
        }
    }

    // Reset monitoring time.
    if (isResetRequired == GOS_TRUE || monitoringTime.seconds > 0)
    {
        monitoringTime.days         = 0u;
        monitoringTime.hours        = 0u;
        monitoringTime.minutes      = 0u;
        monitoringTime.seconds      = 0u;
        monitoringTime.milliseconds = 0u;
        monitoringTime.microseconds = 0u;
    }
}

/*
 * Function: gos_kernelDump
 */
void_t gos_kernelDump (void_t)
{
    /*
     * Local variables.
     */
    u16_t taskIndex = 0u;

    /*
     * Function code.
     */
    (void_t) gos_shellDriverTransmitString("Task dump:\r\n");
    (void_t) gos_shellDriverTransmitString(TASK_DUMP_SEPARATOR);
    (void_t) gos_shellDriverTransmitString(
        "| %6s | %28s | %4s | %16s | %6s | %9s |\r\n",
        "tid",
        "name",
        "prio",
        "privileges",
        "cpu [%]",
        "state"
        );
    (void_t) gos_shellDriverTransmitString(TASK_DUMP_SEPARATOR);

    for (taskIndex = 0u; taskIndex < CFG_TASK_MAX_NUMBER; taskIndex++)
    {
        if (taskDescriptors[taskIndex].taskFunction == NULL)
        {
            break;
        }
        (void_t) gos_shellDriverTransmitString(
                "| 0x%04X | %28s | %4d | " BINARY_PATTERN " | %4u.%02u | %18s |\r\n",
                taskDescriptors[taskIndex].taskId,
                taskDescriptors[taskIndex].taskName,
                taskDescriptors[taskIndex].taskPriority,
                TO_BINARY((u16_t)taskDescriptors[taskIndex].taskPrivilegeLevel),
                taskDescriptors[taskIndex].taskCpuUsage / 100,
                taskDescriptors[taskIndex].taskCpuUsage % 100,
                gos_kernelGetTaskStateString(taskDescriptors[taskIndex].taskState)
                );
    }
    (void_t) gos_shellDriverTransmitString(TASK_DUMP_SEPARATOR"\n");

    // Max CPU load statistics.
    (void_t) gos_shellDriverTransmitString("CPU max loads:\r\n");
    (void_t) gos_shellDriverTransmitString(MAX_CPU_DUMP_SEPARATOR);
    (void_t) gos_shellDriverTransmitString(
        "| %6s | %28s | %11s |\r\n",
        "tid",
        "name",
        "max cpu [%]"
        );
    (void_t) gos_shellDriverTransmitString(MAX_CPU_DUMP_SEPARATOR);

    for (taskIndex = 0u; taskIndex < CFG_TASK_MAX_NUMBER; taskIndex++)
    {
        if (taskDescriptors[taskIndex].taskFunction == NULL)
        {
            break;
        }
        (void_t) gos_shellDriverTransmitString(
                "| 0x%04X | %28s | %8u.%02u |\r\n",
                taskDescriptors[taskIndex].taskId,
                taskDescriptors[taskIndex].taskName,
                taskDescriptors[taskIndex].taskCpuUsageMax / 100,
                taskDescriptors[taskIndex].taskCpuUsageMax % 100
                );
    }
    (void_t) gos_shellDriverTransmitString(MAX_CPU_DUMP_SEPARATOR"\n");

    // Stack stats.
    (void_t) gos_shellDriverTransmitString("Stack statistics:\r\n");
    (void_t) gos_shellDriverTransmitString(STACK_STATS_SEPARATOR);
    (void_t) gos_shellDriverTransmitString(
        "| %6s | %28s | %6s | %14s | %9s |\r\n",
        "tid",
        "name",
        "stack",
        "max stack use",
        "stack [%]"
        );
    (void_t) gos_shellDriverTransmitString(STACK_STATS_SEPARATOR);
    for (taskIndex = 0u; taskIndex < CFG_TASK_MAX_NUMBER; taskIndex++)
    {
        if (taskDescriptors[taskIndex].taskFunction == NULL)
        {
            break;
        }
        (void_t) gos_shellDriverTransmitString(
                "| 0x%04X | %28s | 0x%04X | 0x%-12X | %6u.%02u |%\r\n",
                taskDescriptors[taskIndex].taskId,
                taskDescriptors[taskIndex].taskName,
                taskDescriptors[taskIndex].taskStackSize,
                taskDescriptors[taskIndex].taskStackSizeMaxUsage,
                ((10000 * taskDescriptors[taskIndex].taskStackSizeMaxUsage) / taskDescriptors[taskIndex].taskStackSize) / 100,
                ((10000 * taskDescriptors[taskIndex].taskStackSizeMaxUsage) / taskDescriptors[taskIndex].taskStackSize) % 100
                );
    }
    (void_t) gos_shellDriverTransmitString(STACK_STATS_SEPARATOR"\n");
}

/*
 * Function: gos_kernelSetMaxCpuLoad
 */
gos_result_t gos_kernelSetMaxCpuLoad (u16_t maxCpuLoad)
{
    /*
     * Local variables.
     */
    gos_result_t setMaxCpuLoadResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (maxCpuLoad > 0 && maxCpuLoad <= 10000)
    {
        cpuUseLimit = maxCpuLoad;
        setMaxCpuLoadResult = GOS_SUCCESS;
    }

    return setMaxCpuLoadResult;
}

/*
 * Function: gos_kernelGetMaxCpuLoad
 */
gos_result_t gos_kernelGetMaxCpuLoad (u16_t* maxCpuLoad)
{
    /*
     * Local variables.
     */
    gos_result_t getMaxCpuLoadResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (maxCpuLoad != NULL)
    {
        *maxCpuLoad = cpuUseLimit;
        getMaxCpuLoadResult = GOS_SUCCESS;
    }

    return getMaxCpuLoadResult;
}

/*
 * Function: gos_kernelIsCallerPrivileged
 */
bool_t gos_kernelIsCallerIsr (void_t)
{
    /*
     * Function code.
     */
    return inIsr > 0u ? GOS_TRUE : GOS_FALSE;
}

/*
 * Function: gos_ported_svcHandler
 */
void_t gos_ported_svcHandler (void_t)
{
    /*
     * Function code.
     */
    gos_ported_handleSVC();
}

/*
 * Function: gos_ported_svcHandlerMain
 */
void_t gos_ported_svcHandlerMain (u32_t* sp)
{
    /*
     * Function code.
     */
    gos_ported_handleSVCMain(sp);
}

/*
 * Function: gos_ported_pendSVHandler
 */
void_t gos_ported_pendSVHandler (void_t)
{
    /*
     * Function code.
     */
    gos_ported_doContextSwitch();
}

/**
 * @brief   Checks the stack use of the current task.
 * @details Gets the current stack pointer value and checks whether it is
 *          lower than the allowed threshold value defined for the current stack.
 *          After the overflow check, it calculates the stack usage and updates the
 *          maximum stack usage for the current task. In case of stack overflow, it
 *          goes to system error.
 *
 * @return  -
 */
GOS_STATIC void_t gos_kernelCheckTaskStack (void_t)
{
    /*
     * Local variables.
     */
    u32_t sp = 0u;

    /*
     * Function code.
     */
    __asm volatile ("MRS %0, psp\n\t" : "=r" (sp));
    if (sp != 0 &&
        sp < taskDescriptors[currentTaskIndex].taskStackOverflowThreshold)
    {
        gos_errorHandler(
                GOS_ERROR_LEVEL_OS_FATAL,
                NULL,
                0,
                "Stack overflow detected in <%s>. \r\nPSP: 0x%x overflown by %d bytes.",
                taskDescriptors[currentTaskIndex].taskName,
                sp,
                (taskDescriptors[currentTaskIndex].taskStackOverflowThreshold - sp));
    }
    else
    {
        // No stack overflow was detected.
    }

    if (sp != 0 &&
        (taskDescriptors[currentTaskIndex].taskStackOverflowThreshold - 64 + taskDescriptors[currentTaskIndex].taskStackSize - sp) >
        taskDescriptors[currentTaskIndex].taskStackSizeMaxUsage)
    {
        taskDescriptors[currentTaskIndex].taskStackSizeMaxUsage = (taskDescriptors[currentTaskIndex].taskStackOverflowThreshold - 64 + 32 + taskDescriptors[currentTaskIndex].taskStackSize - sp);
    }
    else
    {
        // Max. value has not been exceeded.
    }
}

/**
 * @brief   Checks the validity of the task descriptor.
 * @details It checks the task function pointer, task priority value, stack, and
 *          size parameters and return with error if the parameters are incorrect.
 *
 * @param   taskDescriptor : Pointer to the task descriptor structure.
 *
 * @return  Result of task descriptor checking.
 *
 * @retval  GOS_SUCCESS    :    Descriptor contains valid data.
 * @retval  GOS_ERROR      :    One or more parameter is invalid:
 *                              - Task function is NULL pointer
 *                              - Task function is the idle task
 *                              - Priority exceeds the maximum priority level
 *                              - Stack size is smaller than the minimum allowed
 *                              - Stack size is greater than the maximum allowed
 *                              - Stack size is not 4-byte aligned
 */
GOS_STATIC gos_result_t gos_kernelCheckTaskDescriptor (gos_taskDescriptor_t* taskDescriptor)
{
    /*
     * Local variables.
     */
    gos_result_t taskDescCheckResult = GOS_SUCCESS;

    /*
     * Function code.
     */
    if (taskDescriptor->taskFunction == NULL                    ||
        taskDescriptor->taskPrivilegeLevel == 0                 ||
        taskDescriptor->taskPriority > GOS_TASK_MAX_PRIO_LEVELS ||
        taskDescriptor->taskFunction == gos_kernelIdleTask      ||
        taskDescriptor->taskStackSize > CFG_TASK_MAX_STACK_SIZE ||
        taskDescriptor->taskStackSize < CFG_TASK_MIN_STACK_SIZE ||
        taskDescriptor->taskStackSize % 4 != 0u)
    {
        taskDescCheckResult = GOS_ERROR;
    }

    return taskDescCheckResult;
}

/**
 * @brief   Reschedules the kernel.
 * @details Based on the privilege, it invokes a kernel reschedule event.
 *
 * @param   privilege : Privilege level.
 *
 * @return  -
 */
GOS_STATIC_INLINE void_t gos_kernelReschedule (gos_kernel_privilege_t privilege)
{
    /*
     * Function code.
     */
    gos_ported_reschedule(privilege);
}

/**
 * @brief    Returns the current PSP.
 * @details  Returns the current PSP.
 *
 * @return   Current PSP value.
 */
GOS_UNUSED GOS_STATIC u32_t gos_kernelGetCurrentPsp (void_t)
{
    /*
     * Function code.
     */
    return taskDescriptors[currentTaskIndex].taskPsp;
}

/**
 * @brief   Saves the current PSP.
 * @details Saves the current PSP.
 *
 * @param   psp : Current PSP value.
 *
 * @return    -
 */
GOS_UNUSED GOS_STATIC void_t gos_kernelSaveCurrentPsp (u32_t psp)
{
    /*
     * Function code.
     */
    taskDescriptors[currentTaskIndex].taskPsp = psp;
}

/**
 * @brief   Selects the next task for execution.
 * @details Loops through the internal task descriptor array and first checks
 *          the sleeping tasks and wakes up the ones that passed their sleeping time.
 *          Then based on the priority of the ready tasks, it selects the one with
 *          the highest priority (lowest number in priority). If there is a swap-hook
 *          function registered, it calls it, and then it refreshes the task run-time
 *          statistics.
 *
 * @return    -
 */
GOS_UNUSED GOS_STATIC void_t gos_kernelSelectNextTask (void_t)
{
    /*
     * Local variables.
     */
    u16_t          taskIndex      = 0u;
    gos_taskPrio_t lowestPrio     = GOS_TASK_IDLE_PRIO;
    u16_t          nextTask       = 0u;
    u16_t          sysTimerActVal = 0u;
    u16_t          currentRunTime = 0u;
    u32_t          elapsedTicks   = sysTicks - previousTick;

    /*
     * Function code.
     */
    // Run stack check.
    gos_kernelCheckTaskStack();

    if (schedDisableCntr == 0u)
    {
        for (taskIndex = 0U; taskIndex < CFG_TASK_MAX_NUMBER; taskIndex++)
        {
            // Wake-up sleeping tasks if their sleep time has elapsed.
            if (taskDescriptors[taskIndex].taskState == GOS_TASK_SLEEPING &&
                (taskDescriptors[taskIndex].taskSleepTickCounter += elapsedTicks) >= taskDescriptors[taskIndex].taskSleepTicks)
            {
                taskDescriptors[taskIndex].taskState = GOS_TASK_READY;
            }
            // Unblock tasks if their timeout time has elapsed.
            else if ((taskDescriptors[taskIndex].taskState == GOS_TASK_BLOCKED) &&
                    (taskDescriptors[taskIndex].taskBlockTicks != GOS_TASK_MAX_BLOCK_TIME_MS) &&
                    ((taskDescriptors[taskIndex].taskBlockTickCounter += elapsedTicks) == taskDescriptors[taskIndex].taskBlockTicks))
            {
                taskDescriptors[taskIndex].taskState = GOS_TASK_READY;
            }
            else
            {
                // Nothing to do.
            }

            // Choose the highest priority task - that is not the current one, and is ready - to run.
            // Also, check if task has reached the CPU usage limit.
            if (taskIndex != currentTaskIndex &&
                taskDescriptors[taskIndex].taskState == GOS_TASK_READY &&
                taskDescriptors[taskIndex].taskPriority < lowestPrio &&
                taskDescriptors[taskIndex].taskCpuMonitoringUsage < taskDescriptors[taskIndex].taskCpuUsageLimit &&
                taskDescriptors[taskIndex].taskCpuUsage < taskDescriptors[taskIndex].taskCpuUsageLimit)
            {
                nextTask = taskIndex;
                lowestPrio = taskDescriptors[taskIndex].taskPriority;
            }
            else
            {
                // Nothing to do.
            }

            if (taskDescriptors[taskIndex].taskFunction == NULL)
            {
                break;
            }
            else
            {
                // Continue.
            }
        }

        // If CPU limit exceeded, override scheduling, and select the idle task to run.
        if (gos_kernelGetCpuUsage() > cpuUseLimit)
        {
            nextTask = 0u;
        }
        else
        {
            // Nothing to do.
        }

        // If there was a task-swap, call the hook function.
        if (currentTaskIndex != nextTask)
        {
            if (kernelSwapHookFunction != NULL)
            {
                kernelSwapHookFunction(taskDescriptors[currentTaskIndex].taskId, taskDescriptors[nextTask].taskId);
            }
            else
            {
                // Nothing to do.
            }
            taskDescriptors[currentTaskIndex].taskCsCounter++;
        }
        else
        {
            // Nothing to do.
        }

        // Calculate current task run-time.
        gos_timerDriverSysTimerGet(&sysTimerActVal);
        currentRunTime = sysTimerActVal - sysTimerValue;

        // Increase monitoring system time and current task runtime.
        (void_t) gos_runTimeAddMicroseconds(&monitoringTime, &taskDescriptors[currentTaskIndex].taskMonitoringRunTime, currentRunTime);

        // Refresh system timer value.
        (void_t) gos_timerDriverSysTimerGet(&sysTimerValue);

        // Set current task.
        currentTaskIndex = nextTask;

        // Update previous tick value.
        previousTick = sysTicks;
    }
}

/**
 * @brief   Kernel idle task.
 * @details This task is executed when there is no other ready task in the system.
 *          When executed, this function refreshes the CPU-usage statistics of tasks.
 *
 * @return    -
 */
GOS_STATIC void_t gos_kernelIdleTask (void_t)
{
    /*
     * Function code.
     */
    gos_errorTraceInit("Starting OS...", GOS_SUCCESS);

    for (;;)
    {
        taskDescriptors[0].taskRunCounter++;

        if (kernelIdleHookFunction != NULL)
        {
            kernelIdleHookFunction();
        }

        gos_kernelCalculateTaskCpuUsages(GOS_FALSE);

        (void_t) gos_kernelTaskYield();
    }
}

/**
 * @brief   Translates the task state to a string.
 * @details Based on the task state it returns a string with a printable form
 *          of the task state.
 *
 * @param   taskState : The task state variable to be translated.
 *
 * @return  String with the task state.
 */
GOS_STATIC char_t* gos_kernelGetTaskStateString (gos_taskState_t taskState)
{
    /*
     * Function code.
     */
    switch (taskState)
    {
        case GOS_TASK_READY:
        {
            return TRACE_FG_GREEN_START"ready"TRACE_FORMAT_RESET;
        }break;
        case GOS_TASK_SLEEPING:
        {
            return TRACE_FG_YELLOW_START"sleeping"TRACE_FORMAT_RESET;
        }break;
        case GOS_TASK_SUSPENDED:
        {
            return TRACE_FG_MAGENTA_START"suspended"TRACE_FORMAT_RESET;
        }break;
        case GOS_TASK_BLOCKED:
        {
            return TRACE_FG_CYAN_START"blocked"TRACE_FORMAT_RESET;
        }break;
        case GOS_TASK_ZOMBIE:
        {
            return TRACE_FG_RED_START"zombie"TRACE_FORMAT_RESET;
        }
        default:
        {
            return "";
        }
    }
}

/**
 * @brief   Kernel processor reset.
 * @details Resets the processor.
 *
 * @return    -
 */
GOS_STATIC void_t gos_kernelProcessorReset (void_t)
{
    /*
     * Function code.
     */
    // Reset processor.
    gos_ported_procReset();

    // Wait for reset.
    for (;;)
    {
        GOS_NOP;
    }
}

/*
 * Fault handlers
 */
/*
 * Function: NMI_Handler
 */
void_t NMI_Handler (void_t)
{
    /*
     * Function code.
     */
    for (;;)
    {
        GOS_NOP;
    }
}

/*
 * Function: HardFault_Handler
 */
void_t HardFault_Handler (void_t)
 {
    /*
     * Function code.
     */

    for (;;)
    {
        GOS_NOP;
    }
}

/*
 * Function: MemManage_Handler
 */
void_t MemManage_Handler (void_t)
{
    /*
     * Function code.
     */
    for (;;)
    {
        GOS_NOP;
    }
}

/*
 * Function: BusFault_Handler
 */
void_t BusFault_Handler (void_t)
{
    /*
     * Function code.
     */
    for (;;)
    {
        GOS_NOP;
    }
}

/*
 * Function: UsageFault_Handler
 */
void_t UsageFault_Handler (void_t)
{
    /*
     * Function code.
     */

    for (;;)
    {
        GOS_NOP;
    }
}
