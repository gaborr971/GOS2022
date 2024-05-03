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
//! @file       bld_statemachine.c
//! @author     Ahmed Gazar
//! @date       2024-02-08
//! @version    1.0
//!
//! @brief      GOS2022 Library / Bootloader State Machine source.
//! @details    For a more detailed description of this driver, please refer to
//!             @ref bld_statemachine.c
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-02-08    Ahmed Gazar     Initial version created.
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
#include <bld.h>
#include <bld_com.h>
#include <string.h>
#include <drv.h>
#include <drv_flash.h>
#include <stdio.h>

/*
 * Macros
 */
/**
 * Progress buffer size.
 */
#define PROGRESS_BUFFER_SIZE   ( 80u )

/**
 * Percentage format buffer size.
 */
#define PERCENTAGE_BUFFER_SIZE ( 16u )

/*
 * Static variables
 */
/**
 * Bootloader state machine state variable.
 */
GOS_STATIC bld_state_t                       bootloaderState;

/**
 * GCP receive buffer.
 */
GOS_STATIC u8_t                              receiveBuffer [BLD_GCP_RECEIVE_BUFFER_SIZE];

/**
 * GCP RX message header.
 */
GOS_STATIC gos_gcpMessageHeader_t            receiveMessageHeader  = {0};

/**
 * GCP TX message header.
 */
GOS_STATIC gos_gcpMessageHeader_t            transmitMessageHeader = {0};

/**
 * Connection request message.
 */
GOS_STATIC bld_com_conn_req_msg_t            connectionRequestMessage  = {0};

/**
 * Connection response message.
 */
GOS_STATIC bld_com_conn_resp_msg_t           connectionResponseMessage = {0};

/**
 * Disconnect request message.
 */
GOS_STATIC bld_com_disconn_req_msg_t         disconnectRequestMessage  = {0};

/**
 * Disconnect response message.
 */
GOS_STATIC bld_com_disconn_resp_msg_t        disconnectResponseMessage = {0};

/**
 * Bootloader data message.
 */
GOS_STATIC bld_com_data_resp_msg_t           bootloaderDataMessage     = {0};

/**
 * Install data message.
 */
GOS_STATIC bld_com_install_data_req_msg_t    installDataMessage    = {0};

/**
 * Install data response message.
 */
GOS_STATIC bld_com_install_data_resp_msg_t   installDataResponseMessage = {0};

/**
 * Install packet request message.
 */
GOS_STATIC bld_com_install_packet_req_msg_t  installPacketRequestMessage = {0};

/**
 * Install packet response message.
 */
GOS_STATIC bld_com_install_packet_resp_msg_t installPacketResponseMessage = {0};

/**
 * Application data structure.
 */
GOS_STATIC bld_appData_t appData = {0};

/**
 * TODO
 */
GOS_STATIC bld_bootloaderConfig_t bldConfig = {0};

/**
 * Buffer for progress printing.
 */
GOS_STATIC char_t                            progressBuffer [PROGRESS_BUFFER_SIZE];

/**
 * Buffer for progress percentage formatting.
 */
GOS_STATIC char_t                            percentageBuffer [PERCENTAGE_BUFFER_SIZE];

/*
 * Function prototypes
 */
GOS_STATIC gos_result_t bld_stateMachineHandleConnectionEstablishment (void_t);
GOS_STATIC gos_result_t bld_stateMachineHandleRequestWait             (void_t);
GOS_STATIC gos_result_t bld_stateMachineHandleDataRequest             (void_t);
GOS_STATIC gos_result_t bld_stateMachineHandleConnectionClose         (void_t);
GOS_STATIC gos_result_t bld_stateMachineHandleAppDataRequest          (void_t);
GOS_STATIC gos_result_t bld_stateMachineHandleAppInstall              (void_t);
GOS_STATIC gos_result_t bld_stateMachineHandleAppCheck                (void_t);

/*
 * Function: bld_stateMachineInitialize
 */
gos_result_t bld_stateMachineInitialize (void_t)
{
    /*
     * Function code.
     */
    bootloaderState = BLD_STATE_CONNECTION_ESTABLISHMENT;

    return bld_configGet(&bldConfig);
}

/*
 * Function: bld_getState
 */
