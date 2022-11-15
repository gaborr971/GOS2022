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
//! @file		gos_kernel.h
//! @author		Gabor Repasi
//! @date		2022-10-21
//! @version	1.1
//!
//! @brief		GOS kernel header.
//! @details	The GOS kernel is the core of the GOS system. It contains the basic type
//! 			definitions of the system (these are used across the OS in other services), and it
//! 			handles the system tick, tasks, and scheduling. The kernel module also provides an
//! 			interface for registering tasks, changing their states, registering hook functions,
//! 			disabling the scheduler. It also provides a service to dump the kernel
//! 			configuration and task information on the log output.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-21	Gabor Repasi	Initial version created.
// 1.1		2022-11-14	Gabor Repasi	+	Task previous state added to task descriptor type
//										+	Dump signal sender enumerators added
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
#define NULL ((void *)0) //! Null pointer.
#endif

/**
 * Default task ID.
 */
#define GOS_DEFAULT_TASK_ID			( (gos_tid_t)0x8000 )

/**
 * valid task ID.
 */
#define GOS_INVALID_TASK_ID			( (gos_tid_t)0x0100 )

/**
 * Static macro.
 */
#define GOS_STATIC					static

/**
 * Inline macro.
 */
#define GOS_INLINE					inline __attribute__((always_inline))

/**
 * Static in-line macro.
 */
#define GOS_STATIC_INLINE           static GOS_INLINE

//! Extern macro.
#define GOS_EXTERN					extern

/**
 * Naked.
 */
#define GOS_NAKED					__attribute__ ((naked))

/**
 * Unused.
 */
#define GOS_UNUSED					__attribute__ ((unused))

/**
 * NOP.
 */
#define GOS_NOP						__asm volatile ("NOP")

/**
 * ASM.
 */
#define GOS_ASM						__asm volatile

/**
 * Atomic operation enter - disable kernel rescheduling.
 */
#define GOS_ATOMIC_ENTER			{											\
										GOS_EXTERN bool_t schedulingDisabled;	\
										schedulingDisabled = GOS_TRUE;			\
									}

/**
 * Atomic operation exit - enable kernel rescheduling.
 */
#define GOS_ATOMIC_EXIT				{											\
										GOS_EXTERN bool_t schedulingDisabled;	\
										schedulingDisabled = GOS_FALSE;			\
									}
/*
 * Type definitions
 */

/*
 * Basic data types.
 */
typedef uint8_t  bool_t;	//!< Boolean logic type.
typedef uint8_t  u8_t;		//!< 8-bit unsigned type.
typedef uint16_t u16_t;		//!< 16-bit unsigned type.
typedef uint32_t u32_t;		//!< 32-bit unsigned type.
typedef uint64_t u64_t;		//!< 64-bit unsigned type.
typedef int8_t	 s8_t;		//!< 8-bit signed type.
typedef int16_t	 s16_t;		//!< 16-bit signed type.
typedef int32_t	 s32_t;		//!< 32-bit signed type.
typedef int64_t	 s64_t;		//!< 64-bit signed type.
typedef char     char_t;	//!< 8-bit character type.
typedef float    float_t;	//!< Single precision float type.
typedef double   double_t;	//!< Double precision float type.
typedef void     void_t;	//!< Void type.

/*
 * Task-related definitions.
 */
typedef u16_t	gos_tid_t;									//!< Task ID type.
typedef char_t	gos_taskName_t [CFG_TASK_MAX_NAME_LENGTH];	//!< Task name type.
typedef void_t  (*gos_task_t)(void_t);						//!< Task function type.
typedef u8_t	gos_taskPrio_t;								//!< Task priority type.
typedef u32_t	gos_taskSleepTick_t;						//!< Sleep tick type.
typedef u32_t	gos_taskPsp_t;								//!< PSP type.
typedef u32_t	gos_taskRunCounter_t;						//!< Run counter type.
typedef u64_t	gos_taskRunTime_t;							//!< Run-time type.
typedef u32_t	gos_taskCSCounter_t;						//!< Context-switch counter type.
typedef u16_t	gos_taskStackSize_t;						//!< Task stack size type.

