################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../os/services/src/gos.c \
../os/services/src/gos_error.c \
../os/services/src/gos_gcp.c \
../os/services/src/gos_message.c \
../os/services/src/gos_mutex.c \
../os/services/src/gos_process.c \
../os/services/src/gos_queue.c \
../os/services/src/gos_shell.c \
../os/services/src/gos_signal.c \
../os/services/src/gos_sysmon.c \
../os/services/src/gos_time.c \
../os/services/src/gos_trace.c \
../os/services/src/gos_trigger.c 

OBJS += \
./os/services/src/gos.o \
./os/services/src/gos_error.o \
./os/services/src/gos_gcp.o \
./os/services/src/gos_message.o \
./os/services/src/gos_mutex.o \
./os/services/src/gos_process.o \
./os/services/src/gos_queue.o \
./os/services/src/gos_shell.o \
./os/services/src/gos_signal.o \
./os/services/src/gos_sysmon.o \
./os/services/src/gos_time.o \
./os/services/src/gos_trace.o \
./os/services/src/gos_trigger.o 

C_DEPS += \
./os/services/src/gos.d \
./os/services/src/gos_error.d \
./os/services/src/gos_gcp.d \
./os/services/src/gos_message.d \
./os/services/src/gos_mutex.d \
./os/services/src/gos_process.d \
./os/services/src/gos_queue.d \
./os/services/src/gos_shell.d \
./os/services/src/gos_signal.d \
./os/services/src/gos_sysmon.d \
./os/services/src/gos_time.d \
./os/services/src/gos_trace.d \
./os/services/src/gos_trigger.d 


# Each subdirectory must supply rules for building sources it contributes
os/services/src/%.o os/services/src/%.su os/services/src/%.cyclo: ../os/services/src/%.c os/services/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I"C:/Users/Gabor/STM32CubeIDE/workspace_1.5.1/GOS2022/os/services/inc" -I"C:/Users/Gabor/STM32CubeIDE/workspace_1.5.1/GOS2022/os/kernel/inc" -I"C:/Users/Gabor/STM32CubeIDE/workspace_1.5.1/GOS2022/os/driver/inc" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-os-2f-services-2f-src

clean-os-2f-services-2f-src:
	-$(RM) ./os/services/src/gos.cyclo ./os/services/src/gos.d ./os/services/src/gos.o ./os/services/src/gos.su ./os/services/src/gos_error.cyclo ./os/services/src/gos_error.d ./os/services/src/gos_error.o ./os/services/src/gos_error.su ./os/services/src/gos_gcp.cyclo ./os/services/src/gos_gcp.d ./os/services/src/gos_gcp.o ./os/services/src/gos_gcp.su ./os/services/src/gos_message.cyclo ./os/services/src/gos_message.d ./os/services/src/gos_message.o ./os/services/src/gos_message.su ./os/services/src/gos_mutex.cyclo ./os/services/src/gos_mutex.d ./os/services/src/gos_mutex.o ./os/services/src/gos_mutex.su ./os/services/src/gos_process.cyclo ./os/services/src/gos_process.d ./os/services/src/gos_process.o ./os/services/src/gos_process.su ./os/services/src/gos_queue.cyclo ./os/services/src/gos_queue.d ./os/services/src/gos_queue.o ./os/services/src/gos_queue.su ./os/services/src/gos_shell.cyclo ./os/services/src/gos_shell.d ./os/services/src/gos_shell.o ./os/services/src/gos_shell.su ./os/services/src/gos_signal.cyclo ./os/services/src/gos_signal.d ./os/services/src/gos_signal.o ./os/services/src/gos_signal.su ./os/services/src/gos_sysmon.cyclo ./os/services/src/gos_sysmon.d ./os/services/src/gos_sysmon.o ./os/services/src/gos_sysmon.su ./os/services/src/gos_time.cyclo ./os/services/src/gos_time.d ./os/services/src/gos_time.o ./os/services/src/gos_time.su ./os/services/src/gos_trace.cyclo ./os/services/src/gos_trace.d ./os/services/src/gos_trace.o ./os/services/src/gos_trace.su ./os/services/src/gos_trigger.cyclo ./os/services/src/gos_trigger.d ./os/services/src/gos_trigger.o ./os/services/src/gos_trigger.su

.PHONY: clean-os-2f-services-2f-src

