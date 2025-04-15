# groups.cmake

# group Source Group 1
add_library(Group_Source_Group_1 OBJECT
  "${SOLUTION_ROOT}/IOTKit_CM33_s/main_s.c"
  "${SOLUTION_ROOT}/IOTKit_CM33_s/stdout_USART.c"
  "${SOLUTION_ROOT}/IOTKit_CM33_s/mtb.c"
  "${SOLUTION_ROOT}/IOTKit_CM33_s/Secure_Functions.c"
  "${SOLUTION_ROOT}/IOTKit_CM33_s/cfa.c"
  "${SOLUTION_ROOT}/IOTKit_CM33_s/Secure_Functions_CFA.c"
)
target_include_directories(Group_Source_Group_1 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_INCLUDE_DIRECTORIES>
)
target_compile_definitions(Group_Source_Group_1 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_DEFINITIONS>
)
target_compile_options(Group_Source_Group_1 PUBLIC
  $<TARGET_PROPERTY:${CONTEXT},INTERFACE_COMPILE_OPTIONS>
)
target_link_libraries(Group_Source_Group_1 PUBLIC
  ${CONTEXT}_ABSTRACTIONS
)
