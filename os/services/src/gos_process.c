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
//! @file		gos_process.c
//! @author		Gabor Repasi
//! @date		2022-11-15
//! @version	1.3
//!
//! @brief		GOS process service source.
//! @details	For a more detailed description of this service, please refer to @ref gos_process.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-22	Gabor Repasi	Initial version created.
// 1.1		2022-10-24	Gabor Repasi	+	Dump mechanism changed: it is now handled by a dedicated
// 											task (gos_procDumpTask)
// 1.2		2022-11-14	Gabor Repasi	+	If service is off, dump signal handler invokes the
//											signal with its own sender ID
// 1.3		2022-11-15	Gabor Repasi	+	License added
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
#include "gos_lock.h"
#include "gos_log.h"
#include "gos_process.h"
#include "gos_signal.h"
#include "gos_timer_driver.h"

#if CFG_PROC_USE_SERVICE == 1
/*
 * Macros
 */
/**
 * Dump separator line.
 */
#define DUMP_SEPARATOR	"+--------+------------------------------+---------------+-------------+---------+-----------+\r\n"

/*
 * Type definitions
 */
#if CFG_PROC_MAX_NUMBER < 255
typedef u8_t	gos_procIndex_t;	//!< Process index type.
#else
typedef u16_t	gos_procIndex_t;	//!< Process index type.
#endif

/*
 * Static variables
 */
/**
 * Current process.
 */
GOS_STATIC u32_t					currentProcIndex 		= 0u;

/**
 * Process idle hook function.
 */
GOS_STATIC gos_procIdleHook_t 		procIdleHookFunction 	= NULL;

/**
 * Process swap hook function.
 */
GOS_STATIC gos_procSwapHook_t		procSwapHookFunction	= NULL;

/**
 * Process resume hook function.
 */
GOS_STATIC gos_procResumeHook_t		procResumeHookFunction	= NULL;

/**
 * Process sleep hook function.
 */
GOS_STATIC gos_procSleepHook_t		procSleepHookFunction	= NULL;

/**
 * Process suspend hook function.
 */
GOS_STATIC gos_procSuspendHook_t	procSuspendHookFunction	= NULL;

/**
 * Process wake-up hook function.
 */
GOS_STATIC gos_procWakeupHook_t		procWakeupHookFunction	= NULL;

/**
 * Process daemon task ID.
 */
GOS_STATIC gos_tid_t				procDaemonTaskId		= GOS_INVALID_TASK_ID;

/**
 * Process dump task ID.
 */
GOS_STATIC gos_pid_t				procDumpTaskId			= GOS_INVALID_TASK_ID;

/**
 * Total process run-time (sum of each process run-time.
 */
GOS_STATIC u64_t					totalProcRunTime		= 0u;

/*
 * Module global functions
 */
/**
 * @brief	Handles the process dump signal.
 * @details	Resumes the process dump task based on the sender ID.
 *
 * @param	senderId	: Sender ID.
 *
 * @return -
 */
void_t gos_procDumpSignalHandler (gos_signalSenderId_t senderId);

/*
 * Function prototypes
 */
GOS_STATIC gos_result_t gos_procCheckProcDescriptor	(gos_procDescriptor_t* procDescriptor);
GOS_STATIC void_t 		gos_idleProc				(void_t);
GOS_STATIC void_t 		gos_procDaemonTask 			(void_t);
GOS_STATIC void_t		gos_procDumpTask	 		(void_t);
GOS_STATIC char_t* 		gos_procGetProcStateString 	(gos_procState_t procState);

/**
 * Internal process array.
 */
GOS_STATIC gos_procDescriptor_t	procDescriptors [CFG_PROC_MAX_NUMBER] = {
	[0] =
		{
			.procFunction	= gos_idleProc,
			.procState		= GOS_PROC_READY,
			.procId 		= GOS_DEFAULT_PROC_ID,
			.procPriority	= CFG_PROC_IDLE_PRIO,
			.procName		= "gos_idle_proc"
		}
};

