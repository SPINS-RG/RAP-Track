#/bin/bash

HOME='IOTKit_CM33_ns/'
SHOME='IOTKit_CM33_s/'
full_file_path="application.c"
APP_OBJ_PATH="IOTKit_CM33_ns/Objects/"
filename="application"
RTE_PATH="IOTKit_CM33_ns/RTE/_V2MMPS2"
IOTKIT_PATH="C://Users/aj4775/AppData/Local/arm/packs/Keil/V2M-MPS2_IOTKit_BSP/1.5.2/Device/IOTKit_CM33/Include"
ARMCORE_PATH="C://Users/aj4775/AppData/Local/arm/packs/ARM/CMSIS/5.9.0/CMSIS/Core/Include/"
includes="-I. -I$HOME"" -I$SHOME"" -I$RTE_PATH -I$IOTKIT_PATH -I$ARMCORE_PATH"  


# arm-none-eabi-gcc.exe "$full_file_path" -mcpu=cortex-m33 -std=gnu11 -O0 -fno-jump-tables -c  $includes -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF""$APP_OBJ_PATH""$filename".d" -MT""$APP_OBJ_PATH""$filename".o" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb  -S -o IOTKit_CM33_ns/$filename".s"



# "command": "c:\\Users\\aj4775\\.vcpkg\\artifacts\\2139c4c6\\compilers.arm.armclang\\6.22.0\\bin\\\\armclang.exe
#  -IP:/Workspace/Git/MTB_CFA/IOTKit_CM33_S_NS/IOTKit_CM33_ns/RTE/_V2MMPS2 
#  -IP:/Workspace/Git/MTB_CFA/IOTKit_CM33_S_NS/IOTKit_CM33_ns/RTE/Device/IOTKit_CM33_FP 
#  -IC:/Users/aj4775/AppData/Local/arm/packs/ARM/CMSIS/5.9.0/CMSIS/Core/Include 
#  -IC:/Users/aj4775/AppData/Local/arm/packs/Keil/MDK-Middleware/7.17.0/Board 
#  -IC:/Users/aj4775/AppData/Local/arm/packs/Keil/V2M-MPS2_IOTKit_BSP/1.5.2/Device/Common/Include 
#  -IC:/Users/aj4775/AppData/Local/arm/packs/Keil/V2M-MPS2_IOTKit_BSP/1.5.2/Device/IOTKit_CM33/Include 
#  -IP:/Workspace/Git/MTB_CFA/IOTKit_CM33_S_NS/IOTKit_CM33_s 
#  -mcpu=Cortex-M33 
#  -mfpu=fpv5-sp-d16 
#  -mfloat-abi=hard 
#  --target=arm-arm-none-eabi 
#  -c 
#  -mlittle-endian
#   -masm=auto 
#   -g 
#   -O1 
#   -Wa,armasm,--pd,\"IOTKit_CM33_FP SETA 1\" 
#   -Wa,armasm,--pd,\"_RTE_ SETA 1\"  
#   -c 
#   -o CMakeFiles\\Group_Source_Group_1.dir\\P_\\Workspace\\Git\\MTB_CFA\\IOTKit_CM33_S_NS\\IOTKit_CM33_ns\\application.o P:\\Workspace\\Git\\MTB_CFA\\IOTKit_CM33_S_NS\\IOTKit_CM33_ns\\application.s",
  

arm-none-eabi-gcc.exe \
    "$full_file_path" \
    -mcpu=cortex-m33+nodsp  \
    -mlittle-endian \
    -std=gnu11  \
    -fshort-wchar \
    -O0  \
    -fno-jump-tables \
    -c \
    $includes \
    -O0 \
    -ffunction-sections \
    -fdata-sections \
    -Wall  \
    -fstack-usage \
    --specs=nano.specs \
    -mfpu=fpv5-sp-d16 \
    -mfloat-abi=hard \
    -mthumb \
    -S \
    -o \
    IOTKit_CM33_ns/$filename".s"
