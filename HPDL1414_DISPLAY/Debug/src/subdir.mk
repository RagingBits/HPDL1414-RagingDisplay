################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/at_cmd.c \
../src/epoch.c \
../src/hpdl1414.c \
../src/io_setup.c \
../src/main.c \
../src/rtc.c \
../src/stm32f10x_it.c \
../src/system_stm32f10x.c \
../src/uart.c 

OBJS += \
./src/at_cmd.o \
./src/epoch.o \
./src/hpdl1414.o \
./src/io_setup.o \
./src/main.o \
./src/rtc.o \
./src/stm32f10x_it.o \
./src/system_stm32f10x.o \
./src/uart.o 

C_DEPS += \
./src/at_cmd.d \
./src/epoch.d \
./src/hpdl1414.d \
./src/io_setup.d \
./src/main.d \
./src/rtc.d \
./src/stm32f10x_it.d \
./src/system_stm32f10x.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"D:/STUFF/work/STM32Workspace/HPDL1414_DISPLAY/StdPeriph_Driver/inc" -I"D:/STUFF/work/STM32Workspace/HPDL1414_DISPLAY/inc" -I"D:/STUFF/work/STM32Workspace/HPDL1414_DISPLAY/CMSIS/device" -I"D:/STUFF/work/STM32Workspace/HPDL1414_DISPLAY/CMSIS/core" -O3 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


