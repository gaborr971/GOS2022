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
//! @file		gos_kernel.c
//! @author		Gabor Repasi
//! @date		2022-12-15
//! @version	1.5
//!
//! @brief		GOS kernel source.
//! @details	For a more detailed description of this module, please refer to @ref gos_kernel.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-21	Gabor Repasi	Initial version created.
// 1.1		2022-11-15	Gabor Repasi	+	License added
// 1.2		2022-12-03	Gabor Repasi	+	gos_kernelTaskRegisterTasks added
//										+	Tasks external task ID handling added
//										+	Kernel configuration structure array added
//										+	Cooperative scheduling introduced
// 1.3		2022-12-04	Gabor Repasi	*	Sleep, wake-up, suspend, resume, block, unblock	hook
//											functions removed
//										+	Kernel dump ready signal added
// 1.4		2022-12-08	Gabor Repasi	+	Task priority setter and getter functions added
// 1.5		2022-12-15	Gabor Repasi	+	Privilege handling introduced, privilege check added to
//											kernel functions
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
/*
 * Includes
 */
#include <string.h>
#include <stdio.h>
#include "gos_kernel.h"
#include "gos_error.h"
#include "gos_log.h"
#include "gos_process.h"
#include "gos_signal.h"
#include "gos_timer_driver.h"

/*
 * Macros
 */
/**
 * System Handler control and state register.
 */
#define SHCSR					*( (volatile u32_t*) 0xE000ED24u )

/**
 * Interrupt Control and State Register.
 */
#define ICSR					*( (volatile u32_t*) 0xE000ED04u )

/**
 * RAM start address.
 */
#define RAM_START				( 0x20000000u )

/**
 * RAM size (128kB).
 */
#define RAM_SIZE				( 128 * 1024 )

/**
 * Main stack.
 */
#define MAIN_STACK				( RAM_START + RAM_SIZE )

/**
 * Global stack.
 */
#define GLOBAL_STACK			( 2048 )

/**
 * Task dump separator line.
 */
#define TASK_DUMP_SEPARATOR		"+--------+------------------------------+--------+------+------------------+---------+-----------+\r\n"

/**
 * Config dump separator line.
 */
#define CONFIG_DUMP_SEPARATOR	"+----------------------------------+--------+\r\n"

/*
 * Type definitions
 */
/**
 * Kernel privilege levels.
 */
typedef enum
{
	GOS_PRIVILEGED		= 0b10110,	//!< GOS_PRIVILEDGED
	GOS_UNPRIVILEGED	= 0b01001	//!< GOS_UNPRIVILEDGED
}gos_kernel_privilege_t;

/**
 * OS configuration descriptor.
 */
typedef struct
{
	char_t	configName [32];	//!< Name of configuration parameter.
	u16_t	configValue;		//!< Value of configuration parameter.
}gos_kernelConfigDescriptor_t;

/*
 * Static variables
 */
/**
 * System tick value.
 */
GOS_STATIC u32_t 					sysTicks 					= 0u;

/**
 * Current task index - for priority scheduling.
 */
GOS_STATIC u32_t 					currentTaskIndex			= 0u;

/**
 * System timer value for run-time calculations.
 */
GOS_STATIC u32_t					sysTimerValue				= 0u;

/**
 * Total system up-time since start.
 */
GOS_STATIC u64_t					totalSystemTime				= 0u;

/**
 * Kernel idle hook function.
 */
GOS_STATIC gos_taskIdleHook_t 		kernelIdleHookFunction 		= NULL;

/**
 * Kernel swap hook function.
 */
GOS_STATIC gos_taskSwapHook_t		kernelSwapHookFunction		= NULL;

/**
 * Kernel dump task ID.
 */
GOS_STATIC gos_tid_t				kernelDumpTaskId			= GOS_INVALID_TASK_ID;

/**
 * Reset required flag.
 */
GOS_STATIC bool_t					resetRequired				= GOS_FALSE;

/**
 * Configuration parameter lookup table.
 */
#if CFG_KERNEL_DUMP_CONFIG == 1
GOS_STATIC const gos_kernelConfigDescriptor_t	kernelConfig [] =
{
	// Maximum task priority levels.
	{
		.configName = "Max. priority levels",
		.configValue = CFG_TASK_MAX_PRIO_LEVELS
	},
	// Idle task priority.
	{
		.configName = "Idle task priority",
		.configValue = CFG_TASK_IDLE_PRIO
	},
	// Maximum task name length.
	{
		.configName = "Max. task name length",
		.configValue = CFG_TASK_MAX_NAME_LENGTH
	},
	// Maximum number of tasks.
	{
		.configName = "Max. number of tasks",
		.configValue = CFG_TASK_MAX_NUMBER
	},
	// Minimum task stack size.
	{
		.configName = "Min. task stack size",
		.configValue = CFG_TASK_MIN_STACK_SIZE
	},
	// Maximum task stack size.
	{
		.configName = "Max. task stack size",
		.configValue = CFG_TASK_MAX_STACK_SIZE
	},
	// Process service use flag.
	{
		.configName = "Process service use flag",
		.configValue = CFG_PROC_USE_SERVICE
	},
	// Maximum process priority levels.
	{
		.configName = "Maximum process priority levels",
		.configValue = CFG_PROC_MAX_PRIO_LEVELS
	},
	// Idle process priority.
	{
		.configName = "Idle process priority",
		.configValue = CFG_PROC_IDLE_PRIO
	},
	// Maximum process name length.
	{
		.configName = "Maximum process name length",
		.configValue = CFG_PROC_MAX_NAME_LENGTH
	},
	// Maximum number of processes.
	{
		.configName = "Maximum number of processes",
		.configValue = CFG_PROC_MAX_NUMBER
	}
};
#endif