/**
 *  Task state enumerator.
 *
 *  @note Hamming distance of
 *  - ready and sleeping:		3
 *  - ready and blocked:		3
 *  - ready and suspended:		4
 *  - ready and zombie:			3
 *  - sleeping and blocked:		4
 *  - sleeping and suspended:	3
 *  - sleeping and zombie:		4
 *  - blocked and suspended:	3
 *  - blocked and zombie:		2
 *  - suspended and zombie:		1
 */
typedef enum
{
	GOS_TASK_READY		= 0b01010,	//!< Task is ready to be scheduled.
	GOS_TASK_SLEEPING	= 0b10110,	//!< Task is sleeping (waiting for wake-up ticks).
	GOS_TASK_BLOCKED	= 0b11001,	//!< Task is blocked (waiting for resource).
	GOS_TASK_SUSPENDED	= 0b00101,	//!< Task is suspended (not to be scheduled), but can be resumed.
	GOS_TASK_ZOMBIE		= 0b01101	//!< Task deleted (physically existing in memory, but cannot be resumed).
}gos_taskState_t;

/*
 * Hook function type definitions.
 */
typedef void_t	(*gos_taskSwapHook_t		)(gos_tid_t, gos_tid_t	);	//!< Task swap hook type.
typedef void_t	(*gos_taskIdleHook_t		)(void_t				);	//!< Task idle hook type.
typedef void_t	(*gos_taskSleepHook_t		)(gos_tid_t				);	//!< Task sleep hook type.
typedef void_t	(*gos_taskWakeupHook_t		)(gos_tid_t				);	//!< Task wake-up hook type.
typedef void_t	(*gos_taskSuspendHook_t		)(gos_tid_t				);	//!< Task suspend hook type.
typedef void_t	(*gos_taskResumeHook_t		)(gos_tid_t				);	//!< Task resume hook type.
typedef void_t	(*gos_taskBlockHook_t		)(gos_tid_t				);	//!< Task block hook type.
typedef void_t	(*gos_taskUnblockHook_t		)(gos_tid_t				);	//!< Task unblock hook type.
typedef void_t	(*gos_taskDeleteHook_t		)(gos_tid_t				);	//!< Task delete hook type.

/**
 * Kernel reset function type.
 */
typedef void_t	(*gos_kernelReset_t			)(void_t				);

/**
 *  Result type enumerator.
 *  @note Hamming distance of
 *  - success and error:	7
 *  - success and busy:		4
 *  - error and busy:		5
 */
typedef enum
{
	GOS_SUCCESS	= 0b01010101,	//!< Success.
	GOS_ERROR	= 0b10101110,	//!< Error.
	GOS_BUSY	= 0b10110001 	//!< Busy.
}gos_result_t;

/**
 *  Boolean values.
 *  @note Hamming distance of true and false value: 7.
 */
typedef enum
{
	GOS_TRUE	= 0b00110110,	//!< True value.
	GOS_FALSE	= 0b01001001	//!< False value.
}gos_boolValue_t;

/**
 * Task descriptor structure.
 */
typedef struct
{
	gos_task_t				taskFunction;			//!< Task function.
	gos_taskState_t			taskState;				//!< Task state.
	gos_taskState_t			taskPreviousState;		//!< Task previous state (for restoration).
	gos_taskPrio_t			taskPriority;			//!< Task priority.
	gos_taskName_t			taskName;				//!< Task name.
	gos_tid_t				taskId;					//!< Task ID.
	gos_taskSleepTick_t		taskSleepTicks;			//!< Task sleep ticks.
	gos_taskPsp_t			taskPsp;				//!< Task PSP.
	gos_taskRunCounter_t	taskRunCounter;			//!< Task run counter.
	gos_taskRunTime_t		taskRunTime;			//!< Task run-time.
	gos_taskCSCounter_t		taskCsCounter;			//!< Task context-switch counter.
	gos_taskStackSize_t		taskStackSize;			//!< Task stack size.
	u8_t					taskCpuUsage;			//!< Task processor usage in [%].
}gos_taskDescriptor_t;

