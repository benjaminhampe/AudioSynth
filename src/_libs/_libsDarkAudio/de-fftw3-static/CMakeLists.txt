set(LIB_FFTW3 de_fftw3)
project(lib${LIB_FFTW3}-dev-v3.3.8)

if (TARGET ${LIB_FFTW3})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_FFTW3}${ColourReset}")
else()

   set(FFTW3_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/fftw3)

   set(FFTW3_HEADERS ${CMAKE_CURRENT_LIST_DIR}/include/fftw3.h
                     ${FFTW3_HEADER_DIR}/fftw3.h)

   message("${BoldBlue}[benni] INTERFACE_LIBRARY: ${LIB_FFTW3}${ColourReset}")

   add_library(${LIB_FFTW3} INTERFACE)

   target_sources(${LIB_FFTW3} INTERFACE ${FFTW3_HEADERS})

   target_include_directories(${LIB_FFTW3} INTERFACE ${FFTW3_HEADER_DIR}/..)
   target_link_directories(${LIB_FFTW3} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lib)
   target_link_libraries(${LIB_FFTW3} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/lib/libfftw3.a)

endif()
