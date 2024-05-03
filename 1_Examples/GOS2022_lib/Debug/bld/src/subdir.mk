################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bld/src/bld.c \
../bld/src/bld_data.c \
../bld/src/bld_statemachine.c 

OBJS += \
./bld/src/bld.o \
./bld/src/bld_data.o \
./bld/src/bld_statemachine.o 

C_DEPS += \
./bld/src/bld.d \
./bld/src/bld_data.d \
./bld/src/bld_statemachine.d 


# Each subdirectory must supply rules for building sources it contributes
bld/src/%.o bld/src/%.su bld/src/%.cyclo: ../bld/src/%.c bld/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DSTM32F446xx -c -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022/os/kernel/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022/os/driver/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022/os/services/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/drv/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/bld/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/Inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/hal/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/hal/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/hal/CMSIS/Include" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/hal/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/svl/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-bld-2f-src

clean-bld-2f-src:
	-$(RM) ./bld/src/bld.cyclo ./bld/src/bld.d ./bld/src/bld.o ./bld/src/bld.su ./bld/src/bld_data.cyclo ./bld/src/bld_data.d ./bld/src/bld_data.o ./bld/src/bld_data.su ./bld/src/bld_statemachine.cyclo ./bld/src/bld_statemachine.d ./bld/src/bld_statemachine.o ./bld/src/bld_statemachine.su

.PHONY: clean-bld-2f-src