gos_result_t bld_getState (bld_state_t* pState)
{
	/*
	 * Local variables.
	 */
	gos_result_t getStateResult = GOS_ERROR;

    /*
     * Function code.
     */
	if (pState != NULL)
	{
		*pState = bootloaderState;
		getStateResult = GOS_SUCCESS;
	}
	else
	{
		// Error.
	}

	return getStateResult;
}

/*
 * Function: bld_stateMachineRun
 */
gos_result_t bld_stateMachineRun (void_t)
{
    /*
     * Function code.
     */
    switch (bootloaderState)
    {
        case BLD_STATE_CONNECTION_ESTABLISHMENT:
        {
            return bld_stateMachineHandleConnectionEstablishment();
        }
        case BLD_STATE_REQUEST_WAIT:
        {
            return bld_stateMachineHandleRequestWait();
        }
        case BLD_STATE_DATA_REQUEST:
        {
            return bld_stateMachineHandleDataRequest();
        }
        case BLD_STATE_CONNECTION_CLOSE:
        {
            return bld_stateMachineHandleConnectionClose();
        }
        case BLD_STATE_APP_INSTALL:
        {
            return bld_stateMachineHandleAppInstall();
        }
        case BLD_STATE_APP_CHECK:
        {
            return bld_stateMachineHandleAppCheck();
        }
        default:
        {
            return GOS_ERROR;
        }
    }
}

/**
 * @brief   TODO
 * @details
 *
 * @return
 *
 * @retval
 * @retval
 */
GOS_STATIC gos_result_t bld_stateMachineHandleConnectionEstablishment (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t connEstResult = GOS_ERROR;
    u32_t        startTick     = gos_kernelGetSysTicks();

    /*
     * Function code.
     */
    (void_t) gos_traceTrace(GOS_FALSE, "\r\nWaiting for bootloader connection");

    while ((gos_kernelGetSysTicks() - startTick) <= bldConfig.connectionTimeout)
    {
        (void_t) gos_traceTrace(GOS_FALSE, ".");

        if (gos_gcpReceiveMessage(BLD_GCP_CHANNEL_NUM, &receiveMessageHeader, receiveBuffer) == GOS_SUCCESS)
        {
            if (receiveMessageHeader.messageId == BLD_MSG_CONN_REQ_ID)
            {
                // Parse payload.
                (void_t) memcpy((void_t*)&connectionRequestMessage, (void_t*)receiveBuffer, sizeof(connectionRequestMessage));
                (void_t) gos_traceTraceFormatted(GOS_FALSE, "\r\nConnection request from: %s\r\n", connectionRequestMessage.client.name);

                // Prepare response payload.
                connectionResponseMessage.result      = BLD_COM_CONNECTION_ACCEPTED;

                // Prepare response header.
                transmitMessageHeader.messageId       = BLD_MSG_CONN_RESP_ID;
                transmitMessageHeader.payloadSize     = sizeof(connectionResponseMessage);
                transmitMessageHeader.protocolVersion = 0u;

                (void_t) gos_gcpTransmitMessage(BLD_GCP_CHANNEL_NUM, &transmitMessageHeader, (void_t*)&connectionResponseMessage);
                (void_t) gos_traceTrace(GOS_FALSE, "Connection request accepted.\r\n");

                bootloaderState = BLD_STATE_REQUEST_WAIT;
                connEstResult = GOS_SUCCESS;

                return connEstResult;
            }
            else
            {
                bootloaderState = BLD_STATE_APP_CHECK;
                (void_t) gos_traceTrace(GOS_FALSE, "\r\nInvalid message received.\r\n");
                return connEstResult;
            }
        }
        else
        {
            // Wait more.
        }
    }

    (void_t) gos_traceTrace(GOS_FALSE, "\r\nRequest timed out.\r\n");

    // Next state is to check application.
    bootloaderState = BLD_STATE_APP_CHECK;
    return connEstResult;
}

/**
 * @brief   TODO
 * @details
 *
 * @return
 *
 * @retval
 * @retval
 */
