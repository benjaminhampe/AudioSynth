set(LIB_NAME de_ogg)
project(${LIB_NAME}-dev C)

if (TARGET ${LIB_NAME})
	message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
	set(OGG_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/ogg)
	set(OGG_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

	set(OGG_HEADERS 
		${OGG_HEADER_DIR}/config_types.h
		${OGG_HEADER_DIR}/os_types.h
      ${OGG_HEADER_DIR}/ogg.h)
		
	set(OGG_SOURCES
		${OGG_SOURCE_DIR}/crctable.h
      ${OGG_SOURCE_DIR}/framing.c
      ${OGG_SOURCE_DIR}/bitwise.c)

	# if (BUILD_SHARED)
		# message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
		# add_library(${LIB_NAME} SHARED "")
	# else()
		message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
		add_library(${LIB_NAME} STATIC "")
	# endif()

   target_include_directories(${LIB_NAME} PUBLIC ${OGG_HEADER_DIR}/..) # user <ogg/ogg.h>

	target_include_directories(${LIB_NAME} PRIVATE ${OGG_SOURCE_DIR})   # sources

   target_sources(${LIB_NAME} PUBLIC ${OGG_HEADERS} PRIVATE ${OGG_SOURCES})
endif()

# add_subdirectory(test)