/**
 * Dump signal sender enumerator.
 */
typedef enum
{
	GOS_DUMP_SENDER_USER,		//!< Sender is user.
	GOS_DUMP_SENDER_KERNEL,		//!< Sender is the kernel.
	GOS_DUMP_SENDER_PROC,		//!< Sender is the process service.
	GOS_DUMP_SENDER_QUEUE,		//!< Sender is the queue.
	GOS_DUMP_SENDER_SIGNAL,		//!< Sender is the signal service.
	GOS_DUMP_SENDER_MESSAGE,	//!< Sender is the message service.
	GOS_DUMP_SENDER_SHELL,		//!< Sender is the shell service.
	GOS_DUMP_SENDER_TIME		//!< Sender is the time service.
}gos_dumpSignalSender_t;

/*
 * Function prototypes
 */

/**
 * @brief	This function initializes the kernel.
 * @details	Initializes the internal task array, fills out the PSP for the idle task,
 * 			and registers the kernel dump task and suspends it.
 *
 * @return	Result of initialization.
 *
 * @retval	GOS_SUCCESS	:	Kernel initialization successful.
 * @retval	GOS_ERROR	:	Kernel dump task or task suspension unsuccessful.
 */
gos_result_t gos_kernelInit (void_t);

/**
 * @brief	This function registers a task for scheduling.
 * @details	Checks the task descriptor parameters and then tries to find the next empty
 * 			slot in the internal task array. When it is found, it registers the task in
 * 			that slot.
 *
 * @param	taskDescriptor	:	Pointer to a task descriptor structure.
 * @param	taskId			:	Pointer to a variable to hold to assigned task ID value.
 *
 * @return	Result of task registration.
 *
 * @retval	GOS_SUCCESS	:	Task registered successfully.
 * @retval	GOS_ERROR	:	Invalid task descriptor (NULL function pointer,
 * 							invalid priority level, invalid stack size, idle task registration,
 * 							or stack size is not 4-byte-aligned) or task array is full.
 */
gos_result_t gos_kernelTaskRegister (gos_taskDescriptor_t* taskDescriptor, gos_tid_t* taskId);

/**
 * @brief	Sends the current task to sleeping state.
 * @details	Checks the current task and its state, modifies it to sleeping, and
 * 			if there is a sleep hook function registered, it calls it. Then, it
 * 			invokes a rescheduling.
 *
 * @param	sleepTicks	:	Minimum number of ticks until the task should
 * 							remain in sleeping state.
 *
 * @return	Result of task sleeping.
 *
 * @retval	GOS_SUCCESS	:	Task successfully sent to sleeping state.
 * @retval	GOS_ERROR	:	Function called from idle task or task state
 * 							is not ready.
 */
gos_result_t gos_kernelTaskSleep (gos_taskSleepTick_t sleepTicks);

/**
 * @brief	Wakes up the given task.
 * @details	Checks the current task and its state, modifies it to ready, and
 * 			if there is a wake-up hook function registered, it calls it.
 *
 * @param	taskId	:	ID of the task to be waken up.
 *
 * @return	Result of task wake-up.
 *
 * @retval	GOS_SUCCESS	:	Task waken up successfully.
 * @retval	GOS_ERROR	:	Task ID is invalid, or task is not sleeping.
 */
gos_result_t gos_kernelTaskWakeup (gos_tid_t taskId);

/**
 * @brief	Sends the given task to suspended state.
 * @details	Checks the given task ID and its state, modified it to suspended, and
 * 			if there is a suspend hook function registered, it calls it. If the
 * 			suspended function is the currently running one, it invokes a rescheduling.
 *
 * @param	taskId	:	ID of the task to be suspended.
 *
 * @return	Result of task suspension.
 *
 * @retval	GOS_SUCESS	:	Task suspended successfully.
 * @retval	GOS_ERROR	:	Task ID is invalid, or task state is not ready or sleeping.
 */
gos_result_t gos_kernelTaskSuspend (gos_tid_t taskId);