GOS_STATIC gos_result_t bld_stateMachineHandleRequestWait (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t reqWaitResult = GOS_ERROR;
    u32_t        startTick     = gos_kernelGetSysTicks();

    /*
     * Function code.
     */
    (void_t) gos_traceTrace(GOS_FALSE, "Waiting for request");

    while ((gos_kernelGetSysTicks() - startTick) <= bldConfig.requestTimeout)
    {
        (void_t) gos_traceTrace(GOS_FALSE, ".");

        if (gos_gcpReceiveMessage(BLD_GCP_CHANNEL_NUM, &receiveMessageHeader, receiveBuffer) == GOS_SUCCESS)
        {
            // Update start tick.
            startTick = gos_kernelGetSysTicks();

            switch (receiveMessageHeader.messageId)
            {
                case BLD_MSG_CONN_REQ_ID:
                {
                    // Parse payload.
                    (void_t) memcpy((void_t*)&connectionRequestMessage, (void_t*)receiveBuffer, sizeof(connectionRequestMessage));
                    (void_t) gos_traceTraceFormatted(GOS_FALSE, "\r\nConnection request from: %s\r\n", connectionRequestMessage.client.name);

                    transmitMessageHeader.messageId       = BLD_MSG_CONN_RESP_ID;
                    transmitMessageHeader.payloadSize     = sizeof(connectionResponseMessage);
                    transmitMessageHeader.protocolVersion = 0u;
                    connectionResponseMessage.result      = BLD_COM_CONNECTION_REFUSED;

                    (void_t) gos_gcpTransmitMessage(BLD_GCP_CHANNEL_NUM, &transmitMessageHeader, (void_t*)&connectionResponseMessage);
                    (void_t) gos_traceTrace(GOS_FALSE, "Connection request refused. Connection already established.\r\n");

                    bootloaderState = BLD_STATE_REQUEST_WAIT;
                    reqWaitResult = GOS_SUCCESS;
                    return reqWaitResult;
                }
                case BLD_MSG_DATA_REQ_ID:
                {
                    reqWaitResult = bld_stateMachineHandleDataRequest();
                    break;
                }
                case BLD_MSG_APP_DATA_REQ_ID:
                {
                    reqWaitResult = bld_stateMachineHandleAppDataRequest();
                    return reqWaitResult;
                }
                case BLD_MSG_DISCONN_REQ_ID:
                {
                    reqWaitResult = bld_stateMachineHandleConnectionClose();
                    return reqWaitResult;
                }
                default:
                {
                    (void_t) gos_traceTraceFormatted(
                            GOS_FALSE,
                            "\r\nUnexpected message received. Received ID: %u\r\n",
                            receiveMessageHeader.messageId);

                    bootloaderState = BLD_STATE_APP_CHECK;
                    return reqWaitResult;
                }
            }
        }
        else
        {
            // Wait more.
        }
    }

    (void_t) gos_traceTrace(GOS_FALSE, "\r\nRequest timed out.\r\n");

    // Next state is to check application.
    bootloaderState = BLD_STATE_APP_CHECK;
    return reqWaitResult;
}

/**
 * @brief   TODO
 * @details
 *
 * @return
 *
 * @retval
 * @retval
 */
GOS_STATIC gos_result_t bld_stateMachineHandleDataRequest (void_t)
{
    (void_t) gos_traceTrace(GOS_FALSE, "\r\nData request received.\r\n");
    (void_t) bld_dataGet(&bootloaderDataMessage.bldData);
    (void_t) bld_appDataGet(&bootloaderDataMessage.appData);

    transmitMessageHeader.messageId       = BLD_MSG_DATA_RESP_ID;
    transmitMessageHeader.payloadSize     = sizeof(bootloaderDataMessage);
    transmitMessageHeader.protocolVersion = 0u;

    (void_t) gos_gcpTransmitMessage(BLD_GCP_CHANNEL_NUM, &transmitMessageHeader, (void_t*)&bootloaderDataMessage);
    (void_t) gos_traceTrace(GOS_FALSE, "Bootloader data sent.\r\n");

    // Stay in request mode.
    bootloaderState = BLD_STATE_REQUEST_WAIT;

    return GOS_SUCCESS;
}

/**
 * @brief   TODO
 * @details
 *
 * @return
 *
 * @retval
 * @retval
 */
