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
//! @file		gos_time.c
//! @author		Gabor Repasi
//! @date		2022-12-08
//! @version	1.2
//!
//! @brief		GOS time service source.
//! @details	For a more detailed description of this service, please refer to @ref gos_time.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-24	Gabor Repasi	Initial version created.
// 1.1		2022-11-15	Gabor Repasi	+	License added
//										-	Elapsed sender ID getter API functions removed
//										+	New elapsed sender ID enumerators are used
// 1.2		2022-12-08	Gabor Repasi	+	Run-time handling added
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
#include "gos_time.h"
#include "gos_error.h"

/*
 * Macros
 */
/**
 * Default year.
 */
#define TIME_DEFAULT_YEAR	( 2022 )

/**
 * Default month.
 */
#define TIME_DEFAULT_MONTH	( GOS_TIME_JANUARY )

/**
 * Default day.
 */
#define TIME_DEFAULT_DAY	( 1 )

/*
 * Static variables
 */
/**
 * System time.
 */
GOS_STATIC gos_time_t systemTime =
{
	.years 	= TIME_DEFAULT_YEAR,
	.months	= TIME_DEFAULT_MONTH,
	.days	= TIME_DEFAULT_DAY
};

/**
 * System run-time.
 */
GOS_STATIC gos_runtime_t systemRunTime;

/**
 * Number of days in each month - lookup table.
 */
GOS_STATIC const gos_day_t dayLookupTable [GOS_TIME_NUMBER_OF_MONTHS] =
{
		[GOS_TIME_JANUARY	- 1]	= 31,
		[GOS_TIME_FEBRUARY	- 1]	= 28,
		[GOS_TIME_MARCH 	- 1]	= 31,
		[GOS_TIME_APRIL 	- 1]	= 30,
		[GOS_TIME_MAY 		- 1]	= 31,
		[GOS_TIME_JUNE 		- 1]	= 30,
		[GOS_TIME_JULY 		- 1]	= 31,
		[GOS_TIME_AUGUST 	- 1]	= 31,
		[GOS_TIME_SEPTEMBER	- 1]	= 30,
		[GOS_TIME_OCTOBER 	- 1]	= 31,
		[GOS_TIME_NOVEMBER 	- 1]	= 30,
		[GOS_TIME_DECEMBER 	- 1]	= 31
};

/**
 * Time task ID.
 */
GOS_STATIC gos_tid_t timeDaemonTaskId;

/*
 * Function prototypes
 */
GOS_STATIC void_t gos_timeDaemonTask (void_t);

/**
 * Time task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t timeDaemonTaskDesc =
{
	.taskFunction 		= gos_timeDaemonTask,
	.taskName			= "gos_time_daemon",
	.taskStackSize		= CFG_TASK_TIME_DAEMON_STACK,
	.taskPriority		= CFG_TASK_TIME_DAEMON_PRIO,
	.taskPrivilegeLevel	= GOS_TASK_PRIVILEGE_KERNEL
};

/*
 * Function: gos_timeInit
 */
gos_result_t gos_timeInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t timeInitResult = GOS_SUCCESS;

	/*
	 * Function code.
	 */
	if (gos_signalCreate(&timeSignalId) != GOS_SUCCESS)
	{
		(void_t) gos_errorHandler(GOS_ERROR_LEVEL_OS_WARNING, __func__, __LINE__, "Time signal registration failed.");
		timeInitResult = GOS_ERROR;
	}

	if (gos_kernelTaskRegister(&timeDaemonTaskDesc, &timeDaemonTaskId) != GOS_SUCCESS)
	{
		(void_t) gos_errorHandler(GOS_ERROR_LEVEL_OS_WARNING, __func__, __LINE__, "Time daemon task registration failed.");
		timeInitResult = GOS_ERROR;
	}

	return timeInitResult;
}

/*
 * Function: gos_timeGet
 */
gos_result_t gos_timeGet (gos_time_t* pTime)
{
	/*
	 * Local variables.
	 */
	gos_result_t timeGetResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (pTime != NULL)
	{
		pTime->seconds 	= systemTime.seconds;
		pTime->minutes 	= systemTime.minutes;
		pTime->hours	= systemTime.hours;
		pTime->days		= systemTime.days;
		pTime->months	= systemTime.months;
		pTime->years	= systemTime.years;

		timeGetResult = GOS_SUCCESS;
	}

	return timeGetResult;
}

/*
 * Function: gos_timeSet
 */
gos_result_t gos_timeSet (gos_time_t* pTime)
{
	/*
	 * Local variables.
	 */
	gos_result_t timeSetResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (pTime != NULL)
	{
		systemTime.seconds 	= pTime->seconds;
		systemTime.minutes 	= pTime->minutes;
		systemTime.hours	= pTime->hours;
		systemTime.days		= pTime->days;
		systemTime.months	= pTime->months;
		systemTime.years	= pTime->years;

		timeSetResult = GOS_SUCCESS;
	}

	return timeSetResult;
}

/*
 * Function: gos_runTimeGet
 */
gos_result_t gos_runTimeGet (gos_runtime_t* pRunTime)
{
	/*
	 * Local variables.
	 */
	gos_result_t runtimeGetResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (pRunTime != NULL)
	{
		pRunTime->seconds 	= systemRunTime.seconds;
		pRunTime->minutes 	= systemRunTime.minutes;
		pRunTime->hours		= systemRunTime.hours;
		pRunTime->days		= systemRunTime.days;

		runtimeGetResult = GOS_SUCCESS;
	}

	return runtimeGetResult;
}

/*
 * Function: gos_timeCompare
 */
