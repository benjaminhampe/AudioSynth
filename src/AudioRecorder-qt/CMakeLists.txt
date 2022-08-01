set(APP_NAME AudioRecorder-qt)
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

   target_compile_options(${APP_NAME} PUBLIC -Wno-deprecated-declarations)

   if (WIN32)  # on Windows:
      target_link_libraries(${APP_NAME} PRIVATE comctl32 user32 gdi32 kernel32)
   else()      # on Linux:
      target_link_libraries(${APP_NAME} PRIVATE X11 Xxf86vm Xi Xcursor)
   endif()
   target_link_libraries(${APP_NAME} PRIVATE stdc++fs)   # Filesystem c++17
   target_link_libraries(${APP_NAME} PRIVATE pthread)    # Threads
   target_link_libraries(${APP_NAME} PRIVATE m)          # Math

   target_link_libraries(${APP_NAME} PRIVATE Qt5::Widgets)


   ### DarkImage 2D images, filesystem, xml, glm, fontawesome, drawing, text, bresenham, rainbowColor, etc...
   target_link_libraries(${APP_NAME} PUBLIC DarkImage)
   #target_link_libraries(${APP_NAME} PUBLIC de_glm)         # Math
   #target_link_libraries(${APP_NAME} PUBLIC de_fontawesome) # Font
   #target_link_libraries(${APP_NAME} PUBLIC de_tinyxml2)    # XML

   ### DarkGPU 3D OpenGLES
   # target_link_libraries(${APP_NAME} PUBLIC DarkGPU)        # 3D
   # target_link_libraries(${APP_NAME} PUBLIC de_irrlicht)    # 3D
   # target_link_libraries(${APP_NAME} PUBLIC de_irrExt)      # 3D

   ### DarkAudio
   # target_link_libraries(${APP_NAME} PUBLIC DarkAudio)   # Audio
   # target_link_libraries(${APP_NAME} PRIVATE de_id3v2)   # Audio Tags
   target_link_libraries(${APP_NAME} PRIVATE de_rtaudio)     # Audio
   target_link_libraries(${APP_NAME} PRIVATE de_lame)      # Audio mp3 encoder
   # target_link_libraries(${APP_NAME} PRIVATE de_vst2sdk)    # Audio Steinberg VST SDK
   # target_link_libraries(${APP_NAME} PRIVATE de_rtmidi)      # General Midi
   # target_link_libraries(${APP_NAME} PRIVATE de_fftw3)       # Audio Spektrum fft
   # target_link_libraries(${APP_NAME} PRIVATE de_kissfft)     # Audio Spektrum fft
   # target_link_libraries(${APP_NAME} PRIVATE de_soundtouch)  # Audio
   # target_link_libraries(${APP_NAME} PRIVATE de_sndfile)    # Audio
   # target_link_libraries(${APP_NAME} PRIVATE de_ffmpeg)     # Audio + Video

   ### New: 2022-02-20 Steinberg VST Plugin Host ( a plugin is a .dll on Win64 and Linux64 )
   # Do they rename .dll's in .vst3 or are these not real binaries?


endif()