GOS_STATIC gos_result_t bld_stateMachineHandleConnectionClose (void_t)
{
    // Parse payload.
    (void_t) memcpy((void_t*)&disconnectRequestMessage, (void_t*)receiveBuffer, sizeof(disconnectRequestMessage));
    (void_t) gos_traceTraceFormatted(GOS_FALSE, "\r\nDisconnect request with code: %u\r\n", disconnectRequestMessage.reason);

    disconnectResponseMessage.result      = BLD_COM_CONNECTION_ACCEPTED;

    transmitMessageHeader.messageId       = BLD_MSG_DISCONN_RESP_ID;
    transmitMessageHeader.payloadSize     = sizeof(disconnectResponseMessage);
    transmitMessageHeader.protocolVersion = 0u;
    (void_t) gos_gcpTransmitMessage(BLD_GCP_CHANNEL_NUM, &transmitMessageHeader, (void_t*)&disconnectResponseMessage);

    (void_t) gos_traceTrace(GOS_FALSE, "Disconnected.\r\n");
    bootloaderState = BLD_STATE_APP_CHECK;

    return GOS_SUCCESS;
}

/**
 * @brief   TODO
 * @details
 *
 * @return
 *
 * @retval
 * @retval
 */
GOS_STATIC gos_result_t bld_stateMachineHandleAppDataRequest (void_t)
{
    (void_t) gos_traceTrace(GOS_FALSE, "\r\nUpdate request received.\r\n");

    // Get payload content with update details.
    (void_t) memcpy((void_t*)&installDataMessage, (void_t*)receiveBuffer, sizeof(installDataMessage));

    switch (installDataMessage.updateType)
    {
        case BLD_COM_UPDATE_TYPE_ERASE:
        {
            (void_t) gos_traceTrace(GOS_FALSE, "Update type:\terase\r\n");
            (void_t) gos_traceTrace(GOS_FALSE, "Erasing application...\r\n");

            if (drv_flashErase(BLD_APP_ROM_START_ADDRESS, BLD_APP_ROM_SIZE) == GOS_SUCCESS)
            {
                (void_t) bld_dataReset();
                installDataResponseMessage.result = BLD_COM_ERASE_SUCCESSFUL;
                (void_t) gos_traceTrace(GOS_FALSE, "Application erase successful.\r\n");
            }
            else
            {
                installDataResponseMessage.result = BLD_COM_ERASE_FAILED;
                (void_t) gos_traceTrace(GOS_FALSE, "Application erase failed.\r\n");
            }

            break;
        }
        case BLD_COM_UPDATE_TYPE_INSTALL:
        {
            (void_t) gos_traceTrace(GOS_FALSE, "Update type:\tinstall\r\n");

            (void_t) gos_traceTraceFormatted(GOS_FALSE, "Name:       \t%s\r\n", installDataMessage.appData.version.name);
            (void_t) gos_traceTraceFormatted(GOS_FALSE, "Author:     \t%s\r\n", installDataMessage.appData.version.author);
            (void_t) gos_traceTraceFormatted(GOS_FALSE, "Description:\t%s\r\n", installDataMessage.appData.version.description);
            (void_t) gos_traceTraceFormatted(GOS_FALSE, "Version:    \t%u.%u.%u\r\n",
                    installDataMessage.appData.version.major,
                    installDataMessage.appData.version.minor,
                    installDataMessage.appData.version.build);
            (void_t) gos_traceTraceFormatted(GOS_FALSE, "Date:       \t%04u-%02u-%02u\r\n",
            		installDataMessage.appData.version.date.years,
					installDataMessage.appData.version.date.months,
					installDataMessage.appData.version.date.days);
            (void_t) gos_traceTraceFormatted(GOS_FALSE, "Size:       \t%u bytes\r\n", installDataMessage.appData.size);

            if (drv_crcCheckCrc32((u8_t*)&installDataMessage.appData,
                sizeof(installDataMessage.appData) - sizeof(installDataMessage.appData.dataCrc),
                installDataMessage.appData.dataCrc, NULL) != DRV_CRC_CHECK_OK)
            {
                installDataResponseMessage.result = BLD_COM_INSTALL_REQUEST_DATA_CRC_ERROR;
                (void_t) gos_traceTrace(GOS_FALSE, "Install data CRC error.\r\n");
            }
            else if (installDataMessage.appData.startAddress != BLD_APP_ROM_START_ADDRESS)
            {
                installDataResponseMessage.result = BLD_COM_INSTALL_REQUEST_START_ADDRESS_ERROR;
                (void_t) gos_traceTrace(GOS_FALSE, "Install application start address error.\r\n");
            }
            else if (installDataMessage.appData.size > BLD_APP_ROM_SIZE ||
                     (installDataMessage.appData.startAddress + installDataMessage.appData.size) > BLD_APP_ROM_END_ADDRESS)
            {
                installDataResponseMessage.result = BLD_COM_INSTALL_REQUEST_SIZE_ERROR;
                (void_t) gos_traceTrace(GOS_FALSE, "Install application size error.\r\n");
            }
            else
            {
                installDataResponseMessage.result = BLD_COM_INSTALL_REQUEST_ACCEPTED;
                bootloaderState = BLD_STATE_APP_INSTALL;
                (void_t) gos_traceTrace(GOS_FALSE, "Install request accepted.\r\n");
            }

            (void_t) gos_traceTrace(GOS_FALSE, "Erasing application...\r\n");

            if (drv_flashErase(BLD_APP_ROM_START_ADDRESS, BLD_APP_ROM_SIZE) == GOS_SUCCESS)
            {
                (void_t) bld_dataReset();
                installDataResponseMessage.result = BLD_COM_ERASE_SUCCESSFUL;
                (void_t) gos_traceTrace(GOS_FALSE, "Application erase successful.\r\n");
            }
            else
            {
                installDataResponseMessage.result = BLD_COM_ERASE_FAILED;
                (void_t) gos_traceTrace(GOS_FALSE, "Application erase failed.\r\n");
            }

            break;
        }
    }

    transmitMessageHeader.messageId       = BLD_MSG_APP_DATA_RESP_ID;
    transmitMessageHeader.payloadSize     = sizeof(installDataResponseMessage);
    transmitMessageHeader.protocolVersion = 0u;
    (void_t) gos_gcpTransmitMessage(BLD_GCP_CHANNEL_NUM, &transmitMessageHeader, (void_t*)&installDataResponseMessage);

    return GOS_SUCCESS;
}

