################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hardware/kit/common/drivers/mx25flash_spi.c 

OBJS += \
./hardware/kit/common/drivers/mx25flash_spi.o 

C_DEPS += \
./hardware/kit/common/drivers/mx25flash_spi.d 


# Each subdirectory must supply rules for building sources it contributes
hardware/kit/common/drivers/mx25flash_spi.o: ../hardware/kit/common/drivers/mx25flash_spi.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-D__STACK_SIZE=0x800' '-DHAL_CONFIG=1' '-D__HEAP_SIZE=0xD00' '-DEFR32BG1B232F256GM56=1' -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\Device\SiliconLabs\EFR32BG1B\Include" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\hardware\kit\common\halconfig" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\emlib\inc" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\Device\SiliconLabs\EFR32BG1B\Source" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\emdrv\tempdrv\src" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\hardware\kit\common\bsp" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\CMSIS\Include" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\halconfig\inc\hal-config" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\hardware\kit\common\drivers" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\emdrv\tempdrv\inc" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\bootloader\api" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\protocol\bluetooth_2.7\ble_stack\inc\common" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\emdrv\sleep\src" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\protocol\bluetooth_2.7\ble_stack\inc\soc" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\radio\rail_lib\common" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\emdrv\uartdrv\inc" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\emdrv\sleep\inc" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\emlib\src" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\hardware\kit\EFR32BG1_BRD4302A\config" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\app\bluetooth_2.7\common\stack_bridge" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\Device\SiliconLabs\EFR32BG1B\Source\GCC" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\emdrv\common\inc" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\radio\rail_lib\chip\efr32" -I"C:\Users\dongr\SimplicityStudio\v4_workspace\sanika_htp_ota_soc-empty_5\platform\bootloader" -O2 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"hardware/kit/common/drivers/mx25flash_spi.d" -MT"hardware/kit/common/drivers/mx25flash_spi.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


