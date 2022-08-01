set(LIB_NAME de_taglib)
project(lib-${LIB_NAME}-1.11.1-dev)

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
   # add_subdirectory(lib)

   set(LIB_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/taglib)
   set(LIB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/taglib)

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
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR})
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/toolkit)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/asf)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/mpeg)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/ogg)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/ogg/flac)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/flac)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/mpc)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/mp4)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/ogg/vorbis)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/ogg/speex)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/ogg/opus)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/mpeg/id3v2)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/mpeg/id3v2/frames)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/mpeg/id3v1)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/ape)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/wavpack)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/trueaudio)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/riff)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/riff/aiff)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/riff/wav)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/mod)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/s3m)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/it)
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_SOURCE_DIR}/xm)

   target_sources(${LIB_NAME} PUBLIC ${LIB_HEADERS} PUBLIC ${LIB_SOURCES})

   #target_compile_definitions(${LIB_NAME} PUBLIC -DTAGLIB_STATIC)
   target_compile_definitions(${LIB_NAME} PUBLIC -DTAGLIB_STATIC=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -DHAVE_ZLIB=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -DWITH_ASF=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -DTAGLIB_WITH_ASF=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -DWITH_MP4=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -DTAGLIB_WITH_MP4=1)

   target_compile_options(${LIB_NAME} PUBLIC -Wno-deprecated-declarations)
   target_compile_options(${LIB_NAME} PUBLIC -Wno-attributes)
   #target_compile_options(${LIB_NAME} PUBLIC -fvisibility=hidden)

   target_compile_definitions(${LIB_NAME} PUBLIC -DTAGLIB_SOVERSION_CURRENT=18)
   target_compile_definitions(${LIB_NAME} PUBLIC -DTAGLIB_SOVERSION_REVISION=0)
   target_compile_definitions(${LIB_NAME} PUBLIC -DTAGLIB_SOVERSION_AGE=17)

#   math(EXPR TAGLIB_SOVERSION_MAJOR "${TAGLIB_SOVERSION_CURRENT} - ${TAGLIB_SOVERSION_AGE}")
#   math(EXPR TAGLIB_SOVERSION_MINOR "${TAGLIB_SOVERSION_AGE}")
#   math(EXPR TAGLIB_SOVERSION_PATCH "${TAGLIB_SOVERSION_REVISION}")

   # target_link_libraries(${LIB_NAME} PUBLIC DarkImage)   # Math,Video

   target_link_libraries(${LIB_NAME} PRIVATE stdc++fs)
   target_link_libraries(${LIB_NAME} PRIVATE pthread)
   target_link_libraries(${LIB_NAME} PRIVATE m)
   target_link_libraries(${LIB_NAME} PRIVATE z)

   # on Windows:
   # if (WIN32)
      # target_link_libraries(${LIB_NAME} PRIVATE comctl32 user32 gdi32 kernel32)
   # # on Linux:
   # else()
      # target_link_libraries(${LIB_NAME} PRIVATE X11 Xxf86vm Xi Xcursor)
   # endif()

   # target_link_libraries(${LIB_NAME} PUBLIC DarkImage)
   # target_link_libraries(${LIB_NAME} PUBLIC de_opengles3)   # Public for all (hardcore) users, like me. Not liking me is your lack and problem, i guarantee.
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

add_subdirectory(test)

