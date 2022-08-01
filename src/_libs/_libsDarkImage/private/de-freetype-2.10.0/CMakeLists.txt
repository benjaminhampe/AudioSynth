set(DELIB_FT2 de_freetype2) # We dont like to clash with system libs.
project(${DELIB_FT2}-dev-v2.10.0 C)

if (TARGET ${DELIB_FT2})
   message("${BoldYellow}[benni] HEADER_GUARD: ${DELIB_FT2}${ColourReset}")
else()
   set(FT_WITH_ZLIB 1)
   set(FT_WITH_BZIP2 1)
   set(FT_WITH_PNG 1)
   set(FT_WITH_HARFBUZZ 0)

   set(FT2_VERSION_MAJOR "2")
   set(FT2_VERSION_MINOR "10")
   set(FT2_VERSION_PATCH "0")
   set(FT2_LIBRARY_VERSION "6.16.0")

   set(FT2_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/freetype3)
   set(FT2_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/freetype3)

   set(FT2_HEADERS
      ${FT2_HEADER_DIR}/libFreeType3.hpp # the benni header you wanna use
      ${FT2_HEADER_DIR}/ft2build.h
      ${FT2_HEADER_DIR}/freetype/freetype.h
      ${FT2_HEADER_DIR}/freetype/ftadvanc.h
      ${FT2_HEADER_DIR}/freetype/ftbbox.h
      ${FT2_HEADER_DIR}/freetype/ftbdf.h
      ${FT2_HEADER_DIR}/freetype/ftbitmap.h
      ${FT2_HEADER_DIR}/freetype/ftbzip2.h
      ${FT2_HEADER_DIR}/freetype/ftcache.h
      ${FT2_HEADER_DIR}/freetype/ftchapters.h
      ${FT2_HEADER_DIR}/freetype/ftcid.h
      ${FT2_HEADER_DIR}/freetype/ftcolor.h
      ${FT2_HEADER_DIR}/freetype/ftdriver.h
      ${FT2_HEADER_DIR}/freetype/fterrdef.h
      ${FT2_HEADER_DIR}/freetype/fterrors.h
      ${FT2_HEADER_DIR}/freetype/ftfntfmt.h
      ${FT2_HEADER_DIR}/freetype/ftgasp.h
      ${FT2_HEADER_DIR}/freetype/ftglyph.h
      ${FT2_HEADER_DIR}/freetype/ftgxval.h
      ${FT2_HEADER_DIR}/freetype/ftgzip.h
      ${FT2_HEADER_DIR}/freetype/ftimage.h
      ${FT2_HEADER_DIR}/freetype/ftincrem.h
      ${FT2_HEADER_DIR}/freetype/ftlcdfil.h
      ${FT2_HEADER_DIR}/freetype/ftlist.h
      ${FT2_HEADER_DIR}/freetype/ftlzw.h
      ${FT2_HEADER_DIR}/freetype/ftmac.h
      ${FT2_HEADER_DIR}/freetype/ftmm.h
      ${FT2_HEADER_DIR}/freetype/ftmodapi.h
      ${FT2_HEADER_DIR}/freetype/ftmoderr.h
      ${FT2_HEADER_DIR}/freetype/ftotval.h
      ${FT2_HEADER_DIR}/freetype/ftoutln.h
      ${FT2_HEADER_DIR}/freetype/ftparams.h
      ${FT2_HEADER_DIR}/freetype/ftpfr.h
      ${FT2_HEADER_DIR}/freetype/ftrender.h
      ${FT2_HEADER_DIR}/freetype/ftsizes.h
      ${FT2_HEADER_DIR}/freetype/ftsnames.h
      ${FT2_HEADER_DIR}/freetype/ftstroke.h
      ${FT2_HEADER_DIR}/freetype/ftsynth.h
      ${FT2_HEADER_DIR}/freetype/ftsystem.h
      ${FT2_HEADER_DIR}/freetype/fttrigon.h
      ${FT2_HEADER_DIR}/freetype/fttypes.h
      ${FT2_HEADER_DIR}/freetype/ftwinfnt.h
      ${FT2_HEADER_DIR}/freetype/t1tables.h
      ${FT2_HEADER_DIR}/freetype/ttnameid.h
      ${FT2_HEADER_DIR}/freetype/tttables.h
      ${FT2_HEADER_DIR}/freetype/tttags.h
     # Config headers
      ${FT2_HEADER_DIR}/freetype/config/ftconfig.h
      ${FT2_HEADER_DIR}/freetype/config/ftheader.h
      ${FT2_HEADER_DIR}/freetype/config/ftmodule.h
      ${FT2_HEADER_DIR}/freetype/config/ftoption.h
      ${FT2_HEADER_DIR}/freetype/config/ftstdlib.h
     # Internal headers
      ${FT2_HEADER_DIR}/freetype/internal/autohint.h
      ${FT2_HEADER_DIR}/freetype/internal/cffotypes.h
      ${FT2_HEADER_DIR}/freetype/internal/cfftypes.h
      ${FT2_HEADER_DIR}/freetype/internal/ftcalc.h
      ${FT2_HEADER_DIR}/freetype/internal/ftdebug.h
      ${FT2_HEADER_DIR}/freetype/internal/ftdrv.h
      ${FT2_HEADER_DIR}/freetype/internal/ftgloadr.h
      ${FT2_HEADER_DIR}/freetype/internal/fthash.h
      ${FT2_HEADER_DIR}/freetype/internal/ftmemory.h
      ${FT2_HEADER_DIR}/freetype/internal/ftobjs.h
      ${FT2_HEADER_DIR}/freetype/internal/ftpsprop.h
      ${FT2_HEADER_DIR}/freetype/internal/ftrfork.h
      ${FT2_HEADER_DIR}/freetype/internal/ftserv.h
      ${FT2_HEADER_DIR}/freetype/internal/ftstream.h
      ${FT2_HEADER_DIR}/freetype/internal/fttrace.h
      ${FT2_HEADER_DIR}/freetype/internal/ftvalid.h
      ${FT2_HEADER_DIR}/freetype/internal/internal.h
      ${FT2_HEADER_DIR}/freetype/internal/psaux.h
      ${FT2_HEADER_DIR}/freetype/internal/pshints.h
      ${FT2_HEADER_DIR}/freetype/internal/sfnt.h
      ${FT2_HEADER_DIR}/freetype/internal/t1types.h
      ${FT2_HEADER_DIR}/freetype/internal/tttypes.h)

   set(FT2_SOURCES
      ${FT2_SOURCE_DIR}/autofit/autofit.c
      ${FT2_SOURCE_DIR}/base/ftbase.c
      ${FT2_SOURCE_DIR}/base/ftbbox.c
      ${FT2_SOURCE_DIR}/base/ftbdf.c
      ${FT2_SOURCE_DIR}/base/ftbitmap.c
      ${FT2_SOURCE_DIR}/base/ftcid.c
      ${FT2_SOURCE_DIR}/base/ftfstype.c
      ${FT2_SOURCE_DIR}/base/ftgasp.c
      ${FT2_SOURCE_DIR}/base/ftglyph.c
      ${FT2_SOURCE_DIR}/base/ftgxval.c
      ${FT2_SOURCE_DIR}/base/ftinit.c
      ${FT2_SOURCE_DIR}/base/ftmm.c
      ${FT2_SOURCE_DIR}/base/ftotval.c
      ${FT2_SOURCE_DIR}/base/ftpatent.c
      ${FT2_SOURCE_DIR}/base/ftpfr.c
      ${FT2_SOURCE_DIR}/base/ftstroke.c
      ${FT2_SOURCE_DIR}/base/ftsynth.c
      ${FT2_SOURCE_DIR}/base/ftsystem.c
      ${FT2_SOURCE_DIR}/base/fttype1.c
      ${FT2_SOURCE_DIR}/base/ftwinfnt.c
      ${FT2_SOURCE_DIR}/bdf/bdf.c
      ${FT2_SOURCE_DIR}/bzip2/ftbzip2.c
      ${FT2_SOURCE_DIR}/cache/ftcache.c
      ${FT2_SOURCE_DIR}/cff/cff.c
      ${FT2_SOURCE_DIR}/cid/type1cid.c
      ${FT2_SOURCE_DIR}/gzip/ftgzip.c
      ${FT2_SOURCE_DIR}/lzw/ftlzw.c
      ${FT2_SOURCE_DIR}/pcf/pcf.c
      ${FT2_SOURCE_DIR}/pfr/pfr.c
      ${FT2_SOURCE_DIR}/psaux/psaux.c
      ${FT2_SOURCE_DIR}/pshinter/pshinter.c
      ${FT2_SOURCE_DIR}/psnames/psnames.c
      ${FT2_SOURCE_DIR}/raster/raster.c
      ${FT2_SOURCE_DIR}/sfnt/sfnt.c
      ${FT2_SOURCE_DIR}/smooth/smooth.c
      ${FT2_SOURCE_DIR}/truetype/truetype.c
      ${FT2_SOURCE_DIR}/type1/type1.c
      ${FT2_SOURCE_DIR}/type42/type42.c
      ${FT2_SOURCE_DIR}/winfonts/winfnt.c)

   if (WIN32)
     enable_language(RC)