/*
 * Global variables
 */
/**
 * Kernel dump signal.
 */
gos_signalId_t						kernelDumpSignal;

/**
 * Kernel dump ready signal.
 */
gos_signalId_t						kernelDumpReadySignal;

/**
 * Kernel task delete signal.
 */
gos_signalId_t						kernelTaskDeleteSignal;

/**
 * Scheduling disabled flag.
 */
bool_t								schedulingDisabled			= GOS_FALSE;

/**
 * Privileged access counter.
 */
u8_t								privilegedAccess			= 0u;

/**
 * In ISR counter.
 */
u8_t 								inIsr						= 0u;

/*
 * Kernel global functions
 */
void_t gos_kernelDumpSignalHandler	(gos_signalSenderId_t senderId);

/*
 * Function prototypes
 */
GOS_STATIC			gos_result_t	gos_kernelCheckTaskDescriptor 	(gos_taskDescriptor_t* taskDescriptor);
GOS_STATIC			u32_t			gos_kernelGetCurrentPsp			(void_t);
GOS_STATIC			void_t			gos_kernelSaveCurrentPsp		(u32_t psp);
GOS_STATIC 			void_t 			gos_kernelSelectNextTask		(void_t);
GOS_STATIC 			void_t 			gos_kernelIdleTask				(void_t);
GOS_STATIC_INLINE 	void_t			gos_kernelReschedule			(gos_kernel_privilege_t privilege);
GOS_STATIC 			char_t* 		gos_kernelGetTaskStateString 	(gos_taskState_t taskState);
GOS_STATIC			void_t			gos_kernelDumpTask				(void_t);
GOS_STATIC 			void_t 			gos_kernelProcessorReset 		(void_t);

/**
 * Internal task array.
 */
GOS_STATIC gos_taskDescriptor_t	taskDescriptors [CFG_TASK_MAX_NUMBER] =
{
	[0] =
		{
			.taskFunction 		= gos_kernelIdleTask,
			.taskId 			= GOS_DEFAULT_TASK_ID,
			.taskPriority 		= GOS_TASK_IDLE_PRIO,
			.taskName			= "gos_idle_task",
			.taskState 			= GOS_TASK_READY,
			.taskStackSize		= CFG_IDLE_TASK_STACK_SIZE,
			.taskPrivilegeLevel	= GOS_TASK_PRIVILEGE_KERNEL
		}
};

/**
 * Kernel dump task descriptor.
 */
gos_taskDescriptor_t kernelDumpTaskDesc =
{
	.taskFunction		= gos_kernelDumpTask,
	.taskPriority		= CFG_TASK_KERNEL_DUMP_PRIO,
	.taskName			= "gos_kernel_dump_task",
	.taskStackSize		= CFG_TASK_KERNEL_DUMP_STACK,
	.taskPrivilegeLevel	= GOS_TASK_PRIVILEGE_KERNEL
};

/*
 * Function: gos_kernelInit
 */
