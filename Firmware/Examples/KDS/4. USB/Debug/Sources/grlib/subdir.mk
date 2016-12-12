################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/grlib/charmap.c \
../Sources/grlib/circle.c \
../Sources/grlib/context.c \
../Sources/grlib/gfx.c \
../Sources/grlib/image.c \
../Sources/grlib/line.c \
../Sources/grlib/offscr1bpp.c \
../Sources/grlib/offscr4bpp.c \
../Sources/grlib/offscr8bpp.c \
../Sources/grlib/rectangle.c \
../Sources/grlib/st7789s.c \
../Sources/grlib/string.c 

OBJS += \
./Sources/grlib/charmap.o \
./Sources/grlib/circle.o \
./Sources/grlib/context.o \
./Sources/grlib/gfx.o \
./Sources/grlib/image.o \
./Sources/grlib/line.o \
./Sources/grlib/offscr1bpp.o \
./Sources/grlib/offscr4bpp.o \
./Sources/grlib/offscr8bpp.o \
./Sources/grlib/rectangle.o \
./Sources/grlib/st7789s.o \
./Sources/grlib/string.o 

C_DEPS += \
./Sources/grlib/charmap.d \
./Sources/grlib/circle.d \
./Sources/grlib/context.d \
./Sources/grlib/gfx.d \
./Sources/grlib/image.d \
./Sources/grlib/line.d \
./Sources/grlib/offscr1bpp.d \
./Sources/grlib/offscr4bpp.d \
./Sources/grlib/offscr8bpp.d \
./Sources/grlib/rectangle.d \
./Sources/grlib/st7789s.d \
./Sources/grlib/string.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/grlib/%.o: ../Sources/grlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"C:/New folder (8)/4. USB/Static_Code/PDD" -I"C:/New folder (8)/4. USB/Static_Code/IO_Map" -I"C:/New folder (8)/4. USB/Sources" -I"C:/New folder (8)/4. USB/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


