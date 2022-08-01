set(APP_NAME AbletonLivePluginScanner-qt)
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

   #add_subdirectory(lib)

   enable_language(RC)

   set(APP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   file( GLOB_RECURSE APP_SOURCES "${APP_SOURCE_DIR}/*.[h,c]pp")
   list(APPEND APP_SOURCES ${CMAKE_CURRENT_LIST_DIR}/icon.rc)

   set(CMAKE_AUTOMOC ON)
   # set(CMAKE_AUTORCC OFF)
   # set(CMAKE_AUTOUIC OFF)

   find_package(Qt5Widgets)
   find_package(Qt5 COMPONENTS Core Widgets)

   include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS})
   add_definitions(${Qt5Widgets_DEFINITIONS})
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

   add_executable(${APP_NAME} ${APP_SOURCES})
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR})
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR}/0_common)
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR}/1_engine)
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR}/2_pluginExplorer)
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR}/3_composition)
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR}/4_help)
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR}/5_track)
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR}/6_fxvst)
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR}/7_footer)

   target_compile_options(${APP_NAME} PUBLIC -Wno-deprecated-declarations)

   if (WIN32)  # on Windows:
      target_link_libraries(${APP_NAME} PRIVATE comctl32 user32 gdi32 kernel32)
   else()      # on Linux:
      target_link_libraries(${APP_NAME} PRIVATE X11 Xxf86vm Xi Xcursor)
   endif()
   target_link_libraries(${APP_NAME} PRIVATE stdc++fs)   # Filesystem c++17
   target_link_libraries(${APP_NAME} PRIVATE pthread)    # Threads
   target_link_libraries(${APP_NAME} PRIVATE m)          # Math

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
   # include_directories(SYSTEM ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
   target_link_libraries(${APP_NAME} PRIVATE Qt5::Widgets)
else()
   target_link_libraries(${APP_NAME} PRIVATE Qt5::Widgets)
endif()

   target_link_libraries(${APP_NAME} PRIVATE DarkImage)  # 2D image io, glm, logger, freetype text, fontawesome, xml, draw
   # target_link_libraries(${APP_NAME} PRIVATE DarkGPU)    # 3D OpenGLES
   target_link_libraries(${APP_NAME} PRIVATE de_rtaudio) # AudioEngine WASAPI low latency on windows 64
   target_link_libraries(${APP_NAME} PRIVATE de_rtmidi)  # No virtual MidiPorts on Windows, bad without extra driver.
   target_link_libraries(${APP_NAME} PRIVATE de_fftw3)   # Can be used
   target_link_libraries(${APP_NAME} PRIVATE de_kissfft) # What i use
   target_link_libraries(${APP_NAME} PRIVATE de_soundtouch) # see GSoundTouch impl
   target_link_libraries(${APP_NAME} PRIVATE de_sndfile) # Audio
   target_link_libraries(${APP_NAME} PRIVATE de_ffmpeg)  # Audio + Video
   target_link_libraries(${APP_NAME} PRIVATE de_lame)    # Audio mp3 encoder
   # target_link_libraries(${APP_NAME} PRIVATE de_id3v2)   # Audio Tags ( relative new )
   target_link_libraries(${APP_NAME} PRIVATE de_vst2sdk) # Audio Steinberg VSTi instrument plugin SDK 2.4 ( only x64 )
endif()