gos_result_t gos_kernelInit (void_t)
{
	/*
	 * Local variables.
	 */
    gos_result_t	initResult	= GOS_ERROR;
    u16_t			taskIndex	= 1u;

    /*
     * Function code.
     */
    // Initialize task descriptors.
    for (taskIndex = 1u; taskIndex < CFG_TASK_MAX_NUMBER; taskIndex++)
    {
    	taskDescriptors[taskIndex].taskFunction			= NULL;
    	taskDescriptors[taskIndex].taskPriority			= GOS_TASK_MAX_PRIO_LEVELS;
    	taskDescriptors[taskIndex].taskOriginalPriority	= GOS_TASK_MAX_PRIO_LEVELS;
    	taskDescriptors[taskIndex].taskState			= GOS_TASK_SUSPENDED;
    	taskDescriptors[taskIndex].taskId				= GOS_INVALID_TASK_ID;
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

	// Enable Fault Handlers
	SHCSR |= (1 << 16); // Memory Fault
	SHCSR |= (1 << 17); // Bus Fault
	SHCSR |= (1 << 18); // Usage Fault

	schedulingDisabled = GOS_FALSE;

	if (gos_kernelTaskRegister(&kernelDumpTaskDesc, &kernelDumpTaskId) == GOS_SUCCESS &&
		gos_kernelTaskSuspend(kernelDumpTaskId) == GOS_SUCCESS)
	{
		initResult = GOS_SUCCESS;
	}

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

	// Prepare PSP of the first task.
	GOS_ASM("BL gos_kernelGetCurrentPsp"); // return PSP in R0
	GOS_ASM("MSR PSP, R0");  // set PSP

	// Change to use PSP.
	GOS_ASM("MRS R0, CONTROL");
	GOS_ASM("ORR R0, R0, #2"); // set bit[1] SPSEL
	GOS_ASM("MSR CONTROL, R0");

	// Move to unprivileged level.
	GOS_ASM("MRS R0, CONTROL");
	GOS_ASM("ORR R0, R0, #1"); // Set bit[0] nPRIV
	GOS_ASM("MSR CONTROL, R0");
	// Right after here, access is limited.

	// Get the handler of the first task by tracing back from PSP which is at R4 slot.
	gos_task_t firstTask = taskDescriptors[currentTaskIndex].taskFunction;

	// Initialize system timer value.
	sysTimerValue = gos_timerDriverSysTimerGet();

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
	u16_t numberOfTasks = 0u;
	u16_t index = 0u;

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
	gos_result_t	taskRegisterResult	= GOS_SUCCESS;
	u16_t			taskIndex			= 0u;
	u32_t			taskStackOffset		= GLOBAL_STACK;
	u32_t*			psp					= NULL;

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
			taskDescriptors[taskIndex].taskPsp	= (u32_t)psp;

			// Initial state.
			taskDescriptors[taskIndex].taskState			= GOS_TASK_READY;
			taskDescriptors[taskIndex].taskFunction			= taskDescriptor->taskFunction;
			taskDescriptors[taskIndex].taskPriority			= taskDescriptor->taskPriority;
			taskDescriptors[taskIndex].taskOriginalPriority	= taskDescriptor->taskPriority;
			taskDescriptors[taskIndex].taskStackSize		= taskDescriptor->taskStackSize;
			taskDescriptors[taskIndex].taskId				= (GOS_DEFAULT_TASK_ID + taskIndex);
			taskDescriptors[taskIndex].taskPrivilegeLevel	= taskDescriptor->taskPrivilegeLevel;

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
		GOS_DISABLE_SCHED
		{
			if (taskDescriptors[currentTaskIndex].taskState == GOS_TASK_READY)
			{
				taskDescriptors[currentTaskIndex].taskState			= GOS_TASK_SLEEPING;
				taskDescriptors[currentTaskIndex].taskSleepTicks	= sysTicks + sleepTicks;

				taskSleepResult = GOS_SUCCESS;
			}
		}
		GOS_ENABLE_SCHED

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
	gos_result_t	taskWakeupResult	= GOS_ERROR;
	u32_t			taskIndex			= 0u;

	/*
	 * Function code.
	 */
	GOS_DISABLE_SCHED
	if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
	{
		taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

		// Check task manipulation privilege.
		if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_MANIPULATE) == GOS_PRIV_TASK_MANIPULATE ||
			privilegedAccess > 0 || inIsr > 0)
		{
			if (taskDescriptors[taskIndex].taskState == GOS_TASK_SLEEPING)
			{
				taskDescriptors[taskIndex].taskState = GOS_TASK_READY;
				taskWakeupResult = GOS_SUCCESS;
			}
		}
		else
		{
			gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to wake up <%s>!",
				taskDescriptors[currentTaskIndex].taskName,
				taskDescriptors[taskIndex].taskName
			);
		}
	}

	GOS_UNPRIVILEGED_ACCESS
	GOS_ENABLE_SCHED

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
	gos_result_t	taskSuspendResult	= GOS_ERROR;
	u32_t			taskIndex			= 0u;

	/*
	 * Function code.
	 */
	GOS_DISABLE_SCHED
	if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
	{
		taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

		// Check task manipulation privilege.
		if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_MANIPULATE) == GOS_PRIV_TASK_MANIPULATE ||
			privilegedAccess > 0 || currentTaskIndex == taskIndex || inIsr > 0)
		{
			if (taskDescriptors[taskIndex].taskState == GOS_TASK_READY ||
				taskDescriptors[taskIndex].taskState == GOS_TASK_SLEEPING ||
				taskDescriptors[taskIndex].taskState == GOS_TASK_BLOCKED)
			{
				taskDescriptors[taskIndex].taskPreviousState = taskDescriptors[taskIndex].taskState;
				taskDescriptors[taskIndex].taskState = GOS_TASK_SUSPENDED;
				taskSuspendResult = GOS_SUCCESS;

				GOS_UNPRIVILEGED_ACCESS
				GOS_ENABLE_SCHED

				if (currentTaskIndex == taskIndex)
				{
					// Unprivileged.
					gos_kernelReschedule(GOS_UNPRIVILEGED);
				}
			}
		}
		else
		{
			gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to suspend <%s>!",
				taskDescriptors[currentTaskIndex].taskName,
				taskDescriptors[taskIndex].taskName
			);
		}
	}

	if (taskSuspendResult != GOS_SUCCESS)
	{
		GOS_UNPRIVILEGED_ACCESS
		GOS_ENABLE_SCHED
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
	gos_result_t	taskResumeResult	= GOS_ERROR;
	u32_t			taskIndex			= 0u;

	/*
	 * Function code.
	 */
	GOS_DISABLE_SCHED
	if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
	{
		taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

		// Check task manipulation privilege.
		if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_MANIPULATE) == GOS_PRIV_TASK_MANIPULATE ||
			privilegedAccess > 0 || inIsr > 0)
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

	GOS_UNPRIVILEGED_ACCESS
	GOS_ENABLE_SCHED
	return taskResumeResult;
}

