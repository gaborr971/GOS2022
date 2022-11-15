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
//! @file		gos_shell.h
//! @author		Gabor Repasi
//! @date		2022-11-04
//! @version	1.0
//!
//! @brief		GOS shell service header.
//! @details	The shell service provides an easy interface to receive and process commands in a
//! 			terminal. Optionally the characters can be echoed back. The user can register
//! 			different commands with callback functions. When the enter key is hit, the shell
//! 			daemon processes the input. If it finds the input string in the command array, it
//! 			calls the corresponding callback function and passes the parameter list as a string
//! 			to the callback for further processing.
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-11-04	Gabor Repasi	Initial version created.
//*************************************************************************************************
#ifndef GOS_SHELL_H
#define GOS_SHELL_H
/*
 * Includes
 */
#include "gos_kernel.h"

/*
 * Type definitions
 */
/**
 * Shell function type.
 */
typedef void_t (*gos_shellFunction)(char_t* params);

/**
 * Shell command type.
 */
typedef struct
{
	char_t 				command  [CFG_SHELL_MAX_COMMAND_LENGTH];	//!< Command name.
	gos_shellFunction	commandHandler;								//!< Command handler function.
}gos_shellCommand_t;

#if CFG_SHELL_MAX_COMMAND_NUMBER < 255
typedef u8_t	gos_shellCommandIndex_t;	//!< Shell command index type.
#else
typedef u16_t	gos_shellCommandIndex_t;	//!< Shell command index type.
#endif

/*
 * Function prototypes
 */
/**
 * @brief	This function initializes the shell service.
 * @details	Initializes the internal command structure, and registers the
 * 			shell daemon task in the kernel.
 *
 * @return	Result of initialization.
 *
 * @retval	GOS_SUCCESS :	Shell initialization successful.
 * @retval	GOS_ERROR   :	Shell daemon task registration failed.
 */
gos_result_t gos_shellInit (void_t);

/**
 * @brief	This function registers a command in the shell.
 * @details	Checks the command structure and registers the command in the next
 * 			empty slot in the internal command array.
 *
 * @param	command	:	Pointer to the command structure to register.
 *
 * @return Result of shell command registration.
 *
 * @retval	GOS_SUCCESS :	Command registration successful.
 * @retval	GOS_ERROR   :	Command function or name is NULL or internal command
 * 							array is full.
 */
gos_result_t gos_shellRegisterCommand (gos_shellCommand_t* command);

/**
 * @brief	Suspends the shell daemon task.
 * @details	Suspends the shell daemon task.
 *
 * @return	Result of shell suspension.
 *
 * @retval	GOS_SUCCESS	:	Shell suspended successfully.
 * @retval	GOS_ERROR	:	Task suspension failed.
 */
gos_result_t gos_shellSuspend (void_t);

/**
 * @brief	Resumes the shell daemon task.
 * @details	Resumes the shell daemon task.
 *
 * @return	Result of shell resumption.
 *
 * @retval	GOS_SUCCESS	:	Shell resumed successfully.
 * @retval	GOS_ERROR	:	Task resumption failed.
 */
gos_result_t gos_shellResume (void_t);

/**
 * @brief	Turns the shell echoing on.
 * @details	Sets the internal echo flag.
 *
 * @return	Result of turning echoing on.
 *
 * @retval	GOS_SUCCESS	:	Echoing turned on successfully.
 */
gos_result_t gos_shellEchoOn (void_t);

/**
 * @brief	Turns the shell echoing off.
 * @details	Resets the internal echo flag.
 *
 * @return	Result of turning echoing off.
 *
 * @retval	GOS_SUCCESS	:	Echoing turned off successfully.
 */
gos_result_t gos_shellEchoOff (void_t);
#endif
