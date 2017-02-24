################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/_write.c \
../src/adc.c \
../src/gpio.c \
../src/main.c \
../src/stm32f0xx_hal_msp.c \
../src/stm32f0xx_it.c 

OBJS += \
./src/_write.o \
./src/adc.o \
./src/gpio.o \
./src/main.o \
./src/stm32f0xx_hal_msp.o \
./src/stm32f0xx_it.o 

C_DEPS += \
./src/_write.d \
./src/adc.d \
./src/gpio.d \
./src/main.d \
./src/stm32f0xx_hal_msp.d \
./src/stm32f0xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DSTM32F051x8 -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f0xx" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


