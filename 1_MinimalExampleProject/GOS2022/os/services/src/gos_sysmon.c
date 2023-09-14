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
//                                      (c) Ahmed Gazar, 2023
//
//*************************************************************************************************
//! @file       gos_sysmon.c
//! @author     Ahmed Gazar
//! @date       2023-09-08
//! @version    1.1
//!
//! @brief      GOS system monitoring service source.
//! @details    For a more detailed description of this service, please refer to @ref gos_sysmon.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version    Date          Author          Description
// ------------------------------------------------------------------------------------------------
// 1.0        2023-07-12    Ahmed Gazar     Initial version created
// 1.1        2023-09-08    Ahmed Gazar     +    Missing comments added
//                                          *    Formatting fixed
//*************************************************************************************************
//
// Copyright (c) 2023 Ahmed Gazar
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
#include <gos_sysmon.h>
#include <gos_sysmon_driver.h>
#include <gos_gcp.h>
#include <string.h>

/*
 * Macros
 */
#define RECEIVE_BUFFER_SIZE ( 128u )

/*
 * Type definitions
 */
/**
 * System monitoring message ID enum.
 */
typedef enum
{
    GOS_SYSMON_MSG_UNKNOWN_ID                = 0,        //!< Unknown message ID.
    GOS_SYSMON_MSG_PING_ID                   = 0x1010,   //!< Ping message ID.
    GOS_SYSMON_MSG_PING_RESP_ID              = 0x50A0,   //!< Ping response message ID.
    GOS_SYSMON_MSG_CPU_USAGE_GET_ID          = 0x1023,   //!< CPU usage get message ID.
    GOS_SYSMON_MSG_CPU_USAGE_GET_RESP_ID     = 0x5C20,   //!< CPU usage get response message ID.
    GOS_SYSMON_MSG_TASK_GET_DATA_ID          = 0x1B67,   //!< Task data get message ID.
    GOS_SYSMON_MSG_TASK_GET_DATA_RESP_ID     = 0x5F8A,   //!< Task data get response message ID.
    GOS_SYSMON_MSG_TASK_GET_VAR_DATA_ID      = 0x12D5,   //!< Task variable data get message ID.
    GOS_SYSMON_MSG_TASK_GET_VAR_DATA_RESP_ID = 0x596B    //!< Task variable data get response message ID.
}gos_sysmonMessageId_t;

/**
 * System monitoring message protocol version enum.
 */
typedef enum
{
    GOS_SYSMON_MSG_UNKNOWN_PV                = 1,        //!< Unknown protocol version.
    GOS_SYSMON_MSG_PING_PV                   = 1,        //!< Ping message protocol version.
    GOS_SYSMON_MSG_PING_ACK_PV               = 1,        //!< Ping acknowledge message protocol version.
    GOS_SYSMON_MSG_CPU_USAGE_GET_PV          = 1,        //!< CPU usage get message protocol version.
    GOS_SYSMON_MSG_CPU_USAGE_GET_RESP_PV     = 1,        //!< CPU usage get response message protocol version.
    GOS_SYSMON_MSG_TASK_GET_DATA_PV          = 1,        //!< Task data get message protocol version.
    GOS_SYSMON_MSG_TASK_GET_DATA_RESP_PV     = 1,        //!< Task data get response message protocol version.
    GOS_SYSMON_MSG_TASK_GET_VAR_DATA_PV      = 1,        //!< Task variable data get message protocol version.
    GOS_SYSMON_MSG_TASK_GET_VAR_DATA_RESP_PV = 1         //!< Task variable data get response message protocol version.
}gos_sysmonMessagePv_t;

/**
 * Message result enum.
 */
typedef enum
{
    GOS_SYSMON_MSG_RES_OK          = 40,                 //!< Message result OK.
    GOS_SYSMON_MSG_RES_ERROR       = 99,                 //!< Message result ERROR.
    GOS_SYSMON_MSG_INV_PV          = 35,                 //!< Invalid protocol version.
    GOS_SYSMON_MSG_INV_PAYLOAD_CRC = 28                  //!< Invalid payload CRC.
}gos_sysmonMessageResult_t;

/**
 * Task data message structure.
 */
