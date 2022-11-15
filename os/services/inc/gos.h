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
//! @file		gos.h
//! @author		Gabor Repasi
//! @date		2022-11-14
//! @version	1.1
//!
//! @brief		GOS header.
//! @details	This header is a wrapper for the inclusion of all OS services and drivers.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-10-28	Gabor Repasi	Initial version created.
// 1.1		2022-11-14	Gabor Repasi	+	Header description added
//										+	Function descriptions added
//*************************************************************************************************
#ifndef GOS_H
#define GOS_H
/*
 * Includes
 */
#include "gos_drv.h"
#include "gos_kernel.h"
#include "gos_lock.h"
#include "gos_log.h"
#include "gos_message.h"
#include "gos_process.h"
#include "gos_queue.h"
#include "gos_signal.h"
#include "gos_shell.h"
#include "gos_time.h"

/*
 * Function prototypes
 */
/**
 * @brief	OS initializer function.
 * @details	Calls the necessary driver and service initializer functions.
 *
 * @return	Result of OS initialization.
 *
 * @retval	GOS_SUCCESS	: OS initialization successful.
 * @retval	GOS_ERROR	: At least one driver or service initialization failed.
 */
gos_result_t gos_Init (void_t);

/**
 * @brief	Starts the OS.
 * @details	Checks whether the initializer function has set the error flag to GOS_FALSE,
 * 			and if so, it starts the kernel (and thus the scheduling of tasks).
 *
 * @return	Result of OS starting.
 *
 * @retval	GOS_ERROR	: OS not started due to initialization error or kernel start error.
 *
 * @remark	This function should only return with error. If the initialization is successful,
 * 			the function is not expected to return.
 */
gos_result_t gos_Start (void_t);
#endif
