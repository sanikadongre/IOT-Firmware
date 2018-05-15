################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.c 

OBJS += \
./platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.o 

C_DEPS += \
./platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.d 


# Each subdirectory must supply rules for building sources it contributes
platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.o: ../platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m4 -mthumb -std=c99 '-DHAL_CONFIG=1' '-D__STACK_SIZE=0x800' '-D__HEAP_SIZE=0xD00' '-DEFR32BG1B232F256GM56=1' -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\lcd" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\src" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\CMSIS\Include" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\Device\SiliconLabs\EFR32BG1B\Include" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\emdrv\tempdrv\inc" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\hardware\kit\common\bsp" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\emlib\src" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\protocol\bluetooth_2.7\ble_stack\inc\common" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\emlib\inc" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\Device\SiliconLabs\EFR32BG1B\Source" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\halconfig\inc\hal-config" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\Device\SiliconLabs\EFR32BG1B\Source\GCC" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\hardware\kit\EFR32BG1_BRD4302A\config" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\hardware\kit\common\drivers" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\emdrv\common\inc" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\hardware\kit\common\halconfig" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\emdrv\tempdrv\src" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\radio\rail_lib\common" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\protocol\bluetooth_2.7\ble_stack\inc\soc" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\emdrv\sleep\src" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\app\bluetooth_2.7\common\stack_bridge" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\bootloader\api" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\emdrv\uartdrv\inc" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\emdrv\gpiointerrupt\inc" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\emdrv\sleep\inc" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\radio\rail_lib\chip\efr32" -I"C:\Users\monis\SimplicityStudio\v4_workspace\Monish_Sanika_Project_Server_2\platform\bootloader" -O2 -Wall -c -fmessage-length=0 -ffunction-sections -fdata-sections -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -MMD -MP -MF"platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.d" -MT"platform/Device/SiliconLabs/EFR32BG1B/Source/system_efr32bg1b.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

