set(LIB_NAME de_rtaudio) # We dont like to clash with system libs.
project(${LIB_NAME}-dev-5.1.0 CXX)

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
   set(RTA_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/RtAudio)
   set(RTA_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

   set(RTA_HEADERS ${RTA_HEADER_DIR}/RtAudio.hpp)
   set(RTA_SOURCES ${RTA_SOURCE_DIR}/RtAudio.cpp)

   # if (WIN32)
      # list(APPEND RTA_SOURCES ${RTA_SOURCE_DIR}/asio/asio.cpp
                              # ${RTA_SOURCE_DIR}/asio/asiodrivers.cpp
                              # ${RTA_SOURCE_DIR}/asio/asiolist.cpp
                              # ${RTA_SOURCE_DIR}/asio/iasiothiscallresolver.cpp  )
   # endif()

   # if (BUILD_SHARED)
      # message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
      # add_library(${LIB_NAME} SHARED "")
   # else()
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
      add_library(${LIB_NAME} STATIC "")
   # endif()

   target_compile_definitions(${LIB_NAME} PUBLIC -DUSE_LIB_RTAUDIO=1)
   target_sources(${LIB_NAME} PUBLIC ${RTA_HEADERS} PRIVATE ${RTA_SOURCES})
   target_include_directories(${LIB_NAME} PUBLIC ${RTA_HEADER_DIR}/..)
   target_include_directories(${LIB_NAME} PUBLIC ${RTA_HEADER_DIR})
   target_include_directories(${LIB_NAME} PRIVATE ${RTA_SOURCE_DIR})

   # target_compile_definitions(${LIB_NAME} PUBLIC -DRTAUDIO_DUMMY=1)
   # target_compile_definitions(${LIB_NAME} PUBLIC -DMACOSX_CORE=1)
   if(WIN32)
      target_compile_definitions(${LIB_NAME} PUBLIC -D__WINDOWS_ASIO__=1)
      target_compile_definitions(${LIB_NAME} PUBLIC -D__WINDOWS_DS__=1)
      target_compile_definitions(${LIB_NAME} PUBLIC -D__WINDOWS_WASAPI__=1)
      target_link_libraries(${LIB_NAME} PUBLIC mfplat mf mfuuid wmcodecdspuuid)
      #target_link_libraries(${LIB_NAME} PUBLIC mf3216 mapi32 mapistub mdminst imm32)
      target_link_libraries(${LIB_NAME} PUBLIC dsound winmm ksguid ksuser ks uuid)
      target_link_libraries(${LIB_NAME} PUBLIC advapi32 ole32 oleaut32 comctl32 kernel32 user32 m)
      target_link_libraries(${LIB_NAME} PUBLIC de_asio)
   else()
      # target_link_libraries(${LIB_NAME} PUBLIC de_sndfile)

      target_compile_definitions(${LIB_NAME} PUBLIC -D__LINUX_ALSA__=1)
      target_link_libraries(${LIB_NAME} PUBLIC asound)
      target_compile_definitions(${LIB_NAME} PUBLIC -D__UNIX_JACK__=1)
      target_link_libraries(${LIB_NAME} PUBLIC jack)


      # target_compile_definitions(${LIB_NAME} PUBLIC -D__LINUX_PULSE__=1)
      # target_compile_definitions(${LIB_NAME} PUBLIC -D__LINUX_OSS__=1)
   endif()
endif()   

add_subdirectory(test)
