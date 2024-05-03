################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../os/kernel/src/gos_kernel.c \
../os/kernel/src/gos_task.c 

OBJS += \
./os/kernel/src/gos_kernel.o \
./os/kernel/src/gos_task.o 

C_DEPS += \
./os/kernel/src/gos_kernel.d \
./os/kernel/src/gos_task.d 


# Each subdirectory must supply rules for building sources it contributes
os/kernel/src/%.o os/kernel/src/%.su os/kernel/src/%.cyclo: ../os/kernel/src/%.c os/kernel/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=c99 -g3 -DSTM32F446xx -DDEBUG -DGOS2022_SYSMON_EXAMPLE -c -I"C:/Users/Gabor/STM32CubeIDE/workspace_1.5.1/GOS2022/os/services/inc" -I"C:/Users/Gabor/STM32CubeIDE/workspace_1.5.1/GOS2022/os/kernel/inc" -I"C:/Users/Gabor/STM32CubeIDE/workspace_1.5.1/GOS2022/os/driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-os-2f-kernel-2f-src

clean-os-2f-kernel-2f-src:
	-$(RM) ./os/kernel/src/gos_kernel.cyclo ./os/kernel/src/gos_kernel.d ./os/kernel/src/gos_kernel.o ./os/kernel/src/gos_kernel.su ./os/kernel/src/gos_task.cyclo ./os/kernel/src/gos_task.d ./os/kernel/src/gos_task.o ./os/kernel/src/gos_task.su

.PHONY: clean-os-2f-kernel-2f-src

