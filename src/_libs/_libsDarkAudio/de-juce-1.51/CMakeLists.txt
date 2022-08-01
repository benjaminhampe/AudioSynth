set(LIB_NAME de_juce)
project(lib-${LIB_NAME}-1.51.14-amalgated-dev)

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
   set(LIB_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/juce)
   set(LIB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

   file(GLOB_RECURSE LIB_HEADERS "${LIB_HEADER_DIR}/*.h")
   file(GLOB_RECURSE LIB_SOURCES "${LIB_SOURCE_DIR}/*.[c,h]pp")

   if (BUILD_SHARED)
      add_library(${LIB_NAME} SHARED "")
      message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
      target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_DLL=1)
   else()
      add_library(${LIB_NAME} STATIC "")
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
      target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_DLL=0)
   endif()

   target_sources(${LIB_NAME} PUBLIC ${LIB_HEADERS} PRIVATE ${LIB_SOURCES})
   target_include_directories(${LIB_NAME} PRIVATE ${LIB_HEADER_DIR})
   target_include_directories(${LIB_NAME} PUBLIC ${LIB_HEADER_DIR}/..)

   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_GCC=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_64BIT=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_OPENGL=0) # GWL_WNDPROC

   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_CHECK_MEMORY_LEAKS=0)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_CATCH_UNHANDLED_EXCEPTIONS=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_ENABLE_REPAINT_DEBUGGING=0)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_ONLY_BUILD_CORE_LIBRARY=0)

#   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_INCLUDE_ZLIB_CODE=1)
#   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_INCLUDE_FLAC_CODE=1)
#   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_INCLUDE_OGGVORBIS_CODE=1)
#   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_INCLUDE_PNGLIB_CODE=1)
#   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_INCLUDE_JPEGLIB_CODE=1)

   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_PLUGINHOST_VST=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_PLUGINHOST_AU=0)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_WEB_BROWSER=0)

   # target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_ALSA=1)
   # target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_JACK=1)
   # target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_QUICKTIME=0)
   # target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_IPHONE=0)

   # We dont use proprietary compilers use free GCC x64 from QtSDK, not MSVC
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_MSVC=0)

   # on Windows:
   if (WIN32)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_WINDOWS=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_ASIO=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_WASAPI=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_DIRECTSOUND=1)
   endif()


   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_USE_FLAC=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_USE_OGGVORBIS=1)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_USE_CDBURNER=0)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_USE_CDREADER=0)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_USE_CAMERA=0) # need #include <dshow.h><qedit.h><dshowasf.h>

   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_USE_XINERAMA=0)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_USE_XSHM=0)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_USE_XRENDER=0)
   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_USE_XCURSOR=0)

   target_compile_definitions(${LIB_NAME} PUBLIC -DJUCE_SUPPORT_CARBON=0)

   target_compile_options(${LIB_NAME} PUBLIC -Wno-reorder)
   target_compile_options(${LIB_NAME} PUBLIC -Wno-register) # warning: ISO C++1z does not allow 'register'
   #target_compile_options(${LIB_NAME} PUBLIC -fvisibility=hidden)

   target_link_libraries(${LIB_NAME} PRIVATE stdc++fs)
   target_link_libraries(${LIB_NAME} PRIVATE pthread)
   target_link_libraries(${LIB_NAME} PRIVATE m)
   target_link_libraries(${LIB_NAME} PRIVATE z)

   # on Windows:
   if (WIN32)
      target_link_libraries(${LIB_NAME} PRIVATE comctl32 user32 gdi32 kernel32)
      target_link_libraries(${LIB_NAME} PUBLIC mfplat mf mfuuid wmcodecdspuuid)
      #target_link_libraries(${LIB_NAME} PUBLIC mf3216 mapi32 mapistub mdminst imm32)
      target_link_libraries(${LIB_NAME} PUBLIC dsound winmm ksguid ksuser ks uuid)
      target_link_libraries(${LIB_NAME} PUBLIC advapi32 ole32 oleaut32 comctl32 kernel32 user32 m)
      target_link_libraries(${LIB_NAME} PUBLIC de_asio)
      target_link_libraries(${LIB_NAME} PUBLIC wsock32)
      target_link_libraries(${LIB_NAME} PUBLIC msvfw32 vfw32) # DrawDibDraw
      target_link_libraries(${LIB_NAME} PUBLIC version)
      target_link_libraries(${LIB_NAME} PUBLIC wininet)
   else()      # on Linux:
      target_link_libraries(${LIB_NAME} PRIVATE X11 Xxf86vm Xi Xcursor)
      target_compile_definitions(${LIB_NAME} PUBLIC -D__LINUX_ALSA__=1)
      target_link_libraries(${LIB_NAME} PUBLIC asound)
      target_compile_definitions(${LIB_NAME} PUBLIC -D__UNIX_JACK__=1)
      target_link_libraries(${LIB_NAME} PUBLIC jack)
   endif()
   # Filesystem c++17 # Threads # Math
   target_link_libraries(${LIB_NAME} PRIVATE stdc++fs)
   target_link_libraries(${LIB_NAME} PRIVATE pthread)
   target_link_libraries(${LIB_NAME} PRIVATE m)

   # DarkImage
   target_link_libraries(${LIB_NAME} PRIVATE de_minizip)
   target_link_libraries(${LIB_NAME} PRIVATE de_unzip)
   target_link_libraries(${LIB_NAME} PRIVATE de_bzip2)
   target_link_libraries(${LIB_NAME} PRIVATE de_z)
   target_link_libraries(${LIB_NAME} PRIVATE de_lzma)
   target_link_libraries(${LIB_NAME} PRIVATE de_png)
   target_link_libraries(${LIB_NAME} PRIVATE de_jpeg8d)
   target_link_libraries(${LIB_NAME} PRIVATE de_tiff4)
   target_link_libraries(${LIB_NAME} PRIVATE de_gif5)
   target_link_libraries(${LIB_NAME} PRIVATE de_freetype2)

   # DarkAudio
   target_link_libraries(${LIB_NAME} PRIVATE de_ogg)
   target_link_libraries(${LIB_NAME} PRIVATE de_flac)
   target_link_libraries(${LIB_NAME} PRIVATE de_opus)
   target_link_libraries(${LIB_NAME} PRIVATE de_theora)
   target_link_libraries(${LIB_NAME} PRIVATE de_vorbis)
   target_link_libraries(${LIB_NAME} PRIVATE de_sndfile)    # Audio

   # Steinberg VST2.4 SDK
   target_link_libraries(${LIB_NAME} PRIVATE de_vst2sdk)    # Audio Steinberg VST SDK
   # target_link_libraries(${LIB_NAME} PUBLIC de_dvdcss)
   # target_link_libraries(${LIB_NAME} PUBLIC de_dvdnav)
   # target_link_libraries(${LIB_NAME} PUBLIC de_dvdread)

endif()

# add_subdirectory(test)

