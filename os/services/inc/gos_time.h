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
//! @file       gos_time.h
//! @author     Gabor Repasi
//! @date       2022-12-11
//! @version    1.4
//!
//! @brief      GOS time service header.
//! @details    Time service provides an easy interface to manipulate time structures, track the
//!             passage of time.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2022-10-24    Gabor Repasi    Initial version created
// 1.1        2022-11-15    Gabor Repasi    +    License added
//                                          *    gos_monthEnum_t renamed to gos_timeMonthEnum_t
//                                          +    gos_timeElapsedSenderId_t type added
//                                          -    Elapsed sender ID getter API functions removed
// 1.2        2022-12-03    Gabor Repasi    +    Function header skeletons added
// 1.3        2022-12-08    Gabor Repasi    +    Run-time type added
// 1.4        2022-12-11    Gabor Repasi    +    Function descriptions completed
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
#ifndef GOS_TIME_H
#define GOS_TIME_H
/*
 * Includes
 */
#include <gos_kernel.h>
#include <gos_signal.h>

/*
 * Type definitions
 */
typedef u8_t    gos_second_t;   //!< Second type.
typedef u8_t    gos_minute_t;   //!< Minute type.
typedef u8_t    gos_hour_t;     //!< Hour type.
typedef u8_t    gos_day_t;      //!< Day type.
typedef u8_t    gos_month_t;    //!< Month type.
typedef u16_t   gos_year_t;     //!< Year type.

/**
 * Time type.
 */
typedef struct
{
    gos_second_t seconds;    //!< Seconds.
    gos_minute_t minutes;    //!< Minutes.
    gos_hour_t   hours;      //!< Hours.
    gos_day_t    days;       //!< Days.
    gos_month_t  months;     //!< Months.
    gos_year_t   years;      //!< Years.
}gos_time_t;

/**
 * Run-time type.
 */
typedef struct
{
    gos_second_t seconds; //!< Seconds.
    gos_minute_t minutes; //!< Minutes.
    gos_hour_t   hours;   //!< Hours.
    gos_day_t    days;    //!< Days.
}gos_runtime_t;

/**
 * Time comparison result enumerator.
 */
typedef enum
{
    GOS_TIME_EARLIER, //!< First time is earlier than second.
    GOS_TIME_LATER,   //!< First time is later than second.
    GOS_TIME_EQUAL    //!< Times are equal.
}gos_timeComprareResult_t;

/**
 * Month enumerator.
 */
typedef enum
{
    GOS_TIME_JANUARY = 1,          //!< January.
    GOS_TIME_FEBRUARY,             //!< February.
    GOS_TIME_MARCH,                //!< March.
    GOS_TIME_APRIL,                //!< April.
    GOS_TIME_MAY,                  //!< May.
    GOS_TIME_JUNE,                 //!< June.
    GOS_TIME_JULY,                 //!< July.
    GOS_TIME_AUGUST,               //!< August.
    GOS_TIME_SEPTEMBER,            //!< September.
    GOS_TIME_OCTOBER,              //!< October.
    GOS_TIME_NOVEMBER,             //!< November.
    GOS_TIME_DECEMBER,             //!< December.

    GOS_TIME_NUMBER_OF_MONTHS = 12 //!< Number of months in a year.
}gos_timeMonthEnum_t;

/**
 * Time elapsed sender IDs.
 */
typedef enum
{
    GOS_TIME_SECOND_ELAPSED_SENDER_ID,  //!< Second elapsed sender ID.
    GOS_TIME_MINUTE_ELAPSED_SENDER_ID,  //!< Minute elapsed sender ID.
    GOS_TIME_HOUR_ELAPSED_SENDER_ID,    //!< Hour elapsed sender ID.
    GOS_TIME_DAY_ELAPSED_SENDER_ID,     //!< Day elapsed sender ID.
    GOS_TIME_MONTH_ELAPSED_SENDER_ID,   //!< Month elapsed sender ID.
    GOS_TIME_YEAR_ELAPSED_SENDER_ID     //!< Year elapsed sender ID.
}gos_timeElapsedSenderId_t;