/*
 * Function: gos_kernelTaskBlock
 */
GOS_INLINE gos_result_t gos_kernelTaskBlock (gos_tid_t taskId)
{
	/*
	 * Local variables.
	 */
	gos_result_t	taskBlockResult	= GOS_ERROR;
	u32_t			taskIndex		= 0u;

	/*
	 * Function code.
	 */
	GOS_DISABLE_SCHED
	if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
	{
		taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

		if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_MANIPULATE) == GOS_PRIV_TASK_MANIPULATE ||
			privilegedAccess > 0 || currentTaskIndex == taskIndex || inIsr > 0)
		{
			if (taskDescriptors[taskIndex].taskState == GOS_TASK_READY)
			{
				taskDescriptors[taskIndex].taskState = GOS_TASK_BLOCKED;
				taskBlockResult = GOS_SUCCESS;

				GOS_UNPRIVILEGED_ACCESS
				GOS_ENABLE_SCHED

				if (currentTaskIndex == taskIndex)
				{
					// Unprivileged.
					gos_kernelReschedule(GOS_UNPRIVILEGED);
				}
			}
		}
		else
		{
			gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to block <%s>!",
				taskDescriptors[currentTaskIndex].taskName,
				taskDescriptors[taskIndex].taskName
			);
		}
	}
	if (taskBlockResult != GOS_SUCCESS)
	{
		GOS_UNPRIVILEGED_ACCESS
		GOS_ENABLE_SCHED
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
	gos_result_t	taskUnblockResult	= GOS_ERROR;
	u32_t			taskIndex			= 0u;

	/*
	 * Function code.
	 */
	GOS_DISABLE_SCHED
	if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
	{
		taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

		if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_MANIPULATE) == GOS_PRIV_TASK_MANIPULATE ||
			privilegedAccess > 0 || inIsr > 0)
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
			gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to unblock <%s>!",
				taskDescriptors[currentTaskIndex].taskName,
				taskDescriptors[taskIndex].taskName
			);
		}
	}

	GOS_UNPRIVILEGED_ACCESS
	GOS_ENABLE_SCHED

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
	gos_result_t	taskDeletekResult	= GOS_ERROR;
	u32_t			taskIndex			= 0u;

	/*
	 * Function code.
	 */
	GOS_DISABLE_SCHED
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
				taskDeletekResult = GOS_SUCCESS;
				// Invoke signal.
				GOS_PRIVILEGED_ACCESS
				gos_signalInvoke(kernelTaskDeleteSignal, taskId);
			}
		}
		else
		{
			gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to delete <%s>!",
				taskDescriptors[currentTaskIndex].taskName,
				taskDescriptors[taskIndex].taskName
			);
		}
	}
	GOS_ENABLE_SCHED

	if (taskDeletekResult == GOS_SUCCESS && currentTaskIndex == taskIndex)
	{
		gos_kernelReschedule(GOS_UNPRIVILEGED);
	}

	return taskDeletekResult;
}

/*
 * Function: gos_kernelTaskSetPriority
 */
gos_result_t gos_kernelTaskSetPriority (gos_tid_t taskId, gos_taskPrio_t taskPriority)
{
	/*
	 * Local variables.
	 */
	gos_result_t	taskSetPriorityResult	= GOS_ERROR;
	u32_t			taskIndex				= 0u;

	/*
	 * Function code.
	 */
	GOS_DISABLE_SCHED
	if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER &&
		taskPriority < GOS_TASK_MAX_PRIO_LEVELS)
	{
		taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

		// Check privilege level.
		if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_PRIO_CHANGE) == GOS_PRIV_TASK_PRIO_CHANGE ||
			privilegedAccess > 0 || inIsr > 0)
		{
			taskDescriptors[taskIndex].taskPriority = taskPriority;
			taskSetPriorityResult = GOS_SUCCESS;
		}
		else
		{
			gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to set the priority of <%s>!",
				taskDescriptors[currentTaskIndex].taskName,
				taskDescriptors[taskIndex].taskName
			);
		}
	}
	GOS_UNPRIVILEGED_ACCESS
	GOS_ENABLE_SCHED

	return taskSetPriorityResult;
}

/*
 * Function: gos_kernelTaskSetOriginalPriority
 */
