################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include gecko_sdk_4.4.6/platform/service/udelay/src/subdir.mk
-include gecko_sdk_4.4.6/platform/service/system/src/subdir.mk
-include gecko_sdk_4.4.6/platform/service/sleeptimer/src/subdir.mk
-include gecko_sdk_4.4.6/platform/service/device_init/src/subdir.mk
-include gecko_sdk_4.4.6/platform/emlib/src/subdir.mk
-include gecko_sdk_4.4.6/platform/driver/pwm/src/subdir.mk
-include gecko_sdk_4.4.6/platform/common/toolchain/src/subdir.mk
-include gecko_sdk_4.4.6/platform/common/src/subdir.mk
-include gecko_sdk_4.4.6/platform/Device/SiliconLabs/EFR32BG22/Source/subdir.mk
-include gecko_sdk_4.4.6/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/subdir.mk
-include gecko_sdk_4.4.6/hardware/board/src/subdir.mk
-include autogen/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := led_strip_gecko
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: led_strip_gecko.axf

# Tool invocations
led_strip_gecko.axf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C Linker'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -T "/Users/snaiderbautista/SimplicityStudio/v5_workspace/led_strip_gecko/autogen/linkerfile.ld" -Wl,--no-warn-rwx-segments -Xlinker --gc-sections -Xlinker -Map="led_strip_gecko.map" -mfpu=fpv5-sp-d16 -mfloat-abi=hard --specs=nano.specs -o led_strip_gecko.axf -Wl,--start-group "./app.o" "./dma_confiig.o" "./main.o" "./neopixel.o" "./autogen/sl_board_default_init.o" "./autogen/sl_device_init_clocks.o" "./autogen/sl_event_handler.o" "./autogen/sl_pwm_init.o" "./gecko_sdk_4.4.6/hardware/board/src/sl_board_control_gpio.o" "./gecko_sdk_4.4.6/hardware/board/src/sl_board_init.o" "./gecko_sdk_4.4.6/hardware/driver/mx25_flash_shutdown/src/sl_mx25_flash_shutdown_usart/sl_mx25_flash_shutdown.o" "./gecko_sdk_4.4.6/platform/Device/SiliconLabs/EFR32BG22/Source/startup_efr32bg22.o" "./gecko_sdk_4.4.6/platform/Device/SiliconLabs/EFR32BG22/Source/system_efr32bg22.o" "./gecko_sdk_4.4.6/platform/common/src/sl_assert.o" "./gecko_sdk_4.4.6/platform/common/src/sl_syscalls.o" "./gecko_sdk_4.4.6/platform/common/toolchain/src/sl_memory.o" "./gecko_sdk_4.4.6/platform/driver/pwm/src/sl_pwm.o" "./gecko_sdk_4.4.6/platform/emlib/src/em_burtc.o" "./gecko_sdk_4.4.6/platform/emlib/src/em_cmu.o" "./gecko_sdk_4.4.6/platform/emlib/src/em_core.o" "./gecko_sdk_4.4.6/platform/emlib/src/em_emu.o" "./gecko_sdk_4.4.6/platform/emlib/src/em_gpio.o" "./gecko_sdk_4.4.6/platform/emlib/src/em_ldma.o" "./gecko_sdk_4.4.6/platform/emlib/src/em_msc.o" "./gecko_sdk_4.4.6/platform/emlib/src/em_rtcc.o" "./gecko_sdk_4.4.6/platform/emlib/src/em_system.o" "./gecko_sdk_4.4.6/platform/emlib/src/em_timer.o" "./gecko_sdk_4.4.6/platform/emlib/src/em_usart.o" "./gecko_sdk_4.4.6/platform/service/device_init/src/sl_device_init_dcdc_s2.o" "./gecko_sdk_4.4.6/platform/service/device_init/src/sl_device_init_emu_s2.o" "./gecko_sdk_4.4.6/platform/service/device_init/src/sl_device_init_hfxo_s2.o" "./gecko_sdk_4.4.6/platform/service/device_init/src/sl_device_init_lfxo_s2.o" "./gecko_sdk_4.4.6/platform/service/device_init/src/sl_device_init_nvic.o" "./gecko_sdk_4.4.6/platform/service/sleeptimer/src/sl_sleeptimer.o" "./gecko_sdk_4.4.6/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.o" "./gecko_sdk_4.4.6/platform/service/sleeptimer/src/sl_sleeptimer_hal_prortc.o" "./gecko_sdk_4.4.6/platform/service/sleeptimer/src/sl_sleeptimer_hal_rtcc.o" "./gecko_sdk_4.4.6/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.o" "./gecko_sdk_4.4.6/platform/service/system/src/sl_system_init.o" "./gecko_sdk_4.4.6/platform/service/system/src/sl_system_process_action.o" "./gecko_sdk_4.4.6/platform/service/udelay/src/sl_udelay.o" "./gecko_sdk_4.4.6/platform/service/udelay/src/sl_udelay_armv6m_gcc.o" -lgcc -lc -lm -lnosys -Wl,--end-group -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building out file: led_strip_gecko.out'
	arm-none-eabi-objcopy "led_strip_gecko.axf" "led_strip_gecko.out"
	@echo ' '

	@echo 'Building hex file: led_strip_gecko.hex'
	arm-none-eabi-objcopy -O ihex "led_strip_gecko.axf" "led_strip_gecko.hex"
	@echo ' '

	@echo 'Building bin file: led_strip_gecko.bin'
	arm-none-eabi-objcopy -O binary "led_strip_gecko.axf" "led_strip_gecko.bin"
	@echo ' '

	@echo 'Building s37 file: led_strip_gecko.s37'
	arm-none-eabi-objcopy -O srec "led_strip_gecko.axf" "led_strip_gecko.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "led_strip_gecko.axf" -A
	@echo ' '

# Other Targets
clean:
	-@$(foreach FILE, $(EXECUTABLES), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(OBJS), $(shell $(RM) $(FILE)) )
	-@$(foreach FILE, $(C_DEPS), $(shell $(RM) $(FILE)) )
	-$(RM) led_strip_gecko.axf
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