/*
 * Global variables
 */
/**
 * Time signal ID. This signal is invoked when a second has elapsed.
 */
gos_signalId_t timeSignalId;

/*
 * Function prototypes
 */
/**
 * @brief   This function initializes the time service.
 * @details Creates the time signal and registers the time daemon in the kernel.
 *
 * @return  Result of initialization.
 *
 * @retval  GOS_SUCCESS : Initialization successful.
 * @retval  GOS_ERROR   : Time signal registration error or time daemon registration error.
 */
gos_result_t gos_timeInit (void_t);

/**
 * @brief   This function gets the system time.
 * @details This function copies the system time value to the given time variable.
 *
 * @param   pTime       : Pointer to a time variable to store the system time value in.
 *
 * @return  Result of time getting.
 *
 * @retval  GOS_SUCCESS : Time getting successful.
 * @retval  GOS_ERROR   : Time variable is NULL pointer.
 */
gos_result_t gos_timeGet (gos_time_t* pTime);

/**
 * @brief   This function sets the system time.
 * @details This function copies the time value from the given time variable to the
 *          system time variable.
 *
 * @param   pTime       : Pointer to a time variable holding the desired time value.
 *
 * @return  Result of time setting.
 *
 * @retval  GOS_SUCCESS : Time setting successful.
 * @retval  GOS_ERROR   : Time structure is NULL pointer.
 */
gos_result_t gos_timeSet (gos_time_t* pTime);

/**
 * @brief   This function compares two time structures.
 * @details This function compares two time structures.
 *
 * @param   pTime1      : Pointer to the first time variable.
 * @param   pTime2      : Pointer to the second time variable.
 * @param   result      : Pointer to the comparison result variable.
 *
 * @return  Result of time comparison.
 *
 * @retval  GOS_SUCCESS : Time comparison successful.
 * @retval  GOS_ERROR   : Either time structure and/or result variable is NULL pointer.
 */
gos_result_t gos_timeCompare (gos_time_t* pTime1, gos_time_t* pTime2, gos_timeComprareResult_t* result);

/**
 * @brief   This function adds the given number of seconds to the given time variable.
 * @details This function adds the given number of seconds to the given time variable.
 *
 * @param   pTime       : Pointer to the time variable.
 * @param   seconds     : Number of seconds to add.
 *
 * @return  Result of time increasing.
 *
 * @retval  GOS_SUCCESS : Seconds added successfully.
 * @retval  GOS_ERROR   : Time variable is NULL pointer.
 */
gos_result_t gos_timeAddSeconds (gos_time_t* pTime, gos_second_t seconds);

/**
 * @brief   This function adds the given number of seconds to the given run-time variable.
 * @details This function adds the given number of seconds to the given run-time variable.
 *
 * @param   pRunTime    : Pointer to a run-time variable.
 * @param   seconds     : Number of seconds to add.
 *
 * @return  Result of time increasing.
 *
 * @retval  GOS_SUCCESS : Seconds added successfully.
 * @retval  GOS_ERROR   : Run-time variable is NULL pointer.
 */
gos_result_t gos_runTimeAddSeconds (gos_runtime_t* pRunTime, gos_second_t seconds);

/**
 * @brief   This function gets the system run-time.
 * @details This function gets the system run-time.
 *
 * @param   pRunTime    : Pointer to a run-time variable to store the system run-time in.
 *
 * @return  Result of run-time getting.
 *
 * @retval  GOS_SUCCESS : Run-time getting is successful.
 * @retval  GOS_ERROR   : Run-time variable is NULL pointer.
 */
gos_result_t gos_runTimeGet (gos_runtime_t* pRunTime);
#endif