/**
 * Process dump task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t procDumpTaskDesc =
{
	.taskFunction 	= gos_procDumpTask,
	.taskName		= "gos_proc_dump_task",
	.taskPriority	= CFG_TASK_PROC_DUMP_PRIO,
	.taskStackSize	= CFG_TASK_PROC_DUMP_STACK
};

/**
 * Process daemon task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t	processDaemonTaskDesc =
{
	.taskFunction 	= gos_procDaemonTask,
	.taskName		= "gos_process_daemon",
	.taskStackSize	= CFG_TASK_PROC_DAEMON_STACK,
	.taskPriority	= CFG_TASK_PROC_DAEMON_PRIO
};

/*
 * Function: gos_procInit
 */
gos_result_t gos_procInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t 	initResult 	= GOS_ERROR;
	gos_procIndex_t	procIndex	= 1u;

	/*
	 * Function code.
	 */

    // Initialize process descriptors.
    for (procIndex = 1u; procIndex < CFG_PROC_MAX_NUMBER; procIndex++)
    {
    	procDescriptors[procIndex].procFunction	= NULL;
    	procDescriptors[procIndex].procPriority	= CFG_PROC_MAX_PRIO_LEVELS;
    	procDescriptors[procIndex].procState	= GOS_PROC_SUSPENDED;
    	procDescriptors[procIndex].procId		= GOS_INVALID_PROC_ID;
    }

    if (gos_kernelTaskRegister(&processDaemonTaskDesc, &procDaemonTaskId) == GOS_SUCCESS &&
    	gos_kernelTaskRegister(&procDumpTaskDesc, &procDumpTaskId) == GOS_SUCCESS &&
    	gos_kernelTaskSuspend(procDumpTaskId) == GOS_SUCCESS)
    {
    	initResult = GOS_SUCCESS;
    }

    return initResult;
}

/*
 * Function: gos_procRegister
 */
gos_result_t gos_procRegister (gos_procDescriptor_t* procDescriptor, gos_pid_t* procId)
{
	/*
	 * Local variables.
	 */
	gos_result_t	procRegisterResult	= GOS_SUCCESS;
	gos_procIndex_t	procIndex			= 0u;

	/*
	 * Function code.
	 */
	// Check descriptor structure.
	if (gos_procCheckProcDescriptor(procDescriptor) == GOS_ERROR)
	{
		procRegisterResult = GOS_ERROR;
	}
	else
	{
		// Find empty slot.
		for (procIndex = 1u; procIndex < CFG_PROC_MAX_NUMBER; procIndex++)
		{
			if (procDescriptors[procIndex].procFunction == NULL)
			{
				break;
			}
		}
		// Check if empty slot was found.
		if (procIndex >= CFG_PROC_MAX_NUMBER)
		{
			procRegisterResult = GOS_ERROR;
		}
		else
		{
			// Initial state.
			procDescriptors[procIndex].procState		= GOS_PROC_READY;
			procDescriptors[procIndex].procFunction		= procDescriptor->procFunction;
			procDescriptors[procIndex].procPriority		= procDescriptor->procPriority;
			procDescriptors[procIndex].procId			= (GOS_DEFAULT_PROC_ID + procIndex);

			// Copy process name.
			if (procDescriptor->procName != NULL &&
				strlen(procDescriptor->procName) <= CFG_PROC_MAX_NAME_LENGTH)
			{
				strcpy(procDescriptors[procIndex].procName, procDescriptor->procName);
			}

			// Set process ID.
			procDescriptor->procId = procDescriptors[procIndex].procId;
			if (procId != NULL)
			{
				*procId = procDescriptors[procIndex].procId;
			}
		}
	}

	return procRegisterResult;
}

/*
 * Function: gos_procSleep
 */
gos_result_t gos_procSleep (gos_procSleepTick_t sleepTicks)
{
	/*
	 * Local variables.
	 */
	gos_result_t procSleepResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (currentProcIndex > 0u)
	{
		GOS_ATOMIC_ENTER
		{
			if (procDescriptors[currentProcIndex].procState	== GOS_PROC_READY)
			{
				procDescriptors[currentProcIndex].procState			= GOS_PROC_SLEEPING;
				procDescriptors[currentProcIndex].procSleepTicks	= gos_kernelGetSysTicks() + sleepTicks;
				procSleepResult = GOS_SUCCESS;
			}
		}
		GOS_ATOMIC_EXIT

		// Call hook.
		if (procSleepResult == GOS_SUCCESS && procSleepHookFunction != NULL)
		{
			procSleepHookFunction((GOS_DEFAULT_PROC_ID + currentProcIndex));
		}
	}
	return procSleepResult;
}

