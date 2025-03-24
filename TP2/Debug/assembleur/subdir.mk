################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../assembleur/startup.S 

OBJS += \
./assembleur/startup.o 


# Each subdirectory must supply rules for building sources it contributes
assembleur/%.o: ../assembleur/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	arm-none-eabi-as -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


