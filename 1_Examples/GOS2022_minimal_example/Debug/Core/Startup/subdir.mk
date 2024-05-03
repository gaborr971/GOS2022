################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f446retx.s 

OBJS += \
./Core/Startup/startup_stm32f446retx.o 

S_DEPS += \
./Core/Startup/startup_stm32f446retx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DSTM32F446xx -c -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022/os/kernel/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022/os/services/inc" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022/os/driver/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_minimal_example/Core/driver/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/drv/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_minimal_example/Core/driver/cfg" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32f446retx.d ./Core/Startup/startup_stm32f446retx.o

.PHONY: clean-Core-2f-Startup