typedef struct __attribute__((packed))
{
    gos_taskState_t          taskState;                  //!< Task state.
    gos_taskPrio_t           taskPriority;               //!< Task priority.
    gos_taskPrio_t           taskOriginalPriority;       //!< Task original priority.
    gos_taskPrivilegeLevel_t taskPrivilegeLevel;         //!< Task privilege level.
    gos_taskName_t           taskName;                   //!< Task name.
    gos_tid_t                taskId;                     //!< Task ID (internal).
    gos_taskCSCounter_t      taskCsCounter;              //!< Task context-switch counter.
    gos_taskStackSize_t      taskStackSize;              //!< Task stack size.
    gos_runtime_t            taskRunTime;                //!< Task run-time.
    u16_t                    taskCpuUsageLimit;          //!< Task CPU usage limit in [% x 100].
    u16_t                    taskCpuUsageMax;            //!< Task CPU usage max value in [% x 100].
    u16_t                    taskCpuUsage;               //!< Task processor usage in [% x 100].
    gos_taskStackSize_t      taskStackMaxUsage;          //!< Task max. stack usage.
}gos_sysmonTaskData_t;

/**
 * Task variable data message structure.
 */
typedef struct __attribute__((packed))
{
    gos_taskState_t          taskState;                  //!< Task state.
    gos_taskPrio_t           taskPriority;               //!< Task priority.
    gos_taskCSCounter_t      taskCsCounter;              //!< Task context-switch counter.
    gos_runtime_t            taskRunTime;                //!< Task run-time.
    u16_t                    taskCpuUsageMax;            //!< Task CPU usage max value in [% x 100].
    u16_t                    taskCpuUsage;               //!< Task processor usage in [% x 100].
    gos_taskStackSize_t      taskStackMaxUsage;          //!< Task max. stack usage.
}gos_sysmonTaskVariableData;

/**
 * Ping message structure.
 */
typedef struct __attribute__((packed))
{
    gos_sysmonMessageResult_t messageResult;             //!< Message result.
}gos_sysmonPingMessage_t;

/**
 * CPU usage message structure.
 */
typedef struct __attribute__((packed))
{
    gos_sysmonMessageResult_t messageResult;             //!< Message result.
    u16_t                     cpuUsage;                  //!< CPU usage x100[%].
}gos_sysmonCpuUsageMessage_t;

/**
 * Task data get message structure.
 */
typedef struct __attribute__((packed))
{
    u16_t taskIndex;                                     //!< Task index.
}gos_sysmonTaskDataGetMessage_t;

/**
 * Task data message structure.
 */
typedef struct __attribute__((packed))
{
    gos_sysmonMessageResult_t messageResult;             //!< Message result.
    gos_sysmonTaskData_t      taskData;                  //!< Task data.
}gos_sysmonTaskDataMessage_t;

/**
 * Task variable data message structure.
 */
typedef struct __attribute__((packed))
{
    gos_sysmonMessageResult_t  messageResult;            //!< Message result.
    gos_sysmonTaskVariableData taskVariableData;         //!< Task variable data.
}gos_sysmonTaskVariableDataMessage_t;

/*
 * Static variables
 */
GOS_STATIC u8_t receiveBuffer [RECEIVE_BUFFER_SIZE];

/*
 * Function prototypes
 */
GOS_STATIC void_t gos_sysmonDaemonTask (void_t);

/**
 * Sysmon daemon task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t sysmonDaemonTaskDesc =
{
    .taskFunction        = gos_sysmonDaemonTask,
    .taskName            = "gos_sysmon_daemon",
    .taskPriority        = CFG_TASK_SYSMON_DAEMON_PRIO,
    .taskStackSize       = CFG_TASK_SYSMON_DAEMON_STACK,
    .taskPrivilegeLevel  = GOS_TASK_PRIVILEGE_KERNEL
};

/*
 * Function: gos_sysmonInit
 */
gos_result_t gos_sysmonInit (void_t)
{
    /*
     * Local variables.
     */
    gos_result_t sysmonInitResult = GOS_ERROR;

    /*
     * Function code.
     */
    if (gos_gcpRegisterPhysicalDriver(CFG_SYSMON_GCP_CHANNEL_NUM, gos_sysmonDriverTransmit,
        gos_sysmonDriverReceive) == GOS_SUCCESS &&
        gos_kernelTaskRegister(&sysmonDaemonTaskDesc, NULL) == GOS_SUCCESS)
    {
        sysmonInitResult = GOS_SUCCESS;
    }

    return sysmonInitResult;
}

/**
 * @brief   System monitoring daemon task.
 * @details Serves the incoming system monitoring messages.
 *
 * @return -
 */
