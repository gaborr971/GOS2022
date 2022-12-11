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
//! @file		gos_gcp.c
//! @author		Gabor Repasi
//! @date		2022-12-10
//! @version	1.0
//!
//! @brief		GOS General Communication Protocol handler service source.
//! @details	For a more detailed description of this service, please refer to @ref gos_gcp.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-12-10	Gabor Repasi	Initial version created.
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
#include "gos_gcp.h"
#include "gos_crc_driver.h"
#include <string.h>

/*
 * Macros
 */
/**
 * Message frame size (bytes).
 */
#define FRAME_SIZE					( 8u )

/**
 * Frame protocol version high byte.
 */
#define FRAME_PROTOCOL_VERSION_H	( 1 )

/**
 * Frame protocol version low byte.
 */
#define FRAME_PROTOCOL_VERSION_L	( 0 )

/**
 * Frame protocol version.
 */
#define FRAME_PROTOCOL_VERSION		( ( FRAME_PROTOCOL_VERSION_H << 8 ) + FRAME_PROTOCOL_VERSION_L )

/*
 * Type definitions
 */
/**
 * GCP frame header type.
 */
typedef struct
{
	u16_t	protocolVersion;	//!< Protocol version.
	u16_t	sessionId;			//!< Session ID.
	u16_t	dataSize;			//!< Data size.
	u16_t	frameSize;			//!< Frame size.
	u32_t	dataCrc;			//!< Data CRC.
	u32_t	headerCrc;			//!< Header CRC.
}gos_gcpFrameHeader_t;

/*
 * Static variables
 */
/**
 * GCP physical layer transmit function.
 */
GOS_STATIC gos_gcpTransmitFunction_t gcpTransmitFunction = NULL;

/**
 * GCP physical layer receive function.
 */
GOS_STATIC gos_gcpReceiveFunction_t gcpReceiveFunction = NULL;

/**
 * Session counter.
 */
GOS_STATIC u16_t					sessionCounter;

/*
 * Function prototypes
 */
GOS_STATIC	gos_result_t gos_gcpTransmitFrames (u8_t* pMessageBytes, u16_t messageSize);
GOS_STATIC	gos_result_t gos_gcpReceiveFrames (u8_t* pTarget, u16_t targetSize);

/*
 * Function: gos_gcpRegisterPhysicalDriver
 */
gos_result_t gos_gcpRegisterPhysicalDriver (gos_gcpTransmitFunction_t transmitFunction,
											gos_gcpReceiveFunction_t receiveFunction)
{
	/*
	 * Local variables.
	 */
	gos_result_t registerPhysicalDriverResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (transmitFunction != NULL && receiveFunction != NULL)
	{
		gcpTransmitFunction = transmitFunction;
		gcpReceiveFunction = receiveFunction;
		registerPhysicalDriverResult = GOS_SUCCESS;
	}

	return registerPhysicalDriverResult;
}

/*
 * Function: gos_gcpTransmitMessage
 */
gos_result_t gos_gcpTransmitMessage (gos_gcpMessageHeader_t* pMessageHeader, void_t* pMessagePayload)
{
	/*
	 * Local variables.
	 */
	gos_result_t transmitMessageResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (pMessageHeader != NULL &&
		(pMessagePayload != NULL || (pMessagePayload == NULL && pMessageHeader->payloadSize == 0u)) &&
		gcpTransmitFunction != NULL)
	{
		// Calculate payload CRC.
		pMessageHeader->payloadCrc = gos_crcDriverGetCrc((u8_t*)pMessagePayload, pMessageHeader->payloadSize);

		// Transmit message header and payload.
		if (gos_gcpTransmitFrames((u8_t*)pMessageHeader, (u16_t)sizeof(*pMessageHeader)) == GOS_SUCCESS &&
			gos_gcpTransmitFrames((u8_t*)pMessagePayload, pMessageHeader->payloadSize) == GOS_SUCCESS)
		{
			transmitMessageResult = GOS_SUCCESS;
		}
	}

	return transmitMessageResult;
}

/*
 * Function: gos_gcpReceiveMessage
 */
gos_result_t gos_gcpReceiveMessage (gos_gcpMessageHeader_t* pTargetMessageHeader, void_t* pPayloadTarget)
{
	/*
	 * Local variables.
	 */
	gos_result_t receiveMessageResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	if (pTargetMessageHeader != NULL && pPayloadTarget != NULL && gcpReceiveFunction != NULL)
	{
		// Receive header and frames, check CRC.
		if (gos_gcpReceiveFrames((u8_t*)pTargetMessageHeader, (u16_t)sizeof(*pTargetMessageHeader)) == GOS_SUCCESS &&
			gos_gcpReceiveFrames((u8_t*)pPayloadTarget, pTargetMessageHeader->payloadSize) == GOS_SUCCESS &&
			pTargetMessageHeader->payloadCrc == gos_crcDriverGetCrc((u8_t*)pPayloadTarget, (u32_t)pTargetMessageHeader->payloadSize))
		{
			receiveMessageResult = GOS_SUCCESS;
		}
	}

	return receiveMessageResult;
}

