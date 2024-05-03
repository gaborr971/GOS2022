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
//! @file       svl_bldHandler.c
//! @author     Ahmed Gazar
//! @date       2024-04-24
//! @version    1.0
//!
//! @brief      GOS2022 Library / Bootloader Handler Service source.
//! @details    For a more detailed description of this driver, please refer to @ref svl_bldHandler.h
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
#include <svl_bldHandler.h>
#include <bld_data.h>
#include <bld_com.h>

/*
 * Function prototypes
 */
GOS_STATIC void_t svl_bldHandlerBootModeReqMsgReceived (void_t);
GOS_STATIC void_t svl_bldHandlerBootDataReqMsgReceived (void_t);

/*
 * Static variables
 */
/**
 * Boot mode request message.
 */
GOS_STATIC gos_sysmonUserMessageDescriptor_t bootMsg =
{
	.callback        = svl_bldHandlerBootModeReqMsgReceived,
	.messageId       = BLD_MSG_BOOT_MODE_REQ_ID,
	.payload         = NULL,
	.payloadSize     = 0u,
	.protocolVersion = 1u
};

/**
 * Bootloader data request message.
 */
GOS_STATIC gos_sysmonUserMessageDescriptor_t bootDataReqMsg =
{
	.callback        = svl_bldHandlerBootDataReqMsgReceived,
	.messageId       = BLD_MSG_DATA_REQ_ID,
	.payload         = NULL,
	.payloadSize     = 0u,
	.protocolVersion = 1u
};

/*
 * Function: svl_bldHandlerInit
 */
gos_result_t svl_bldHandlerInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t bldHandlerInitResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (gos_sysmonRegisterUserMessage(&bootMsg)        == GOS_SUCCESS &&
	    gos_sysmonRegisterUserMessage(&bootDataReqMsg) == GOS_SUCCESS)
	{
		bldHandlerInitResult = GOS_SUCCESS;
	}
	else
	{
		// Register error.
	}

	return bldHandlerInitResult;
}

/**
 * @brief   Handles the boot message.
 * @details Sets the update mode flag in the bootloader data.
 *
 * @return  -
 */
GOS_STATIC void_t svl_bldHandlerBootModeReqMsgReceived (void_t)
{
	/*
	 * Local variables.
	 */
	bld_bootloaderData_t bldData = {0};

	/*
	 * Function code.
	 */
	if (bld_dataGet(&bldData) == GOS_SUCCESS)
	{
		// Set update mode flag.
		bldData.updateMode = GOS_TRUE;

		// Store data.
		if (bld_dataSet(&bldData) == GOS_SUCCESS)
		{
			gos_kernelReset();
		}
		else
		{
			// Data set error.
		}
	}
	else
	{
		// Data error.
	}
}

/**
 * @brief   Handles the bootloader data request message.
 * @details Sends out the bootloader data via the sysmon GCP channel.
 *
 * @return  -
 */
GOS_STATIC void_t svl_bldHandlerBootDataReqMsgReceived (void_t)
{
	/*
	 * Local variables.
	 */
	bld_com_data_resp_msg_t bootloaderDataMessage = {0};
	gos_gcpMessageHeader_t  transmitMessageHeader = {0};

	/*
	 * Function code.
	 */
	if (bld_dataGet(&bootloaderDataMessage.bldData) == GOS_SUCCESS &&
	    bld_appDataGet(&bootloaderDataMessage.appData) == GOS_SUCCESS)
	{
		transmitMessageHeader.messageId       = BLD_MSG_DATA_RESP_ID;
	    transmitMessageHeader.payloadSize     = sizeof(bootloaderDataMessage);
	    transmitMessageHeader.protocolVersion = 0u;

	    (void_t) gos_gcpTransmitMessage(
	    		CFG_SYSMON_GCP_CHANNEL_NUM, &transmitMessageHeader, (void_t*)&bootloaderDataMessage);
	}
	else
	{
		// Data get error.
	}
}