/*
 * Function: gos_procWakeup
 */
gos_result_t gos_procWakeup (gos_pid_t procId)
{
	/*
	 * Local variables.
	 */
	gos_result_t	procWakeupResult	= GOS_ERROR;
	gos_procIndex_t	procIndex			= 0u;

	/*
	 * Function code.
	 */
	if (procId > GOS_DEFAULT_PROC_ID && (procId - GOS_DEFAULT_PROC_ID) < CFG_PROC_MAX_NUMBER)
	{
		procIndex = (u32_t)(procId - GOS_DEFAULT_PROC_ID);

		GOS_ATOMIC_ENTER
		{
			if (procDescriptors[procIndex].procState == GOS_PROC_SLEEPING)
			{
				procDescriptors[procIndex].procState = GOS_PROC_READY;
				procWakeupResult = GOS_SUCCESS;
			}
		}
		GOS_ATOMIC_EXIT

		// Call hook.
		if (procWakeupResult == GOS_SUCCESS && procWakeupHookFunction != NULL)
		{
			procWakeupHookFunction(procId);
		}
	}
	return procWakeupResult;
}

/*
 * Function: gos_procSuspend
 */
gos_result_t gos_procSuspend (gos_pid_t procId)
{
	/*
	 * Local variables.
	 */
	gos_result_t 	procSuspendResult 	= GOS_ERROR;
	gos_procIndex_t	procIndex			= 0u;

	/*
	 * Function code.
	 */
	if (procId > GOS_DEFAULT_PROC_ID && (procId - GOS_DEFAULT_PROC_ID) < CFG_PROC_MAX_NUMBER)
	{
		procIndex = (u32_t)(procId - GOS_DEFAULT_PROC_ID);

		GOS_ATOMIC_ENTER
		{
			if (procDescriptors[procIndex].procState == GOS_PROC_READY ||
				procDescriptors[procIndex].procState == GOS_PROC_SLEEPING)
			{
				procDescriptors[procIndex].procState = GOS_PROC_SUSPENDED;
				procSuspendResult = GOS_SUCCESS;
			}
		}
		GOS_ATOMIC_EXIT

		// Call hook.
		if (procSuspendResult == GOS_SUCCESS && procSuspendHookFunction != NULL)
		{
			procSuspendHookFunction(procId);
		}
	}

	return procSuspendResult;
}

/*
 * Function: gos_procResume
 */
gos_result_t gos_procResume (gos_pid_t procId)
{
	/*
	 * Local variables.
	 */
	gos_result_t 	procResumeResult 	= GOS_ERROR;
	gos_procIndex_t	procIndex			= 0u;

	/*
	 * Function code.
	 */
	if (procId > GOS_DEFAULT_PROC_ID && (procId - GOS_DEFAULT_PROC_ID) < CFG_PROC_MAX_NUMBER)
	{
		procIndex = (u32_t)(procId - GOS_DEFAULT_PROC_ID);

		GOS_ATOMIC_ENTER
		{
			if (procDescriptors[procIndex].procState == GOS_PROC_SUSPENDED)
			{
				procDescriptors[procIndex].procState = GOS_PROC_READY;
				procResumeResult = GOS_SUCCESS;
			}
		}
		GOS_ATOMIC_EXIT

		// Call hook.
		if (procResumeResult == GOS_SUCCESS && procResumeHookFunction != NULL)
		{
			procResumeHookFunction(procId);
		}
	}

	return procResumeResult;
}

/*
 * Function: gos_procGetName
 */
gos_result_t gos_procGetName (gos_pid_t procId, gos_procName_t procName)
{
	/*
	 * Local variables.
	 */
	gos_result_t 	procGetNameResult 	= GOS_ERROR;
	gos_procIndex_t	procIndex			= 0u;

	/*
	 * Function code.
	 */
	if (procId > GOS_DEFAULT_PROC_ID && (procId - GOS_DEFAULT_PROC_ID) < CFG_PROC_MAX_NUMBER &&
		procName != NULL)
	{
		procIndex = (u32_t)(procId - GOS_DEFAULT_PROC_ID);

		strcpy(procName, procDescriptors[procIndex].procName);

		procGetNameResult = GOS_SUCCESS;
	}

	return procGetNameResult;
}

