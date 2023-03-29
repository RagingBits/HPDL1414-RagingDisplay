################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32.s 

OBJS += \
./startup/startup_stm32.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	arm-none-eabi-as -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -I"D:/STUFF/work/STM32Workspace/HPDL1414_DISPLAY/StdPeriph_Driver/inc" -I"D:/STUFF/work/STM32Workspace/HPDL1414_DISPLAY/inc" -I"D:/STUFF/work/STM32Workspace/HPDL1414_DISPLAY/CMSIS/device" -I"D:/STUFF/work/STM32Workspace/HPDL1414_DISPLAY/CMSIS/core" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


