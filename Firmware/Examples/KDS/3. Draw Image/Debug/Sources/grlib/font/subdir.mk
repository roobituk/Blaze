################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/grlib/font/fontorbitron12.c \
../Sources/grlib/font/fontorbitron16.c \
../Sources/grlib/font/fontorbitron24.c \
../Sources/grlib/font/fontorbitronb12.c \
../Sources/grlib/font/fontorbitronb16.c \
../Sources/grlib/font/fontorbitronb24.c \
../Sources/grlib/font/fontorbitronb32.c \
../Sources/grlib/font/fontorbitronb48.c \
../Sources/grlib/font/fontorbitronb72.c 

OBJS += \
./Sources/grlib/font/fontorbitron12.o \
./Sources/grlib/font/fontorbitron16.o \
./Sources/grlib/font/fontorbitron24.o \
./Sources/grlib/font/fontorbitronb12.o \
./Sources/grlib/font/fontorbitronb16.o \
./Sources/grlib/font/fontorbitronb24.o \
./Sources/grlib/font/fontorbitronb32.o \
./Sources/grlib/font/fontorbitronb48.o \
./Sources/grlib/font/fontorbitronb72.o 

C_DEPS += \
./Sources/grlib/font/fontorbitron12.d \
./Sources/grlib/font/fontorbitron16.d \
./Sources/grlib/font/fontorbitron24.d \
./Sources/grlib/font/fontorbitronb12.d \
./Sources/grlib/font/fontorbitronb16.d \
./Sources/grlib/font/fontorbitronb24.d \
./Sources/grlib/font/fontorbitronb32.d \
./Sources/grlib/font/fontorbitronb48.d \
./Sources/grlib/font/fontorbitronb72.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/grlib/font/%.o: ../Sources/grlib/font/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"C:/New folder (8)/3. Draw Image/Static_Code/PDD" -I"C:/New folder (8)/3. Draw Image/Static_Code/IO_Map" -I"C:/New folder (8)/3. Draw Image/Sources" -I"C:/New folder (8)/3. Draw Image/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