gos_result_t gos_timeCompare (gos_time_t* pTime1, gos_time_t* pTime2, gos_timeComprareResult_t* result)
{
	/*
	 * Local variables.
	 */
	gos_result_t timeCompareResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (pTime1 != NULL && pTime2 != NULL && result != NULL)
	{
		if (pTime1->hours	== pTime2->hours 	&&
			pTime1->minutes == pTime2->minutes 	&&
			pTime1->seconds == pTime2->seconds)
		{
			*result = GOS_TIME_EQUAL;
		}
		else if (pTime1->hours < pTime2->hours || (
			pTime1->hours == pTime2->hours &&
			pTime1->minutes < pTime2->minutes) || (
			pTime1->hours == pTime2->hours &&
			pTime1->minutes == pTime2->minutes &&
			pTime1->seconds < pTime2->seconds))
		{
			*result = GOS_TIME_EARLIER;
		}
		else
		{
			*result = GOS_TIME_LATER;
		}
		timeCompareResult = GOS_SUCCESS;
	}

	return timeCompareResult;
}

/*
 * Function: gos_timeAddSeconds
 */
gos_result_t gos_timeAddSeconds (gos_time_t* pTime, gos_second_t seconds)
{
	/*
	 * Local variables.
	 */
	gos_result_t	timeAddSecondsResult	= GOS_ERROR;
	gos_second_t	secondCounter			= 0u;

	/*
	 * Function code.
	 */
	if (pTime != NULL)
	{
		while (secondCounter++ < seconds)
		{
			pTime->seconds++;
			if (pTime->seconds >= 60)
			{
				pTime->seconds = 0U;
				pTime->minutes++;

				// Check minutes.
				if (pTime->minutes >= 60)
				{
					pTime->minutes = 0U;
					pTime->hours++;

					// Check hours.
					if (pTime->hours >= 24)
					{
						pTime->hours = 0U;
						pTime->days++;

						// Check days.
						if (pTime->years % 4 == 0  && pTime->months == GOS_TIME_FEBRUARY && pTime->days >= 30)
						{
							pTime->days = 1U;
							pTime->months++;
						}
						else if (pTime->years % 4 == 0 && pTime->months == GOS_TIME_FEBRUARY)
						{
							// Wait.
						}
						else if (pTime->days >= (dayLookupTable[pTime->months - 1] + 1))
						{
							pTime->days = 1U;
							pTime->months++;
						}

						// Check months.
						if (pTime->months == 13)
						{
							pTime->months = 1;
							pTime->years++;
						}
					}
				}
			}
		}
		timeAddSecondsResult = GOS_SUCCESS;
	}

	return timeAddSecondsResult;
}

/*
 * Function: gos_runTimeAddSeconds
 */
gos_result_t gos_runTimeAddSeconds (gos_runtime_t* pRunTime, gos_second_t seconds)
{
	/*
	 * Local variables.
	 */
	gos_result_t	runtimeAddSecondsResult	= GOS_ERROR;
	gos_second_t	secondCounter			= 0u;

	/*
	 * Function code.
	 */
	if (pRunTime != NULL)
	{
		while (secondCounter++ < seconds)
		{
			pRunTime->seconds++;
			if (pRunTime->seconds >= 60)
			{
				pRunTime->seconds = 0U;
				pRunTime->minutes++;

				// Check minutes.
				if (pRunTime->minutes >= 60)
				{
					pRunTime->minutes = 0U;
					pRunTime->hours++;

					// Check hours.
					if (pRunTime->hours >= 24)
					{
						pRunTime->hours = 0U;
						pRunTime->days++;
					}
				}
			}
		}
		runtimeAddSecondsResult = GOS_SUCCESS;
	}

	return runtimeAddSecondsResult;
}

/**
 * @brief	Time daemon task.
 * @details	Increases the system time approximately every second and invokes the
 * 			elapsed signals.
 *
 * @return	-
 */
GOS_STATIC void_t gos_timeDaemonTask (void_t)
{
	/*
	 * Local variables.
	 */
	gos_time_t previousTime = {0};

	/*
	 * Function code.
	 */
	// Initialize previous time.
	(void_t) gos_timeGet(&previousTime);

	for(;;)
	{
		// Increase time by 1 second.
		(void_t) gos_timeAddSeconds(&systemTime, 1);

		// Increase run-time by 1 second.
		(void_t) gos_runTimeAddSeconds(&systemRunTime, 1);

		// Invoke second elapsed signal.
		if (systemTime.seconds > previousTime.seconds)
		{
			(void_t) gos_signalInvoke(timeSignalId, GOS_TIME_SECOND_ELAPSED_SENDER_ID);
		}

		// Invoke minute elapsed signal.
		if (systemTime.minutes > previousTime.minutes)
		{
			(void_t) gos_signalInvoke(timeSignalId, GOS_TIME_MINUTE_ELAPSED_SENDER_ID);
		}

		// Invoke hour elapsed signal.
		if (systemTime.hours > previousTime.hours)
		{
			(void_t) gos_signalInvoke(timeSignalId, GOS_TIME_HOUR_ELAPSED_SENDER_ID);
		}

		// Invoke day elapsed signal.
		if (systemTime.days > previousTime.days)
		{
			(void_t) gos_signalInvoke(timeSignalId, GOS_TIME_DAY_ELAPSED_SENDER_ID);
		}

		// Invoke month elapsed signal.
		if (systemTime.months > previousTime.months)
		{
			(void_t) gos_signalInvoke(timeSignalId, GOS_TIME_MONTH_ELAPSED_SENDER_ID);
		}

		// Invoke year elapsed signal.
		if (systemTime.years > previousTime.years)
		{
			(void_t) gos_signalInvoke(timeSignalId, GOS_TIME_YEAR_ELAPSED_SENDER_ID);
		}

		(void_t) gos_kernelTaskSleep(1000);
	}
}
