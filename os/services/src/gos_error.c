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
//! @file		gos_error.c
//! @author		Gabor Repasi
//! @date		2022-12-13
//! @version	1.1
//!
//! @brief		GOS error handler service source.
//! @details	For a more detailed description of this service, please refer to @ref gos_error.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-12-10	Gabor Repasi	Initial version created.
// 1.1		2022-12-13	Gabor Repasi	*	Continuous scheduling disabling added to error handler
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
#include "gos_error.h"
#include "gos_timer_driver.h"
#include <stdio.h>

/*
 * Macros
 */
/**
 * Separator line.
 */
#define SEPARATOR_LINE "+------------------------------------------------------------------------+\r\n"

/*
 * Static variables
 */
/**
 * Buffer for error message formatting.
 */
GOS_STATIC char_t errorBuffer [80];


/*
 * Function: gos_printStartupLogo
 */
void_t gos_printStartupLogo (void_t)
{
	/*
	 * Function code.
	 */
	(void_t) gos_logLogFormattedUnsafe("**************************************************************************\r\n");
	(void_t) gos_logLogFormattedUnsafe("\r\n");
	(void_t) gos_logLogFormattedUnsafe("                 #####             #####             #####                \r\n");
	(void_t) gos_logLogFormattedUnsafe("               #########         #########         #########              \r\n");
	(void_t) gos_logLogFormattedUnsafe("              ##                ##       ##       ##                      \r\n");
	(void_t) gos_logLogFormattedUnsafe("             ##                ##         ##        #####                 \r\n");
	(void_t) gos_logLogFormattedUnsafe("             ##     #####      ##         ##           #####              \r\n");
	(void_t) gos_logLogFormattedUnsafe("              ##       ##       ##       ##                ##             \r\n");
	(void_t) gos_logLogFormattedUnsafe("               #########         #########         #########              \r\n");
	(void_t) gos_logLogFormattedUnsafe("                 #####             #####             #####                \r\n");
	(void_t) gos_logLogFormattedUnsafe("                                                                          \r\n");
	(void_t) gos_logLogFormattedUnsafe("                           (c) Gabor Repasi, 2022                         \r\n");
	(void_t) gos_logLogFormattedUnsafe("\r\n");
	(void_t) gos_logLogFormattedUnsafe("**************************************************************************\r\n");
}

/*
 * Function: gos_errorHandler
 */
void_t gos_errorHandler (gos_errorLevel_t errorLevel, const char_t* function, u32_t line, const char_t* errorMessage, ...)
{
	/*
	 * Local variables.
	 */
	va_list args;

	/*
	 * Function code.
	 */
	(void_t) gos_logLogFormattedUnsafe(SEPARATOR_LINE);

	if (errorLevel == GOS_ERROR_LEVEL_OS_FATAL)
	{
		(void_t) gos_logLogFormattedUnsafe(
				LOG_FG_RED_START
				"OS-level error - system stopped.\r\n"
				LOG_FORMAT_RESET
				);
	}
	else if (errorLevel == GOS_ERROR_LEVEL_OS_WARNING)
	{
		(void_t) gos_logLogFormattedUnsafe(
				LOG_FG_YELLOW_START
				"OS-level error - warning.\r\n"
				LOG_FORMAT_RESET
				);
	}
	else if (errorLevel == GOS_ERROR_LEVEL_USER_FATAL)
	{
		(void_t) gos_logLogFormattedUnsafe(
				LOG_FG_RED_START
				"User-level error - system stopped.\r\n"
				LOG_FORMAT_RESET
				);
	}
	else if (errorLevel == GOS_ERROR_LEVEL_USER_WARNING)
	{
		(void_t) gos_logLogFormattedUnsafe(
				LOG_FG_YELLOW_START
				"User-level error - warning.\r\n"
				LOG_FORMAT_RESET
				);
	}

	if (function != NULL)
	{
		(void_t) gos_logLogFormattedUnsafe("Function: <"
								  LOG_FG_YELLOW_START
								  "%s"
								  LOG_FORMAT_RESET
								  ">, line: %d\r\n", function, line);
	}
	else
	{
		(void_t) gos_logLogFormattedUnsafe("Function: <"
								  LOG_FG_YELLOW_START
								  "unknown"
								  LOG_FORMAT_RESET
								  ">, line: %d\r\n", line);
	}

	if (errorMessage != NULL)
	{
		va_start(args, errorMessage);
		(void_t) vsprintf(errorBuffer, errorMessage, args);
		va_end(args);

		(void_t) gos_logLogFormattedUnsafe("%s\r\n", errorBuffer);
	}

	(void_t) gos_logLogFormattedUnsafe(SEPARATOR_LINE);

	if (errorLevel == GOS_ERROR_LEVEL_OS_FATAL || errorLevel == GOS_ERROR_LEVEL_USER_FATAL)
	{
		for(;;)
		{
			GOS_DISABLE_SCHED
		}
	}
}

/*
 * Function: gos_traceInit
 */
gos_result_t gos_traceInit (const char_t* initDescription, gos_result_t initResult)
{
	/*
	 * Function code.
	 */
	switch (initResult)
	{
		case GOS_SUCCESS:
		{
			(void_t) gos_logLogFormattedUnsafe("[%-10d]\t%-48s [ "
					LOG_FG_GREEN_START
					" OK  "
					LOG_FORMAT_RESET
					" ]\r\n",
					gos_timerDriverSysTimerGet(),
					initDescription);
			break;
		}
		case GOS_ERROR:
		{
			(void_t) gos_logLogFormattedUnsafe("[%-10d]\t%-48s [ "
					LOG_FG_RED_START
					"ERROR"
					LOG_FORMAT_RESET
					" ]\r\n",
					gos_timerDriverSysTimerGet(),
					initDescription);
			break;
		}
		default: break;
	}

	return initResult;
}
