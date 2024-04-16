# Repository content
This repository contains the design and production files for the Main Board. The board is designed in Fritzing.

# Description
The purpose of the main board is to provide a unified platform for the GOS2022 test application.
The board contains the following hardware features:
- UART / USB converter circuits with mini USB connectors (3)
- 24LC256 on-board EEPROMs (2)
- LM75 on-board temperature sensor (1)
- MCP23017 on-board I/O extender (2)
- Slot for STM32F446RE
- Slot for WeMos D1 mini
- Dedicated connector pin header for SPI TFT display
- Pin headers for: I2C bus, SPI bus, ADC channels, DAC output, PWM outputs, EXTI pins, 3V3 and 5V power lines.

# Revision history
Current revision: A1 (2024-03-13)

# Known issues
- I2C2 line does not work (wrong wiring) [ confirmed issue ]
	Workaround: cut SDA and SCL lines and connect to I2C1 (pins will function as extra connection points to I2C1)
- Only one 24LC256 works properly (address: 0x51) [ under testing ]
- USART2 connection may not be working [ under testing ]

# Remarks
Contribution to this project is encouraged and appriciated.
If you have any questions or ideas, feel free to contact me: gaborr97@gmail.com

Last updated: 2024-04-16
(c) 2024, Ahmed Gazar
