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
//! @date		2022-10-24
//! @version	1.0
//!
//! @brief		GOS time service source.
//! @details	For a more detailed description of this service, please refer to @ref gos_time.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-24	Gabor Repasi	Initial version created.
//*************************************************************************************************
/*
 * Includes
 */
#include "gos_time.h"

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
 * Time signal sender IDs. Each to distinguish which event triggered the signal.
 */
GOS_STATIC const gos_signalSenderId_t	secondElapsedSenderId 	= 0;	//!< Second elapsed sender ID.
GOS_STATIC const gos_signalSenderId_t	minuteElapsedSenderId 	= 1;	//!< Minute elapsed sender ID.
GOS_STATIC const gos_signalSenderId_t	hourElapsedSenderId 	= 2;	//!< Hour elapsed sender ID.
GOS_STATIC const gos_signalSenderId_t	dayElapsedSenderId 		= 3;	//!< Day elapsed sender ID.
GOS_STATIC const gos_signalSenderId_t	monthElapsedSenderId 	= 4;	//!< Month elapsed sender ID.
GOS_STATIC const gos_signalSenderId_t	yearElapsedSenderId		= 5;	//!< Year elapsed sender ID.

/*
 * Function prototypes
 */
GOS_STATIC void_t gos_timeDaemonTask (void_t);

/**
 * Time task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t timeDaemonTaskDesc =
{
	.taskFunction 	= gos_timeDaemonTask,
	.taskName		= "gos_time_task",
	.taskStackSize	= CFG_TASK_TIME_STACK,
	.taskPriority	= CFG_TASK_TIME_PRIO
};

/*
 * Function: gos_timeInit
 */
gos_result_t gos_timeInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t timeInitResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (gos_signalCreate(&timeSignalId) == GOS_SUCCESS &&
		gos_kernelTaskRegister(&timeDaemonTaskDesc, &timeDaemonTaskId) == GOS_SUCCESS)
	{
		timeInitResult = GOS_SUCCESS;
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
 * Function: gos_timeGetSecondElapsedSenderId
 */
const gos_signalSenderId_t gos_timeGetSecondElapsedSenderId (void_t)
{
	/*
	 * Function code.
	 */
	return secondElapsedSenderId;
}

/*
 * Function: gos_timeGetMinuteElapsedSenderId
 */
const gos_signalSenderId_t gos_timeGetMinuteElapsedSenderId (void_t)
{
	/*
	 * Function code.
	 */
	return minuteElapsedSenderId;
}

/*
 * Function: gos_timeGetHourElapsedSenderId
 */
const gos_signalSenderId_t gos_timeGetHourElapsedSenderId (void_t)
{
	/*
	 * Function code.
	 */
	return hourElapsedSenderId;
}

/*
 * Function: gos_timeGetDayElapsedSenderId
 */
const gos_signalSenderId_t gos_timeGetDayElapsedSenderId (void_t)
{
	/*
	 * Function code.
	 */
	return dayElapsedSenderId;
}

/*
 * Function: gos_timeGetMonthElapsedSenderId
 */
const gos_signalSenderId_t gos_timeGetMonthElapsedSenderId (void_t)
{
	/*
	 * Function code.
	 */
	return monthElapsedSenderId;
}

/*
 * Function: gos_timeGetYearElapsedSenderId
 */
const gos_signalSenderId_t gos_timeGetYearElapsedSenderId (void_t)
{
	/*
	 * Function code.
	 */
	return yearElapsedSenderId;
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
	gos_timeGet(&previousTime);

	for(;;)
	{
		// Increase time by 1 second.
		gos_timeAddSeconds(&systemTime, 1);

		// Invoke second elapsed signal.
		if (systemTime.seconds > previousTime.seconds)
		{
			gos_signalInvoke(timeSignalId, secondElapsedSenderId);
		}

		// Invoke minute elapsed signal.
		if (systemTime.minutes > previousTime.minutes)
		{
			gos_signalInvoke(timeSignalId, minuteElapsedSenderId);
		}

		// Invoke hour elapsed signal.
		if (systemTime.hours > previousTime.hours)
		{
			gos_signalInvoke(timeSignalId, hourElapsedSenderId);
		}

		// Invoke day elapsed signal.
		if (systemTime.days > previousTime.days)
		{
			gos_signalInvoke(timeSignalId, dayElapsedSenderId);
		}

		// Invoke month elapsed signal.
		if (systemTime.months > previousTime.months)
		{
			gos_signalInvoke(timeSignalId, monthElapsedSenderId);
		}

		// Invoke year elapsed signal.
		if (systemTime.years > previousTime.years)
		{
			gos_signalInvoke(timeSignalId, yearElapsedSenderId);
		}

		gos_kernelTaskSleep(1002);
	}
}