/*
 * Function: gos_procGetId
 */
gos_result_t gos_procGetId (gos_procName_t procName, gos_pid_t* procId)
{
	/*
	 * Local variables.
	 */
	gos_result_t 	procGetIdResult = GOS_ERROR;
	gos_procIndex_t	procIndex		= 0u;

	/*
	 * Function code.
	 */
	for (procIndex = 0u; procIndex < CFG_PROC_MAX_NUMBER && procId != NULL; procIndex++)
	{
		if (strcmp(procName, procDescriptors[procIndex].procName) == 0u)
		{
			*procId = procDescriptors[procIndex].procId;
			procGetIdResult = GOS_SUCCESS;
			break;
		}
	}

	return procGetIdResult;
}

/*
 * Function: gos_procGetData
 */
gos_result_t gos_procGetData (gos_pid_t procId, gos_procDescriptor_t* procData)
{
	/*
	 * Local variables.
	 */
	gos_result_t 	procGetDataResult 	= GOS_ERROR;
	gos_procIndex_t	procIndex			= 0u;

	/*
	 * Function code.
	 */
	if (procId >= GOS_DEFAULT_PROC_ID && (procId - GOS_DEFAULT_PROC_ID) < CFG_PROC_MAX_NUMBER &&
		procData != NULL)
	{
		procIndex = (u32_t)(procId - GOS_DEFAULT_PROC_ID);

		memcpy((void*)procData, (void*)&procDescriptors[procIndex], sizeof(*procData));

		procGetDataResult = GOS_SUCCESS;
	}

	return procGetDataResult;
}

/*
 * Function: gos_procRegisterSwapHook
 */
gos_result_t gos_procRegisterSwapHook (gos_procSwapHook_t swapHookFunction)
{
	/*
	 * Local variables.
	 */
	gos_result_t hookRegisterResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (swapHookFunction != NULL && procSwapHookFunction == NULL)
	{
		procSwapHookFunction = swapHookFunction;
		hookRegisterResult = GOS_SUCCESS;
	}

	return hookRegisterResult;
}

/*
 * Function: gos_procRegisterIdleHook
 */
gos_result_t gos_procRegisterIdleHook (gos_procIdleHook_t idleHookFunction)
{
	/*
	 * Local variables.
	 */
	gos_result_t hookRegisterResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (idleHookFunction != NULL && procIdleHookFunction == NULL)
	{
		procIdleHookFunction = idleHookFunction;
		hookRegisterResult = GOS_SUCCESS;
	}

	return hookRegisterResult;
}

/*
 * Function: gos_procRegisterSleepHook
 */
gos_result_t gos_procRegisterSleepHook (gos_procSleepHook_t sleepHookFunction)
{
	/*
	 * Local variables.
	 */
	gos_result_t hookRegisterResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (sleepHookFunction != NULL && procSleepHookFunction == NULL)
	{
		procSleepHookFunction = sleepHookFunction;
		hookRegisterResult = GOS_SUCCESS;
	}

	return hookRegisterResult;
}

/*
 * Function: gos_procRegisterWakeupHook
 */
gos_result_t gos_procRegisterWakeupHook (gos_procWakeupHook_t wakeupHookFunction)
{
	/*
	 * Local variables.
	 */
	gos_result_t hookRegisterResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (wakeupHookFunction != NULL && procWakeupHookFunction == NULL)
	{
		procWakeupHookFunction = wakeupHookFunction;
		hookRegisterResult = GOS_SUCCESS;
	}

	return hookRegisterResult;
}

/*
 * Function: gos_procRegisterSuspendHook
 */
gos_result_t gos_procRegisterSuspendHook (gos_procSuspendHook_t suspendHookFunction)
{
	/*
	 * Local variables.
	 */
	gos_result_t hookRegisterResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (suspendHookFunction != NULL && procSuspendHookFunction == NULL)
	{
		procSuspendHookFunction = suspendHookFunction;
		hookRegisterResult = GOS_SUCCESS;
	}

	return hookRegisterResult;
}