GOS_STATIC void_t gos_sysmonDaemonTask (void_t)
{
    /*
     * Local variables.
     */
    gos_gcpMessageHeader_t              receiveMessageHeader  = {0};
    gos_gcpMessageHeader_t              transmitMessageHeader = {0};
    gos_sysmonPingMessage_t             pingMessage           = {0};
    gos_sysmonCpuUsageMessage_t         cpuMessage            = {0};
    gos_sysmonTaskDataGetMessage_t      taskDataGetMsg        = {0};
    gos_sysmonTaskDataMessage_t         taskDataMsg           = {0};
    gos_sysmonTaskVariableDataMessage_t taskVariableDataMsg   = {0};
    gos_taskDescriptor_t                taskDesc              = {0};
    u16_t                               taskIndex             = 0u;

    /*
     * Function code.
     */
    for (;;)
    {
        // Check if a message was received.
        if (gos_gcpReceiveMessage(CFG_SYSMON_GCP_CHANNEL_NUM, &receiveMessageHeader, receiveBuffer) == GOS_SUCCESS)
        {
            // Handle messages.
            switch (receiveMessageHeader.messageId)
            {
                case GOS_SYSMON_MSG_PING_ID:
                {
                    if (receiveMessageHeader.protocolVersion == GOS_SYSMON_MSG_PING_PV)
                    {
                        pingMessage.messageResult = GOS_SYSMON_MSG_RES_OK;
                    }
                    else
                    {
                        pingMessage.messageResult = GOS_SYSMON_MSG_INV_PV;
                    }

                    transmitMessageHeader.messageId       = GOS_SYSMON_MSG_PING_RESP_ID;
                    transmitMessageHeader.payloadSize     = sizeof(pingMessage);
                    transmitMessageHeader.protocolVersion = GOS_SYSMON_MSG_PING_ACK_PV;

                    (void_t) gos_gcpTransmitMessage(CFG_SYSMON_GCP_CHANNEL_NUM, &transmitMessageHeader, &pingMessage);
                    break;
                }
                case GOS_SYSMON_MSG_CPU_USAGE_GET_ID:
                {
                    if (receiveMessageHeader.protocolVersion == GOS_SYSMON_MSG_CPU_USAGE_GET_PV)
                    {
                        cpuMessage.cpuUsage      = gos_kernelGetCpuUsage();
                        cpuMessage.messageResult = GOS_SYSMON_MSG_RES_OK;
                    }
                    else
                    {
                        cpuMessage.messageResult = GOS_SYSMON_MSG_INV_PV;
                    }

                    transmitMessageHeader.messageId       = GOS_SYSMON_MSG_CPU_USAGE_GET_RESP_ID;
                    transmitMessageHeader.payloadSize     = sizeof(cpuMessage);
                    transmitMessageHeader.protocolVersion = GOS_SYSMON_MSG_CPU_USAGE_GET_RESP_PV;

                    (void_t) gos_gcpTransmitMessage(CFG_SYSMON_GCP_CHANNEL_NUM, &transmitMessageHeader, &cpuMessage);
                    break;
                }
                case GOS_SYSMON_MSG_TASK_GET_DATA_ID:
                {
                    if (receiveMessageHeader.protocolVersion == GOS_SYSMON_MSG_TASK_GET_DATA_PV)
                    {
                        (void_t) memcpy((void_t*)&taskDataGetMsg, (void_t*)receiveBuffer, receiveMessageHeader.payloadSize);

                        // Task get data message OK.
                        if (taskDataGetMsg.taskIndex == 0xFFFF)
                        {
                            // Send all task data.
                            for (taskIndex = 0; taskIndex < CFG_TASK_MAX_NUMBER; taskIndex++)
                            {
                                (void_t) gos_kernelTaskGetDataByIndex(taskIndex, &taskDesc);

                                if (taskDesc.taskId != GOS_INVALID_TASK_ID)
                                {
                                    taskDataMsg.taskData.taskId               = taskDesc.taskId;
                                    taskDataMsg.taskData.taskStackSize        = taskDesc.taskStackSize;
                                    taskDataMsg.taskData.taskStackMaxUsage    = taskDesc.taskStackMaxUsage;
                                    taskDataMsg.taskData.taskCpuUsageLimit    = taskDesc.taskCpuUsageLimit;
                                    taskDataMsg.taskData.taskCpuUsage         = taskDesc.taskCpuUsage;
                                    taskDataMsg.taskData.taskCpuUsageMax      = taskDesc.taskCpuUsageMax;
                                    taskDataMsg.taskData.taskOriginalPriority = taskDesc.taskOriginalPriority;
                                    taskDataMsg.taskData.taskPriority         = taskDesc.taskPriority;
                                    taskDataMsg.taskData.taskCsCounter        = taskDesc.taskCsCounter;
                                    taskDataMsg.taskData.taskPrivilegeLevel   = taskDesc.taskPrivilegeLevel;
                                    taskDataMsg.taskData.taskState            = taskDesc.taskState;

                                    (void_t) memcpy((void_t*)&taskDataMsg.taskData.taskRunTime, (void_t*)&taskDesc.taskRunTime, sizeof(taskDesc.taskRunTime));
                                    (void_t) strcpy(taskDataMsg.taskData.taskName, taskDesc.taskName);

                                    taskDataMsg.messageResult = GOS_SYSMON_MSG_RES_OK;
                                }
                                else
                                {
                                    // Last task found.
                                    taskDataMsg.messageResult = GOS_SYSMON_MSG_RES_ERROR;
                                }

                                transmitMessageHeader.messageId = GOS_SYSMON_MSG_TASK_GET_DATA_RESP_ID;
                                transmitMessageHeader.payloadSize = sizeof(taskDataMsg);
                                transmitMessageHeader.protocolVersion = GOS_SYSMON_MSG_TASK_GET_DATA_RESP_PV;

                                (void_t) gos_gcpTransmitMessage(CFG_SYSMON_GCP_CHANNEL_NUM, &transmitMessageHeader, &taskDataMsg);
                            }

                            break;
                        }
                        else
                        {
                            // Send specific task data.
                            (void_t) gos_kernelTaskGetDataByIndex(taskDataGetMsg.taskIndex, &taskDesc);

                            taskDataMsg.taskData.taskId               = taskDesc.taskId;
                            taskDataMsg.taskData.taskStackSize        = taskDesc.taskStackSize;
                            taskDataMsg.taskData.taskStackMaxUsage    = taskDesc.taskStackMaxUsage;
                            taskDataMsg.taskData.taskCpuUsageLimit    = taskDesc.taskCpuUsageLimit;
                            taskDataMsg.taskData.taskCpuUsage         = taskDesc.taskCpuUsage;
                            taskDataMsg.taskData.taskCpuUsageMax      = taskDesc.taskCpuUsageMax;
                            taskDataMsg.taskData.taskOriginalPriority = taskDesc.taskOriginalPriority;
                            taskDataMsg.taskData.taskPriority         = taskDesc.taskPriority;
                            taskDataMsg.taskData.taskCsCounter        = taskDesc.taskCsCounter;
                            taskDataMsg.taskData.taskPrivilegeLevel   = taskDesc.taskPrivilegeLevel;
                            taskDataMsg.taskData.taskState            = taskDesc.taskState;

                            (void_t) memcpy((void_t*)&taskDataMsg.taskData.taskRunTime, (void_t*)&taskDesc.taskRunTime, sizeof(taskDesc.taskRunTime));
                            (void_t) strcpy(taskDataMsg.taskData.taskName, taskDesc.taskName);

                            taskDataMsg.messageResult = GOS_SYSMON_MSG_RES_OK;
                        }
                    }
                    else
                    {
                        taskDataMsg.messageResult = GOS_SYSMON_MSG_INV_PV;
                    }
                    transmitMessageHeader.messageId       = GOS_SYSMON_MSG_TASK_GET_DATA_RESP_ID;
                    transmitMessageHeader.payloadSize     = sizeof(taskDataMsg);
                    transmitMessageHeader.protocolVersion = GOS_SYSMON_MSG_TASK_GET_DATA_RESP_PV;

                    (void_t) gos_gcpTransmitMessage(CFG_SYSMON_GCP_CHANNEL_NUM, &transmitMessageHeader, &taskDataMsg);
                    break;
                }
                case GOS_SYSMON_MSG_TASK_GET_VAR_DATA_ID:
                {
                    if (receiveMessageHeader.protocolVersion == GOS_SYSMON_MSG_TASK_GET_VAR_DATA_PV)
                    {
                        (void_t) memcpy((void_t*)&taskDataGetMsg, (void_t*)receiveBuffer, receiveMessageHeader.payloadSize);

                        // Task get data message OK.
                        if (taskDataGetMsg.taskIndex == 0xFFFF)
                        {
                            // Send all task data.
                            for (taskIndex = 0; taskIndex < CFG_TASK_MAX_NUMBER; taskIndex++)
                            {
                                (void_t) gos_kernelTaskGetDataByIndex(taskIndex, &taskDesc);

                                if (taskDesc.taskId != GOS_INVALID_TASK_ID)
                                {
                                    taskVariableDataMsg.taskVariableData.taskStackMaxUsage = taskDesc.taskStackMaxUsage;
                                    taskVariableDataMsg.taskVariableData.taskCpuUsage      = taskDesc.taskCpuUsage;
                                    taskVariableDataMsg.taskVariableData.taskCpuUsageMax   = taskDesc.taskCpuUsageMax;
                                    taskVariableDataMsg.taskVariableData.taskPriority      = taskDesc.taskPriority;
                                    taskVariableDataMsg.taskVariableData.taskCsCounter     = taskDesc.taskCsCounter;
                                    taskVariableDataMsg.taskVariableData.taskState         = taskDesc.taskState;

                                    (void_t) memcpy((void_t*)&taskVariableDataMsg.taskVariableData.taskRunTime, (void_t*)&taskDesc.taskRunTime, sizeof(taskDesc.taskRunTime));

                                    taskVariableDataMsg.messageResult = GOS_SYSMON_MSG_RES_OK;

                                }
                                else
                                {
                                    // Last task found.
                                    taskVariableDataMsg.messageResult = GOS_SYSMON_MSG_RES_ERROR;
                                }

                                transmitMessageHeader.messageId       = GOS_SYSMON_MSG_TASK_GET_VAR_DATA_RESP_ID;
                                transmitMessageHeader.payloadSize     = sizeof(taskVariableDataMsg);
                                transmitMessageHeader.protocolVersion = GOS_SYSMON_MSG_TASK_GET_VAR_DATA_RESP_PV;

                                (void_t) gos_gcpTransmitMessage(CFG_SYSMON_GCP_CHANNEL_NUM, &transmitMessageHeader, &taskVariableDataMsg);
                            }
                            break;
                        }
                        else
                        {
                            // Send specific task data.
                            (void_t) gos_kernelTaskGetDataByIndex(taskDataGetMsg.taskIndex, &taskDesc);

                            taskVariableDataMsg.taskVariableData.taskStackMaxUsage = taskDesc.taskStackMaxUsage;
                            taskVariableDataMsg.taskVariableData.taskCpuUsage      = taskDesc.taskCpuUsage;
                            taskVariableDataMsg.taskVariableData.taskCpuUsageMax   = taskDesc.taskCpuUsageMax;
                            taskVariableDataMsg.taskVariableData.taskPriority      = taskDesc.taskPriority;
                            taskVariableDataMsg.taskVariableData.taskCsCounter     = taskDesc.taskCsCounter;
                            taskVariableDataMsg.taskVariableData.taskState         = taskDesc.taskState;

                            (void_t) memcpy((void_t*)&taskVariableDataMsg.taskVariableData.taskRunTime, (void_t*)&taskDesc.taskRunTime, sizeof(taskDesc.taskRunTime));

                            taskVariableDataMsg.messageResult = GOS_SYSMON_MSG_RES_OK;
                        }
                    }
                    else
                    {
                        taskVariableDataMsg.messageResult = GOS_SYSMON_MSG_INV_PV;
                    }
                    transmitMessageHeader.messageId       = GOS_SYSMON_MSG_TASK_GET_VAR_DATA_RESP_ID;
                    transmitMessageHeader.payloadSize     = sizeof(taskVariableDataMsg);
                    transmitMessageHeader.protocolVersion = GOS_SYSMON_MSG_TASK_GET_VAR_DATA_RESP_PV;

                    (void_t) gos_gcpTransmitMessage(CFG_SYSMON_GCP_CHANNEL_NUM, &transmitMessageHeader, &taskVariableDataMsg);
                    break;
                }
                default:
                {
                    transmitMessageHeader.messageId = GOS_SYSMON_MSG_UNKNOWN_ID;
                    transmitMessageHeader.payloadSize = 0u;
                    transmitMessageHeader.protocolVersion = GOS_SYSMON_MSG_UNKNOWN_PV;

                    (void_t) gos_gcpTransmitMessage(CFG_SYSMON_GCP_CHANNEL_NUM, &transmitMessageHeader, NULL);
                    break;
                }
            }
        }
    }
}
