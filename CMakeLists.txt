
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#################################################################################
#                       FROM BUILD CALLER
#################################################################################

message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")


#################################################################################
#                           COMPILER DETECTION AND SETTING
#################################################################################

# cmake "try out" the compiler but crossGCC cannot link without flags so we force STATIC_LIBRARY
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY" )
# embedded system without OS => Generic
set(CMAKE_SYSTEM_NAME Generic)

find_program(GNU_ARM_SIZE_TOOL arm-none-eabi-size)

if(NOT GNU_ARM_SIZE_TOOL)
    message("arm-none-eabi-size.exe has not been found")
endif()


#################################################################################
#
#################################################################################


#get_filename_component(TOOLCHAIN_DIR . ABSOLUTE)

set(OUTPUT_FILE_NAME outHexFile_${CMAKE_BUILD_TYPE})

########################################################################
#                     APP_TYPE DEPENDANT
########################################################################
set(CPU cortex-m4)
set(CPU_NAME STM32L496xx)
set(MARCH armv7e-m)
set(FPU fpv4-sp-d16)


########################################################################
#                     PROJECT
########################################################################

cmake_minimum_required (VERSION 3.15)
project (ninjaBug_fw C CXX ASM)

# ensure all developpers use same toolchain
#if(NOT ((${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL 7.3.1)
#        AND (${CMAKE_C_COMPILER_ID} STREQUAL "GNU"
#        AND (${CMAKE_C_COMPILER} MATCHES "(arm-none-eabi-gcc.*)$"))))
#    message(FATAL_ERROR "Build requires GCC 7.3.1 (7-2018-Q2) but found ${CMAKE_C_COMPILER_ID} version ${CMAKE_C_COMPILER_VERSION} at ${CMAKE_C_COMPILER}")
#endif()




if(CMAKE_C_FLAGS)
    message(AUTHOR_WARNING "C_FLAGS was not empty (JST) : ${CMAKE_C_FLAGS}")
endif(CMAKE_C_FLAGS)

if(CMAKE_CXX_FLAGS)
    message(AUTHOR_WARNING "CXX_FLAGS was not empty (JST) : ${CMAKE_CXX_FLAGS}")
endif(CMAKE_CXX_FLAGS)

if(CMAKE_ASM_FLAGS)
    message(AUTHOR_WARNING "ASM_FLAGS was not empty (JST) : ${CMAKE_ASM_FLAGS}")
endif(CMAKE_ASM_FLAGS)

if(CMAKE_EXE_LINKER_FLAGS)
    message(AUTHOR_WARNING "LINKER_FLAGS was not empty (JST) : ${CMAKE_EXE_LINKER_FLAGS}")
endif(CMAKE_EXE_LINKER_FLAGS)


set(CMAKE_C_FLAGS " -D${CPU_NAME}  -mcpu=${CPU} -mthumb -mfloat-abi=hard -mfpu=${FPU} \
-Wfatal-errors -Wall -Wno-unused-function \
-std=c99 -fdata-sections -ffunction-sections")
# -c option is set by cmake

set(CMAKE_CXX_FLAGS " -D${CPU_NAME}  -mcpu=${CPU} -mthumb -mfloat-abi=hard -mfpu=${FPU} \
-Wfatal-errors -Wall -Wno-unused-function \
-fdata-sections -ffunction-sections \
-fno-exceptions -fno-non-call-exceptions -fno-rtti -fno-use-cxa-atexit \
-Wno-unknown-pragmas ")

# assembler flags
set(CMAKE_ASM_FLAGS "-mcpu=${CPU} -mthumb -mfloat-abi=hard -mfpu=${FPU}")

# linker flags
# add --verbose to linker flags to have more information
set(CMAKE_EXE_LINKER_FLAGS
"--specs=nano.specs -u _printf_float -Wl,--gc-sections --verbose \
-Wl,-Map=${OUTPUT_FILE_NAME}.map ")


#set additional flags for Debug target
set(CMAKE_C_FLAGS_DEBUG "-O0 -g -DUSE_FULL_ASSERT")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG " " )
set(CMAKE_ASM_FLAGS_DEBUG " ")

#DNDEBUG is needed to remove assert macro from gcc
#set additional flags for Release target
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE " " )
set(CMAKE_ASM_FLAGS_RELEASE " ")

#set additional flags for RelWithDebInfo target
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO " " )
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO " ")

# workaround because response file are not used by default
# http://stackoverflow.com/questions/43184251/cmake-command-line-too-long-windows/43188057#43188057
# check also for bug https://gitlab.kitware.com/cmake/cmake/issues/16771
#TODO 20190827 check this is still needed in cmake 3.15
SET(CMAKE_C_USE_RESPONSE_FILE_FOR_OBJECTS 1)
SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_OBJECTS 1)

SET(CMAKE_C_USE_RESPONSE_FILE_FOR_INCLUDES 1)
SET(CMAKE_CXX_USE_RESPONSE_FILE_FOR_INCLUDES 1)

SET(CMAKE_C_RESPONSE_FILE_LINK_FLAG "@")
SET(CMAKE_CXX_RESPONSE_FILE_LINK_FLAG "@")

# must be put after compile definitions
# take care that subdirectories must be included from bottom to top

add_subdirectory(L3)
add_subdirectory(L2)
add_subdirectory(L1)


add_executable(${OUTPUT_FILE_NAME}.elf  main.c
                                        syscallsGCC.c)

target_link_libraries(${OUTPUT_FILE_NAME}.elf PUBLIC    L1A
                                                        L1B
                                                        L1C)

#dependency to ensure rebuild if linker file is modified
#set_target_properties(${OUTPUT_FILE_NAME}.elf PROPERTIES LINK_DEPENDS ${CMAKE_SOURCE_DIR}/${LINKER_FILE})



ADD_CUSTOM_COMMAND(TARGET ${OUTPUT_FILE_NAME}.hex POST_BUILD ${OUTPUT_FILE_NAME}.elf COMMAND ${CMAKE_OBJCOPY} --verbose -Oihex ${OUTPUT_FILE_NAME}.elf ${OUTPUT_FILE_NAME}.hex)
ADD_CUSTOM_COMMAND(TARGET ${OUTPUT_FILE_NAME}.bin POST_BUILD ${OUTPUT_FILE_NAME}.elf COMMAND ${CMAKE_OBJCOPY} --verbose -Obinary ${OUTPUT_FILE_NAME}.elf ${OUTPUT_FILE_NAME}.bin)
ADD_CUSTOM_COMMAND(TARGET DEBUG.out POST_BUILD ${OUTPUT_FILE_NAME}.elf COMMAND ${CMAKE_COMMAND} -E copy ${OUTPUT_FILE_NAME}.elf DEBUG.out)

ADD_CUSTOM_TARGET(do_always ALL
        COMMAND ${CMAKE_COMMAND} -E echo ------------------------------------------------------------------------------
        COMMAND ${GNU_ARM_SIZE_TOOL} -Ax ${OUTPUT_FILE_NAME}.elf
        COMMAND ${GNU_ARM_SIZE_TOOL} ${OUTPUT_FILE_NAME}.elf
        COMMAND ${CMAKE_COMMAND} -E echo
        COMMAND ${CMAKE_COMMAND} -E echo Build configuration is ${CMAKE_BUILD_TYPE}
        COMMAND ${CMAKE_COMMAND} -E echo ------------------------------------------------------------------------------
        DEPENDS ${OUTPUT_FILE_NAME}.elf)
