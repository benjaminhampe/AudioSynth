set(APP_NAME Hypnotoad)
project(${APP_NAME}-dev)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(APP_NAME "${APP_NAME}d")
endif()

if( BUILD_SHARED EQUAL 1 )
else()
  set(APP_NAME "${APP_NAME}s") # static suffix
endif()

if (TARGET ${APP_NAME})
   message("${BoldRed}[benni] HEADER_GUARD: ${APP_NAME}${ColourReset}")
else()
   message("${BoldYellow}[benni] APPLICATION: ${APP_NAME}${ColourReset}")
   enable_language(RC)

   set(APP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   file(GLOB_RECURSE APP_SOURCES "${APP_SOURCE_DIR}/*.[h,c]pp")

   list(APPEND APP_SOURCES ${CMAKE_CURRENT_LIST_DIR}/icon.rc)

   add_executable(${APP_NAME} "")
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR})
   target_sources(${APP_NAME} PRIVATE ${APP_SOURCES})
   target_link_libraries(${APP_NAME} PRIVATE de_glfw)
   target_link_libraries(${APP_NAME} PRIVATE DarkGPU)
   target_link_libraries(${APP_NAME} PRIVATE de_rtaudio)     # Audio
   target_link_libraries(${APP_NAME} PRIVATE de_rtmidi)      # General Midi
   target_link_libraries(${APP_NAME} PRIVATE de_fftw3)       # Audio Spektrum fft
   target_link_libraries(${APP_NAME} PRIVATE de_kissfft)     # Audio Spektrum fft
   target_link_libraries(${APP_NAME} PRIVATE de_soundtouch)  # Audio
   target_link_libraries(${APP_NAME} PRIVATE de_sndfile)    # Audio
   target_link_libraries(${APP_NAME} PRIVATE de_ffmpeg)     # Audio + Video
   target_link_libraries(${APP_NAME} PRIVATE de_lame)      # Audio mp3 encoder
endif()