/*
 * Function: gos_procRegisterResumeHook
 */
gos_result_t gos_procRegisterResumeHook (gos_procResumeHook_t resumeHookFunction)
{
	/*
	 * Local variables.
	 */
	gos_result_t hookRegisterResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (resumeHookFunction != NULL && procResumeHookFunction == NULL)
	{
		procResumeHookFunction = resumeHookFunction;
		hookRegisterResult = GOS_SUCCESS;
	}

	return hookRegisterResult;
}
#endif

/*
 * Function: gos_procDumpSignalHandler
 */
void_t gos_procDumpSignalHandler (gos_signalSenderId_t senderId)
{
	/*
	 * Function code.
	 */
#if CFG_PROC_USE_SERVICE == 1
	if (senderId == GOS_DUMP_SENDER_KERNEL)
	{
		gos_kernelTaskResume(procDumpTaskId);
	}
#else
	if (senderId == GOS_DUMP_SENDER_KERNEL)
	{
		GOS_EXTERN gos_signalId_t kernelDumpSignal;
		gos_signalInvoke(kernelDumpSignal, GOS_DUMP_SENDER_PROC);
	}
#endif
}

#if CFG_PROC_USE_SERVICE == 1
/**
 * @brief	Checks the process descriptor.
 * @details	Returns with error if the process function is NULL, the priority is invalid,
 * 			or the process function is the idle process.
 *
 * @param 	procDescriptor : Pointer to the process descriptor structure.
 *
 * @return	Check result.
 *
 * @retval	@ref GOS_SUCCESS 	: Process descriptor valid.
 * @retval	@ref GOS_ERROR		: Process descriptor invalid.
 */
GOS_STATIC gos_result_t gos_procCheckProcDescriptor	(gos_procDescriptor_t* procDescriptor)
{
	/*
	 * Local variables.
	 */
	gos_result_t procDescCheckResult = GOS_SUCCESS;

    /*
     * Function code.
     */
	if (procDescriptor->procFunction == NULL 					||
		procDescriptor->procPriority > CFG_PROC_MAX_PRIO_LEVELS	||
		procDescriptor->procFunction == gos_idleProc
		)
	{
		procDescCheckResult = GOS_ERROR;
	}

	return procDescCheckResult;
}

/**
 * @brief	Idle process.
 * @details	Increases the idle process run counter and calls the idle process hook function
 * 			(if registered).
 * @return	-
 */
GOS_STATIC void_t gos_idleProc (void_t)
{
	/*
	 * Local variables.
	 */
	gos_procIndex_t procIndex = 0u;

	/*
	 * Function code.
	 */
	procDescriptors[0].procRunCounter++;
	if (procIdleHookFunction != NULL)
	{
		procIdleHookFunction();
	}
	for (procIndex = 0u; procIndex < CFG_PROC_MAX_NUMBER; procIndex++)
	{
		procDescriptors[procIndex].procCpuUsage = 100 * procDescriptors[procIndex].procRunTime / totalProcRunTime;
		if (procDescriptors[procIndex].procFunction == NULL)
		{
			break;
		}
	}
}

/**
 * @brief	Process daemon task.
 * @details	Polls the process states and selects the next process to run based on
 * 			process states and priorities. Calls the next process function and calculates
 * 			the related run statistics.
 * @return	-
 */
