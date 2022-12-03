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
//! @file		gos_shell.c
//! @author		Gabor Repasi
//! @date		2022-12-03
//! @version	1.2
//!
//! @brief		GOS shell service source.
//! @details	For a more detailed description of this service, please refer to @ref gos_shell.h
//*************************************************************************************************
// History
// ------------------------------------------------------------------------------------------------
// Version	Date		Author			Description
// ------------------------------------------------------------------------------------------------
// 1.0		2022-11-04	Gabor Repasi	Initial version created.
// 1.1		2022-11-15	Gabor Repasi	+	License added
// 1.2		2022-12-03	Gabor Repasi	+	gos_shellRegisterCommands added
//										*	Actual command and command parameter buffers
//											made modul global instead of task local
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
#include <string.h>
#include <stdio.h>
#include "gos_log.h"
#include "gos_shell.h"
#include "gos_uart.h"

/*
 * Static variables
 */
/**
 * Shell command array.
 */
GOS_STATIC gos_shellCommand_t	shellCommands		[CFG_SHELL_MAX_COMMAND_NUMBER];

/**
 * Shell daemon task ID.
 */
GOS_STATIC gos_tid_t			shellDaemonTaskId;

/**
 * Command buffer.
 */
GOS_STATIC char_t				commandBuffer		[CFG_SHELL_MAX_COMMAND_LENGTH + 1];

/**
 * Command buffer index.
 */
GOS_STATIC u16_t				commandBufferIndex;

/**
 * Shell echo flag.
 */
GOS_STATIC bool_t				useEcho;

/**
 * Actual command buffer.
 */
GOS_STATIC char_t				actualCommand	[CFG_SHELL_MAX_COMMAND_LENGTH];

/**
 * Command parameters buffer.
 */
GOS_STATIC char_t				commandParams	[CFG_SHELL_MAX_COMMAND_LENGTH];

/*
 * Function prototypes
 */
GOS_STATIC void_t gos_shellDaemonTask (void_t);

/**
 * Shell daemon task descriptor.
 */
GOS_STATIC gos_taskDescriptor_t shellDaemonTaskDesc =
{
	.taskFunction 	= gos_shellDaemonTask,
	.taskName		= "gos_shell_task",
	.taskPriority	= CFG_TASK_SHELL_DAEMON_PRIO,
	.taskStackSize	= CFG_TASK_SHELL_DAEMON_STACK
};

/*
 * Function: gos_shellInit
 */
gos_result_t gos_shellInit (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t 			shellInitResult	= GOS_ERROR;
	gos_shellCommandIndex_t	index			= 0u;

	/*
	 * Function code.
	 */
	useEcho = GOS_TRUE;

	for (index = 0u; index < CFG_SHELL_MAX_COMMAND_NUMBER; index++)
	{
		shellCommands[index].commandHandler = NULL;
	}

	if (gos_kernelTaskRegister(&shellDaemonTaskDesc, &shellDaemonTaskId) == GOS_SUCCESS)
	{
		shellInitResult = GOS_SUCCESS;
	}
	return shellInitResult;
}

/*
 * Function: gos_shellRegisterCommands
 */
gos_result_t gos_shellRegisterCommands (gos_shellCommand_t* commands, u16_t arraySize)
{
	/*
	 * Local variables.
	 */
	gos_result_t registerResult = GOS_ERROR;
	u16_t numberOfCommands = 0u;
	u16_t index = 0u;

	/*
	 * Function code.
	 */
	if (commands != NULL)
	{
		numberOfCommands = arraySize / sizeof(commands[0]);

		for (index = 0u; index < numberOfCommands; index++)
		{
			if (gos_shellRegisterCommand(&commands[index]) != GOS_SUCCESS)
			{
				break;
			}
		}

		if (index == numberOfCommands)
		{
			registerResult = GOS_SUCCESS;
		}
	}

	return registerResult;
}

/*
 * Function: gos_shellRegisterCommand
 */
gos_result_t gos_shellRegisterCommand (gos_shellCommand_t* command)
{
	/*
	 * Local variables.
	 */
	gos_result_t 			shellRegisterCommandResult	= GOS_ERROR;
	gos_shellCommandIndex_t	index						= 0u;

	/*
	 * Function code.
	 */
	if (command->commandHandler != NULL && command->command != NULL)
	{
		for (index = 0u; index < CFG_SHELL_MAX_COMMAND_NUMBER; index++)
		{
			if (shellCommands[index].commandHandler == NULL)
			{
				shellCommands[index].commandHandler = command->commandHandler;
				strcpy(shellCommands[index].command, command->command);
				shellRegisterCommandResult = GOS_SUCCESS;
				break;
			}
		}
	}
	return shellRegisterCommandResult;
}

