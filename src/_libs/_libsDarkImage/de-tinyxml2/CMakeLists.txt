set(DELIB_TINYXML2 de_tinyxml2) # We dont like to clash with system libs.
project(de-lib-tinyxml2 CXX)

if (TARGET ${DELIB_TINYXML2})
	message("${BoldYellow}[benni] HEADER_GUARD: ${DELIB_TINYXML2}${ColourReset}")
else()
	set(TXML2_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/tinyxml2)
	set(TXML2_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src/tinyxml2)
	set(TXML2_HEADERS ${TXML2_HEADER_DIR}/tinyxml2.h)
	set(TXML2_SOURCES ${TXML2_SOURCE_DIR}/tinyxml2.cpp)
	# if (BUILD_SHARED)
		# message("${BoldBlue}[benni] SHARED_LIBRARY: ${DELIB_TINYXML2}${ColourReset}")
		# add_library(${DELIB_TINYXML2} SHARED "")	
	# else()
		message("${BoldBlue}[benni] STATIC_LIBRARY: ${DELIB_TINYXML2}${ColourReset}")
		add_library(${DELIB_TINYXML2} STATIC "")
	# endif()
	target_sources(${DELIB_TINYXML2} PUBLIC ${TXML2_HEADERS} PRIVATE ${TXML2_SOURCES})
   target_include_directories(${DELIB_TINYXML2} PUBLIC ${TXML2_HEADER_DIR}/..) # For the libuser
   target_include_directories(${DELIB_TINYXML2} PRIVATE ${TXML2_HEADER_DIR}) # For the libuser
	target_include_directories(${DELIB_TINYXML2} PRIVATE ${TXML2_SOURCE_DIR}) # For the sources
	target_link_libraries(${DELIB_TINYXML2} PUBLIC m)
endif()
