set(LIB_NAME de_kissfft) # We dont like to clash with system libs.
project(${LIB_NAME}-dev CXX)

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
   set(KISSFFT_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
   set(KISSFFT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   set(KISSFFT_HEADERS ${KISSFFT_HEADER_DIR}/kiss_fft.h
                       ${KISSFFT_HEADER_DIR}/kiss_fft.hpp)
   set(KISSFFT_SOURCES ${KISSFFT_SOURCE_DIR}/_kiss_fft_guts.h
                       ${KISSFFT_SOURCE_DIR}/kiss_fft.c )
   if (BUILD_SHARED)
      message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
      add_library(${LIB_NAME} SHARED "")
   else()
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
      add_library(${LIB_NAME} STATIC "")
   endif()
   # target_compile_definitions(${LIB_NAME} PUBLIC -DUSE_LIB_RTAUDIO=1)
   # target_compile_options(${LIB_NAME} PRIVATE -O3)
   target_sources(${LIB_NAME} PUBLIC ${KISSFFT_HEADERS} PRIVATE ${KISSFFT_SOURCES})
   target_include_directories(${LIB_NAME} PUBLIC ${KISSFFT_HEADER_DIR}) # for users
   target_include_directories(${LIB_NAME} PRIVATE ${KISSFFT_SOURCE_DIR}) # for lib
      
   if (WIN32)
      # target_compile_definitions(${LIB_NAME} PUBLIC -DWINDOWS_ASIO=1)
      # target_compile_definitions(${LIB_NAME} PUBLIC -D__WINDOWS_DS__=1)
      # target_compile_definitions(${LIB_NAME} PUBLIC -D__WINDOWS_WASAPI__=1)

      # target_link_libraries(${LIB_NAME} PUBLIC 
      #                                advapi32 ole32 comctl32 ksguid ksuser ks
      #                                kernel32 uuid dsound winmm m) # delib_harfbuzz
   else()
      # target_compile_definitions(${LIB_NAME} PUBLIC -D__LINUX_ALSA__=1)
      # target_compile_definitions(${LIB_NAME} PUBLIC -D__UNIX_JACK__=1)
      # target_link_libraries(${LIB_NAME} PUBLIC de_sndfile)
      # target_link_libraries(${LIB_NAME} PUBLIC asound)
      # target_link_libraries(${LIB_NAME} PUBLIC jack)
      # target_compile_definitions(${LIB_NAME} PUBLIC -D__LINUX_PULSE__=1)
      # target_link_libraries(${LIB_NAME} PUBLIC pulse)
      # target_compile_definitions(${LIB_NAME} PUBLIC -D__LINUX_OSS__=1)
   endif()
endif()   

# file(GLOB_RECURSE LICHSON_HEADERS "${LICHSON_HEADER_DIR}/*.[h,c]pp")
# file(GLOB_RECURSE LICHSON_SOURCES "${LICHSON_SOURCE_DIR}/*.[h,c]pp")