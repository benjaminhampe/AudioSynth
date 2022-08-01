set(DELIB_JPEG8 de_jpeg8d) # We dont like to clash with system libs.
project(${DELIB_JPEG8}-dev C)

if (TARGET ${DELIB_JPEG8})
	message("${BoldYellow}[benni] HEADER_GUARD: ${DELIB_JPEG8}${ColourReset}")
else()
   set(JPEG_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/jpeg8d)
	set(JPEG_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
	set(JPEG_HEADERS 
		${JPEG_HEADER_DIR}/jconfig.h
		${JPEG_HEADER_DIR}/cderror.h
		# ${JPEG_HEADER_DIR}/jchuff.h
		${JPEG_HEADER_DIR}/jdct.h
		# ${JPEG_HEADER_DIR}/jdhuff.h
		${JPEG_HEADER_DIR}/jerror.h
		${JPEG_HEADER_DIR}/jinclude.h
		${JPEG_HEADER_DIR}/jmemsys.h
		${JPEG_HEADER_DIR}/jmorecfg.h
		${JPEG_HEADER_DIR}/jpegint.h
		${JPEG_HEADER_DIR}/jpeglib.h
		${JPEG_HEADER_DIR}/jversion.h)
	
	set(JPEG_SOURCES
		${JPEG_SOURCE_DIR}/jaricom.c
		${JPEG_SOURCE_DIR}/jcapimin.c
		${JPEG_SOURCE_DIR}/jcapistd.c
		${JPEG_SOURCE_DIR}/jcarith.c
		${JPEG_SOURCE_DIR}/jccoefct.c
		${JPEG_SOURCE_DIR}/jccolor.c
		${JPEG_SOURCE_DIR}/jcdctmgr.c
		${JPEG_SOURCE_DIR}/jchuff.c
		${JPEG_SOURCE_DIR}/jcinit.c
		${JPEG_SOURCE_DIR}/jcmainct.c
		${JPEG_SOURCE_DIR}/jcmarker.c
		${JPEG_SOURCE_DIR}/jcmaster.c
		${JPEG_SOURCE_DIR}/jcomapi.c
		${JPEG_SOURCE_DIR}/jcparam.c
		${JPEG_SOURCE_DIR}/jcprepct.c
		${JPEG_SOURCE_DIR}/jcsample.c
		${JPEG_SOURCE_DIR}/jctrans.c
		${JPEG_SOURCE_DIR}/jdapimin.c
		${JPEG_SOURCE_DIR}/jdapistd.c
		${JPEG_SOURCE_DIR}/jdarith.c
		${JPEG_SOURCE_DIR}/jdatadst.c
		${JPEG_SOURCE_DIR}/jdatasrc.c
		${JPEG_SOURCE_DIR}/jdcoefct.c
		${JPEG_SOURCE_DIR}/jdcolor.c
		${JPEG_SOURCE_DIR}/jddctmgr.c
		${JPEG_SOURCE_DIR}/jdhuff.c
		${JPEG_SOURCE_DIR}/jdinput.c
		${JPEG_SOURCE_DIR}/jdmainct.c
		${JPEG_SOURCE_DIR}/jdmarker.c
		${JPEG_SOURCE_DIR}/jdmaster.c
		${JPEG_SOURCE_DIR}/jdmerge.c
		${JPEG_SOURCE_DIR}/jdpostct.c
		${JPEG_SOURCE_DIR}/jdsample.c
		${JPEG_SOURCE_DIR}/jdtrans.c
		${JPEG_SOURCE_DIR}/jerror.c
		${JPEG_SOURCE_DIR}/jfdctflt.c
		${JPEG_SOURCE_DIR}/jfdctfst.c
		${JPEG_SOURCE_DIR}/jfdctint.c
		${JPEG_SOURCE_DIR}/jidctflt.c
		${JPEG_SOURCE_DIR}/jidctfst.c
		${JPEG_SOURCE_DIR}/jidctint.c
		${JPEG_SOURCE_DIR}/jmemmgr.c
		${JPEG_SOURCE_DIR}/jmemnobs.c
		${JPEG_SOURCE_DIR}/jquant1.c
		${JPEG_SOURCE_DIR}/jquant2.c
		${JPEG_SOURCE_DIR}/jutils.c
   )

	# if (BUILD_SHARED)
		# message("${BoldBlue}[benni] SHARED_LIBRARY: ${DELIB_JPEG8}${ColourReset}")
		# add_library(${DELIB_JPEG8} SHARED "")	
	# else()
		message("${BoldBlue}[benni] STATIC_LIBRARY: ${DELIB_JPEG8}${ColourReset}")
		add_library(${DELIB_JPEG8} STATIC "")
	# endif()
	target_sources(${DELIB_JPEG8} PUBLIC ${JPEG_HEADERS} PRIVATE ${JPEG_SOURCES})

   target_compile_options(${DELIB_JPEG8} PRIVATE -Wno-unused-parameter )
   target_compile_options(${DELIB_JPEG8} PRIVATE -Wno-implicit-fallthrough )
   target_compile_options(${DELIB_JPEG8} PRIVATE -Wno-shift-negative-value )
   target_compile_options(${DELIB_JPEG8} PRIVATE -Wno-main ) # jdmainct.c
   
	target_include_directories(${DELIB_JPEG8} PUBLIC ${JPEG_HEADER_DIR}/..) # user
	target_include_directories(${DELIB_JPEG8} PUBLIC ${JPEG_HEADER_DIR}) # sources I
   target_include_directories(${DELIB_JPEG8} PRIVATE ${JPEG_SOURCE_DIR}) # sources II


   #	file(COPY ${JPEG_HEADERS} DESTINATION ${JPEG_INCLUDE_DIR}) # For the SDK

endif()

# add_subdirectory(test)
