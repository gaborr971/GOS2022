# Minimal example project
This project demonstrates the setup of the OS, and a minimal application to show how to use the APIs.

The project is created for an STM32F446RE board with the following properties:
-	UART2 is used as the trace output (no additional hardware is required to see this output, it is connected via the USB)
-	UART5 is used as the shell input/output (an additional UART-USB converter is needed to have access to the shell)
-	Both UARTs are configured to run at 115200 baud
-	Besides the system tasks, the following user tasks are defined:
	-	LED blink task (to toggle the user LED on the board every 1 second, and increment the lifesign trigger)
	-	Life sign task (to receive the trigger from the LED task with a timeout, or reset the system in case the trigger was not received within 3 seconds)
	-	Button task (to receive a trigger from the user button interrupt callback and call a kernel dump)
	-	Low, medium, and high priority tasks for mutex demonstration

Observe the code and the behavior of the system. The shell is configured, built-in commands can be run (type 'shell help' for available command parameters).
The syntax to execute shell commands is 'shell <parameter>'. For example, 'shell runtime' will print out the system runtime from last reset on the shell output.

Last updated: 2023-09-14
(c) 2023, Ahmed Gazar
