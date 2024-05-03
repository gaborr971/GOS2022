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
//! @file       svl_frs.c
//! @author     Ahmed Gazar
//! @date       2024-04-24
//! @version    1.0
//!
//! @brief      GOS2022 Library / Fault Reporting Service source.
//! @details    For a more detailed description of this driver, please refer to @ref svl_frs.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-04-24    Ahmed Gazar     Initial version created.
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
#include <svl_frs.h>

/*
 * Macros
 */
#define SVL_FRS_MUTEX_TMO_MS ( 2000u )

/*
 * Static variables
 */
/**
 * Read function pointer.
 */
GOS_STATIC svl_frsReadWriteFunc_t frsReadFunction = NULL;

/**
 * Write function pointer.
 */
GOS_STATIC svl_frsReadWriteFunc_t frsWriteFunction = NULL;

/**
 * Service mutex.
 */
GOS_STATIC gos_mutex_t            frsMutex;

/*
 * Function: svl_frsInit
 */
gos_result_t svl_frsInit (svl_frsInitStruct_t* pInitStruct)
{
	/*
	 * Local variables.
	 */
	gos_result_t frsInitResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (pInitStruct != NULL && pInitStruct->readFunction != NULL && pInitStruct->writeFunction != NULL)
	{
		frsReadFunction  = pInitStruct->readFunction;
		frsWriteFunction = pInitStruct->writeFunction;
		frsInitResult    = gos_mutexInit(&frsMutex);
	}
	else
	{
		// NULL parameter.
	}

	return frsInitResult;
}

/*
 * Function: svl_frsCreate
 */
gos_result_t svl_frsCreate (svl_frsFault_t* pFault)
{
	/*
	 * Local variables.
	 */
	gos_result_t frsCreateResult = GOS_ERROR;
	u32_t        numOfEntries    = 0u;
	u32_t        address         = 0u;

	/*
	 * Function code.
	 */
	if (pFault != NULL)
	{
		// Get the number of faults.
		if (svl_frsGetNumOfEntries(&numOfEntries)          == GOS_SUCCESS &&
			gos_mutexLock(&frsMutex, SVL_FRS_MUTEX_TMO_MS) == GOS_SUCCESS)
		{
			// Calculate next fault address.s
			address = SVL_FRS_ENTRY_START_ADDR + numOfEntries * SVL_FRS_ENTRY_SIZE;

			// Write new fault.
			if (frsWriteFunction != NULL &&
				frsWriteFunction(address, (u8_t*)pFault, SVL_FRS_ENTRY_SIZE) == GOS_SUCCESS)
			{
				// Update number of entries.
				numOfEntries++;

				frsCreateResult = frsWriteFunction(SVL_FRS_ENTRY_NUM_ADDR, (u8_t*)&numOfEntries, sizeof(numOfEntries));
			}
			else
			{
				// FRS write failed.
			}
		}
		else
		{
			// Mutex error or getting number of entries failed.
		}

		(void_t) gos_mutexUnlock(&frsMutex);
	}
	else
	{
		// NULL parameter.
	}

	return frsCreateResult;
}

/*
 * Function: svl_frsGetNumOfEntries
 */
gos_result_t svl_frsGetNumOfEntries (u32_t* pNumOfEntries)
{
	/*
	 * Local variables.
	 */
	gos_result_t frsGetNumOfEntriesResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (frsReadFunction != NULL &&
		gos_mutexLock(&frsMutex, SVL_FRS_MUTEX_TMO_MS) == GOS_SUCCESS)
	{
		frsGetNumOfEntriesResult = frsReadFunction(SVL_FRS_ENTRY_NUM_ADDR, (u8_t*)pNumOfEntries, sizeof(*pNumOfEntries));
	}
	else
	{
		// Read function is NULL.
	}

	(void_t) gos_mutexUnlock(&frsMutex);

	return frsGetNumOfEntriesResult;
}

/*
 * Function: svl_frsGetFaultByIndex
 */
gos_result_t svl_frsGetFaultByIndex (u16_t index, svl_frsFault_t *pFault)
{
	/*
	 * Local variables.
	 */
	gos_result_t getFaultResult = GOS_ERROR;
	u32_t        numOfEntries   = 0u;

	/*
	 * Function code.
	 */
	if (pFault != NULL &&
		svl_frsGetNumOfEntries(&numOfEntries) == GOS_SUCCESS &&
		gos_mutexLock(&frsMutex, SVL_FRS_MUTEX_TMO_MS) == GOS_SUCCESS)
	{
		if (index < numOfEntries)
		{
			getFaultResult = frsReadFunction(
					SVL_FRS_ENTRY_START_ADDR + index * SVL_FRS_ENTRY_SIZE,
					(u8_t*)pFault,
					SVL_FRS_ENTRY_SIZE
					);
		}
		else
		{
			// Index error.
		}
	}
	else
	{
		// NULL parameter or number of entries getting error or mutex lock error.
	}

	(void_t) gos_mutexUnlock(&frsMutex);

	return getFaultResult;
}

/*
 * Function: svl_frsGetAllFaults
 */
gos_result_t svl_frsGetAllFaults (svl_frsFault_t* pFaultArray, u16_t arraySize)
{
	/*
	 * Local variables.
	 */
	gos_result_t getFaultResult = GOS_ERROR;
	u32_t        numOfEntries   = 0u;
	u32_t        entryIndex     = 0u;

	/*
	 * Function code.
	 */
	if (pFaultArray != NULL &&
		svl_frsGetNumOfEntries(&numOfEntries) == GOS_SUCCESS &&
		gos_mutexLock(&frsMutex, SVL_FRS_MUTEX_TMO_MS) == GOS_SUCCESS &&
		arraySize >= (numOfEntries * SVL_FRS_ENTRY_SIZE))
	{
		for (entryIndex = 0u; entryIndex < numOfEntries; entryIndex++)
		{
			getFaultResult = frsReadFunction(
					SVL_FRS_ENTRY_START_ADDR + entryIndex * SVL_FRS_ENTRY_SIZE,
					(u8_t*)&pFaultArray[entryIndex],
					SVL_FRS_ENTRY_SIZE
					);

			if (getFaultResult == GOS_ERROR)
			{
				// There was an error. Break from loop.
				break;
			}
			else
			{
				// Continue.
			}
		}
	}
	else
	{
		// NULL parameter or number of entries getting error or mutex lock error or
		// array size error.
	}

	(void_t) gos_mutexUnlock(&frsMutex);

	return getFaultResult;
}

/*
 * Function: svl_frsGetAllFaults
 */
gos_result_t svl_frsClearFaults (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t clearFaultResult = GOS_ERROR;
	u32_t        numOfEntries     = 0u;

	/*
	 * Function code.
	 */
	clearFaultResult = frsWriteFunction(
			SVL_FRS_ENTRY_NUM_ADDR,
			(u8_t*)&numOfEntries,
			sizeof(numOfEntries)
			);

	return clearFaultResult;
}
