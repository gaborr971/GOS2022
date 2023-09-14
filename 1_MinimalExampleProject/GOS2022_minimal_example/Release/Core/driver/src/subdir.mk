################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/driver/src/driver.c \
../Core/driver/src/gpio_driver.c \
../Core/driver/src/iodef.c \
../Core/driver/src/rcc_driver.c \
../Core/driver/src/timer_driver.c \
../Core/driver/src/uart_driver.c 

OBJS += \
./Core/driver/src/driver.o \
./Core/driver/src/gpio_driver.o \
./Core/driver/src/iodef.o \
./Core/driver/src/rcc_driver.o \
./Core/driver/src/timer_driver.o \
./Core/driver/src/uart_driver.o 

C_DEPS += \
./Core/driver/src/driver.d \
./Core/driver/src/gpio_driver.d \
./Core/driver/src/iodef.d \
./Core/driver/src/rcc_driver.d \
./Core/driver/src/timer_driver.d \
./Core/driver/src/uart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
Core/driver/src/%.o Core/driver/src/%.su Core/driver/src/%.cyclo: ../Core/driver/src/%.c Core/driver/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I"C:/Users/Gabor/STM32CubeIDE/workspace_1.5.1/GOS2022" -I"C:/Users/Gabor/STM32CubeIDE/workspace_1.5.1/GOS2022/os/driver/inc" -I"C:/Users/Gabor/STM32CubeIDE/workspace_1.5.1/GOS2022/os/kernel/inc" -I"C:/Users/Gabor/STM32CubeIDE/workspace_1.5.1/GOS2022/os/services/inc" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Gabor/STM32CubeIDE/workspace_1.5.1/GOS2022_minimal_example/Core/driver/inc" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-driver-2f-src

clean-Core-2f-driver-2f-src:
	-$(RM) ./Core/driver/src/driver.cyclo ./Core/driver/src/driver.d ./Core/driver/src/driver.o ./Core/driver/src/driver.su ./Core/driver/src/gpio_driver.cyclo ./Core/driver/src/gpio_driver.d ./Core/driver/src/gpio_driver.o ./Core/driver/src/gpio_driver.su ./Core/driver/src/iodef.cyclo ./Core/driver/src/iodef.d ./Core/driver/src/iodef.o ./Core/driver/src/iodef.su ./Core/driver/src/rcc_driver.cyclo ./Core/driver/src/rcc_driver.d ./Core/driver/src/rcc_driver.o ./Core/driver/src/rcc_driver.su ./Core/driver/src/timer_driver.cyclo ./Core/driver/src/timer_driver.d ./Core/driver/src/timer_driver.o ./Core/driver/src/timer_driver.su ./Core/driver/src/uart_driver.cyclo ./Core/driver/src/uart_driver.d ./Core/driver/src/uart_driver.o ./Core/driver/src/uart_driver.su

.PHONY: clean-Core-2f-driver-2f-src

