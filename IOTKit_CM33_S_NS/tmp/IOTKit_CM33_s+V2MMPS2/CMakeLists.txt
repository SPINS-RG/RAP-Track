cmake_minimum_required(VERSION 3.27)

# Roots
include("../roots.cmake")

set(CONTEXT IOTKit_CM33_s+V2MMPS2)
set(TARGET ${CONTEXT})
set(OUT_DIR "${SOLUTION_ROOT}/out/IOTKit_CM33_s/V2MMPS2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMSE_LIB "IOTKit_CM33_s_CMSE_Lib.o")
set(LD_SCRIPT "${SOLUTION_ROOT}/IOTKit_CM33_s/RTE/Device/IOTKit_CM33_FP/ac6_arm.sct")
set(LD_SCRIPT_PP ${LD_SCRIPT})

# Processor Options
set(CPU Cortex-M33)
set(FPU SP_FPU)
set(DSP DSP)
set(SECURE Secure)
set(BYTE_ORDER Little-endian)

# Toolchain config map
include("toolchain.cmake")

# Setup project
project(${CONTEXT} LANGUAGES C)

# Compilation database
add_custom_target(database
  COMMAND ${CMAKE_COMMAND} -E make_directory "${OUT_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" "${OUT_DIR}"
)
set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES})

# Setup context
add_executable(${CONTEXT})
set_target_properties(${CONTEXT} PROPERTIES PREFIX "" SUFFIX ".axf" OUTPUT_NAME "IOTKit_CM33_s")
set_target_properties(${CONTEXT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})
add_library(${CONTEXT}_GLOBAL INTERFACE)

# Includes
target_include_directories(${CONTEXT} PUBLIC
  ${SOLUTION_ROOT}/IOTKit_CM33_s/RTE/_V2MMPS2
  ${SOLUTION_ROOT}/IOTKit_CM33_s/RTE/Device/IOTKit_CM33_FP
  ${CMSIS_PACK_ROOT}/ARM/CMSIS/5.9.0/CMSIS/Core/Include
  ${CMSIS_PACK_ROOT}/ARM/CMSIS/5.9.0/CMSIS/Driver/Include
  ${CMSIS_PACK_ROOT}/Keil/MDK-Middleware/7.17.0/Board
  ${CMSIS_PACK_ROOT}/Keil/V2M-MPS2_IOTKit_BSP/1.5.2/Boards/ARM/V2M-MPS2/Common
  ${CMSIS_PACK_ROOT}/Keil/V2M-MPS2_IOTKit_BSP/1.5.2/Device/Common/Include
  ${CMSIS_PACK_ROOT}/Keil/V2M-MPS2_IOTKit_BSP/1.5.2/Device/IOTKit_CM33/Include
)

# Defines
target_compile_definitions(${CONTEXT} PUBLIC
  $<$<COMPILE_LANGUAGE:C,CXX>:
    IOTKit_CM33_FP
    _RTE_
  >
)

# Compile options
target_compile_options(${CONTEXT} PUBLIC
  $<$<COMPILE_LANGUAGE:C>:
    "SHELL:${CC_CPU}"
    "SHELL:${CC_FLAGS}"
    "SHELL:${CC_SECURE}"
    "SHELL:${CC_BYTE_ORDER}"
    "SHELL:-std=c11"
    "SHELL:-fno-rtti"
    "SHELL:-funsigned-char"
    "SHELL:-fshort-enums"
    "SHELL:-fshort-wchar"
    "SHELL:-ffunction-sections"
    "SHELL:-Wno-packed"
    "SHELL:-Wno-missing-variable-declarations"
    "SHELL:-Wno-missing-prototypes"
    "SHELL:-Wno-missing-noreturn"
    "SHELL:-Wno-sign-conversion"
    "SHELL:-Wno-nonportable-include-path"
    "SHELL:-Wno-reserved-id-macro"
    "SHELL:-Wno-unused-macros"
    "SHELL:-Wno-documentation-unknown-command"
    "SHELL:-Wno-documentation"
    "SHELL:-Wno-license-management"
    "SHELL:-Wno-parentheses-equality"
  >
)

# Compile Options Abstractions
add_library(${CONTEXT}_ABSTRACTIONS INTERFACE)
cbuild_set_options_flags(CC "debug" "on" "" "" CC_OPTIONS_FLAGS_${CONTEXT})
target_compile_options(${CONTEXT}_ABSTRACTIONS INTERFACE
  $<$<COMPILE_LANGUAGE:C>:
    "SHELL:${CC_OPTIONS_FLAGS_${CONTEXT}}"
  >
)

# Add groups and components
include("groups.cmake")
include("components.cmake")

target_link_libraries(${CONTEXT} PUBLIC
  Group_Source_Group_1
  ARM_CMSIS_CORE_5_6_0
  Keil_Board_Support_V2M-MPS2_IOT-Kit_Graphic_LCD_2_0_0
  Keil_Board_Support_V2M-MPS2_IOT-Kit_LED_2_0_0
  Keil_CMSIS_Driver_USART_1_0_0
  Keil_Compiler_ARM_Compiler_I_O_STDOUT_User_1_2_0
  Keil_Device_Startup_C_Startup_1_2_0
)

# Linker options
target_link_options(${CONTEXT} PUBLIC
  "SHELL:${LD_CPU}"
  "SHELL:${_LS}\"${LD_SCRIPT_PP}\""
  "SHELL:${LD_SECURE}"
  "SHELL:--map"
  "SHELL:--load_addr_map_info"
  "SHELL:--xref"
  "SHELL:--callgraph"
  "SHELL:--symbols"
  "SHELL:--info sizes"
  "SHELL:--info totals"
  "SHELL:--info unused"
  "SHELL:--info veneers"
  "SHELL:--strict"
  "SHELL:--summary_stderr"
  "SHELL:--info summarysizes"
)
set_target_properties(${CONTEXT} PROPERTIES LINK_DEPENDS ${LD_SCRIPT})
