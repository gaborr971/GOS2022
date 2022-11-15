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
//! @file		gos_time.h
//! @author		Gabor Repasi
//! @date		2022-10-24
//! @version	1.0
//!
//! @brief		GOS time service header.
//! @details	TODO
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-24	Gabor Repasi	Initial version created.
//*************************************************************************************************
#ifndef GOS_TIME_H
#define GOS_TIME_H
/*
 * Includes
 */
#include "gos_kernel.h"
#include "gos_signal.h"

/*
 * Type definitions
 */
typedef u8_t	gos_second_t;	//!< Second type.
typedef u8_t	gos_minute_t;	//!< Minute type.
typedef u8_t	gos_hour_t;		//!< Hour type.
typedef u8_t	gos_day_t;		//!< Day type.
typedef	u8_t	gos_month_t;	//!< Month type.
typedef u16_t	gos_year_t;		//!< Year type.

/**
 * Time type.
 */
typedef struct
{
	gos_second_t	seconds;	//!< Seconds.
	gos_minute_t	minutes;	//!< Minutes.
	gos_hour_t		hours;		//!< Hours.
	gos_day_t		days;		//!< Days.
	gos_month_t		months;		//!< Months.
	gos_year_t		years;		//!< Years.
}gos_time_t;

/**
 * Time comparison result enumerator.
 */
typedef enum
{
	GOS_TIME_EARLIER,		//!< First time is earlier than second.
	GOS_TIME_LATER,			//!< First time is later than second.
	GOS_TIME_EQUAL			//!< Times are equal.
}gos_timeComprareResult_t;

/**
 * Month enumerator.
 */
typedef enum
{
	GOS_TIME_JANUARY = 1,	//!< January.
	GOS_TIME_FEBRUARY,		//!< February.
	GOS_TIME_MARCH,			//!< March.
	GOS_TIME_APRIL,			//!< April.
	GOS_TIME_MAY,			//!< May.
	GOS_TIME_JUNE,			//!< June.
	GOS_TIME_JULY,			//!< July.
	GOS_TIME_AUGUST,		//!< August.
	GOS_TIME_SEPTEMBER,		//!< September.
	GOS_TIME_OCTOBER,		//!< October.
	GOS_TIME_NOVEMBER,		//!< November.
	GOS_TIME_DECEMBER,		//!< December.

	GOS_TIME_NUMBER_OF_MONTHS = 12	//!< Number of months in a year.
}gos_monthEnum_t;

/*
 * Global variables
 */
/**
 * Time signal ID. This signal is invoked when a second has elapsed.
 */
gos_signalId_t	timeSignalId;

/*
 * Function prototypes
 */
//! TODO
gos_result_t gos_timeInit (void_t);

//! TODO
gos_result_t gos_timeGet (gos_time_t* pTime);

//! TODO
gos_result_t gos_timeSet (gos_time_t* pTime);

//! TODO
gos_result_t gos_timeCompare (gos_time_t* pTime1, gos_time_t* pTime2, gos_timeComprareResult_t* result);

//! TODO
gos_result_t gos_timeAddSeconds (gos_time_t* pTime, gos_second_t seconds);

//! TODO
const gos_signalSenderId_t gos_timeGetSecondElapsedSenderId (void_t);

//! TODO
const gos_signalSenderId_t gos_timeGetMinuteElapsedSenderId (void_t);

//! TODO
const gos_signalSenderId_t gos_timeGetHourElapsedSenderId (void_t);

//! TODO
const gos_signalSenderId_t gos_timeGetDayElapsedSenderId (void_t);

//! TODO
const gos_signalSenderId_t gos_timeGetMonthElapsedSenderId (void_t);

//! TODO
const gos_signalSenderId_t gos_timeGetYearElapsedSenderId (void_t);
#endif
