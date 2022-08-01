cmake_minimum_required(VERSION 3.10)

project(de-lib-openddlparser CXX)

set(LIB_NAME de_openddlparser) # We dont like to clash with system libs.

if (TARGET ${LIB_NAME})
	message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()

	add_definitions( -DOPENDDL_STATIC_LIBARY )
	add_definitions( -DOPENDDLPARSER_BUILD )
	add_definitions( -D_VARIADIC_MAX=10 )
	add_definitions( -DGTEST_HAS_PTHREAD=0 )
	add_definitions( -DDDL_DEBUG_HEADER_NAME)
	
	set(OPENDDL_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/openddlparser)
	set(OPENDDL_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(OPENDDL_INCLUDE_DIR ${OPENDDL_HEADER_DIR}/..)

	set(OPENDDL_HEADERS 
		${OPENDDL_HEADER_DIR}/OpenDDLParser.h
		${OPENDDL_HEADER_DIR}/OpenDDLParserUtils.h
		${OPENDDL_HEADER_DIR}/OpenDDLCommon.h
		${OPENDDL_HEADER_DIR}/OpenDDLExport.h
		${OPENDDL_HEADER_DIR}/OpenDDLStream.h
		${OPENDDL_HEADER_DIR}/DDLNode.h
		${OPENDDL_HEADER_DIR}/Value.h)
		
	set(OPENDDL_SOURCES
		${OPENDDL_SOURCE_DIR}/OpenDDLParser.cpp
		${OPENDDL_SOURCE_DIR}/DDLNode.cpp
		${OPENDDL_SOURCE_DIR}/OpenDDLCommon.cpp
		${OPENDDL_SOURCE_DIR}/OpenDDLExport.cpp
		${OPENDDL_SOURCE_DIR}/Value.cpp
		${OPENDDL_SOURCE_DIR}/OpenDDLStream.cpp)
		
	# if (BUILD_SHARED)
		# message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
		# add_library(${LIB_NAME} SHARED "")
	# else()
		message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
		add_library(${LIB_NAME} STATIC "")
	# endif()				
	target_sources(${LIB_NAME} PUBLIC ${OPENDDL_HEADERS} PRIVATE ${OPENDDL_SOURCES})
	target_include_directories(${LIB_NAME} PUBLIC ${OPENDDL_INCLUDE_DIR})
	target_include_directories(${LIB_NAME} PUBLIC ${OPENDDL_INCLUDE_DIR}/openddlparser)

	# file(COPY ${OPENDDL_HEADERS} DESTINATION ${OPENDDL_INCLUDE_DIR}) # For the SDK	
endif()