/**
 * @brief	This function transmits the message frames.
 * @details	This function prepares and transmits the header frame, and then it
 * 			transmits the data frames.
 *
 * @param	pMessageBytes	:	Pointer to the message buffer to be transmitted.
 * @param	messageSize		:	Size of the message.
 *
 * @return	Result of frame transmission.
 *
 * @retval	GOS_SUCCESS	:	Frame transmission successful.
 * @retval	GOS_ERROR	:	Transmit function is NULL pointer or frame transmission failed.
 */
GOS_STATIC gos_result_t gos_gcpTransmitFrames (u8_t* pMessageBytes, u16_t messageSize)
{
	/*
	 * Local variables.
	 */
	gos_result_t			transmitFramesResult = GOS_ERROR;
	gos_gcpFrameHeader_t	frameHeader;
	u16_t					frameNumber;
	u16_t					frameCounter;

	/*
	 * Function code.
	 */
	if (gcpTransmitFunction != NULL)
	{
		// Fill out frame header.
		frameHeader.protocolVersion	= FRAME_PROTOCOL_VERSION;
		frameHeader.dataSize		= messageSize;
		frameHeader.frameSize		= FRAME_SIZE;
		frameHeader.sessionId		= sessionCounter++;
		frameHeader.dataCrc			= gos_crcDriverGetCrc(pMessageBytes, messageSize);
		frameHeader.headerCrc		= gos_crcDriverGetCrc((u8_t*)&frameHeader, (u32_t)(sizeof(frameHeader) - sizeof(frameHeader.headerCrc)));

		// Calculate frame number (with padding).
		frameNumber = messageSize / FRAME_SIZE + messageSize % FRAME_SIZE;

		// Transmit header.
		if (gcpTransmitFunction((u8_t*)&frameHeader, (u16_t)sizeof(frameHeader)) == GOS_SUCCESS)
		{
			// Transmit frames.
			for (frameCounter = 0u; frameCounter < frameNumber; frameCounter++)
			{
				// Transmit frame.
				if (gcpTransmitFunction((u8_t*)(pMessageBytes + (frameCounter * FRAME_SIZE)), (u16_t)FRAME_SIZE) != GOS_SUCCESS)
				{
					break;
				}
			}

			if (frameCounter == frameNumber)
			{
				transmitFramesResult = GOS_SUCCESS;
			}
		}
	}

	return transmitFramesResult;
}

/**
 * @brief	This function receives the message frames.
 * @details	This function receives the header frame, and based on the data in the
 * 			header, it receives the data frames.
 *
 * @param	pTarget		:	Target buffer to store the frames in.
 * @param	targetSize	:	Size of target buffer.
 *
 * @return	Result of frame reception.
 *
 * @retval	GOS_SUCCESS	:	Frames received successfully.
 * @retval	GOS_ERROR	:	Receive function is NULL pointer, frame reception error or
 * 							invalid message CRC.
 */
GOS_STATIC gos_result_t gos_gcpReceiveFrames (u8_t* pTarget, u16_t targetSize)
{
	/*
	 * Local variables.
	 */
	gos_result_t 			receiveFramesResult = GOS_ERROR;
	gos_gcpFrameHeader_t	frameHeader;
	u16_t					frameNumber;
	u16_t					frameCounter;

	/*
	 * Function code.
	 */
	// Receive header.
	if (gcpReceiveFunction((u8_t*)&frameHeader, (u16_t)sizeof(frameHeader)) == GOS_SUCCESS)
	{
		// Check header integrity, protocol version and target size.
		if (frameHeader.headerCrc == gos_crcDriverGetCrc((u8_t*)&frameHeader, (u32_t)(sizeof(frameHeader) - sizeof(frameHeader.headerCrc))) &&
			frameHeader.protocolVersion == FRAME_PROTOCOL_VERSION &&
			targetSize >= frameHeader.dataSize)
		{
			// Calculate frame number.
			frameNumber = frameHeader.dataSize / frameHeader.frameSize + frameHeader.dataSize % frameHeader.frameSize;

			// Receive frames.
			for (frameCounter = 0u; frameCounter < frameNumber; frameCounter++)
			{
				// Receive frame.
				if (gcpReceiveFunction((u8_t*)(pTarget + frameCounter * frameHeader.frameSize), frameHeader.frameSize) != GOS_SUCCESS)
				{
					break;
				}
			}

			// Check data integrity.
			if (frameCounter == frameNumber &&
				frameHeader.dataCrc == gos_crcDriverGetCrc(pTarget, (u32_t)frameHeader.dataSize))
			{
				receiveFramesResult = GOS_SUCCESS;
			}
		}
	}

	return receiveFramesResult;
}
