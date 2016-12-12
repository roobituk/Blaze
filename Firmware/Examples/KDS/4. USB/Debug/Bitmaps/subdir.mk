################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Bitmaps/Earth.c 

OBJS += \
./Bitmaps/Earth.o 

C_DEPS += \
./Bitmaps/Earth.d 


# Each subdirectory must supply rules for building sources it contributes
Bitmaps/%.o: ../Bitmaps/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"C:/New folder (8)/4. USB/Static_Code/PDD" -I"C:/New folder (8)/4. USB/Static_Code/IO_Map" -I"C:/New folder (8)/4. USB/Sources" -I"C:/New folder (8)/4. USB/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