GOS_STATIC void_t gos_procDaemonTask (void_t)
{
	/*
	 * Local variables.
	 */
	gos_procIndex_t	procIndex			= 0u;
	u16_t			currentProc 		= currentProcIndex;
	gos_taskPrio_t	lowestPrio			= CFG_PROC_IDLE_PRIO;
	u16_t			nextProc			= 0u;
	u32_t 			sysTimerCurrVal		= 0u;
	u64_t			currentRunTime		= 0u;

	/*
     * Function code.
     */
	for (;;)
	{
		currentProc = currentProcIndex;
		lowestPrio	= CFG_PROC_IDLE_PRIO;
		nextProc	= 0u;

		for (procIndex = 0U; procIndex < CFG_PROC_MAX_NUMBER; procIndex++)
		{
			// Wake-up sleeping processes if their sleep time has elapsed.
			if (procDescriptors[procIndex].procState == GOS_PROC_SLEEPING &&
				gos_kernelGetSysTicks() >= procDescriptors[procIndex].procSleepTicks)
			{
				procDescriptors[procIndex].procState = GOS_PROC_READY;
			}

			// Choose the highest priority process - that is not the current one, and is ready - to run.
			if (procIndex != currentProc &&
				procDescriptors[procIndex].procFunction != NULL &&
				procDescriptors[procIndex].procState == GOS_PROC_READY &&
				procDescriptors[procIndex].procPriority < lowestPrio)
			{
				nextProc = procIndex;
				lowestPrio = procDescriptors[procIndex].procPriority;
			}
		}

		// If there was a process-swap, call the hook function.
		if (currentProcIndex != nextProc && procSwapHookFunction != NULL)
		{
			procSwapHookFunction(procDescriptors[currentProc].procId, procDescriptors[nextProc].procId);
		}

		currentProcIndex = nextProc;

		// Call selected process.
		sysTimerCurrVal = gos_timerDriverSysTimerGet();
		procDescriptors[currentProcIndex].procFunction();
		procDescriptors[currentProcIndex].procRunCounter++;
		currentRunTime = (gos_timerDriverSysTimerGet() - sysTimerCurrVal);
		procDescriptors[currentProcIndex].procRunTime += currentRunTime;
		totalProcRunTime += currentRunTime;

		gos_kernelTaskSleep(1);
	}
}

/**
 * @brief	Translates the process state variable to a string.
 * @details	Returns the corresponding string for the process state enumerator.
 *
 * @param 	procState	: The process state to be translated.
 *
 * @return	String based on the process state.
 */
GOS_STATIC char_t* gos_procGetProcStateString (gos_procState_t procState)
{
	/*
	 * Function code.
	 */
	switch (procState)
	{
		case GOS_PROC_READY:
		{
			return LOG_FG_GREEN_START"ready    ";
		}break;
		case GOS_PROC_SLEEPING:
		{
			return LOG_FG_YELLOW_START"sleeping " ;
		}break;
		case GOS_PROC_SUSPENDED:
		{
			return LOG_FG_MAGENTA_START"suspended";
		}break;
		default:
		{
			return "";
		}
	}
}

/**
 * @brief	Process dump task.
 * @details	Prints the process data of all processes to the log output and suspends itself.
 * 			This task is resumed by the kernel dump signal (through its handler function).
 *
 * @return	-
 */
GOS_STATIC void_t gos_procDumpTask (void_t)
{
	/*
	 * Local variables.
	 */
	u16_t 						procIndex = 0u;
	GOS_EXTERN gos_signalId_t 	kernelDumpSignal;

	/*
	 * Function code.
	 */
	for (;;)
	{
		gos_logLogFormatted("Process dump:\r\n");
		gos_logLogFormatted(DUMP_SEPARATOR);
		gos_logLogFormatted(
				"| %6s | %28s | %13s | %11s | %7s | %9s |\r\n",
				"pid",
				"name",
				"prio",
				"time [us]",
				"[%]",
				"state"
			);
		gos_logLogFormatted(DUMP_SEPARATOR);

		for (procIndex = 0u; procIndex < CFG_PROC_MAX_NUMBER; procIndex++)
		{
			if (procDescriptors[procIndex].procFunction == NULL)
			{
				break;
			}
			gos_logLogFormatted(
					"| 0x%04X | %28s | %13d | %11lu | %7d | %9s "LOG_FORMAT_RESET"|\r\n",
					procDescriptors[procIndex].procId,
					procDescriptors[procIndex].procName,
					procDescriptors[procIndex].procPriority,
					(u32_t)procDescriptors[procIndex].procRunTime,
					procDescriptors[procIndex].procCpuUsage,
					gos_procGetProcStateString(procDescriptors[procIndex].procState)
					);
		}
		gos_logLogFormatted(DUMP_SEPARATOR"\n");
		gos_signalInvoke(kernelDumpSignal, GOS_DUMP_SENDER_PROC);
		gos_kernelTaskSuspend(procDumpTaskId);
	}
}
#endif