/**
 * @brief	Resumes the given task.
 * @details	Checks the given task ID and its state, modified it to ready, and
 * 			if there is a resume hook function registered, it calls it.
 *
 * @param	taskId	:	ID of the task to be resumed.
 *
 * @return	Result of task resumption.
 *
 * @retval	GOS_SUCESS	:	Task resumed successfully.
 * @retval	GOS_ERROR	:	Task ID is invalid, or task is not suspended.
 */
gos_result_t gos_kernelTaskResume (gos_tid_t taskId);

/**
 * @brief	Sends the given task to blocked state.
 * @details	Checks the given task ID and its state, modified it to blocked, and
 * 			if there is a block hook function registered, it calls it. If the blocked
 * 			function is the currently running one, it invokes a rescheduling.
 *
 * @param	taskId	:	ID of the task to be blocked.
 *
 * @return	Result of task blocking.
 *
 * @retval	GOS_SUCESS	:	Task blocked successfully.
 * @retval	GOS_ERROR	:	Task ID is invalid, or task state is not ready.
 */
gos_result_t gos_kernelTaskBlock (gos_tid_t taskId);

/**
 * @brief	Unblocks the given task.
 * @details	Checks the given task ID and its state, modified it to ready, and
 * 			if there is an unblock hook function registered, it calls it.
 *
 * @param	taskId	:	ID of the task to be unblocked.
 *
 * @return	Result of task unblocking.
 *
 * @retval	GOS_SUCESS	:	Task unblocked successfully.
 * @retval	GOS_ERROR	:	Task ID is invalid, or task is not blocked.
 */
gos_result_t gos_kernelTaskUnblock (gos_tid_t taskId);

/**
 * @brief	Deletes the given task from the scheduling array.
 * @details	Checks the given task ID and its state, modifies it to zombie, and
 *			if there is a delete hook function registered, it calls it.
 *
 * @param 	taskId	:	ID of the task to be deleted.
 *
 * @return	Result of deletion.
 *
 * @retval	GOS_SUCCESS	:	Task deleted successfully.
 * @retval	GOS_ERROR	:	Task is already a zombie.
 */
gos_result_t gos_kernelTaskDelete (gos_tid_t taskId);

/**
 * @brief	Gets the task name of the task with the given ID.
 * @details	Copies the task name corresponding with the given task ID to the task name
 * 			variable.
 *
 * @param 	taskId		:	Pointer to a task ID variable to store the returned ID.
 * @param 	taskName	:	Task name pointer to store the returned task name.
 *
 * @return	Success of task name get.
 *
 * @retval	GOS_SUCCESS	:	Task name found successfully.
 * @retval	GOS_ERROR	:	Invalid task ID or task name variable is NULL.
 */
gos_result_t gos_kernelTaskGetName (gos_tid_t taskId, gos_taskName_t taskName);

/**
 * @brief	Gets the task ID of the task with the given name.
 * @details	This function loops through the internal task array and tries to find the
 * 			given task name to get the corresponding task ID.
 *
 * @param 	taskName	:	Name of the task (string).
 * @param 	taskId		:	Pointer to a task ID variable to store the returned ID.
 *
 * @return	Success of task ID get.
 *
 * @retval	GOS_SUCCESS	:	Task ID found successfully.
 * @retval	GOS_ERROR	:	Task name not found.
 */
gos_result_t gos_kernelTaskGetId (gos_taskName_t taskName, gos_tid_t* taskId);

/**
 * @brief	Returns the ID of the currently running task.
 * @details	Returns the ID of the currently running task.
 *
 * @param 	taskId	:	Pointer to a task ID variable to store the current task ID.
 *
 * @return	Result of current task ID get.
 *
 * @retval	GOS_SUCCESS	:	Current task ID returned successfully.
 * @retval	GOS_ERROR	:	Task ID pointer is NULL.
 */
gos_result_t gos_kernelTaskGetCurrentId (gos_tid_t* taskId);

