set(LIB_NAME de_theora) # We dont like to clash with system libs.
project(${LIB_NAME}-dev-2.10.0 C)

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
#    set(FT_WITH_ZLIB 1)
#    set(THEORA_VERSION_MAJOR "2")
#    set(THEORA_VERSION_MINOR "10")
#    set(THEORA_VERSION_PATCH "0")
#    set(THEORA_LIBRARY_VERSION "6.16.0")

   set(THEORA_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/theora)

   set(THEORA_HEADERS
      ${THEORA_HEADER_DIR}/codec.h
      ${THEORA_HEADER_DIR}/theora.h
      ${THEORA_HEADER_DIR}/theoradec.h
      ${THEORA_HEADER_DIR}/theoraenc.h
   )

   set(THEORA_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

   set(THEORA_SOURCES
      ${THEORA_SOURCE_DIR}/analyze.c
      ${THEORA_SOURCE_DIR}/apiwrapper.c
      ${THEORA_SOURCE_DIR}/apiwrapper.h
      ${THEORA_SOURCE_DIR}/bitpack.c
      ${THEORA_SOURCE_DIR}/bitpack.h
      ${THEORA_SOURCE_DIR}/cpu.c
      ${THEORA_SOURCE_DIR}/cpu.h
      ${THEORA_SOURCE_DIR}/dct.h
      ${THEORA_SOURCE_DIR}/decapiwrapper.c
      ${THEORA_SOURCE_DIR}/decinfo.c
      ${THEORA_SOURCE_DIR}/decint.h
      ${THEORA_SOURCE_DIR}/decode.c
      ${THEORA_SOURCE_DIR}/dequant.c
      ${THEORA_SOURCE_DIR}/dequant.h
      ${THEORA_SOURCE_DIR}/encapiwrapper.c
      ${THEORA_SOURCE_DIR}/encfrag.c
      ${THEORA_SOURCE_DIR}/encinfo.c
      ${THEORA_SOURCE_DIR}/encint.h
      ${THEORA_SOURCE_DIR}/encode.c
      # ${THEORA_SOURCE_DIR}/encoder_disabled.c # dummy
      ${THEORA_SOURCE_DIR}/enquant.c
      ${THEORA_SOURCE_DIR}/enquant.h
      ${THEORA_SOURCE_DIR}/fdct.c
      ${THEORA_SOURCE_DIR}/fragment.c
      ${THEORA_SOURCE_DIR}/huffdec.c
      ${THEORA_SOURCE_DIR}/huffdec.h
      ${THEORA_SOURCE_DIR}/huffenc.c
      ${THEORA_SOURCE_DIR}/huffenc.h
      ${THEORA_SOURCE_DIR}/huffman.h
      ${THEORA_SOURCE_DIR}/idct.c
      ${THEORA_SOURCE_DIR}/info.c
      ${THEORA_SOURCE_DIR}/internal.c
      ${THEORA_SOURCE_DIR}/internal.h
      ${THEORA_SOURCE_DIR}/mathops.c
      ${THEORA_SOURCE_DIR}/mathops.h
      ${THEORA_SOURCE_DIR}/mcenc.c
      ${THEORA_SOURCE_DIR}/modedec.h
      ${THEORA_SOURCE_DIR}/ocintrin.h
      ${THEORA_SOURCE_DIR}/quant.c
      ${THEORA_SOURCE_DIR}/quant.h
      ${THEORA_SOURCE_DIR}/rate.c
      ${THEORA_SOURCE_DIR}/state.c
      ${THEORA_SOURCE_DIR}/tokenize.c
      #${THEORA_SOURCE_DIR}/x86_vc/
      ${THEORA_SOURCE_DIR}/x86/mmxencfrag.c
      ${THEORA_SOURCE_DIR}/x86/mmxfdct.c
      ${THEORA_SOURCE_DIR}/x86/mmxfrag.c
      ${THEORA_SOURCE_DIR}/x86/mmxfrag.h
      ${THEORA_SOURCE_DIR}/x86/mmxidct.c
      ${THEORA_SOURCE_DIR}/x86/mmxloop.h
      ${THEORA_SOURCE_DIR}/x86/mmxstate.c
      ${THEORA_SOURCE_DIR}/x86/sse2fdct.c
      ${THEORA_SOURCE_DIR}/x86/x86enc.c
      ${THEORA_SOURCE_DIR}/x86/x86enc.h
      ${THEORA_SOURCE_DIR}/x86/x86int.h
      ${THEORA_SOURCE_DIR}/x86/x86state.c
   )

   # if (WIN32)
      # enable_language(RC)
      # list(APPEND THEORA_SOURCES
            # ${THEORA_SOURCE_DIR}/common/win32thread.c
            # ${THEORA_SOURCE_DIR}/common/win32thread.h )
   # endif ()

   # if (BUILD_SHARED)
      # message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
      # add_library(${LIB_NAME} SHARED "")
   # else()
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
      add_library(${LIB_NAME} STATIC "")
   # endif()

   target_compile_definitions(${LIB_NAME} PUBLIC -DUSE_LIB_OGG_THEORA)
   target_sources(${LIB_NAME} PUBLIC ${THEORA_HEADERS} PRIVATE ${THEORA_SOURCES})
   target_include_directories(${LIB_NAME} PUBLIC ${THEORA_HEADER_DIR}/..)
   target_include_directories(${LIB_NAME} PRIVATE ${THEORA_SOURCE_DIR})

   target_compile_options(${LIB_NAME} PRIVATE -Wno-parentheses
                                              -Wno-implicit-fallthrough
                                              -Wno-unused-parameter
                                              -Wno-unused-function
                                              -Wno-sign-compare
                                              -Wno-unused-but-set-variable
                                              -Wno-shift-negative-value
                                              -Wno-aggressive-loop-optimizations
   )

   target_link_libraries(${LIB_NAME} PUBLIC de_ogg) # publishes <ogg/ogg.h> to others.

endif()

# add_subdirectory(test)
