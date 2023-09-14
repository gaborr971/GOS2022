################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../os/driver/src/gos_crc_driver.c \
../os/driver/src/gos_driver.c \
../os/driver/src/gos_shell_driver.c \
../os/driver/src/gos_sysmon_driver.c \
../os/driver/src/gos_timer_driver.c \
../os/driver/src/gos_trace_driver.c 

OBJS += \
./os/driver/src/gos_crc_driver.o \
./os/driver/src/gos_driver.o \
./os/driver/src/gos_shell_driver.o \
./os/driver/src/gos_sysmon_driver.o \
./os/driver/src/gos_timer_driver.o \
./os/driver/src/gos_trace_driver.o 

C_DEPS += \
./os/driver/src/gos_crc_driver.d \
./os/driver/src/gos_driver.d \
./os/driver/src/gos_shell_driver.d \
./os/driver/src/gos_sysmon_driver.d \
./os/driver/src/gos_timer_driver.d \
./os/driver/src/gos_trace_driver.d 


# Each subdirectory must supply rules for building sources it contributes
os/driver/src/%.o os/driver/src/%.su os/driver/src/%.cyclo: ../os/driver/src/%.c os/driver/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=c99 -g3 -DSTM32F446xx -DDEBUG -c -I"C:/Users/Gabor/STM32CubeIDE/workspace_1.5.1/GOS2022/os/services/inc" -I"C:/Users/Gabor/STM32CubeIDE/workspace_1.5.1/GOS2022/os/kernel/inc" -I"C:/Users/Gabor/STM32CubeIDE/workspace_1.5.1/GOS2022/os/driver/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-os-2f-driver-2f-src

clean-os-2f-driver-2f-src:
	-$(RM) ./os/driver/src/gos_crc_driver.cyclo ./os/driver/src/gos_crc_driver.d ./os/driver/src/gos_crc_driver.o ./os/driver/src/gos_crc_driver.su ./os/driver/src/gos_driver.cyclo ./os/driver/src/gos_driver.d ./os/driver/src/gos_driver.o ./os/driver/src/gos_driver.su ./os/driver/src/gos_shell_driver.cyclo ./os/driver/src/gos_shell_driver.d ./os/driver/src/gos_shell_driver.o ./os/driver/src/gos_shell_driver.su ./os/driver/src/gos_sysmon_driver.cyclo ./os/driver/src/gos_sysmon_driver.d ./os/driver/src/gos_sysmon_driver.o ./os/driver/src/gos_sysmon_driver.su ./os/driver/src/gos_timer_driver.cyclo ./os/driver/src/gos_timer_driver.d ./os/driver/src/gos_timer_driver.o ./os/driver/src/gos_timer_driver.su ./os/driver/src/gos_trace_driver.cyclo ./os/driver/src/gos_trace_driver.d ./os/driver/src/gos_trace_driver.o ./os/driver/src/gos_trace_driver.su

.PHONY: clean-os-2f-driver-2f-src

