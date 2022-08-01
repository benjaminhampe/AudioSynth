set(DELIB_MINIZIP de_minizip) # We dont like to clash with system libs.
project(${DELIB_MINIZIP}-dev C)

if (TARGET ${DELIB_MINIZIP})
	message("${BoldYellow}[benni] HEADER_GUARD: ${DELIB_MINIZIP}${ColourReset}")
else()
	set(MINIZIP_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/minizip)
	set(MINIZIP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/minizip)
	set(MINIZIP_HEADERS 
		${MINIZIP_HEADER_DIR}/miniz.h
		${MINIZIP_HEADER_DIR}/zip.h)
	set(MINIZIP_SOURCES
		${MINIZIP_SOURCE_DIR}/zip.c)

	# if (BUILD_SHARED)
		# message("${BoldBlue}[benni] SHARED_LIBRARY: ${DELIB_MINIZIP}${ColourReset}")
		# add_library(${DELIB_MINIZIP} SHARED "")
	# else()
		message("${BoldBlue}[benni] STATIC_LIBRARY: ${DELIB_MINIZIP}${ColourReset}")
		add_library(${DELIB_MINIZIP} STATIC "")
	# endif()
	target_sources(${DELIB_MINIZIP} PUBLIC ${MINIZIP_HEADERS} PRIVATE ${MINIZIP_SOURCES})
	target_include_directories(${DELIB_MINIZIP} PUBLIC ${MINIZIP_HEADER_DIR}/..) # For the SDK	
	target_include_directories(${DELIB_MINIZIP} PRIVATE ${MINIZIP_HEADER_DIR}) # For the sources
	   
	#	file(COPY ${MINIZIP_HEADERS} DESTINATION ${MINIZIP_INCLUDE_DIR}) # For the SDK
endif()
