set(LIB_NAME DarkVLC)
project(lib-${LIB_NAME}-dev)

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
   # add_subdirectory(lib)

   set(LIB_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
   set(LIB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   file(GLOB_RECURSE LIB_HEADERS "${LIB_HEADER_DIR}/*.[c,h]pp")
   file(GLOB_RECURSE LIB_SOURCES "${LIB_SOURCE_DIR}/*.[c,h]pp")

   if (BUILD_SHARED)
      add_library(${LIB_NAME} SHARED "")
      message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
   else()
      add_library(${LIB_NAME} STATIC "")
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
   endif()

   target_include_directories(${LIB_NAME} PUBLIC ${LIB_HEADER_DIR})
   target_include_directories(${LIB_NAME} PRIVATE ${LIB_SOURCE_DIR})
   target_sources(${LIB_NAME} PUBLIC ${LIB_HEADERS} PRIVATE ${LIB_SOURCES})
   #target_compile_options(${LIB_NAME} PUBLIC -Wno-deprecated-declarations)
   target_compile_definitions(${LIB_NAME} PUBLIC -DUSE_RTAUDIO=1)
   target_link_libraries(${LIB_NAME} PUBLIC DarkGPU)
   target_link_libraries(${LIB_NAME} PUBLIC de_sndfile)  # Audio
   target_link_libraries(${LIB_NAME} PUBLIC de_ffmpeg)   # public for now
   target_link_libraries(${LIB_NAME} PRIVATE de_lame)     # public for now
   # target_link_libraries(${LIB_NAME} PRIVATE de_twolame)  # public for now

   target_link_libraries(${LIB_NAME} PUBLIC de_rtaudio)  # Audio
   target_link_libraries(${LIB_NAME} PUBLIC de_rtmidi)   # Audio Midi

   target_link_libraries(${LIB_NAME} PUBLIC de_soundtouch)  # Audio

   target_link_libraries(${LIB_NAME} PUBLIC de_fftw3)    # Audio Spektrum fft
   target_link_libraries(${LIB_NAME} PUBLIC de_kissfft)    # Audio Spektrum fft
   # target_link_libraries(${APP_NAME} PUBLIC de_boost)  # Math
   # target_link_libraries(${APP_NAME} PUBLIC de_dvdcss)
   # target_link_libraries(${APP_NAME} PUBLIC de_dvdnav)
   # target_link_libraries(${APP_NAME} PUBLIC de_dvdread)

endif()

add_subdirectory(test)