gos_result_t gos_kernelTaskSetOriginalPriority (gos_tid_t taskId, gos_taskPrio_t taskPriority)
{
	/*
	 * Local variables.
	 */
	gos_result_t	taskSetPriorityResult	= GOS_ERROR;
	u32_t			taskIndex				= 0u;

	/*
	 * Function code.
	 */
	GOS_DISABLE_SCHED
	if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER &&
		taskPriority < GOS_TASK_MAX_PRIO_LEVELS)
	{
		taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

		// Check privilege level.
		if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_TASK_PRIO_CHANGE) == GOS_PRIV_TASK_PRIO_CHANGE ||
			privilegedAccess > 0 || inIsr > 0)
		{
			taskDescriptors[taskIndex].taskOriginalPriority = taskPriority;
			taskSetPriorityResult = GOS_SUCCESS;
		}
		else
		{
			gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to set the priority of <%s>!",
				taskDescriptors[currentTaskIndex].taskName,
				taskDescriptors[taskIndex].taskName
			);
		}
	}
	GOS_UNPRIVILEGED_ACCESS
	GOS_ENABLE_SCHED
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
	gos_result_t	taskGetPriorityResult	= GOS_ERROR;
	u32_t			taskIndex				= 0u;

	/*
	 * Function code.
	 */
	if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER &&
		taskPriority != NULL)
	{
		taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

		GOS_DISABLE_SCHED
		{
			*taskPriority = taskDescriptors[taskIndex].taskPriority;
		}
		GOS_ENABLE_SCHED

		taskGetPriorityResult = GOS_SUCCESS;
	}
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
	gos_result_t	taskGetPriorityResult	= GOS_ERROR;
	u32_t			taskIndex				= 0u;

	/*
	 * Function code.
	 */
	if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER &&
		taskPriority != NULL)
	{
		taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

		GOS_DISABLE_SCHED
		{
			*taskPriority = taskDescriptors[taskIndex].taskOriginalPriority;
		}
		GOS_ENABLE_SCHED

		taskGetPriorityResult = GOS_SUCCESS;
	}
	return taskGetPriorityResult;
}

/*
 * Function: gos_kernelTaskAddPrivilege
 */
gos_result_t gos_kernelTaskAddPrivilege (gos_tid_t taskId, gos_taskPrivilegeLevel_t privileges)
{
	/*
	 * Local variables.
	 */
	gos_result_t 	taskAddPrivilegeResult 	= GOS_ERROR;
	u32_t			taskIndex				= 0u;

	/*
	 * Function code.
	 */
	GOS_DISABLE_SCHED
	if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
	{
		taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

		// Check privilege level.
		if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_MODIFY) == GOS_PRIV_MODIFY)
		{
			taskDescriptors[taskIndex].taskPrivilegeLevel |= privileges;
			taskAddPrivilegeResult = GOS_SUCCESS;
		}
		else
		{
			gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to add privileges to <%s>!",
				taskDescriptors[currentTaskIndex].taskName,
				taskDescriptors[taskIndex].taskName
			);
		}
	}
	GOS_ENABLE_SCHED
	return taskAddPrivilegeResult;
}

/*
 * Function: gos_kernelTaskRemovePrivilege
 */
gos_result_t gos_kernelTaskRemovePrivilege (gos_tid_t taskId, gos_taskPrivilegeLevel_t privileges)
{
	/*
	 * Local variables.
	 */
	gos_result_t 	taskRemovePrivilegeResult 	= GOS_ERROR;
	u32_t			taskIndex				= 0u;

	/*
	 * Function code.
	 */
	GOS_DISABLE_SCHED
	if (taskId > GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER)
	{
		taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

		// Check privilege level.
		if ((taskDescriptors[currentTaskIndex].taskPrivilegeLevel & GOS_PRIV_MODIFY) == GOS_PRIV_MODIFY)
		{
			taskDescriptors[taskIndex].taskPrivilegeLevel &= ~privileges;
			taskRemovePrivilegeResult = GOS_SUCCESS;
		}
		else
		{
			gos_errorHandler(GOS_ERROR_LEVEL_OS_FATAL, __func__, __LINE__, "<%s> has no privilege to remove privileges from <%s>!",
				taskDescriptors[currentTaskIndex].taskName,
				taskDescriptors[taskIndex].taskName
			);
		}
	}
	GOS_ENABLE_SCHED
	return taskRemovePrivilegeResult;
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
	gos_result_t 	taskGetNameResult 	= GOS_ERROR;
	u32_t			taskIndex			= 0u;

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
	gos_result_t 	taskGetIdResult = GOS_ERROR;
	u32_t			taskIndex		= 0u;

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
gos_result_t gos_kernelTaskGetCurrentId (gos_tid_t* taskId)
{
	/*
	 * Local variables.
	 */
	gos_result_t taskGetCurrentIdResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (taskId != NULL)
	{
		*taskId = (gos_tid_t)(GOS_DEFAULT_TASK_ID + currentTaskIndex);
		taskGetCurrentIdResult = GOS_SUCCESS;
	}

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
	gos_result_t	taskGetDataResult	= GOS_ERROR;
	u32_t			taskIndex			= 0u;

	/*
	 * Function code.
	 */
	if (taskId >= GOS_DEFAULT_TASK_ID && (taskId - GOS_DEFAULT_TASK_ID) < CFG_TASK_MAX_NUMBER &&
		taskData != NULL)
	{
		taskIndex = (u32_t)(taskId - GOS_DEFAULT_TASK_ID);

		memcpy((void*)taskData, (void*)&taskDescriptors[taskIndex], sizeof(*taskData));

		taskGetDataResult = GOS_SUCCESS;
	}

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
		subscriptionResult = gos_signalSubscribe(kernelTaskDeleteSignal, deleteSignalHandler);
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
		subscriptionResult = gos_signalSubscribe(kernelDumpReadySignal, dumpReadySignalHandler);
	}

	return subscriptionResult;
}

/*
 * Function: gos_kernelDump
 */
