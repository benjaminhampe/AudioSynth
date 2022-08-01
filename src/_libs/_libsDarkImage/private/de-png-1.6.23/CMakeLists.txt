set(DELIB_PNG de_png) # We dont like to clash with system libs.
project(${DELIB_PNG}-dev C)

if (TARGET ${DELIB_PNG})
	message("${BoldYellow}[benni] HEADER_GUARD: ${DELIB_PNG}${ColourReset}")
else()
	set(PNG_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/png)
	set(PNG_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(PNG_HEADERS 
		${PNG_HEADER_DIR}/png.h
		${PNG_HEADER_DIR}/pngconf.h
		${PNG_HEADER_DIR}/pnglibconf.h
		${PNG_HEADER_DIR}/pngdebug.h
		${PNG_HEADER_DIR}/pnginfo.h
		${PNG_HEADER_DIR}/pngstruct.h)

	set(PNG_SOURCES
		${PNG_SOURCE_DIR}/pngpriv.h
		${PNG_SOURCE_DIR}/png.c
		${PNG_SOURCE_DIR}/pngerror.c
		${PNG_SOURCE_DIR}/pngget.c
		${PNG_SOURCE_DIR}/pngmem.c
		${PNG_SOURCE_DIR}/pngpread.c
		${PNG_SOURCE_DIR}/pngread.c
		${PNG_SOURCE_DIR}/pngrio.c
		${PNG_SOURCE_DIR}/pngrtran.c
		${PNG_SOURCE_DIR}/pngrutil.c
		${PNG_SOURCE_DIR}/pngset.c
		${PNG_SOURCE_DIR}/pngtrans.c
		${PNG_SOURCE_DIR}/pngwio.c
		${PNG_SOURCE_DIR}/pngwrite.c
		${PNG_SOURCE_DIR}/pngwtran.c
		${PNG_SOURCE_DIR}/pngwutil.c)
	
	# if (BUILD_SHARED)
		# message("${BoldBlue}[benni] SHARED_LIBRARY: ${DELIB_PNG}${ColourReset}")
		# add_library(${DELIB_PNG} SHARED "")	
	# else()
		message("${BoldBlue}[benni] STATIC_LIBRARY: ${DELIB_PNG}${ColourReset}")
		add_library(${DELIB_PNG} STATIC "")
	# endif()
	
	target_sources(${DELIB_PNG} PUBLIC ${PNG_HEADERS} PRIVATE ${PNG_SOURCES})
	target_include_directories(${DELIB_PNG} PUBLIC ${PNG_HEADER_DIR}/..) # For the SDK
	target_include_directories(${DELIB_PNG} PRIVATE ${PNG_HEADER_DIR}) # For the sources
	target_link_libraries(${DELIB_PNG} PRIVATE de_bzip2 de_z m)	
	
   target_compile_definitions(${DELIB_PNG} PUBLIC -DPNG_bKGD_SUPPORTED=1)
   target_compile_definitions(${DELIB_PNG} PUBLIC -DPNG_READ_BACKGROUND_SUPPORTED=1)

   target_compile_options(${DELIB_PNG} PRIVATE -Wno-implicit-fallthrough 
                                             -Wno-unused-variable )
	
	# file(COPY ${PNG_HEADERS} DESTINATION ${PNG_INCLUDE_DIR}) # For the SDK

endif()

add_subdirectory(test)
