set(LIB_NAME de_opengles3)
project(lib-${LIB_NAME}-dev)

if (TARGET ${LIB_NAME})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_NAME}${ColourReset}")
else()
   set(USE_GLES 1)
   message("${BoldWhite}[benni] USE_GLES = ${USE_GLES} ${ColourReset}")

   set(KHRONOS_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
   set(KHRONOS_HEADERS
      ${KHRONOS_HEADER_DIR}/de/opengles3.hpp
      ${KHRONOS_HEADER_DIR}/de/GL_Validate.hpp
   )

   set(KHRONOS_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   set(KHRONOS_SOURCES
      ${KHRONOS_SOURCE_DIR}/de/GL_Validate.cpp
   )

   if (USE_GLES EQUAL 1)
      list(APPEND KHRONOS_HEADERS ${KHRONOS_HEADER_DIR}/KHR/khrplatform.h)
      list(APPEND KHRONOS_HEADERS ${KHRONOS_HEADER_DIR}/EGL/egl.h)
      list(APPEND KHRONOS_HEADERS ${KHRONOS_HEADER_DIR}/EGL/eglext.h)
      list(APPEND KHRONOS_HEADERS ${KHRONOS_HEADER_DIR}/EGL/eglplatform.h)
      list(APPEND KHRONOS_HEADERS ${KHRONOS_HEADER_DIR}/GLES/egl.h)
      # ${KHRONOS_HEADER_DIR}/GLES/gl.h
      # ${KHRONOS_HEADER_DIR}/GLES/glext.h
      # ${KHRONOS_HEADER_DIR}/GLES/glplatform.h
      # ${KHRONOS_HEADER_DIR}/GLES2/gl2.h
      # ${KHRONOS_HEADER_DIR}/GLES2/gl2ext.h
      # ${KHRONOS_HEADER_DIR}/GLES2/gl2platform.h
      # ${KHRONOS_HEADER_DIR}/GLES3/gl3.h
      # ${KHRONOS_HEADER_DIR}/GLES3/gl3platform.h
      list(APPEND KHRONOS_HEADERS ${KHRONOS_HEADER_DIR}/GLES3/gl31.h)
      # list(APPEND KHRONOS_HEADERS ${KHRONOS_HEADER_DIR}/GLES3/gl32.h
      list(APPEND KHRONOS_HEADERS ${KHRONOS_HEADER_DIR}/de/EGL_Validate.hpp)
      list(APPEND KHRONOS_SOURCES ${KHRONOS_SOURCE_DIR}/de/EGL_Validate.cpp)
   endif()

   add_library(${LIB_NAME} STATIC "")
   message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_NAME}${ColourReset}")
   target_include_directories(${LIB_NAME} PUBLIC ${KHRONOS_HEADER_DIR})
   target_sources(${LIB_NAME} PUBLIC ${KHRONOS_HEADERS} PRIVATE ${KHRONOS_SOURCES})

   # Embedded OpenGLES 3.x
   if (USE_GLES EQUAL 1)
      target_compile_definitions(${LIB_NAME} PUBLIC -DUSE_OPENGLES_AND_EGL=1)
      # on windows:
      # Copy dlls (EGL,GLESv2) from QTSDK to my bin directory as runtimes.
      if (WIN32)
         ### Debug mode
         if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            file(COPY "${BENNI_QTSDK_DIR}/bin/libEGLd.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )   # bin
            file(COPY "${BENNI_QTSDK_DIR}/bin/libGLESv2d.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # bin
            target_link_libraries(${LIB_NAME} PUBLIC
                                    "${BENNI_QTSDK_DIR}/bin/libEGLd.dll"
                                    "${BENNI_QTSDK_DIR}/bin/libGLESv2d.dll" )
         ### Release mode
         else()
            file(COPY "${BENNI_QTSDK_DIR}/bin/libEGL.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ) # Root CMakeLists does link to correct <bin> output directory, so use it now.
            file(COPY "${BENNI_QTSDK_DIR}/bin/libGLESv2.dll" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) # Use correct bin directory
            target_link_libraries(${LIB_NAME} PUBLIC
                                    "${BENNI_QTSDK_DIR}/bin/libEGL.dll"
                                    "${BENNI_QTSDK_DIR}/bin/libGLESv2.dll" )
         endif()
      # on Linux:
      else()
         target_link_libraries(${LIB_NAME} PUBLIC GLESv2 EGL Xxf86vm)
      endif()
   # Desktop OpenGL
   else()
      # on windows:
      if (WIN32)
         target_link_libraries(${LIB_NAME} PUBLIC glmf32 opengl32 glu32)
      # on Linux:
      else()
         target_link_libraries(${LIB_NAME} PUBLIC GL GLU Xxf86vm)
      endif()
   endif()

endif()

# add_subdirectory(test)
