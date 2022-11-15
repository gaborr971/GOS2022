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
//! @file		gos_tmr.h
//! @author		Gabor Repasi
//! @date		2022-11-14
//! @version	1.1
//!
//! @brief		GOS timer driver header.
//! @details	This is the timer driver skeleton. It contains the required interface functions for
//! 			the OS. These functions shall be implemented by the user in order to be able to use
//! 			microsecond delay and task run-time monitoring.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-11-10	Gabor Repasi	Initial version created.
// 1.1		2022-11-14	Gabor Repasi	+	API documentation added for functions
//										+	Header description added
//*************************************************************************************************
#ifndef GOS_TMR_H
#define GOS_TMR_H
/*
 * Includes
 */
#include "gos_kernel.h"

/*
 * Type definitions
 */
/**
 * System timer initializer function type.
 */
typedef gos_result_t	(*gos_timerSysTimerInit_t	)(void_t);

/**
 * System timer value get function type.
 */
typedef u32_t			(*gos_timerSysTimerGet_t	)(void_t);

/**
 * Driver function list type.
 */
typedef struct
{
	gos_timerSysTimerInit_t	sysTimerInitFunction;	//!< Initializer function.
	gos_timerSysTimerGet_t	sysTimerGetFunction;	//!< Transmit function.
}gos_timerFunctions_t;

/*
 * Function prototypes
 */
/**
 * @brief	Sets up the timer driver.
 * @details	This function is used to register the user timer driver that is
 * 			required by the OS for microsecond delay and task run-time monitoring.
 *
 * @param	timerFunctions	: Set of timer driver function pointers.
 *
 * @return	Result of driver setup.
 *
 * @retval	GOS_SUCCESS	: Driver setup successful.
 * @retval	GOS_ERROR	: At least one of the driver functions is NULL pointer.
 */
gos_result_t gos_timerDriverSetup (gos_timerFunctions_t* timerFunctions);

/**
 * @brief	System timer initializer skeleton.
 * @details	If registered, it calls the custom system timer initializer function.
 *
 * @return	Result of system timer initialization.
 *
 * @retval	GOS_SUCCESS	: According to user implementation.
 * @retval	GOS_ERROR	: According to user implementation / function not registered.
 */
gos_result_t gos_timerSysTimerInit (void_t);

/**
 * @brief	System timer value getter skeleton.
 * @details	If registered, it calls the custom system timer getter function.
 *
 * @return	Result of system timer value getting.
 *
 * @retval	GOS_SUCCESS	: According to user implementation.
 * @retval	GOS_ERROR	: According to user implementation / function not registered.
 */
u32_t gos_timerSysTimerGet (void_t);
#endif
