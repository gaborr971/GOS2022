################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/driver/cfg/dma_cfg.c \
../Core/driver/cfg/gpio_cfg.c \
../Core/driver/cfg/it_cfg.c \
../Core/driver/cfg/uart_cfg.c 

OBJS += \
./Core/driver/cfg/dma_cfg.o \
./Core/driver/cfg/gpio_cfg.o \
./Core/driver/cfg/it_cfg.o \
./Core/driver/cfg/uart_cfg.o 

C_DEPS += \
./Core/driver/cfg/dma_cfg.d \
./Core/driver/cfg/gpio_cfg.d \
./Core/driver/cfg/it_cfg.d \
./Core/driver/cfg/uart_cfg.d 


# Each subdirectory must supply rules for building sources it contributes
Core/driver/cfg/%.o Core/driver/cfg/%.su Core/driver/cfg/%.cyclo: ../Core/driver/cfg/%.c Core/driver/cfg/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=c99 -g3 -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -c -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022/os/kernel/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022/os/services/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022/os/driver/inc" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_minimal_example/Core/driver/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/drv/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_minimal_example/Core/driver/cfg" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-driver-2f-cfg

clean-Core-2f-driver-2f-cfg:
	-$(RM) ./Core/driver/cfg/dma_cfg.cyclo ./Core/driver/cfg/dma_cfg.d ./Core/driver/cfg/dma_cfg.o ./Core/driver/cfg/dma_cfg.su ./Core/driver/cfg/gpio_cfg.cyclo ./Core/driver/cfg/gpio_cfg.d ./Core/driver/cfg/gpio_cfg.o ./Core/driver/cfg/gpio_cfg.su ./Core/driver/cfg/it_cfg.cyclo ./Core/driver/cfg/it_cfg.d ./Core/driver/cfg/it_cfg.o ./Core/driver/cfg/it_cfg.su ./Core/driver/cfg/uart_cfg.cyclo ./Core/driver/cfg/uart_cfg.d ./Core/driver/cfg/uart_cfg.o ./Core/driver/cfg/uart_cfg.su

.PHONY: clean-Core-2f-driver-2f-cfg

