################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.c \
C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.c \
C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.c 

S_UPPER_SRCS += \
C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S 

OBJS += \
./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.o \
./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o \
./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.o \
./common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.o 

S_UPPER_DEPS += \
./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d 

C_DEPS += \
./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.d \
./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.d \
./common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.o: C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.c common/ThirdParty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/FreeRTOS/include" -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/Config" -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/OS" -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/SEGGER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o: C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.S common/ThirdParty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/Config" -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/SEGGER" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.o: C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.c common/ThirdParty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/FreeRTOS/include" -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/Config" -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/OS" -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/SEGGER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.o: C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.c common/ThirdParty/SEGGER/SEGGER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/FreeRTOS/include" -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/Config" -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/OS" -I"C:/RTOS_Folder/RTOS_workspace/common/ThirdParty/SEGGER/SEGGER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-common-2f-ThirdParty-2f-SEGGER-2f-SEGGER

clean-common-2f-ThirdParty-2f-SEGGER-2f-SEGGER:
	-$(RM) ./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.cyclo ./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.d ./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.o ./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT.su ./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.d ./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_ASM_ARMv7M.o ./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.cyclo ./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.d ./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.o ./common/ThirdParty/SEGGER/SEGGER/SEGGER_RTT_printf.su ./common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.cyclo ./common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.d ./common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.o ./common/ThirdParty/SEGGER/SEGGER/SEGGER_SYSVIEW.su

.PHONY: clean-common-2f-ThirdParty-2f-SEGGER-2f-SEGGER

