# It is only dark
# because i only live in the shadows of the irrlicht engine b(y|eim) Nikolaus
# and quake3 engine and halflife 2 source engine ( that has great physics ) and battlefield2 and such real realtime games.
# Most versatile and intelligent games are Gothic 1,2,3,?,Risen3 because replaying them is only repetetive for loot boxes and special weapons.
# Its hard todo deep games because its already hard todo gaming in RT and be beautiful and intelligent at once.
# e.g. The counterstrike source AI bots are so CPU heavy that you feel over 10 on most CPUs because it scales so bad.
# having over 20 bots is so bad that the rate drops below 10 fps completly unsync to screen rate or whatever. Just insane bad CPU cycle wasting.
# And this game had already like 27 iterations on professional teams since the predecessor counterstrike 1.6 over years and years ( no source ).
# Even multicore option ( probably only for GPU stuff ) does not help to scale better. Its hardcore CPU limiting by bad programming.
# Or the programmer had a better PC in 2008 than me in 2021 ( Zen2 2700 3.2 GHz 65W + R580X 200W + 32GB DDR4 RAM 40W + SanDisk 960 evo ~10W ).
# Maybe Radeon Driver issues, but i runs completly smoothly with 10 bot or so. Massive scaling impact on CPU side with this AI?

set(LIB_NAME DarkGPU)
project(lib-${LIB_NAME}-1.2-dev)

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

   target_compile_options(${LIB_NAME} PUBLIC -Wno-deprecated-declarations)

   # target_link_libraries(${LIB_NAME} PUBLIC DarkImage)   # Math,Video

   target_link_libraries(${LIB_NAME} PUBLIC stdc++fs)
   target_link_libraries(${LIB_NAME} PUBLIC pthread)
   target_link_libraries(${LIB_NAME} PUBLIC m)

   # on Windows:
   if (WIN32)
      target_link_libraries(${LIB_NAME} PRIVATE comctl32 user32 gdi32 kernel32)
   # on Linux:
   else()
      target_link_libraries(${LIB_NAME} PRIVATE X11 Xxf86vm Xi Xcursor)
   endif()

   target_link_libraries(${LIB_NAME} PUBLIC DarkImage)
   target_link_libraries(${LIB_NAME} PUBLIC de_opengles3)   # Public for all (hardcore) users, like me. Not liking me is your lack and problem, i guarantee.
   # target_link_libraries(${LIB_NAME} PRIVATE de_assimp)     # Encapsulated by SMeshIO, assimp is really big and mostly ugly, but i still love it since it knows what the beloved irrlicht-engine is and was.

   # target_link_libraries(${LIB_NAME} PRIVATE de_rtaudio)  # Audio
   # target_link_libraries(${LIB_NAME} PRIVATE de_sndfile)  # Audio
   # target_link_libraries(${LIB_NAME} PRIVATE de_rtmidi)   # Audio Midi

   # target_link_libraries(${LIB_NAME} PUBLIC de_ffmpeg)   # public for now
   # target_link_libraries(${LIB_NAME} PRIVATE de_fftw3)    # Math

   # target_link_libraries(${APP_NAME} PUBLIC de_boost)  # Math
   # target_link_libraries(${APP_NAME} PUBLIC de_dvdcss)
   # target_link_libraries(${APP_NAME} PUBLIC de_dvdnav)
   # target_link_libraries(${APP_NAME} PUBLIC de_dvdread)

endif()

# add_subdirectory(test)

