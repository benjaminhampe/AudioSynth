set(DELIB_BZIP2 de_bzip2) # We dont like to clash with system libs.
project(${DELIB_BZIP2}-dev C)

if (TARGET ${DELIB_BZIP2})
	message("${BoldYellow}[benni] HEADER_GUARD: ${DELIB_BZIP2}${ColourReset}")
else()
	set(BZIP2_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/bzip2)
	set(BZIP2_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

	set(BZIP2_HEADERS 
		${BZIP2_HEADER_DIR}/bzlib.h)
		
	set(BZIP2_SOURCES
		${BZIP2_SOURCE_DIR}/blocksort.c
		${BZIP2_SOURCE_DIR}/bzcompress.c
		${BZIP2_SOURCE_DIR}/bzlib.c
		${BZIP2_SOURCE_DIR}/bzlib_private.h
		${BZIP2_SOURCE_DIR}/crctable.c
		${BZIP2_SOURCE_DIR}/decompress.c
		${BZIP2_SOURCE_DIR}/huffman.c
		${BZIP2_SOURCE_DIR}/randtable.c)

	# if (BUILD_SHARED)
		# message("${BoldBlue}[benni] SHARED_LIBRARY: ${DELIB_BZIP2}${ColourReset}")
		# add_library(${DELIB_BZIP2} SHARED "")	
	# else()
		message("${BoldBlue}[benni] STATIC_LIBRARY: ${DELIB_BZIP2}${ColourReset}")
		add_library(${DELIB_BZIP2} STATIC "")
	# endif()
	
	target_sources(${DELIB_BZIP2} PUBLIC ${BZIP2_HEADERS} PRIVATE ${BZIP2_SOURCES})
	target_include_directories(${DELIB_BZIP2} PRIVATE ${BZIP2_SOURCE_DIR})	
   target_include_directories(${DELIB_BZIP2} PUBLIC ${BZIP2_HEADER_DIR}) # For ffmpeg
   target_include_directories(${DELIB_BZIP2} PUBLIC ${BZIP2_HEADER_DIR}/..) # For the SDK

	target_compile_options(${DELIB_BZIP2} PRIVATE   -Wno-implicit-fallthrough
                                                   -Wno-maybe-uninitialized
                                                   -Wno-unused-parameter
                                                   -Wno-unused-but-set-variable )
                                                
	# file(COPY ${BZIP2_HEADERS} DESTINATION ${BZIP2_INCLUDE_DIR}) # For the SDK  
   
endif()

add_subdirectory(test)
