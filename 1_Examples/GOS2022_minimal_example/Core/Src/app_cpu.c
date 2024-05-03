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
//                                      (c) Ahmed Gazar, 2024
//
//*************************************************************************************************
//! @file       app_cpu.c
//! @author     Ahmed Gazar
//! @date       2024-05-03
//! @version    1.0
//!
//! @brief      CPU load monitor component source.
//! @details    This component contains the initializer and task function for the CPU load monitor.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-05-03    Ahmed Gazar     Initial version created.
//*************************************************************************************************
//
// Copyright (c) 2024 Ahmed Gazar
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
#include "gos.h"

/*
 * Macros
 */
/**
 * CPU task sleep time in [ms].
 */
#define CPU_TASK_SLEEP_TIME_MS ( 128u )

/**
 * Number of CPU load measurements for average calculation.
 */
#define CPU_LOAD_SAMPLES       ( 16u  )

/*
 * Static function prototypes
 */
GOS_STATIC void_t APP_CpuTask (void_t);

/**
 * CPU task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t cpuTaskDesc =
{
	.taskFunction       = APP_CpuTask,
	.taskStackSize      = 0x200,
	.taskPriority       = 0,
	.taskName           = "app_cpu_task",
	.taskPrivilegeLevel = GOS_TASK_PRIVILEGED_USER
};

/*
 * CPU initializer
 *
 * Registers the CPU task in the scheduler.
 */
gos_result_t APP_CpuInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t cpuInitResult = GOS_SUCCESS;

	/*
	 * Function code.
	 */
	cpuInitResult = gos_taskRegister(&cpuTaskDesc, NULL);

	return cpuInitResult;
}

/*
 * CPU task
 *
 * Every 10 milliseconds, it traces the current CPU load.
 */
GOS_STATIC void_t APP_CpuTask (void_t)
{
	/*
	 * Local variables.
	 */
	u16_t cpuLoad = 0u;
	u16_t loadArray [CPU_LOAD_SAMPLES];
	u8_t  measIndex = 0u;

	/*
	 * Function code.
	 */
	for (;;)
	{
		loadArray[measIndex++ % CPU_LOAD_SAMPLES] = gos_kernelGetCpuUsage();
		cpuLoad = 0u;

		for (u8_t idx = 0u; idx < CPU_LOAD_SAMPLES; idx++)
		{
			cpuLoad += loadArray[idx];
		}

		cpuLoad /= CPU_LOAD_SAMPLES;

		(void_t) gos_traceTraceFormatted(GOS_TRUE, "CPU load: %3u.%02u%%\r", cpuLoad / 100, cpuLoad % 100);
		(void_t) gos_taskSleep(CPU_TASK_SLEEP_TIME_MS);
	}
}