/**
 * @brief	Returns the task data of the given task.
 * @details	Based on the task ID, it copies the content of the internal task
 * 			descriptor array element to the given task descriptor.
 *
 * @param 	taskId		:	ID of the task to get the data of.
 * @param 	taskData	:	Pointer to the task descriptor to save the task data in.
 *
 * @return	Result of task data get.
 *
 * @retval	GOS_SUCCESS	:	Task data coopied successfully.
 * @retval	GOS_ERROR	:	Invalid task ID or task data pointer is NULL.
 */
gos_result_t gos_kernelTaskGetData (gos_tid_t taskId, gos_taskDescriptor_t* taskData);

/**
 * @brief	Yields the current task.
 * @details	Invokes a rescheduling.
 *
 * @return	Result of task yield.
 *
 * @retval	GOS_SUCCESS	:	Yield successful.
 */
gos_result_t gos_kernelTaskYield (void_t);

/**
 * @brief	Registers a task swap hook function.
 * @details	Checks whether the param is NULL pointer and a hook function is already
 * 			registered, and both conditions are false, it registers the hook function.
 *
 * @param 	swapHookFunction	:	Swap hook function.
 *
 * @return	Result of registration.
 *
 * @retval	GOS_SUCCESS	:	Registration successful.
 * @retval	GOS_ERROR	:	Registration failed (hook function already exists or parameter is NULL).
 */
gos_result_t gos_kernelRegisterSwapHook (gos_taskSwapHook_t swapHookFunction);

/**
 * @brief	Registers an idle hook function.
 * @details	Checks whether the param is NULL pointer and a hook function is already
 * 			registered, and both conditions are false, it registers the hook function.
 *
 * @param 	idleHookFunction	:	Idle hook function.
 *
 * @return	Result of registration.
 *
 * @retval	GOS_SUCCESS	:	Registration successful.
 * @retval	GOS_ERROR	:	Registration failed (hook function already exists or parameter is NULL).
 */
gos_result_t gos_kernelRegisterIdleHook (gos_taskIdleHook_t idleHookFunction);

/**
 * @brief	Registers a task sleep hook function.
 * @details	Checks whether the param is NULL pointer and a hook function is already
 * 			registered, and both conditions are false, it registers the hook function.
 *
 * @param 	sleepHookFunction	:	Sleep hook function.
 *
 * @return	Result of registration.
 *
 * @retval	GOS_SUCCESS	:	Registration successful.
 * @retval	GOS_ERROR	:	Registration failed (hook function already exists or parameter is NULL).
 */
gos_result_t gos_kernelRegisterTaskSleepHook (gos_taskSleepHook_t sleepHookFunction);

/**
 * @brief	Registers a task wake-up hook function.
 * @details	Checks whether the param is NULL pointer and a hook function is already
 * 			registered, and both conditions are false, it registers the hook function.
 *
 * @param 	wakeupHookFunction	: Wake-up hook function.
 *
 * @return	Result of registration.
 *
 * @retval	GOS_SUCCESS	:	Registration successful.
 * @retval	GOS_ERROR	:	Registration failed (hook function already exists or parameter is NULL).
 */
gos_result_t gos_kernelRegisterTaskWakeupHook (gos_taskWakeupHook_t wakeupHookFunction);

/**
 * @brief	Registers a task suspend hook function.
 * @details	Checks whether the param is NULL pointer and a hook function is already
 * 			registered, and both conditions are false, it registers the hook function.
 *
 * @param 	suspendHookFunction	:	Suspend hook function.
 *
 * @return	Result of registration.
 *
 * @retval	GOS_SUCCESS	:	Registration successful.
 * @retval	GOS_ERROR	:	Registration failed (hook function already exists or parameter is NULL).
 */
gos_result_t gos_kernelRegisterTaskSuspendHook (gos_taskSuspendHook_t suspendHookFunction);

/**
 * @brief	Registers a task resume hook function.
 * @details	Checks whether the param is NULL pointer and a hook function is already
 * 			registered, and both conditions are false, it registers the hook function.
 *
 * @param 	resumeHookFunction	:	Resume hook function.
 *
 * @return	Result of registration.
 *
 * @retval	GOS_SUCCESS	:	Registration successful.
 * @retval	GOS_ERROR	:	Registration failed (hook function already exists or parameter is NULL).
 */
