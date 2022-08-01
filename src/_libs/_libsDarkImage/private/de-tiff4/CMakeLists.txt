set(DELIB_TIF4 de_tiff4)
project(${DELIB_TIF4}-dev C)

if (TARGET ${DELIB_TIF4})
   message("${BoldYellow}[benni] HEADER_GUARD: ${DELIB_TIF4}${ColourReset}")
else()
   set(TIFF4_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/tiff4)
   set(TIFF4_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   set(TIFF4_HEADERS
      ${TIFF4_HEADER_DIR}/tiffconf.h
      ${TIFF4_HEADER_DIR}/tiff.h
      ${TIFF4_HEADER_DIR}/tiffio.h
      ${TIFF4_HEADER_DIR}/tiffvers.h
   )
   set(TIFF4_SOURCES
      # ${TIFF4_SOURCE_DIR}/mkg3states.c # app
      # ${TIFF4_SOURCE_DIR}/mkspans.c # app
      ${TIFF4_SOURCE_DIR}/t4.h
      ${TIFF4_SOURCE_DIR}/tif_aux.c
      ${TIFF4_SOURCE_DIR}/tif_close.c
      ${TIFF4_SOURCE_DIR}/tif_codec.c
      ${TIFF4_SOURCE_DIR}/tif_color.c
      ${TIFF4_SOURCE_DIR}/tif_compress.c
      ${TIFF4_SOURCE_DIR}/tif_config.h
      ${TIFF4_SOURCE_DIR}/tif_dir.c
      ${TIFF4_SOURCE_DIR}/tif_dir.h
      ${TIFF4_SOURCE_DIR}/tif_dirinfo.c
      ${TIFF4_SOURCE_DIR}/tif_dirread.c
      ${TIFF4_SOURCE_DIR}/tif_dirwrite.c
      ${TIFF4_SOURCE_DIR}/tif_dumpmode.c
      ${TIFF4_SOURCE_DIR}/tif_error.c
      ${TIFF4_SOURCE_DIR}/tif_extension.c
      ${TIFF4_SOURCE_DIR}/tif_fax3.c
      ${TIFF4_SOURCE_DIR}/tif_fax3.h
      ${TIFF4_SOURCE_DIR}/tif_fax3sm.c
      ${TIFF4_SOURCE_DIR}/tiffconf.h
      ${TIFF4_SOURCE_DIR}/tiffio.hxx
      ${TIFF4_SOURCE_DIR}/tiffiop.h
      ${TIFF4_SOURCE_DIR}/tif_flush.c
      ${TIFF4_SOURCE_DIR}/tiffvers.h.in
      ${TIFF4_SOURCE_DIR}/tif_getimage.c
      ${TIFF4_SOURCE_DIR}/tif_jbig.c
      ${TIFF4_SOURCE_DIR}/tif_jpeg_12.c
      ${TIFF4_SOURCE_DIR}/tif_jpeg.c
      ${TIFF4_SOURCE_DIR}/tif_luv.c
      ${TIFF4_SOURCE_DIR}/tif_lzma.c
      ${TIFF4_SOURCE_DIR}/tif_lzw.c
      ${TIFF4_SOURCE_DIR}/tif_next.c
      ${TIFF4_SOURCE_DIR}/tif_ojpeg.c
      ${TIFF4_SOURCE_DIR}/tif_open.c
      ${TIFF4_SOURCE_DIR}/tif_packbits.c
      ${TIFF4_SOURCE_DIR}/tif_pixarlog.c
      ${TIFF4_SOURCE_DIR}/tif_predict.c
      ${TIFF4_SOURCE_DIR}/tif_predict.h
      ${TIFF4_SOURCE_DIR}/tif_print.c
      ${TIFF4_SOURCE_DIR}/tif_read.c
      # ${TIFF4_SOURCE_DIR}/tif_stream.cxx
      ${TIFF4_SOURCE_DIR}/tif_strip.c
      ${TIFF4_SOURCE_DIR}/tif_swab.c
      ${TIFF4_SOURCE_DIR}/tif_thunder.c
      ${TIFF4_SOURCE_DIR}/tif_tile.c
      ${TIFF4_SOURCE_DIR}/tif_version.c
      ${TIFF4_SOURCE_DIR}/tif_warning.c
      ${TIFF4_SOURCE_DIR}/tif_write.c
      ${TIFF4_SOURCE_DIR}/tif_zip.c
      ${TIFF4_SOURCE_DIR}/uvcode.h
      # ${TIFF4_SOURCE_DIR}/tif_vms.c
   )

   if (WIN32)
      list(APPEND TIFF4_SOURCES ${TIFF4_SOURCE_DIR}/tif_win32.c)
   endif ()

   if (WINCE)
      list(APPEND TIFF4_SOURCES ${TIFF4_SOURCE_DIR}/tif_wince.c)
   endif ()

   if (UNIX)
      list(APPEND TIFF4_SOURCES ${TIFF4_SOURCE_DIR}/tif_unix.c)
   endif ()

   # if (BUILD_SHARED)
      # message("${BoldBlue}[benni] SHARED_LIBRARY: ${DELIB_TIF4}${ColourReset}")
      # add_library(${DELIB_TIF4} SHARED "")
   # else()
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${DELIB_TIF4}${ColourReset}")
      add_library(${DELIB_TIF4} STATIC "")
   # endif()

   target_compile_definitions(${DELIB_TIF4} PUBLIC -DUSE_LIB_TIFF4)
   target_sources(${DELIB_TIF4} PUBLIC ${TIFF4_HEADERS} PRIVATE ${TIFF4_SOURCES})

   target_include_directories(${DELIB_TIF4} PUBLIC ${TIFF4_HEADER_DIR}/..)
   target_include_directories(${DELIB_TIF4} PRIVATE ${TIFF4_HEADER_DIR})
   target_include_directories(${DELIB_TIF4} PRIVATE ${TIFF4_SOURCE_DIR})

   # target_compile_options(${DELIB_TIF4} PRIVATE -O3)
   target_compile_options(${DELIB_TIF4} PRIVATE -Wno-implicit-fallthrough
                                                -Wno-unused-parameter
                                                -Wno-unused-but-set-variable
                                                -Wno-int-to-pointer-cast
                                                -Wno-pointer-to-int-cast
                                                -Wno-format
                                                -Wno-format-extra-args
                                                -Wno-misleading-indentation
   )

   target_link_libraries(${DELIB_TIF4} PRIVATE de_jpeg8d de_z)
endif()
