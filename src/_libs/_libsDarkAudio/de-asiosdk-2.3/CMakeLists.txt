set(LIB_NAME de_asio)
project(lib-Steinbenni-ASIOSDK-2.3-dev CXX)

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
   set(ASIO_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
   set(ASIO_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

   set(ASIO_HEADERS
      ${ASIO_HEADER_DIR}/asio.h
      ${ASIO_HEADER_DIR}/ASIOConvertSamples.h
      ${ASIO_HEADER_DIR}/asiosys.h
      ${ASIO_HEADER_DIR}/iasiodrv.h)

   set(ASIO_SOURCES 
      ${ASIO_SOURCE_DIR}/asio.cpp
      ${ASIO_SOURCE_DIR}/ASIOConvertSamples.cpp
      ${ASIO_SOURCE_DIR}/asiodrivers.cpp
      ${ASIO_SOURCE_DIR}/asiolist.cpp)

   # if (BUILD_SHARED)
      # message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
      # add_library(${LIB_NAME} SHARED "")
   # else()
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
      add_library(${LIB_NAME} STATIC "")
   # endif()

   target_compile_definitions(${LIB_NAME} PUBLIC -DUSE_ASIOSDK=1)
   target_sources(${LIB_NAME} PUBLIC ${ASIO_HEADERS} PRIVATE ${ASIO_SOURCES})
   target_include_directories(${LIB_NAME} PUBLIC ${ASIO_HEADER_DIR})
   target_include_directories(${LIB_NAME} PRIVATE ${ASIO_SOURCE_DIR})

   # target_compile_definitions(${LIB_NAME} PUBLIC -DRTAUDIO_DUMMY=1)
   # target_compile_definitions(${LIB_NAME} PUBLIC -DMACOSX_CORE=1)
      
   if(WIN32)
      target_link_libraries(${LIB_NAME} PUBLIC dsound winmm ksguid ksuser ks uuid 
                                               advapi32 ole32 comctl32 kernel32 m)
   else()
      # target_link_libraries(${LIB_NAME} PUBLIC de_sndfile)
      # target_link_libraries(${LIB_NAME} PUBLIC asound)
      # target_link_libraries(${LIB_NAME} PUBLIC jack)
      # target_link_libraries(${LIB_NAME} PUBLIC pulse)
      # target_compile_definitions(${LIB_NAME} PUBLIC -D__LINUX_OSS__=1)
   endif()
endif()   