/*
 * Function: gos_shellSuspend
 */
gos_result_t gos_shellSuspend (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t shellSuspendResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	shellSuspendResult = gos_kernelTaskSuspend(shellDaemonTaskId);

	return shellSuspendResult;
}

/*
 * Function: gos_shellResume
 */
gos_result_t gos_shellResume (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t shellResumeResult = GOS_ERROR;

	/*
	 * Function code.
	 */
	shellResumeResult = gos_kernelTaskResume(shellDaemonTaskId);

	return shellResumeResult;
}

/*
 * Function: gos_shellEchoOn
 */
gos_result_t gos_shellEchoOn (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t shellEchoOnResult = GOS_SUCCESS;

	/*
	 * Function code.
	 */
	useEcho = GOS_TRUE;

	return shellEchoOnResult;
}

/*
 * Function: gos_shellEchoOff
 */
gos_result_t gos_shellEchoOff (void_t)
{
	/*
	 * Local variables.
	 */
	gos_result_t shellEchoOffResult = GOS_SUCCESS;

	/*
	 * Function code.
	 */
	useEcho = GOS_FALSE;

	return shellEchoOffResult;
}

/**
 * @brief	Shell daemon task.
 * @details	Receives a character from the log serial line, if the echoing is on, then
 * 			sends the same character back. When an enter key is received, it processes
 * 			the command typed in, and loops through the command array. When the command
 * 			is found, it calls the command handler function with the parameter list as
 * 			a string.
 *
 * @return	-
 */
GOS_STATIC void_t gos_shellDaemonTask (void_t)
{
	/*
	 * Local variables.
	 */
	gos_shellCommandIndex_t index = 0u;
	u16_t					actualCommandIndex;
	u16_t					paramIndex;

	/*
	 * Function code.
	 */
	for(;;)
	{
		if (gos_uartReceiveChar(CFG_LOG_PORT, &commandBuffer[commandBufferIndex]) == GOS_SUCCESS)
		{
			if (useEcho == GOS_TRUE)
			{
				gos_uartTransmit(CFG_LOG_PORT, &commandBuffer[commandBufferIndex]);
			}

			/*
			 * Backspace character check
			 */
			if (commandBuffer[commandBufferIndex] == '\177')
			{
				commandBuffer[commandBufferIndex] = '\0';
				commandBufferIndex--;
			}

			/*
			 * Enter character check
			 */
			else if (commandBuffer[commandBufferIndex] == '\r')
			{
				if (useEcho == GOS_TRUE)
				{
					gos_uartTransmit(CFG_LOG_PORT, "\n");
				}

				commandBuffer[commandBufferIndex] = '\0';
				actualCommandIndex = 0u;
				paramIndex = 0u;

				// Get command.
				while (commandBuffer[actualCommandIndex] != ' ' && commandBuffer[actualCommandIndex] != '\0')
				{
					actualCommand[actualCommandIndex] = commandBuffer[actualCommandIndex];
					actualCommandIndex++;
				}
				actualCommand[actualCommandIndex] = '\0';
				actualCommandIndex++;

				// Get parameters.
				while (commandBuffer[actualCommandIndex] != '\0')
				{
					commandParams[paramIndex++] = commandBuffer[actualCommandIndex++];
				}
				commandParams[paramIndex] = '\0';

				for (index = 0u; index < CFG_SHELL_MAX_COMMAND_NUMBER; index++)
				{
					if (strcmp(shellCommands[index].command, actualCommand) == 0)
					{
						if (shellCommands[index].commandHandler != NULL)
						{
							shellCommands[index].commandHandler(commandParams);
						}
						break;
					}
				}

				// If command not found.
				if (index == CFG_SHELL_MAX_COMMAND_NUMBER)
				{
					gos_logLog("Unrecognized command!\r\n");
				}

				memset((void_t*)commandBuffer, '\0', (CFG_SHELL_MAX_COMMAND_LENGTH + 1) * sizeof(char_t));
				commandBufferIndex = 0u;
			}
			else
			{
				commandBufferIndex++;
			}
		}
		gos_kernelTaskSleep(50);
	}
}
