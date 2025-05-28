################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../gecko_sdk_4.4.6/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.c 

OBJS += \
./gecko_sdk_4.4.6/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.o 

C_DEPS += \
./gecko_sdk_4.4.6/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.d 


# Each subdirectory must supply rules for building sources it contributes
gecko_sdk_4.4.6/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.o: ../gecko_sdk_4.4.6/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.c gecko_sdk_4.4.6/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DDEBUG_EFM=1' '-DEFR32BG22C224F512IM40=1' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DHFXO_FREQ=38400000' '-DSL_BOARD_NAME="BRD4184A"' '-DSL_BOARD_REV="A01"' '-DSL_COMPONENT_CATALOG_PRESENT=1' -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/config" -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/autogen" -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko" -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/gecko_sdk_4.4.6/platform/Device/SiliconLabs/EFR32BG22/Include" -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/gecko_sdk_4.4.6/platform/common/inc" -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/gecko_sdk_4.4.6/hardware/board/inc" -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/gecko_sdk_4.4.6/platform/CMSIS/Core/Include" -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/gecko_sdk_4.4.6/platform/service/device_init/inc" -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/gecko_sdk_4.4.6/platform/emlib/inc" -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/gecko_sdk_4.4.6/hardware/driver/mx25_flash_shutdown/inc/sl_mx25_flash_shutdown_usart" -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/gecko_sdk_4.4.6/platform/driver/pwm/inc" -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/gecko_sdk_4.4.6/platform/common/toolchain/inc" -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/gecko_sdk_4.4.6/platform/service/system/inc" -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/gecko_sdk_4.4.6/platform/service/sleeptimer/inc" -I"/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/gecko_sdk_4.4.6/platform/service/udelay/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse --specs=nano.specs -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.4.6/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


