set(DELIB_Z de_z) # zlib v1.28
project(${DELIB_Z}-dev-1.28 C)

if (TARGET ${DELIB_Z})
   message("${BoldYellow}[benni] HEADER_GUARD: ${DELIB_Z}${ColourReset}")
else()
   set(ZLIB_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/zlib)
   set(ZLIB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

   set(ZLIB_HEADERS
      ${ZLIB_HEADER_DIR}/crc32.h
      ${ZLIB_HEADER_DIR}/deflate.h
      ${ZLIB_HEADER_DIR}/gzguts.h
      ${ZLIB_HEADER_DIR}/inffast.h
      ${ZLIB_HEADER_DIR}/inffixed.h
      ${ZLIB_HEADER_DIR}/inflate.h
      ${ZLIB_HEADER_DIR}/inftrees.h
      ${ZLIB_HEADER_DIR}/trees.h
      ${ZLIB_HEADER_DIR}/zconf.h
      ${ZLIB_HEADER_DIR}/zlib.h
      ${ZLIB_HEADER_DIR}/zutil.h
   )
   set(ZLIB_SOURCES
      ${ZLIB_SOURCE_DIR}/adler32.c
      ${ZLIB_SOURCE_DIR}/compress.c
      ${ZLIB_SOURCE_DIR}/crc32.c
      ${ZLIB_SOURCE_DIR}/deflate.c
      ${ZLIB_SOURCE_DIR}/inflate.c
      ${ZLIB_SOURCE_DIR}/inffast.c
      ${ZLIB_SOURCE_DIR}/inftrees.c
      ${ZLIB_SOURCE_DIR}/trees.c
      ${ZLIB_SOURCE_DIR}/uncompr.c
      ${ZLIB_SOURCE_DIR}/zutil.c
   )

   # if (BUILD_SHARED)
		# message("${BoldBlue}[benni] SHARED_LIBRARY: ${DELIB_Z}${ColourReset}")
		# add_library(${DELIB_Z} SHARED "")	
        # target_compile_definitions(${DELIB_Z} PUBLIC -DZLIB_DLL=1)
	# else()
		message("${BoldBlue}[benni] STATIC_LIBRARY: ${DELIB_Z}${ColourReset}")
		add_library(${DELIB_Z} STATIC "")
	# endif()
	
   target_compile_definitions(${DELIB_Z} PUBLIC -DZLIB_VERNUM=0x1280 )
   target_compile_options(${DELIB_Z} PRIVATE -Wno-implicit-fallthrough )
   target_sources(${DELIB_Z} PUBLIC ${ZLIB_HEADERS} PRIVATE ${ZLIB_SOURCES})
   target_include_directories(${DELIB_Z} PUBLIC ${ZLIB_HEADER_DIR}) # For public /usr/include access to #include <zlib/*.h>
   target_include_directories(${DELIB_Z} PUBLIC ${ZLIB_HEADER_DIR}/..) # For public /usr/include access to #include <zlib/*.h>

endif()
