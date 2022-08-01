set(LIB_NAME de_vst2sdk)
project(lib-${LIB_NAME}-1.2-dev)

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
   # add_subdirectory(lib)

   set(LIB_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR})
   set(LIB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

   set(LIB_VST2_PLUGIF_HEADERS
      ${LIB_HEADER_DIR}/pluginterfaces/vst2.x/aeffect.h
      ${LIB_HEADER_DIR}/pluginterfaces/vst2.x/aeffectx.h
      ${LIB_HEADER_DIR}/pluginterfaces/vst2.x/vstfxstore.h
   )

   set(LIB_VST2_PUBLIC_HEADERS
      ${LIB_HEADER_DIR}/public.sdk/source/vst2.x/aeffeditor.h
      ${LIB_HEADER_DIR}/public.sdk/source/vst2.x/audioeffect.h
      ${LIB_HEADER_DIR}/public.sdk/source/vst2.x/audioeffectx.h
   )

   set(LIB_VST2_PUBLIC_SOURCES
      ${LIB_SOURCE_DIR}/public.sdk/source/vst2.x/audioeffect.cpp
      ${LIB_SOURCE_DIR}/public.sdk/source/vst2.x/audioeffectx.cpp
      # ${LIB_SOURCE_DIR}/public.sdk/source/vst2.x/vstplugmain.cpp
   )

   # file(GLOB_RECURSE LIB_HEADERS "${LIB_HEADER_DIR}/*.[c,h]pp")
   # file(GLOB_RECURSE LIB_SOURCES "${LIB_SOURCE_DIR}/*.[c,h]pp")

   # file(GLOB_RECURSE LIB_HEADERS "${LIB_HEADER_DIR}/*.[c,h]pp")
   set(LIB_HEADERS ${LIB_VST2_PLUGIF_HEADERS} ${LIB_VST2_PUBLIC_HEADERS})
   set(LIB_SOURCES ${LIB_VST2_PUBLIC_SOURCES})

   # if (BUILD_SHARED)
      # add_library(${LIB_NAME} SHARED "")
      # message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
   # else()
      add_library(${LIB_NAME} STATIC "")
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
   # endif()

   target_include_directories(${LIB_NAME} PUBLIC ${LIB_HEADER_DIR})
   target_include_directories(${LIB_NAME} PRIVATE ${LIB_SOURCE_DIR})
   target_sources(${LIB_NAME} PUBLIC ${LIB_HEADERS} PRIVATE ${LIB_SOURCES})
   target_compile_options(${LIB_NAME} PUBLIC -Wno-deprecated-declarations)

endif()


