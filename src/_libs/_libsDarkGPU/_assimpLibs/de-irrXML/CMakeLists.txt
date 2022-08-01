# cmake_minimum_required(VERSION 3.1)

project(de-lib-IrrXML CXX)

set(LIB_NAME de_irrXML)
if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
   set(IRRXML_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/irrXML)
	set(IRRXML_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   set(IRRXML_INCLUDE_DIR ${IRRXML_HEADER_DIR}/..)
   
	set(IRRXML_HEADERS
      ${IRRXML_HEADER_DIR}/CXMLReaderImpl.h
      ${IRRXML_HEADER_DIR}/heapsort.h
      ${IRRXML_HEADER_DIR}/irrArray.h
      ${IRRXML_HEADER_DIR}/irrString.h
      ${IRRXML_HEADER_DIR}/irrTypes.h
      ${IRRXML_HEADER_DIR}/irrXML.h
      ${IRRXML_HEADER_DIR}/fast_atof.h
      ${IRRXML_HEADER_DIR}/pstdint.h)
   set(IRRXML_SOURCES
      ${IRRXML_SOURCE_DIR}/irrXML.cpp)

	# if (BUILD_SHARED)
		# message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_NAME}${ColourReset}")
		# add_library(${LIB_NAME} SHARED "")	
	# else()
		message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
		add_library(${LIB_NAME} STATIC "")
	# endif()
	
   target_sources(${LIB_NAME} PUBLIC ${IRRXML_HEADERS} PRIVATE ${IRRXML_SOURCES})
   target_include_directories(${LIB_NAME} PUBLIC ${IRRXML_INCLUDE_DIR})

   #  file(COPY ${IRRXML_HEADERS} DESTINATION ${IRRXML_INCLUDE_DIR}) # For the SDK		
endif()
