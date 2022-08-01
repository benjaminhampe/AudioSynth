set(LIB_GLFW de_glfw)
project(${LIB_GLFW}-3.2.2-dev C)

if (TARGET ${LIB_GLFW})
   message("${BoldYellow}[benni] HEADER_GUARD: ${LIB_GLFW}${ColourReset}")
else()
   set(GLFW_HEADER_DIR ${CMAKE_CURRENT_LIST_DIR}/include/GLFW)
   set(GLFW_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
   set(GLFW_HEADERS
      ${GLFW_HEADER_DIR}/glfw3.h
      ${GLFW_HEADER_DIR}/glfw3native.h
   )
   set(GLFW_SOURCES
      ${GLFW_SOURCE_DIR}/context.c
      ${GLFW_SOURCE_DIR}/egl_context.c
      ${GLFW_SOURCE_DIR}/egl_context.h
      ${GLFW_SOURCE_DIR}/init.c
      ${GLFW_SOURCE_DIR}/input.c
      ${GLFW_SOURCE_DIR}/internal.h
      ${GLFW_SOURCE_DIR}/mappings.h
      ${GLFW_SOURCE_DIR}/monitor.c
      # ${GLFW_SOURCE_DIR}/posix_thread.c
      # ${GLFW_SOURCE_DIR}/posix_thread.h
      # ${GLFW_SOURCE_DIR}/posix_time.c
      # ${GLFW_SOURCE_DIR}/posix_time.h
      ${GLFW_SOURCE_DIR}/window.c
   )

   set(GLFW_SOURCES_WIN32
      ${GLFW_SOURCE_DIR}/win32/wgl_context.c
      ${GLFW_SOURCE_DIR}/win32/wgl_context.h
      ${GLFW_SOURCE_DIR}/win32/win32_init.c
      ${GLFW_SOURCE_DIR}/win32/win32_joystick.c
      ${GLFW_SOURCE_DIR}/win32/win32_joystick.h
      ${GLFW_SOURCE_DIR}/win32/win32_monitor.c
      ${GLFW_SOURCE_DIR}/win32/win32_platform.h
      ${GLFW_SOURCE_DIR}/win32/win32_thread.c
      ${GLFW_SOURCE_DIR}/win32/win32_time.c
      ${GLFW_SOURCE_DIR}/win32/win32_window.c
      ${GLFW_SOURCE_DIR}/win32/osmesa_context.c
      ${GLFW_SOURCE_DIR}/win32/osmesa_context.h
   )

   # if (BUILD_SHARED)
      # message("${BoldBlue}[benni] SHARED_LIBRARY: ${LIB_GLFW}${ColourReset}")
      # add_library(${LIB_GLFW} SHARED "")
      # target_compile_definitions(${LIB_GLFW} PUBLIC -DGLFW_DLL=1)
      # # target_compile_definitions(${LIB_GLFW} PUBLIC -D_GLFW_BUILD_DLL=1)
   # else()
      message("${BoldBlue}[benni] STATIC_LIBRARY: ${LIB_GLFW}${ColourReset}")
      add_library(${LIB_GLFW} STATIC "")
   # endif()

   target_compile_definitions(${LIB_GLFW} PUBLIC -D_GLFW_USE_CONFIG_H=1)
   target_compile_definitions(${LIB_GLFW} PUBLIC -DGLFW_INCLUDE_ES32=1)
   target_compile_definitions(${LIB_GLFW} PUBLIC -DGLFW_INCLUDE_GLU=1)
   # target_compile_definitions(${LIB_GLFW} PUBLIC -DGLFW_INCLUDE_GLEXT=1)

   if (WIN32)
      list(APPEND GLFW_SOURCES ${GLFW_SOURCES_WIN32})
      target_compile_definitions(${LIB_GLFW} PUBLIC -D_GLFW_WIN32=1)
      #target_compile_definitions(${LIB_GLFW} PUBLIC -D_GLFW_VULKAN_STATIC=1)
      #target_compile_definitions(${LIB_GLFW} PUBLIC -D_GLFW_USE_HYBRID_HPG=1)
   endif()
   if (UNIX)
      #define _GLFW_X11
      #define _GLFW_WAYLAND
      #define _GLFW_OSMESA
      #define HAVE_XKBCOMMON_COMPOSE_H
      #define HAVE_MEMFD_CREATE
   endif()   
   
   if(MAC)
      #define _GLFW_COCOA
   endif()

   # target_compile_options(${LIB_GLFW} PRIVATE -O3)

   target_sources(${LIB_GLFW} PUBLIC ${GLFW_HEADERS} PRIVATE ${GLFW_SOURCES})
   target_include_directories(${LIB_GLFW} PUBLIC ${GLFW_HEADER_DIR}/..)
   target_include_directories(${LIB_GLFW} PRIVATE ${GLFW_SOURCE_DIR})
   # target_include_directories(${LIB_GLFW} PRIVATE ${GLFW_HEADER_DIR})
   
   target_link_libraries(${LIB_GLFW} PUBLIC de_opengles3)

   # target_compile_options(${LIB_GLFW} PRIVATE -Wno-sign-compare )
   # -Wno-implicit-fallthrough
   # -Wmaybe-uninitialized

endif()

add_subdirectory(test)
## Packaging
#set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
#set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The FreeType font rendering library.")
#set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/docs/LICENSE.TXT")

#set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
#set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
#set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
#set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

#if (WIN32)
#  set(CPACK_GENERATOR ZIP)
#else()
#  set(CPACK_GENERATOR TGZ)
#endif()

#set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Libraries")
#set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C/C++ Headers")
#set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION
#  "Library used to build programs which use FreeType")
#set(CPACK_COMPONENT_HEADERS_DESCRIPTION
#  "C/C++ header files for use with FreeType")
#set(CPACK_COMPONENT_HEADERS_DEPENDS libraries)
#set(CPACK_COMPONENT_LIBRARIES_GROUP "Development")
#set(CPACK_COMPONENT_HEADERS_GROUP "Development")

#include(CPack)
