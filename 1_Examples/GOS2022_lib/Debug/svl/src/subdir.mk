################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../svl/src/svl_bldHandler.c \
../svl/src/svl_dhs.c \
../svl/src/svl_frs.c \
../svl/src/svl_hwi.c 

OBJS += \
./svl/src/svl_bldHandler.o \
./svl/src/svl_dhs.o \
./svl/src/svl_frs.o \
./svl/src/svl_hwi.o 

C_DEPS += \
./svl/src/svl_bldHandler.d \
./svl/src/svl_dhs.d \
./svl/src/svl_frs.d \
./svl/src/svl_hwi.d 


# Each subdirectory must supply rules for building sources it contributes
svl/src/%.o svl/src/%.su svl/src/%.cyclo: ../svl/src/%.c svl/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DSTM32F446xx -c -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022/os/kernel/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022/os/driver/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022/os/services/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/drv/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/bld/inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/Inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/hal/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/hal/STM32F4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/hal/CMSIS/Include" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/hal/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Gabor/Documents/GitHub/GOS2022/1_Examples/GOS2022_lib/svl/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-svl-2f-src

clean-svl-2f-src:
	-$(RM) ./svl/src/svl_bldHandler.cyclo ./svl/src/svl_bldHandler.d ./svl/src/svl_bldHandler.o ./svl/src/svl_bldHandler.su ./svl/src/svl_dhs.cyclo ./svl/src/svl_dhs.d ./svl/src/svl_dhs.o ./svl/src/svl_dhs.su ./svl/src/svl_frs.cyclo ./svl/src/svl_frs.d ./svl/src/svl_frs.o ./svl/src/svl_frs.su ./svl/src/svl_hwi.cyclo ./svl/src/svl_hwi.d ./svl/src/svl_hwi.o ./svl/src/svl_hwi.su

.PHONY: clean-svl-2f-src