gos_result_t gos_kernelDump (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t kernelDumpResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	kernelDumpResult = gos_signalInvoke(kernelDumpSignal, 0);

	return kernelDumpResult;
}

/*
 * Function: SysTick_Handler
 */
void_t SysTick_Handler (void_t)
{
    /*
     * Function code.
     */
	sysTicks++;

#if CFG_SCHED_COOPERATIVE == 0
	if (schedulingDisabled == GOS_FALSE)
	{
		// Privileged.
		gos_kernelReschedule(GOS_PRIVILEGED);
	}
#endif
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
 * Function: gos_kernelGetTotalSysTime
 */
u64_t gos_kernelGetTotalSysTime (void_t)
{
	/*
	 * Function code.
	 */
	return totalSystemTime;
}

/*
 * Function: gos_kernelDumpSignalHandler
 */
void_t gos_kernelDumpSignalHandler (gos_signalSenderId_t senderId)
{
	/*
	 * Function code.
	 */
	if (senderId == GOS_DUMP_SENDER_USER)
	{
		gos_kernelTaskResume(kernelDumpTaskId);
	}
	else if (senderId == GOS_DUMP_SENDER_LAST)
	{
		gos_signalInvoke(kernelDumpReadySignal, 0);
	}
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
	u32_t tmrValue = 0u;

	/*
	 * Function code.
	 */
	tmrValue = gos_timerDriverSysTimerGet();
	while ((gos_timerDriverSysTimerGet() - tmrValue) < microseconds);
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
	while ((sysTicks - sysTickVal) < milliseconds);
}

/*
 * Function: SVC_Handler
 */
GOS_NAKED void_t SVC_Handler (void_t)
{
    /*
     * Function code.
     */
	// Check LR to know which stack is used.
	GOS_ASM("TST LR, 4");
	// 2 next instructions are conditional.
	GOS_ASM("ITE EQ");
	// Save MSP if bit 2 is 0.
	GOS_ASM("MRSEQ R0, MSP");
	// Save PSP if bit 2 is 1.
	GOS_ASM("MRSNE R0, PSP");

	// Check if reset is requried.
	if (resetRequired == GOS_TRUE)
	{
		resetRequired = GOS_FALSE;
		gos_kernelProcessorReset();
	}

	// Pass R0 as the argument.
	GOS_ASM("B gos_kernelSVC_HandlerMain");
}

/*
 * Function: gos_kernelSVC_HandlerMain
 */
void_t gos_kernelSVC_HandlerMain (u32_t* SP)
{
    /*
     * Function code.
     */
	// Get the address of the instruction saved in PC.
	u8_t* pInstruction = (u8_t*)(SP[6]);

	// Go back 2 bytes (16-bit opcode).
	pInstruction -= 2;

	// Get the opcode, in little-endian.
	u8_t svcNum = *pInstruction;

	switch (svcNum)
	{
		case 0xFF:
			// Trigger PendSV.
			ICSR |= (1 << 28);
			break;
		default: break;
	}
}

/*
 * Function: PendSV_Handler
 */
GOS_NAKED void_t PendSV_Handler (void_t)
{
    /*
     * Function code.
     */
	// Save LR back to main, must do this firstly.
	GOS_ASM("PUSH {LR}");

	// Save the context of current task.
	// Get current PSP.
	GOS_ASM("MRS R0, PSP");
	// Save R4 to R11 to PSP Frame Stack.
	GOS_ASM("STMDB R0!, {R4-R11}"); // R0 is updated after decrement
	// Save current value of PSP.
	GOS_ASM("BL gos_kernelSaveCurrentPsp"); // R0 is first argument

	// Do scheduling.
	// Select next task.
	GOS_ASM("BL gos_kernelSelectNextTask");

	// Retrieve the context of next task.
	// Get its past PSP value.
	GOS_ASM("BL gos_kernelGetCurrentPsp"); // return PSP is in R0
	// Retrieve R4-R11 from PSP Fram Stack.
	GOS_ASM("LDMIA R0!, {R4-R11}"); // R0 is updated after increment
	// Update PSP.
	GOS_ASM("MSR PSP, R0");

	// Exit.
	GOS_ASM("POP {LR}");
	GOS_ASM("BX LR");
}

/**
 * @brief	Checks the validity of the task descriptor.
 * @details	It checks the task function pointer, task priority value, stack, and
 * 			size parameters and return with error if the parameters are incorrect.
 *
 * @param	taskDescriptor	:	Pointer to the task descriptor structure.
 *
 * @return	Result of task descriptor checking.
 *
 * @retval	GOS_SUCCESS	:	Descriptor contains valid data.
 * @retval	GOS_ERROR	:	One or more parameter is invalid:
 * 							- Task function is NULL pointer
 * 							- Task function is the idle task
 * 							- Priority exceeds the maximum priority level
 * 							- Stack size is smaller than the minimum allowed
 * 							- Stack size is greater than the maximum allowed
 * 							- Stack size is not 4-byte aligned
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
	if (taskDescriptor->taskFunction == NULL 					||
		taskDescriptor->taskPrivilegeLevel == 0					||
		taskDescriptor->taskPriority > GOS_TASK_MAX_PRIO_LEVELS	||
		taskDescriptor->taskFunction == gos_kernelIdleTask		||
		taskDescriptor->taskStackSize > CFG_TASK_MAX_STACK_SIZE ||
		taskDescriptor->taskStackSize < CFG_TASK_MIN_STACK_SIZE	||
		taskDescriptor->taskStackSize % 4 != 0u)
	{
		taskDescCheckResult = GOS_ERROR;
	}

	return taskDescCheckResult;
}

/**
 * @brief	Reschedules the kernel.
 * @details	Based on the privilege, it invokes a kernel reschedule event.
 *
 * @param	privilege	: Privilege level.
 *
 * @return	-
 */
GOS_STATIC_INLINE void_t gos_kernelReschedule (gos_kernel_privilege_t privilege)
{
    /*
     * Function code.
     */
	if (privilege == GOS_PRIVILEGED)
	{
		// Trigger PendSV directly.
		ICSR |= (1 << 28);
	}
	else
	{
		// Call Supervisor exception to get Privileged access.
		GOS_ASM("SVC #255");
	}
}

/**
 * @brief	Returns the current PSP.
 * @details	Returns the current PSP.
 *
 * @return	Current PSP value.
 */
GOS_UNUSED GOS_STATIC u32_t gos_kernelGetCurrentPsp (void_t)
{
    /*
     * Function code.
     */
	return taskDescriptors[currentTaskIndex].taskPsp;
}

/**
 * @brief	Saves the current PSP.
 * @details	Saves the current PSP.
 *
 * @param	psp	:	Current PSP value.
 *
 * @return	-
 */
GOS_UNUSED GOS_STATIC void_t gos_kernelSaveCurrentPsp (u32_t psp)
{
    /*
     * Function code.
     */
	taskDescriptors[currentTaskIndex].taskPsp = psp;
}

/**
 * @brief	Selects the next task for execution.
 * @details	Loops through the internal task descriptor array and first checks
 * 			the sleeping tasks and wakes up the ones that passed their sleeping time.
 * 			Then based on the priority of the ready tasks, it selects the one with
 * 			the highest priority (lowest number in priority). If there is a swap-hook
 * 			function registered, it calls it, and then it refreshes the task run-time
 * 			statistics.
 *
 * @return	-
 */
GOS_UNUSED GOS_STATIC void_t gos_kernelSelectNextTask (void_t)
{
	/*
	 * Local variables.
	 */
	u16_t			taskIndex			= 0u;
	gos_taskPrio_t	lowestPrio			= GOS_TASK_IDLE_PRIO;
	u16_t			nextTask			= 0u;
	u64_t			currentRunTime		= 0u;

	/*
     * Function code.
     */
	if (schedulingDisabled == GOS_FALSE)
	{
		for (taskIndex = 0U; taskIndex < CFG_TASK_MAX_NUMBER; taskIndex++)
		{
			// Wake-up sleeping tasks if their sleep time has elapsed.
			if (taskDescriptors[taskIndex].taskState == GOS_TASK_SLEEPING &&
				sysTicks >= taskDescriptors[taskIndex].taskSleepTicks)
			{
				taskDescriptors[taskIndex].taskState = GOS_TASK_READY;
			}

			// Choose the highest priority task - that is not the current one, and is ready - to run.
			if (taskIndex != currentTaskIndex &&
				taskDescriptors[taskIndex].taskState == GOS_TASK_READY &&
				taskDescriptors[taskIndex].taskPriority < lowestPrio)
			{
				nextTask = taskIndex;
				lowestPrio = taskDescriptors[taskIndex].taskPriority;
			}
		}

		// If there was a task-swap, call the hook function.
		if (currentTaskIndex != nextTask && kernelSwapHookFunction != NULL)
		{
			kernelSwapHookFunction(taskDescriptors[currentTaskIndex].taskId, taskDescriptors[nextTask].taskId);
			taskDescriptors[currentTaskIndex].taskCsCounter++;
		}

		// Update run-time statistics.
		currentRunTime = (gos_timerDriverSysTimerGet() - sysTimerValue);
		// Increase system up-time.
		totalSystemTime += currentRunTime;
		taskDescriptors[currentTaskIndex].taskRunTime += currentRunTime;
		sysTimerValue = gos_timerDriverSysTimerGet();

		// Set current task.
		currentTaskIndex = nextTask;
	}
}

/**
 * @brief	Kernel idle task.
 * @details	This task is executed when there is no other ready task in the system.
 * 			When executed, this function refreshes the CPU-usage statistics of tasks.
 *
 * @return	-
 */
GOS_STATIC void_t gos_kernelIdleTask (void_t)
{
	/*
	 * Local variables.
	 */
	u16_t taskIndex = 0u;

	/*
	 * Function code.
	 */
	gos_traceInit("Starting OS...", GOS_SUCCESS);

	for (;;)
	{
		taskDescriptors[0].taskRunCounter++;
		if (kernelIdleHookFunction != NULL)
		{
			kernelIdleHookFunction();
		}
		for (taskIndex = 0u; taskIndex < CFG_TASK_MAX_NUMBER; taskIndex++)
		{
			taskDescriptors[taskIndex].taskCpuUsage = (u16_t)((10000 * taskDescriptors[taskIndex].taskRunTime) / totalSystemTime);
			if (taskDescriptors[taskIndex].taskFunction == NULL)
			{
				break;
			}
		}
		gos_kernelTaskYield();
	}
}

/**
 * @brief	Translates the task state to a string.
 * @details	Based on the task state it returns a string with a printable form
 * 			of the task state.
 *
 * @param 	taskState	:	The task state variable to be translated.
 *
 * @return	String with the task state.
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
			return LOG_FG_GREEN_START"ready    ";
		}break;
		case GOS_TASK_SLEEPING:
		{
			return LOG_FG_YELLOW_START"sleeping ";
		}break;
		case GOS_TASK_SUSPENDED:
		{
			return LOG_FG_MAGENTA_START"suspended";
		}break;
		case GOS_TASK_BLOCKED:
		{
			return LOG_FG_CYAN_START"blocked  ";
		}break;
		case GOS_TASK_ZOMBIE:
		{
			return LOG_FG_RED_START"zombie   ";
		}
		default:
		{
			return "";
		}
	}
}

#define BINARY_PATTERN "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s"
#define TO_BINARY(word)  \
	(word & 0x8000 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x4000 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x2000 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x1000 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x0800 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x0400 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x0200 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x0100 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x0080 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x0040 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x0020 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x0010 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x0008 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x0004 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x0002 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0"), \
	(word & 0x0001 ? LOG_FG_GREEN_START"1" : LOG_FG_RED_START"0")

/**
 * @brief	Kernel dump task.
 * @details	This task prints the kernel configuration and task data to
 * 			the log output. And suspends itself. This task is resumed by
 * 			the kernel dump signal.
 *
 * @return	-
 */
GOS_STATIC void_t gos_kernelDumpTask (void_t)
{
	/*
	 * Local variables.
	 */
	u16_t 	taskIndex = 0u;
#if CFG_KERNEL_DUMP_CONFIG == 1
	u8_t	confIndex = 0u;
#endif

	/*
	 * Function code.
	 */
	for (;;)
	{
#if CFG_KERNEL_DUMP_CONFIG == 1
		// Log configuration.
		gos_logLogFormatted("\nKernel configuration parameters:\r\n");
		gos_logLogFormatted(CONFIG_DUMP_SEPARATOR);
		gos_logLogFormatted(
				"| %32s | %6s |\r\n",
				"parameter",
				"value"
				);
		gos_logLogFormatted(CONFIG_DUMP_SEPARATOR);

		for (confIndex = 0u; confIndex < sizeof(kernelConfig) / sizeof(kernelConfig[0]); confIndex++)
		{
			gos_logLogFormatted(
					"| %32s | %6d |\r\n",
					kernelConfig[confIndex].configName,
					kernelConfig[confIndex].configValue
					);
		}
		gos_logLogFormatted(CONFIG_DUMP_SEPARATOR"\n");
#endif

		gos_logLogFormatted("Task dump:\r\n");
		gos_logLogFormatted(TASK_DUMP_SEPARATOR);
		gos_logLogFormatted(
			"| %6s | %28s | %6s | %4s | %16s | %6s | %9s |\r\n",
			"tid",
			"name",
			"stack",
			"prio",
			"privileges",
			"cpu [%]",
			"state"
			);
		gos_logLogFormatted(TASK_DUMP_SEPARATOR);

		for (taskIndex = 0u; taskIndex < CFG_TASK_MAX_NUMBER; taskIndex++)
		{
			if (taskDescriptors[taskIndex].taskFunction == NULL)
			{
				break;
			}
			gos_logLogFormatted(
					"| 0x%04X | %28s | 0x%04X | %4d | "BINARY_PATTERN LOG_FORMAT_RESET" | %4u.%02u | %9s "LOG_FORMAT_RESET"|\r\n",
					taskDescriptors[taskIndex].taskId,
					taskDescriptors[taskIndex].taskName,
					taskDescriptors[taskIndex].taskStackSize,
					taskDescriptors[taskIndex].taskPriority,
					TO_BINARY((u16_t)taskDescriptors[taskIndex].taskPrivilegeLevel),
					taskDescriptors[taskIndex].taskCpuUsage / 100,
					taskDescriptors[taskIndex].taskCpuUsage % 100,
					gos_kernelGetTaskStateString(taskDescriptors[taskIndex].taskState)
					);
		}
		gos_logLogFormatted(TASK_DUMP_SEPARATOR"\n");
		gos_signalInvoke(kernelDumpSignal, GOS_DUMP_SENDER_KERNEL);
		gos_kernelTaskSuspend(kernelDumpTaskId);
	}
}

/**
 * @brief	Kernel processor reset.
 * @details	Resets the processor.
 *
 * @return	-
 */
GOS_STATIC void_t gos_kernelProcessorReset (void_t)
{
	/*
	 * Function code.
	 */
	GOS_ASM ("dsb 0xF":::"memory");
	*(u32_t*)(0xE000ED0CUL) = (u32_t)((0x5FAUL << 16U) | (*(u32_t*)(0xE000ED0CUL) & (7UL << 8U)) | (1UL << 2U));
	GOS_ASM ("dsb 0xF":::"memory");

	// Wait for reset.
	for(;;)
	{
		GOS_NOP;
	}
}

/*
 * Fault handlers.
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
