set(DELIB_GIF de_gif5)
project(${DELIB_GIF}-5.2.1-dev C)

if (TARGET ${DELIB_GIF})
   message("${BoldYellow}[benni] HEADER_GUARD: ${DELIB_GIF}${ColourReset}")
else()
   set(GIF_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/gif5)
   set(GIF_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/gif5)
   set(GIF_HEADERS
      ${GIF_HEADER_DIR}/gif_lib.h
      ${GIF_HEADER_DIR}/gif_hash.h
   )
   set(GIF_SOURCES
      ${GIF_SOURCE_DIR}/dgif_lib.c
      ${GIF_SOURCE_DIR}/egif_lib.c
      ${GIF_SOURCE_DIR}/gif_err.c
      ${GIF_SOURCE_DIR}/gif_font.c
      ${GIF_SOURCE_DIR}/gif_hash.c
      ${GIF_SOURCE_DIR}/gif_lib_private.h
      ${GIF_SOURCE_DIR}/gifalloc.c
      ${GIF_SOURCE_DIR}/openbsd-reallocarray.c
      ${GIF_SOURCE_DIR}/qprintf.c
      ${GIF_SOURCE_DIR}/quantize.c
   )

   if (WIN32)
     enable_language(RC)
#	  list(APPEND GIF_SOURCES ${GIF_SOURCE_DIR}/builds/windows/ftdebug.c ${GIF_SOURCE_DIR}/base/ftver.rc)
   elseif (WINCE)
#	  list(APPEND GIF_SOURCES ${GIF_SOURCE_DIR}/builds/wince/ftdebug.c)
   else ()
#	  list(APPEND GIF_SOURCES ${GIF_SOURCE_DIR}/base/ftdebug.c)
   endif ()

   # list(APPEND GIF_SOURCES ${GIF_SOURCE_DIR}/base/ftdebug.c)

   # if (BUILD_SHARED)
      # message("${BoldBlue}[benni] SHARED_LIBRARY: ${DELIB_GIF}${ColourReset}")
      # add_library(${DELIB_GIF} SHARED "")
   # else()
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${DELIB_GIF}${ColourReset}")
      add_library(${DELIB_GIF} STATIC "")
   # endif()

   # target_compile_definitions(${DELIB_GIF} PUBLIC -DGIF_BUILD_LIBRARY)
   # target_compile_options(${DELIB_GIF} PRIVATE -O3)

   target_sources(${DELIB_GIF} PUBLIC ${GIF_HEADERS} PRIVATE ${GIF_SOURCES})
   target_include_directories(${DELIB_GIF} PUBLIC ${GIF_HEADER_DIR}/..)
   target_include_directories(${DELIB_GIF} PRIVATE ${GIF_HEADER_DIR})
   target_include_directories(${DELIB_GIF} PRIVATE ${GIF_SOURCE_DIR})
   
   # target_link_libraries(${DELIB_GIF} PRIVATE de_z) # delib_harfbuzz
   target_compile_options(${DELIB_GIF} PRIVATE -Wno-sign-compare )
   # -Wno-implicit-fallthrough
   # -Wmaybe-uninitialized
                                            
   ### add_custom_command(TARGET foo POST_BUILD
    ###    COMMAND ${CMAKE_COMMAND} -E copy
    ###            ${CMAKE_SOURCE_DIR}/test/input.txt
    ###            ${CMAKE_CURRENT_BINARY_DIR}/input.txt)
endif()

# add_subdirectory(test)
# set(GIF_APP_SOURCES
   # ${GIF_SOURCE_DIR}/getarg.c
   # ${GIF_SOURCE_DIR}/getarg.h
   # ${GIF_SOURCE_DIR}/gif2rgb.c
   # ${GIF_SOURCE_DIR}/gifbg.c
   # ${GIF_SOURCE_DIR}/gifbuild.c
   # ${GIF_SOURCE_DIR}/gifclrmp.c
   # ${GIF_SOURCE_DIR}/gifcolor.c
   # ${GIF_SOURCE_DIR}/gifecho.c
   # ${GIF_SOURCE_DIR}/giffilter.c
   # ${GIF_SOURCE_DIR}/giffix.c
   # ${GIF_SOURCE_DIR}/gifhisto.c
   # ${GIF_SOURCE_DIR}/gifinto.c
   # ${GIF_SOURCE_DIR}/gifsponge.c
   # ${GIF_SOURCE_DIR}/giftext.c
   # ${GIF_SOURCE_DIR}/giftool.c
   # ${GIF_SOURCE_DIR}/gifwedge.c
# )

## Packaging
#set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
#set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The FreeType font rendering library.")
#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/docs/LICENSE.TXT")

#set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
#set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
#set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
#set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

#if (WIN32)
#  set(CPACK_GENERATOR ZIP)
#else()
#  set(CPACK_GENERATOR TGZ)
#endif()

#set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Libraries")
#set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C/C++ Headers")
#set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION
#  "Library used to build programs which use FreeType")
#set(CPACK_COMPONENT_HEADERS_DESCRIPTION
#  "C/C++ header files for use with FreeType")
#set(CPACK_COMPONENT_HEADERS_DEPENDS libraries)
#set(CPACK_COMPONENT_LIBRARIES_GROUP "Development")
#set(CPACK_COMPONENT_HEADERS_GROUP "Development")

#include(CPack)