/**
 * @brief   TODO
 * @details
 *
 * @return
 *
 * @retval
 * @retval
 */
GOS_STATIC gos_result_t bld_stateMachineHandleAppInstall (void_t)
{
    /*
     * Local variables.
     */
    u32_t appBytesCounter = 0u;
    u32_t sequenceCounter = 0u;
    u32_t perc            = 0u;
    u32_t startTick       = gos_kernelGetSysTicks();

    /*
     * Function code.
     */

    (void_t) drv_flashUnlock();

    while ((appBytesCounter < installDataMessage.appData.size) && (gos_kernelGetSysTicks() - startTick) <= bldConfig.installTimeout)
    {
        if (gos_gcpReceiveMessage(BLD_GCP_CHANNEL_NUM, &receiveMessageHeader, receiveBuffer) == GOS_SUCCESS)
        {
            if (receiveMessageHeader.messageId == BLD_MSG_PACKET_REQ_ID)
            {
                (void_t) memcpy((void_t*)&installPacketRequestMessage, (void_t*)receiveBuffer, sizeof(installPacketRequestMessage));

                if (installPacketRequestMessage.sequenceCounter <= sequenceCounter)
                {
                    // TODO: sequence counter error.
                    installPacketResponseMessage.result = BLD_COM_INSTALL_PACKET_REPEAT;
                    (void_t) gos_traceTrace(GOS_FALSE, "\r\nSequence counter error.\r\n");
                }
                else if (installPacketRequestMessage.sequenceCounter == (sequenceCounter + 1))
                {
                    if (drv_crcCheckCrc32(
                            installPacketRequestMessage.packetBuffer,
                            installPacketRequestMessage.packetSize,
                            installPacketRequestMessage.packetChk,
                            NULL) == DRV_CRC_CHECK_OK)
                    {
                        // Sequence counter and CRC OK.
                        // Save packet in FLASH.
                        (void_t) drv_flashWriteWithoutLock((BLD_APP_ROM_START_ADDRESS + appBytesCounter), installPacketRequestMessage.packetBuffer, installPacketRequestMessage.packetSize);

                        appBytesCounter += installPacketRequestMessage.packetSize;
                        sequenceCounter = installPacketRequestMessage.sequenceCounter + 1;

                        installPacketResponseMessage.sequenceCounter = sequenceCounter;
                        installPacketResponseMessage.result = BLD_COM_INSTALL_PACKET_ACCEPTED;

                        transmitMessageHeader.messageId       = BLD_MSG_PACKET_RESP_ID;
                        transmitMessageHeader.payloadSize     = sizeof(installPacketResponseMessage);
                        transmitMessageHeader.protocolVersion = 0u;

                        // Send response.
                        (void_t) gos_gcpTransmitMessage(BLD_GCP_CHANNEL_NUM, &transmitMessageHeader, (void_t*)&installPacketResponseMessage);

                        // Display progress.
                        perc = 100 * 100 * appBytesCounter / installDataMessage.appData.size;

                        (void_t) sprintf(progressBuffer, "\rProgress: [ "TRACE_FG_YELLOW_START);

                        for (int i = 1; i <= 25; i++)
                        {
                            if (i * 2 <= (perc / 100))
                            {
                                (void_t) strcat(progressBuffer, "=");
                            }
                            else
                            {
                                (void_t) strcat(progressBuffer, " ");
                            }
                        }

                        (void_t) sprintf(percentageBuffer, "%3u.%02u%% ", (u8_t)(perc / 100), (u8_t)(perc % 100));
                        (void_t) strcat(progressBuffer, percentageBuffer);

                        for (int i = 26; i <= 50; i++)
                        {
                            if (i * 2 <= (perc / 100))
                            {
                                (void_t) strcat(progressBuffer, "=");
                            }
                            else
                            {
                                (void_t) strcat(progressBuffer, " ");
                            }
                        }

                        (void_t) strcat(progressBuffer, TRACE_FORMAT_RESET" ]");
                        (void_t) gos_traceTrace(GOS_FALSE, progressBuffer);
                    }
                    else
                    {
                        (void_t) gos_traceTrace(GOS_FALSE, "\r\nPacket CRC error.\r\n");
                    }
                }
                else
                {
                    // Packet missed.
                    installPacketResponseMessage.result = BLD_COM_INSTALL_PACKET_REPEAT;
                    (void_t) gos_traceTrace(GOS_FALSE, "\r\nLost packet.\r\n");
                }
            }
            else
            {
                // Unexpected message.
                (void_t) gos_traceTrace(GOS_FALSE, "\r\nUnexpected message received.\r\n");
                bootloaderState = BLD_STATE_CONNECTION_CLOSE;
                disconnectRequestMessage.reason = 0u;
                return GOS_SUCCESS;
            }
        }
    }

    (void_t) drv_flashLock();

    if (appBytesCounter < installDataMessage.appData.size)
    {
    	(void_t) gos_traceTrace(GOS_FALSE, "\r\nInstall timed out.\r\n");
    }
    else
    {
        (void_t) bld_appDataGet(&appData);
        (void_t) memcpy((void_t*)&appData, (void_t*)&installDataMessage.appData, sizeof(appData));
        appData.initPattern = 0u;
        (void_t) bld_appDataSet(&appData);

        (void_t) gos_traceTrace(GOS_FALSE, "\r\nInstall finished.\r\n");
    }

    bootloaderState = BLD_STATE_APP_CHECK;

    return GOS_SUCCESS;
}

/**
 * @brief   TODO
 * @details
 *
 * @return
 *
 * @retval
 * @retval
 */
GOS_STATIC gos_result_t bld_stateMachineHandleAppCheck (void_t)
{
    /*
     * Local variables.
     */
    bld_bootloaderData_t bldData = {0};
    bld_appData_t appData = {0};

    /*
     * Function code.
     */
    (void_t) gos_traceTrace(GOS_FALSE, "Checking application...\r\n");
    (void_t) bld_dataGet(&bldData);
    (void_t) bld_appDataGet(&appData);

    if (bld_checkApplication(&appData, GOS_TRUE) == GOS_SUCCESS)
    {
        (void_t) gos_traceTrace(GOS_FALSE, "Exiting update mode...\r\n");
        bldData.updateMode = GOS_FALSE;
        (void_t) bld_dataSet(&bldData);
        (void_t) gos_taskSleep(300);
        gos_kernelReset();
    }
    else
    {
        // Application check failed, go back to connection establishment
        // phase.
    }

    bootloaderState = BLD_STATE_CONNECTION_ESTABLISHMENT;
    return GOS_ERROR;
}
