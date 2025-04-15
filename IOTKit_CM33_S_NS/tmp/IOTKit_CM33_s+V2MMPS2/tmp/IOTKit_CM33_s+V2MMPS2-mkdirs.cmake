# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "P:/Workspace/Git/MTB_CFA/IOTKit_CM33_S_NS/tmp/IOTKit_CM33_s+V2MMPS2"
  "P:/Workspace/Git/MTB_CFA/IOTKit_CM33_S_NS/tmp/1"
  "P:/Workspace/Git/MTB_CFA/IOTKit_CM33_S_NS/tmp/IOTKit_CM33_s+V2MMPS2"
  "P:/Workspace/Git/MTB_CFA/IOTKit_CM33_S_NS/tmp/IOTKit_CM33_s+V2MMPS2/tmp"
  "P:/Workspace/Git/MTB_CFA/IOTKit_CM33_S_NS/tmp/IOTKit_CM33_s+V2MMPS2/src/IOTKit_CM33_s+V2MMPS2-stamp"
  "P:/Workspace/Git/MTB_CFA/IOTKit_CM33_S_NS/tmp/IOTKit_CM33_s+V2MMPS2/src"
  "P:/Workspace/Git/MTB_CFA/IOTKit_CM33_S_NS/tmp/IOTKit_CM33_s+V2MMPS2/src/IOTKit_CM33_s+V2MMPS2-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "P:/Workspace/Git/MTB_CFA/IOTKit_CM33_S_NS/tmp/IOTKit_CM33_s+V2MMPS2/src/IOTKit_CM33_s+V2MMPS2-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "P:/Workspace/Git/MTB_CFA/IOTKit_CM33_S_NS/tmp/IOTKit_CM33_s+V2MMPS2/src/IOTKit_CM33_s+V2MMPS2-stamp${cfgdir}") # cfgdir has leading slash
endif()
