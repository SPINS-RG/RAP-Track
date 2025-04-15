# components.cmake

# component ARM::CMSIS:CORE@5.6.0
add_library(ARM_CMSIS_CORE_5_6_0 INTERFACE)
target_include_directories(ARM_CMSIS_CORE_5_6_0 INTERFACE
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
  ${CMSIS_PACK_ROOT}/ARM/CMSIS/5.9.0/CMSIS/Core/Include
)
target_compile_definitions(ARM_CMSIS_CORE_5_6_0 INTERFACE
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)

# component Keil::Board Support&V2M-MPS2 IOT-Kit:LED@2.0.0
add_library(Keil_Board_Support_V2M-MPS2_IOT-Kit_LED_2_0_0 OBJECT
  "${CMSIS_PACK_ROOT}/Keil/V2M-MPS2_IOTKit_BSP/1.5.2/Boards/ARM/V2M-MPS2/Common/LED_V2M-MPS2.c"
)
target_include_directories(Keil_Board_Support_V2M-MPS2_IOT-Kit_LED_2_0_0 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Keil_Board_Support_V2M-MPS2_IOT-Kit_LED_2_0_0 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Keil_Board_Support_V2M-MPS2_IOT-Kit_LED_2_0_0 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Keil_Board_Support_V2M-MPS2_IOT-Kit_LED_2_0_0 PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)

# component Keil::Device:Startup&C Startup@1.2.0
add_library(Keil_Device_Startup_C_Startup_1_2_0 OBJECT
  "${SOLUTION_ROOT}/IOTKit_CM33_ns/RTE/Device/IOTKit_CM33_FP/startup_IOTKit_CM33.c"
  "${SOLUTION_ROOT}/IOTKit_CM33_ns/RTE/Device/IOTKit_CM33_FP/system_IOTKit_CM33.c"
)
target_include_directories(Keil_Device_Startup_C_Startup_1_2_0 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
  ${SOLUTION_ROOT}/IOTKit_CM33_ns/RTE/Device/IOTKit_CM33_FP
  ${CMSIS_PACK_ROOT}/Keil/V2M-MPS2_IOTKit_BSP/1.5.2/Device/Common/Include
  ${CMSIS_PACK_ROOT}/Keil/V2M-MPS2_IOTKit_BSP/1.5.2/Device/IOTKit_CM33/Include
)
target_compile_definitions(Keil_Device_Startup_C_Startup_1_2_0 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Keil_Device_Startup_C_Startup_1_2_0 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Keil_Device_Startup_C_Startup_1_2_0 PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)
