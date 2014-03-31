################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Loader.cpp \
../src/Track.cpp \
../src/stdafx.cpp 

OBJS += \
./src/Loader.o \
./src/Track.o \
./src/stdafx.o 

CPP_DEPS += \
./src/Loader.d \
./src/Track.d \
./src/stdafx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/andrey/boost_1_53_0 -O0 -g -Wextra -Werror -Wconversion -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


