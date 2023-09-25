# System monitoring example project
This project demonstrates the setup of the OS, and a minimal application to show how to use the APIs.

The project is created for an STM32F446RE board with the following properties:
-	UART2 is used as the trace output (no additional hardware is required to see this output, it is connected via the USB)
-	UART5 is used as the system monitoring input/output (an additional UART-USB converter is needed to have access to the sysmon)
-	Both UARTs are configured to run at 115200 baud
-	Besides the system tasks, the following user tasks are defined:
	-	LED blink task (to toggle the user LED on the board every 1 second, and increment the lifesign trigger)
	-	Life sign task (to receive the trigger from the LED task with a timeout, or reset the system in case the trigger was not received within 3 seconds)
	-	Button task (to receive a trigger from the user button interrupt callback and call a kernel dump)
	-	Low, medium, and high priority tasks for mutex demonstration

Observe the code and the behavior of the system. The system monitoring service is enabled and runs in the background when a request is
received. To try this service, use the attached GUI to retrieve monitoring data from the OS.

Last updated: 2023-09-25
(c) 2023, Ahmed Gazar
