set(LIB_NAME de_lame) # We dont like to clash with system libs.
project(${LIB_NAME} C)

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
   set(LAME_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/lame)
   set(LAME_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/libmp3lame)
     
   set(LAME_SOURCES
      ${LAME_SOURCE_DIR}/bitstream.c
      # // depcomp*
      ${LAME_SOURCE_DIR}/encoder.c
      ${LAME_SOURCE_DIR}/fft.c
      ${LAME_SOURCE_DIR}/gain_analysis.c
      ${LAME_SOURCE_DIR}/id3tag.c
      ${LAME_SOURCE_DIR}/lame.c
      ${LAME_SOURCE_DIR}/mpglib_interface.c
      ${LAME_SOURCE_DIR}/newmdct.c
      ${LAME_SOURCE_DIR}/presets.c
      ${LAME_SOURCE_DIR}/psymodel.c
      ${LAME_SOURCE_DIR}/quantize.c
      ${LAME_SOURCE_DIR}/quantize_pvt.c
      ${LAME_SOURCE_DIR}/reservoir.c
      ${LAME_SOURCE_DIR}/set_get.c
      ${LAME_SOURCE_DIR}/tables.c
      ${LAME_SOURCE_DIR}/takehiro.c
      ${LAME_SOURCE_DIR}/util.c
      ${LAME_SOURCE_DIR}/vbrquantize.c
      ${LAME_SOURCE_DIR}/VbrTag.c
      ${LAME_SOURCE_DIR}/version.c
      # ${LAME_SOURCE_DIR}/vector/lame_intrin.h
      ${LAME_SOURCE_DIR}/vector/xmm_quantize_sub.c
   )

   # enable_language(NASM)
   enable_language(ASM_NASM)

   set(LAME_SOURCES_ASM
      ${LAME_SOURCE_DIR}/i386/choose_table.nas
      ${LAME_SOURCE_DIR}/i386/cpu_feat.nas
      ${LAME_SOURCE_DIR}/i386/fft.nas
      ${LAME_SOURCE_DIR}/i386/fft3dn.nas
      ${LAME_SOURCE_DIR}/i386/fftfpu.nas
      ${LAME_SOURCE_DIR}/i386/fftsse.nas
      ${LAME_SOURCE_DIR}/i386/ffttbl.nas
      ${LAME_SOURCE_DIR}/i386/nasm.h
      ${LAME_SOURCE_DIR}/i386/scalar.nas
   )

   # if (WIN32)
   #  enable_language(RC) # WINDRES
      # list(APPEND LAME_SOURCES 
         # ${LAME_SOURCE_DIR}/lame.rc
         # ${LAME_SOURCE_DIR}/logoe.ico )
   # endif ()

   if (BUILD_SHARED)
      message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
      add_library(${LIB_NAME} SHARED "")
   else()
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
      add_library(${LIB_NAME} STATIC "")
   endif()

   target_include_directories(${LIB_NAME} PUBLIC ${LAME_HEADER_DIR}/..) # user <lame/lame.h>
   
   target_include_directories(${LIB_NAME} PRIVATE ${LAME_SOURCE_DIR}) # for src <config.h>
   target_include_directories(${LIB_NAME} PRIVATE ${LAME_HEADER_DIR}) # for src
   
   target_compile_definitions(${LIB_NAME} PRIVATE -DHAVE_CONFIG_H=1)

   target_sources(${LIB_NAME} PUBLIC ${LAME_HEADERS} PRIVATE ${LAME_SOURCES})  
      
   target_compile_options(${LIB_NAME} PRIVATE -Wno-sign-compare
                                              -Wno-unused-parameter
                                              -Wno-unused-variable
                                              -Wno-unknown-pragmas
                                              -Wno-implicit-fallthrough
                                              -Wno-sign-compare
                                              -Wno-unused-const-variable
                                              -Wno-unused-but-set-variable
                                              -Wno-shift-negative-value
   )

   target_link_libraries(${LIB_NAME} PRIVATE de_sndfile)

endif()

# add_subdirectory(test)
