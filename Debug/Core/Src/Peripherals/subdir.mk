################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Peripherals/gpiob.c \
../Core/Src/Peripherals/uart.c 

OBJS += \
./Core/Src/Peripherals/gpiob.o \
./Core/Src/Peripherals/uart.o 

C_DEPS += \
./Core/Src/Peripherals/gpiob.d \
./Core/Src/Peripherals/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Peripherals/%.o Core/Src/Peripherals/%.su Core/Src/Peripherals/%.cyclo: ../Core/Src/Peripherals/%.c Core/Src/Peripherals/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/tomasz/STM32CubeIDE/workspace_1.16.1/PracaInz_v6/Core/Src/Peripherals" -I"/home/tomasz/STM32CubeIDE/workspace_1.16.1/PracaInz_v6/Core/Inc/Peripherals" -I"/home/tomasz/STM32CubeIDE/workspace_1.16.1/PracaInz_v6/Core/Inc/Tasks" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Peripherals

clean-Core-2f-Src-2f-Peripherals:
	-$(RM) ./Core/Src/Peripherals/gpiob.cyclo ./Core/Src/Peripherals/gpiob.d ./Core/Src/Peripherals/gpiob.o ./Core/Src/Peripherals/gpiob.su ./Core/Src/Peripherals/uart.cyclo ./Core/Src/Peripherals/uart.d ./Core/Src/Peripherals/uart.o ./Core/Src/Peripherals/uart.su

.PHONY: clean-Core-2f-Src-2f-Peripherals

