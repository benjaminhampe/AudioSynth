set(LIB_NAME de_soundtouch)
project (${LIB_NAME}-dev-1.9.2 CXX)

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()

   set(SOUNDTOUCH_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
   set(SOUNDTOUCH_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

   file(GLOB_RECURSE SOUNDTOUCH_HEADERS "${SOUNDTOUCH_HEADER_DIR}/*.[c,h]pp")
   file(GLOB_RECURSE SOUNDTOUCH_SOURCES "${SOUNDTOUCH_SOURCE_DIR}/*.[c,h]pp")
   list(APPEND SOUNDTOUCH_SOURCES ${SOUNDTOUCH_HEADERS})

   # if (BUILD_SHARED)
      # message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
      # add_library(${LIB_NAME} SHARED "")
   # else()
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
      add_library(${LIB_NAME} STATIC "")
   # endif()

   target_sources(${LIB_NAME} PUBLIC ${SOUNDTOUCH_HEADERS} PRIVATE ${SOUNDTOUCH_SOURCES})
   target_include_directories(${LIB_NAME} PUBLIC ${SOUNDTOUCH_HEADER_DIR})
   target_include_directories(${LIB_NAME} PRIVATE ${SOUNDTOUCH_HEADER_DIR}/SoundTouch)
   target_include_directories(${LIB_NAME} PRIVATE ${SOUNDTOUCH_SOURCE_DIR})
   target_compile_definitions(${LIB_NAME} PUBLIC -DSOUNDTOUCH_FLOAT_SAMPLES=1 )
   # target_compile_definitions(${LIB_NAME} PUBLIC -DSOUNDTOUCH_INTEGER_SAMPLES=1 )
   # target_compile_definitions(${LIB_NAME} PUBLIC -DSOUNDTOUCH_ALLOW_X86_OPTIMIZATIONS=1 ) # needs nasm?
   # target_compile_options(${LIB_NAME} PRIVATE -Wno-implicit-fallthrough
                                              # -Wno-expansion-to-defined
                                              # -Wno-unused-but-set-parameter )

   target_link_libraries(${LIB_NAME} PUBLIC m)

endif()

add_subdirectory(test)