#	  list(APPEND FT2_SOURCES ${FT2_SOURCE_DIR}/builds/windows/ftdebug.c ${FT2_SOURCE_DIR}/base/ftver.rc)
   elseif (WINCE)
#	  list(APPEND FT2_SOURCES ${FT2_SOURCE_DIR}/builds/wince/ftdebug.c)
   else ()
#	  list(APPEND FT2_SOURCES ${FT2_SOURCE_DIR}/base/ftdebug.c)
   endif ()

   list(APPEND FT2_SOURCES ${FT2_SOURCE_DIR}/base/ftdebug.c)

   if (BUILD_SHARED)
      message("${BoldBlue}[benni] SHARED_LIBRARY: ${DELIB_FT2}${ColourReset}")
      add_library(${DELIB_FT2} SHARED "")
   else()
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${DELIB_FT2}${ColourReset}")
      add_library(${DELIB_FT2} STATIC "")
   endif()

   target_compile_definitions(${DELIB_FT2} PUBLIC -DFT2_BUILD_LIBRARY)
   target_compile_definitions(${DELIB_FT2} PUBLIC -DFT_WITH_ZLIB=1)
   target_compile_definitions(${DELIB_FT2} PUBLIC -DFT_WITH_BZIP2=1)
   target_compile_definitions(${DELIB_FT2} PUBLIC -DFT_WITH_PNG=1)
   target_compile_definitions(${DELIB_FT2} PUBLIC -DFT_WITH_HARFBUZZ=1)
   target_compile_options(${DELIB_FT2} PRIVATE -O3)
   target_sources(${DELIB_FT2} PUBLIC ${FT2_HEADERS} PRIVATE ${FT2_SOURCES})
   target_include_directories(${DELIB_FT2} PUBLIC ${FT2_HEADER_DIR}/..)
   target_include_directories(${DELIB_FT2} PUBLIC ${FT2_HEADER_DIR})
   target_include_directories(${DELIB_FT2} PRIVATE ${FT2_HEADER_DIR}/freetype)
   target_include_directories(${DELIB_FT2} PRIVATE ${FT2_HEADER_DIR}/freetype/config)
   target_include_directories(${DELIB_FT2} PRIVATE ${FT2_HEADER_DIR}/freetype/internal)
   target_include_directories(${DELIB_FT2} PRIVATE ${FT2_SOURCE_DIR})

   # target_link_libraries(${DELIB_FT2} PRIVATE de_png de_bzip2 de_z) # delib_harfbuzz
   target_link_libraries(${DELIB_FT2} PRIVATE de_z) # delib_harfbuzz

   target_compile_options(${DELIB_FT2} PRIVATE -Wno-implicit-fallthrough
                                              -Wmaybe-uninitialized )
                                             
   ### add_custom_command(TARGET foo POST_BUILD
    ###    COMMAND ${CMAKE_COMMAND} -E copy
    ###            ${CMAKE_SOURCE_DIR}/test/input.txt
    ###            ${CMAKE_CURRENT_BINARY_DIR}/input.txt)
endif()

add_subdirectory(test)

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
