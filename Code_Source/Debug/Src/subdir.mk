################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Data_Transmission_functional_unit.c \
../Src/Diagnostic_Communication_Management_functional_unit.c \
../Src/InputOutput_Control_functional_unit.c \
../Src/Routine_functional_unit.c \
../Src/Stored_Data_Transmission_functional_unit.c \
../Src/Upload_Download_functional_unit.c \
../Src/it.c \
../Src/main.c \
../Src/msp.c \
../Src/system_stm32f4xx.c \
../Src/uds_services.c 

OBJS += \
./Src/Data_Transmission_functional_unit.o \
./Src/Diagnostic_Communication_Management_functional_unit.o \
./Src/InputOutput_Control_functional_unit.o \
./Src/Routine_functional_unit.o \
./Src/Stored_Data_Transmission_functional_unit.o \
./Src/Upload_Download_functional_unit.o \
./Src/it.o \
./Src/main.o \
./Src/msp.o \
./Src/system_stm32f4xx.o \
./Src/uds_services.o 

C_DEPS += \
./Src/Data_Transmission_functional_unit.d \
./Src/Diagnostic_Communication_Management_functional_unit.d \
./Src/InputOutput_Control_functional_unit.d \
./Src/Routine_functional_unit.d \
./Src/Stored_Data_Transmission_functional_unit.d \
./Src/Upload_Download_functional_unit.d \
./Src/it.d \
./Src/main.d \
./Src/msp.d \
./Src/system_stm32f4xx.d \
./Src/uds_services.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F446xx -I"D:/STM32work2/STM32WBworkspace/MasteringMCU2/Implementation_UDS_CAN/Inc" -I../unity -I"D:/STM32work2/STM32WBworkspace/MasteringMCU2/Implementation_UDS_CAN/Drivers/STM32F4xx_HAL_Driver/Inc" -I"D:/STM32work2/STM32WBworkspace/MasteringMCU2/Implementation_UDS_CAN/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"D:/STM32work2/STM32WBworkspace/MasteringMCU2/Implementation_UDS_CAN/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/STM32work2/STM32WBworkspace/MasteringMCU2/Implementation_UDS_CAN/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


