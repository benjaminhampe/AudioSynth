# Benni Khronos glm library:
project(de-lib-glm)

set(LIB_GLM de_glm) # cmake target name

if (TARGET ${LIB_GLM})
	message("${BoldRed}[benni] HEADER_GUARD: ${LIB_GLM}${ColourReset}")
else()

   message("${BoldBlue}[benni] INTERFACE_LIBRARY: ${LIB_GLM} - Khronos OpenGL Math, header only.${ColourReset}")

   set(GLM_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include") 
   # file(GLOB_RECURSE GLM_HEADERS "${GLM_INCLUDE_DIR}/glm/*.hpp")
   # file(GLOB_RECURSE GLM_SOURCES "${GLM_INCLUDE_DIR}/glm/*.cpp")

   add_library(${LIB_GLM} INTERFACE)

   target_compile_definitions(${LIB_GLM} INTERFACE -DBENNI_WITH_GLM=1)  # Used as #define in .cpp source code.
   target_compile_definitions(${LIB_GLM} INTERFACE -DDE_WITH_GLM=1)     # Used as #define in .cpp source code.
   target_compile_definitions(${LIB_GLM} INTERFACE -DUSE_LIB_GLM=1)     # Used as #define in .cpp source code.

   target_include_directories(${LIB_GLM} INTERFACE ${GLM_INCLUDE_DIR})
   
endif()