gos_result_t gos_kernelRegisterTaskResumeHook (gos_taskResumeHook_t resumeHookFunction);

/**
 * @brief	Registers a task block hook function.
 * @details	Checks whether the param is NULL pointer and a hook function is already
 * 			registered, and both conditions are false, it registers the hook function.
 *
 * @param 	blockHookFunction	:	Block hook function.
 *
 * @return	Result of registration.
 *
 * @retval	GOS_SUCCESS	:	Registration successful.
 * @retval	GOS_ERROR	:	Registration failed (hook function already exists or parameter is NULL).
 */
gos_result_t gos_kernelRegisterTaskBlockHook (gos_taskBlockHook_t blockHookFunction);

/**
 * @brief	Registers a task unblock hook function.
 * @details	Checks whether the param is NULL pointer and a hook function is already
 * 			registered, and both conditions are false, it registers the hook function.
 *
 * @param 	unblockHookFunction	:	Unblock hook function.
 *
 * @return	Result of registration.
 *
 * @retval	GOS_SUCCESS	:	Registration successful.
 * @retval	GOS_ERROR	:	Registration failed (hook function already exists or parameter is NULL).
 */
gos_result_t gos_kernelRegisterTaskUnblockHook (gos_taskUnblockHook_t unblockHookFunction);

/**
 * @brief	Registers a task delete hook function.
 * @details	Checks whether the param is NULL pointer and a hook function is already
 * 			registered, and both conditions are false, it registers the hook function.
 *
 * @param 	deleteHookFunction	:	Delete hook function.
 *
 * @return	Result of registration.
 *
 * @retval	GOS_SUCCESS	:	Registration successful.
 * @retval	GOS_ERROR	:	Registration failed (hook function already exists or parameter is NULL).
 */
gos_result_t gos_kernelRegisterTaskDeleteHook (gos_taskDeleteHook_t deleteHookFunction);

/**
 * @brief	Invokes the kernel dump signal.
 * @details	Invokes the kernel dump signal.
 *
 * @return	Result of kernel dump.
 *
 * @retval	GOS_SUCCESS	:	Kernel dump signal invoking successful.
 * @retval	GOS_ERROR	:	Signal invoking failed.
 */
gos_result_t gos_kernelDump (void_t);

/**
 * @brief	Returns the system ticks.
 * @details	Returns the internal system tick counter value.
 *
 * @return	System ticks.
 */
u32_t		 gos_kernelGetSysTicks (void_t);

/**
 * @brief	Returns the total system run-time.
 * @details	Returns the internal total system run-time variable.
 *
 * @return	Total run-time in microseconds.
 */
u64_t 		 gos_kernelGetTotalSysTime (void_t);

/**
 * @brief	Starts the kernel.
 * @details	Prepares the PSP for the first task, changes to unprivileged level,
 * 			initializes the system timer value, and starts executing the first task.
 *
 * @return	Result of kernel start.
 *
 * @retval	GOS_ERROR	:	First task terminated.
 */
gos_result_t gos_kernelStart (void_t);

/**
 * @brief	Resets the microcontroller.
 * @details	Sets the reset required flag and gets privileged access to reset
 * 			the controller.
 *
 * @return	-
 */
void_t gos_kernelReset (gos_kernelReset_t resetFunction);

/**
 * @brief	Blocking delay in microsecond range.
 * @details	This function waits in a while loop until the given number
 * 			of milliseconds have elapsed based on the system timer.
 *
 * @param	microseconds	: Microseconds to wait.
 *
 * @return	-
 */
void_t gos_kernelDelayUs (u16_t microseconds);

/**
 * @brief	Blocking delay in millisecond range.
 * @details	This function waits in a while loop until the given number
 * 			of system ticks have elapsed.
 *
 * @param	milliseconds	:	Milliseconds to wait.
 *
 * @return	-
 */
void_t gos_kernelDelayMs (u16_t milliseconds);
#endif
