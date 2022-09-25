set(APP_NAME benni2console)
project(${APP_NAME}-dev)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(APP_NAME "${APP_NAME}d")
endif()

if( BUILD_SHARED EQUAL 1 )
else()
  set(APP_NAME "${APP_NAME}s") # static suffix
endif()

if (TARGET ${APP_NAME})
   message("${BoldRed}[benni] HEADER_GUARD: ${APP_NAME}${ColourReset}")
else()
   message("${BoldYellow}[benni] APPLICATION: ${APP_NAME}${ColourReset}")

   enable_language(RC)
   file( GLOB_RECURSE APP_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.[h,c]pp")
   list(APPEND APP_SOURCES ${CMAKE_CURRENT_LIST_DIR}/res/resource.rc)

   add_executable(${APP_NAME} ${APP_SOURCES})
   target_include_directories(${APP_NAME} PRIVATE ${APP_SOURCE_DIR})

   target_compile_options(${APP_NAME} PUBLIC -Wno-deprecated-declarations)
   target_link_options(${APP_NAME} PRIVATE -mwindows)

   if (WIN32)  # on Windows:
      target_link_libraries(${APP_NAME} PRIVATE comctl32 user32 gdi32 kernel32)
   else()      # on Linux:
      target_link_libraries(${APP_NAME} PRIVATE X11 Xxf86vm Xi Xcursor)
   endif()

endif()
