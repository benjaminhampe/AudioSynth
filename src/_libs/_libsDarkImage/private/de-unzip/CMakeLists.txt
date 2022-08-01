set(DELIB_UNZIP de_unzip) # We dont like to clash with system libs.
project(${DELIB_UNZIP}-dev C)

if (TARGET ${DELIB_UNZIP})
	message("${BoldYellow}[benni] HEADER_GUARD: ${DELIB_UNZIP}${ColourReset}")
else()
	set(UNZIP_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/unzip)
	set(UNZIP_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/unzip)
	set(UNZIP_HEADERS
		${UNZIP_HEADER_DIR}/crypt.h
		${UNZIP_HEADER_DIR}/ioapi.h
		${UNZIP_HEADER_DIR}/unzip.h
	)

	set(UNZIP_SOURCES
		${UNZIP_SOURCE_DIR}/ioapi.c
		${UNZIP_SOURCE_DIR}/unzip.c
	)
	
	# if (BUILD_SHARED)
		# message("${BoldBlue}[benni] SHARED_LIBRARY: ${DELIB_UNZIP}${ColourReset}")
		# add_library(${DELIB_UNZIP} SHARED "")
	# else()
		message("${BoldBlue}[benni] STATIC_LIBRARY: ${DELIB_UNZIP}${ColourReset}")
		add_library(${DELIB_UNZIP} STATIC "")
	# endif()
	
	target_sources(${DELIB_UNZIP} PUBLIC ${UNZIP_HEADERS} PRIVATE ${UNZIP_SOURCES})
	
   target_include_directories(${DELIB_UNZIP} PUBLIC ${UNZIP_HEADER_DIR}) # For the SDK
   target_include_directories(${DELIB_UNZIP} PUBLIC ${UNZIP_HEADER_DIR}/..) # For the SDK
   
   target_compile_options(${DELIB_UNZIP} PRIVATE -Wno-unused-parameter
                                              -Wno-unused-variable )

   target_link_libraries(${DELIB_UNZIP} PRIVATE de_z)
                                             
	# file(COPY ${UNZIP_HEADERS} DESTINATION ${UNZIP_HEADER_DIR}) # For the SDK
endif()
