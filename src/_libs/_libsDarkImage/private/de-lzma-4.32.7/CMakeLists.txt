# cmake_minimum_required(VERSION 2.8.12)

project(de-lib-lzma C ASM)

set(LIB_NAME de_lzma) # We dont like to clash with system libs.

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()

   set(LZMA_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/lzma)
   set(LZMA_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/lib)

   set(LZMA_HEADERS ${LZMA_HEADER_DIR}/lzmadec.h)
      
   set(LZMA_SOURCES
      ${LZMA_SOURCE_DIR}/config.h
      ${LZMA_SOURCE_DIR}/lzmadec_private.h
      ${LZMA_SOURCE_DIR}/lzmadec_buffer.c
      ${LZMA_SOURCE_DIR}/lzmadec_io.c
      ${LZMA_SOURCE_DIR}/lzmadec_main.c
   )

   # enable_language(C ASM)
   # list(APPEND LZMA_SOURCES 
         # ${LZMA_SOURCE_DIR}/config.h
         # ${LZMA_SOURCE_DIR}/Asm/x86/7zCrcOpt.asm
         # ${LZMA_SOURCE_DIR}/Asm/x86/AesOpt.asm
         # ${LZMA_SOURCE_DIR}/Asm/x86/LzmaDecOpt.asm         
         # ${LZMA_SOURCE_DIR}/Asm/x86/XzCrc64Opt.asm              
   # )

   # if (BUILD_SHARED)
      # message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
      # add_library(${LIB_NAME} SHARED "")
   # else()
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
      add_library(${LIB_NAME} STATIC "")
   # endif()
      
   target_compile_definitions(${LIB_NAME} PUBLIC -DUSE_LIB_LZMA)
   target_sources(${LIB_NAME} PUBLIC ${LZMA_HEADERS} PRIVATE ${LZMA_SOURCES})
   
   target_include_directories(${LIB_NAME} PUBLIC ${LZMA_HEADER_DIR}/..)
   target_include_directories(${LIB_NAME} PRIVATE ${LZMA_HEADER_DIR})
   target_include_directories(${LIB_NAME} PRIVATE ${LZMA_SOURCE_DIR})

   target_link_libraries(${LIB_NAME} PUBLIC m)

   target_compile_options(${LIB_NAME} PRIVATE -Wno-unused-parameter -Wno-implicit-fallthrough )
      
endif()
