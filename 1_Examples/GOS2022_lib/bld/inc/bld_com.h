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
//! @file       bld_com.h
//! @author     Ahmed Gazar
//! @date       2024-02-07
//! @version    1.0
//!
//! @brief      GOS2022 Library / Bootloader communication header.
//! @details    TODO
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2024-02-07    Ahmed Gazar     Initial version created.
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
#ifndef BLD_COM_H
#define BLD_COM_H

/*
 * Includes
 */
#include <gos.h>
#include <bld_data.h>
#include <bld_cfg.h>

/*
 * Type definitions
 */
/**
 * TODO
 */
typedef enum
{
    BLD_COM_UPDATE_TYPE_INSTALL = 0,
    BLD_COM_UPDATE_TYPE_ERASE   = 1
}bld_com_update_t;

/**
 * TODO
 */
typedef enum
{
    BLD_COM_CONNECTION_ACCEPTED = 100,
    BLD_COM_CONNECTION_REFUSED  = 1,
    BLD_COM_DISCONNECT_ACCEPTED = 2,
    BLD_COM_DISCONNECT_REFUSED  = 3,
    BLD_COM_DISCONNECT_TMO      = 4
}bld_com_conn_result_t;

/**
 * TODO
 */
typedef enum
{
    BLD_COM_INSTALL_REQUEST_ACCEPTED             = 100,
    BLD_COM_INSTALL_REQUEST_SIZE_ERROR           = 1,
    BLD_COM_INSTALL_REQUEST_START_ADDRESS_ERROR  = 2,
    BLD_COM_INSTALL_REQUEST_DATA_CRC_ERROR       = 3,
    BLD_COM_ERASE_SUCCESSFUL                     = 101,
    BLD_COM_ERASE_FAILED                         = 4
}bld_com_install_req_result_t;

/**
 * TODO
 */
typedef enum
{
    BLD_COM_INSTALL_PACKET_ACCEPTED = 100,
    BLD_COM_INSTALL_PACKET_REPEAT   = 0
}bld_com_install_packet_req_result_t;

/**
 * TODO
 */
typedef struct
{
    char_t name [32];
}bld_com_client_data_t;

/**
 * TODO
 */
typedef struct
{
    bld_com_client_data_t client;
}bld_com_conn_req_msg_t;

/**
 * TODO
 */
typedef struct
{
    bld_com_conn_result_t result;
}bld_com_conn_resp_msg_t;

/**
 * TODO
 */
typedef struct
{
    // No message content.
}bld_com_data_req_msg_t;

/**
 * TODO
 */
typedef struct
{
    bld_bootloaderData_t bldData;
    bld_appData_t        appData;
}bld_com_data_resp_msg_t;

/**
 * TODO
 */
typedef struct
{
    u16_t reason;
}bld_com_disconn_req_msg_t;

/**
 * TODO
 */
typedef struct
{
    bld_com_conn_result_t result;
}bld_com_disconn_resp_msg_t;

/**
 * TODO
 */
typedef struct
{
    bld_com_update_t updateType;
    bld_appData_t    appData;
}bld_com_install_data_req_msg_t;

/**
 * TODO
 */
typedef struct
{
    bld_com_install_req_result_t result;
}bld_com_install_data_resp_msg_t;

/**
 * TODO
 */
typedef struct
{
    u16_t packetSize;
    u32_t sequenceCounter;
    u32_t packetChk;
    u8_t  packetBuffer [BLD_INSTALL_PACKET_MAX_SIZE];
}bld_com_install_packet_req_msg_t;

/**
 * TODO
 */
typedef struct
{
    u32_t sequenceCounter;
    bld_com_install_packet_req_result_t result;
}bld_com_install_packet_resp_msg_t;

/**
 * Bootloader message ID enum.
 */
typedef enum
{
    BLD_MSG_UNKNOWN_ID       = 0,        //!< Unknown message ID.
    BLD_MSG_CONN_REQ_ID      = 0x1011,   //!< todo message ID.
    BLD_MSG_CONN_RESP_ID     = 0xA011,   //!< todo response message ID.
    BLD_MSG_DATA_REQ_ID      = 0x1020,
    BLD_MSG_DATA_RESP_ID     = 0xA020,
    BLD_MSG_APP_DATA_REQ_ID  = 0x1030,
    BLD_MSG_APP_DATA_RESP_ID = 0xA030,
    BLD_MSG_PACKET_REQ_ID    = 0x1040,
    BLD_MSG_PACKET_RESP_ID   = 0xA040,
    BLD_MSG_DISCONN_REQ_ID   = 0x1050,
    BLD_MSG_DISCONN_RESP_ID  = 0xA050,
	BLD_MSG_BOOT_MODE_REQ_ID = 0x9999
}bld_com_message_id_t;

#endif
